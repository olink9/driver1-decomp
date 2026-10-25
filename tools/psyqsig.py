#!/usr/bin/env python3
"""Find PsyQ library objects in SLUS_008.42 with lab313ru/psx_psyq_signatures.

    tools/psyqsig.py [VERSION ...]     # default: try them all, report counts
    tools/psyqsig.py --apply 4.3 4.2 4.4   # write names into symbol_addrs.txt (psyq section)
    tools/psyqsig.py --target nfmv [--apply ...]   # NFMV.EXE: symbol_addrs.nfmv.txt,
                                                   # pilot/psyq_objs-nfmv.tsv

Each signature is a whole .OBJ's code with relocated bytes masked (??); a hit
names every label in it at load address + offset.
"""
import json, os, re, struct, sys

SIGS = os.environ.get("PSYQ_SIGS", os.path.expanduser("~/dev/psx_psyq_signatures"))
EXE = os.path.join(os.path.dirname(__file__), "..", "orig", "SLUS_008.42")
SYMS, OBJS = "symbol_addrs.txt", "psyq_objs.tsv"
# separate PS-X EXEs with their own libraries (the overlays have none)
EXES = {"nfmv": ("NFMV.EXE", "symbol_addrs.nfmv.txt", "psyq_objs-nfmv.tsv")}


def load_exe():
    d = open(EXE, "rb").read()
    t_addr, t_size = struct.unpack_from("<II", d, 0x18)
    return d[0x800:0x800 + t_size], t_addr


def regex(sig):
    out = b""
    for tok in sig.split():
        out += b"." if tok == "??" else re.escape(bytes([int(tok, 16)]))
    return re.compile(out, re.DOTALL)


def scan(ver, text, base):
    hits = []
    for fn in sorted(os.listdir(os.path.join(SIGS, ver))):
        if not fn.endswith(".json"):
            continue
        lib = fn[:-5]
        for o in json.load(open(os.path.join(SIGS, ver, fn))):
            if "sig" not in o:
                continue
            sig = o["sig"].strip()
            if len(sig.split()) < 64:          # short sigs (stubs) hit unrelated code
                continue
            ms = [m.start() for m in regex(sig).finditer(text) if m.start() % 4 == 0]
            if len(ms) == 1:
                labels = o["labels"] if isinstance(o["labels"], list) else eval(o["labels"])
                hits.append((lib, o["name"], base + ms[0], len(sig.split()), labels))
    return hits


def main():
    global EXE, SYMS, OBJS
    args = sys.argv[1:]
    if args[:1] == ["--target"]:
        if args[1] != "main":
            exe, SYMS, OBJS = EXES[args[1]]
            EXE = os.path.join(os.path.dirname(EXE), exe)
        args = args[2:]
    text, base = load_exe()
    if args[:1] == ["--apply"]:
        hits = []                                  # first version listed wins an address
        for ver in [a.replace(".", "") for a in args[1:]]:
            hits += scan(ver, text, base)
        names = {}
        for lib, obj, addr, size, labels in hits:
            for l in labels:
                if not l["name"].startswith(("loc_", "locret_", "def_", "jpt_", "text_")):
                    names.setdefault(addr + l["offset"], (l["name"], lib, obj))
        path = os.path.join(os.path.dirname(__file__), "..", SYMS)
        # splat reads key:value pairs after //, so the library goes in its own file
        keep = [l for l in open(path).read().splitlines() if l.strip() and not any(
            l.startswith(n + " ") for n, _, _ in names.values())] if os.path.exists(path) else []
        lines = [f"{n} = 0x{a:08X}; // type:func" for a, (n, lib, obj) in sorted(names.items())]
        open(path, "w").write("\n".join(keep + lines) + "\n")
        with open(os.path.join(os.path.dirname(path), "pilot", OBJS), "w") as f:
            f.write("addr\tname\tlib\tobj\n")
            for a, (n, lib, obj) in sorted(names.items()):
                f.write(f"0x{a:08X}\t{n}\t{lib}\t{obj}\n")
        print(f"{len(hits)} objects, {len(names)} names written")
        return
    vers = [v.replace(".", "") for v in args] or sorted(d for d in os.listdir(SIGS) if d.isdigit())
    for v in vers:
        hits = scan(v, text, base)
        print(v, len(hits), "objects", sum(h[3] for h in hits), "bytes")


if __name__ == "__main__":
    main()
