#!/usr/bin/env python3
"""Driver 1 PC port: the data side of the link (a build step, see port/CMakeLists.txt).

    pc_gendata.py fixups REPO OUT/pc_fixups.s
    pc_gendata.py syms   REPO OUT/pc_syms.ld OBJ... [--lib=ARCHIVE_OR_SO ...]
                    (OBJ = every object of the exe; --lib only sorts the report)

Outputs are build products, never committed. The model (port/README.md): PS1 RAM is mapped at its own
addresses (pc_psxram = 0), SLUS_008.42 is loaded there at startup and each overlay when the game loads it, so
all PS1 data the C files do not own yet lives at its PS1 address with its PS1 contents, and every pointer
inside it (0x001C00D8 ...) is already a valid PC pointer. Two things need help:

pc_fixups.s: the data words splat identified as pointers (`.word func_x`, `.word D_x + n`), per image, as
  {PS1 address, PC symbol}. pc_main.c rewrites them after loading the image: function pointers get the PC
  function, pointers to C-owned data the C object (other pointers are rewritten to the value they had).

pc_syms.ld: one PROVIDE() per symbol the objects leave undefined (a PROVIDE only binds undefined symbols):
  1. PsyQ library functions known only by address -> their implementation (PSYQ_ALIAS)
  2. a name inside an object a C file defines -> owner + offset, from the PS1 addresses encoded in both
     names (D_000C4C7C = D_000C4690 + 0x5EC)
  3. data in a PS1 image (main 0x10000-0xD0054 incl. .bss, the overlays' ranges) -> the PS1 address itself
  4. splat's short overlay call names (func_1E0290) -> the overlay's function at that address
  A name none of these resolve is left to the linker and listed in pc_syms.txt, next to every resolution.
"""
import glob, os, re, subprocess, sys

DATA_FILES = [
    ("main", "asm/data/800.rodata.s"), ("main", "asm/data/75B14.data.s"),
    ("frontend", "asm/frontend/data/frontend_rodata.rodata.s"), ("frontend", "asm/frontend/data/frontend_data.data.s"),
    ("mcard", "asm/mcard/data/mcard_rodata.rodata.s"), ("mcard", "asm/mcard/data/mcard_data.data.s"),
    ("iview", "asm/iview/data/iview_rodata.rodata.s"), ("iview", "asm/iview/data/iview_data.data.s"),
    ("cred", "asm/cred/data/cred_rodata.rodata.s"), ("cred", "asm/cred/data/cred_data.data.s"),
]
# PS1 images resident in PS1 RAM: main (rodata+text+data+bss), overlays (file incl. zero tail; CRED/IVIEW share)
IMAGE_RANGES = [("main", 0x10000, 0xD0054), ("mcard", 0xE6600, 0xF2A88), ("frontend", 0x1C0000, 0x1D3308),
                ("cred/iview", 0x1E0000, 0x1F3878)]
CODE_RANGES = [("mcard", 0xE6600, 0xED52C), ("frontend", 0x1C0000, 0x1CDC80),
               ("iview", 0x1E0000, 0x1E3358), ("cred", 0x1E0000, 0x1E1AB4)]

# PsyQ functions the game calls by address -> implementation (PsyCross, or port/pc/pc_cd.c for libcd).
# How each was identified: port/README.md. The ones with PC-only behaviour are defined in port/pc/pc_psyq.c.
PSYQ_ALIAS = {
    "func_000739B4": "SetGeomOffset", "func_00077D6C": "SetGeomScreen",
    "func_00073F88": "ResetGraph", "func_000740FC": "SetGraphDebug",
    "func_00074374": "ClearImage", "func_00074404": "LoadImage", "func_000744C4": "MoveImage",
    "func_0007457C": "ClearOTagR", "func_00074684": "DrawOTag", "func_00074CAC": "SetDrawArea",
    "func_000766A8": "StoreImage", "func_000776F4": "ResetCallback", "func_000777E8": "StopCallback",
    "func_00079800": "SetVideoMode", "func_0007A7F4": "GetClut", "func_0007D608": "LoadClut2",
    "func_0007D66C": "SetDefDrawEnv", "func_0007D720": "SetDefDispEnv",
    "func_0007AEB4": "PadStartCom", "func_0007AED4": "PadGetState", "func_0007AF94": "PadInfoMode",
    "func_0007B160": "PadSetActAlign", "func_0007B198": "PadSetAct",
    "func_0007C7D4": "SpuSetReverbVoice",
    # (libmcrd: pc_psyq.c - PsyCross's LIBMCRD indexes a 2-entry array with chan = port << 4 and fseeks a NULL
    # FILE when 0.MCD is missing)
    # libcd (port/pc/pc_cd.c)
    "func_00079F70": "CdSync", "func_0007A12C": "CdReadyCallback", "func_0007A27C": "CdControlF",
    "func_0007A4FC": "CdGetSector", "func_0007A53C": "CdDataCallback", "func_0007A580": "CdIntToPos",
    "func_0007E8FC": "CdReadFile",
    # (the BIOS libc calls - memcpy, strcmp ... - are wrappers in pc_psyq.c: a linker-script alias can't name a
    # shared-library symbol)
}


def gen_fixups(repo, out_path):
    """pc_fixups.s: per PS1 image (main, each overlay) the words of its .rodata/.data that hold a pointer splat
    identified (`.word func_x` / `.word D_x + n`): {PS1 address, PC symbol}. pc_main.c applies them after the
    image is loaded into PS1 RAM, so tables of function pointers and pointers to C-owned data point at the PC
    objects. Pointers into PS1 RAM itself need nothing: the image sits at its own addresses."""
    line_re = re.compile(r"^\s*/\*\s*([0-9A-F]+)\s+([0-9A-F]{8})(?:\s+([0-9A-F]+))?\s*\*/\s*\.word\s+(.*)$")
    fix = {}
    for ov, path in DATA_FILES:
        for l in open(os.path.join(repo, path), encoding="latin1"):
            m = line_re.match(l)
            if not m:
                continue
            arg = m.group(4).split("/*")[0].strip()
            if re.fullmatch(r"-?(0x[0-9A-Fa-f]+|\d+)", arg) or arg.startswith(".L"):
                continue                     # a number, or a jump table entry (only the PS1 asm used those)
            rv = int.from_bytes(bytes.fromhex(m.group(3)), "little")
            if "+" in arg and rv % 4:
                continue                     # splat's `D_00010000 + 0x1` etc.: flag bytes, not pointers
            t = re.match(r"(?:func|D)_([0-9A-F]+)", arg)
            ta = int(t.group(1), 16) if t else None
            if ta is not None and arg.startswith("func_") and 0x738F0 <= ta < 0x85314:
                continue                     # PsyQ's own tables (GPU/pad/SPU dispatch): only the replaced library read them
            if ta is not None and 0x1F800000 <= ta < 0x1F800400:
                arg = f"pc_scratchpad + 0x{ta - 0x1F800000:X}" + arg[t.end():]
            fix.setdefault(ov, []).append((int(m.group(2), 16), arg))
    out = ["# generated by port/pc/pc_gendata.py from the PS1 data asm - do not edit", "\t.section .rodata", "\t.p2align 2"]
    for ov in ("main", "frontend", "mcard", "cred", "iview"):
        f = fix.get(ov, [])
        out += [f"\t.globl pc_fixups_{ov}", f"pc_fixups_{ov}:", f"\t.long {len(f)}"]
        out += [f"\t.long 0x{a:08X}, {e}" for a, e in f]
    out.append('\t.section .note.GNU-stack,"",@progbits')
    open(out_path, "w").write("\n".join(out) + "\n")


def gen_syms(repo, out_path, objs, libs):
    sym_addr, addr_names, funcs = {}, {}, set()
    for f in sorted(glob.glob(os.path.join(repo, "symbol_addrs*.txt"))):
        ov = os.path.basename(f).split(".")[1] if f.count(".") > 1 else "main"
        for l in open(f, encoding="latin1"):
            m = re.match(r"\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)", l)
            if m:
                sym_addr.setdefault(m.group(1), int(m.group(2), 16))
                addr_names.setdefault((ov, int(m.group(2), 16)), m.group(1))
                if "type:func" in l:
                    funcs.add(m.group(1))

    def addr_of(n):
        m = re.fullmatch(r"(?:D|func)_([0-9A-F]{5,8})", n)
        return int(m.group(1), 16) if m else sym_addr.get(n)

    nm = os.environ.get("NM", "nm")
    defined, undef = {}, set()
    for o in objs:
        for l in subprocess.run([nm, "-S", o], capture_output=True, text=True, check=True).stdout.splitlines():
            p = l.split()
            if len(p) == 2 and p[0] == "U":
                undef.add(p[1])
            elif len(p) == 4:
                t, n = p[2], p[3]
                strong = t not in "vVwW"
                if n not in defined or (strong and not defined[n][1]):
                    defined[n] = (int(p[1], 16), strong, t)
            elif len(p) == 3 and p[1] not in "Uu":
                defined.setdefault(p[2], (0, p[1] not in "vVwW", p[1]))
    undef -= set(defined)
    owners = []                     # C-defined data objects with a known PS1 address
    for n, (size, strong, t) in defined.items():
        a = addr_of(n)
        if a is not None and size > 0 and strong and t.upper() in "BDRCGS":
            owners.append((a, a + size, strong, n))
    owners.sort()
    ld = ["/* generated by port/pc/pc_gendata.py - do not edit */",
          "PROVIDE(pc_psxram = 0x0);   /* PS1 main RAM: pages 0x10000-0x1FFFFF mapped by port/pc/pc_main.c */"]
    rep, unres = [], []
    for n in sorted(undef):
        if n == "pc_psxram" or n.startswith(("__wrap_", "__real_")):
            continue
        if n in PSYQ_ALIAS:
            ld += [f"EXTERN({PSYQ_ALIAS[n]});", f"PROVIDE({n} = {PSYQ_ALIAS[n]});"]
            rep.append(f"psyq   {n} -> {PSYQ_ALIAS[n]}")
            continue
        a = addr_of(n)
        if a is None:
            unres.append(n)
            continue
        isfunc = n.startswith("func_") or n in funcs
        hits = [] if isfunc else [o for o in owners if o[0] <= a < o[1]]
        if hits:
            o = max(hits)                                   # the innermost
            ld.append(f"PROVIDE({n} = {o[3]} + 0x{a - o[0]:X});")
            rep.append(f"owner  {n} -> {o[3]} + 0x{a - o[0]:X}")
            continue
        img = [r for r in IMAGE_RANGES if r[1] <= a < r[2]]
        if img and not isfunc:
            ld.append(f"PROVIDE({n} = 0x{a:08X});")
            rep.append(f"ps1    {n} -> PS1 RAM 0x{a:X} ({img[0][0]})")
            continue
        m = re.fullmatch(r"func_([0-9A-F]{5,6})", n)
        tgt = None
        if m:
            for ov, lo, hi in CODE_RANGES:
                if lo <= a < hi:
                    for c in (addr_names.get((ov, a)), f"func_{a:08X}"):
                        if c and c in defined:
                            tgt = c
                            break
                if tgt:
                    break
        if tgt:
            ld.append(f"PROVIDE({n} = {tgt});")
            rep.append(f"ovcall {n} -> {tgt}")
            continue
        unres.append(n)
    libdef = set()
    for lib in libs:
        r = subprocess.run([nm, "-D" if ".so" in lib else "-g", "--defined-only", lib], capture_output=True, text=True)
        for l in r.stdout.splitlines():
            p = l.split()
            if len(p) >= 3:
                libdef.add(p[2].split("@")[0])
    fromlib = [n for n in unres if n in libdef]
    unres = [n for n in unres if n not in libdef]
    open(out_path, "w").write("\n".join(ld) + "\n")
    with open(os.path.splitext(out_path)[0] + ".txt", "w") as f:
        f.write("\n".join(rep) + "\n\n# from the libraries (PsyCross / libc):\n" + " ".join(fromlib) +
                "\n\n# MISSING (left to the linker):\n" + "\n".join(unres) + "\n")
    kinds = {}
    for r in rep:
        kinds[r.split()[0]] = kinds.get(r.split()[0], 0) + 1
    print("pc_gendata syms: " + ", ".join(f"{k} {v}" for k, v in sorted(kinds.items())) +
          f", from libraries {len(fromlib)}, MISSING {len(unres)}" + (": " + " ".join(unres) if unres else ""))


if __name__ == "__main__":
    if sys.argv[1] == "fixups":
        gen_fixups(sys.argv[2], sys.argv[3])
    else:
        args = sys.argv[4:]
        libs = [a[6:] for a in args if a.startswith("--lib=")]
        gen_syms(sys.argv[2], sys.argv[3], [a for a in args if not a.startswith("--lib=")], libs)
