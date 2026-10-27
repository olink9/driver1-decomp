#!/usr/bin/env python3
"""Lockstep comparison of Driver 1 game-state traces: the PC build (port/pc/pc_trace.c, D1_TRACE) against the PS1
game in an emulator (tools/pc/compare/lrhost.c). See port/README.md "Lockstep comparison".

  lockstep.py info  TRACE                    games (runs of the frame counter) found in a trace
  lockstep.py diff  PC.trace EMU.trace [opts] align the games and report the first divergence
  lockstep.py show  TRACE GAME FRAME [label] dump one record (or one region of it) as words

Alignment: a "game" is a run of records whose frame counter (D_00097534, reset to 0 at the start of every
func_0002360C loop) counts up 0,1,2,..; games are paired in order (--pc-game/--emu-game pick others), frames by
that counter - never by vblank, which differs (CD timing, FMV, the PC's skipped FMVs).
"""
import argparse
import struct
import sys
from collections import namedtuple

Region = namedtuple("Region", "label addr size off")


class Trace:
    def __init__(self, path):
        with open(path, "rb") as f:
            data = f.read()
        magic, self.recsize, n = struct.unpack_from("<III", data, 0)
        if magic != 0x31434C44:
            sys.exit(f"{path}: not a lockstep trace")
        self.regions = []
        off = 12
        payoff = 0
        for _ in range(n):
            label = data[off:off + 32].split(b"\0")[0].decode()
            addr, size = struct.unpack_from("<II", data, off + 32)
            self.regions.append(Region(label, addr, size, payoff))
            payoff += size
            off += 40
        self.data = data
        self.base = off
        self.count = (len(data) - off) // self.recsize
        self.by_label = {r.label: r for r in self.regions}

    def rec(self, i):
        o = self.base + i * self.recsize
        seq, vbl = struct.unpack_from("<II", self.data, o)
        return seq, vbl, memoryview(self.data)[o + 8:o + self.recsize]

    def word(self, payload, label, off=0):
        r = self.by_label[label]
        return struct.unpack_from("<i", payload, r.off + off)[0]

    def games(self):
        """[(first record, record count)] for each run of frame = 0,1,2,..."""
        out = []
        start = None
        prev = None
        for i in range(self.count):
            fr = self.word(self.rec(i)[2], "frame")
            if fr == 0 or prev is None or fr != prev + 1:
                if start is not None:
                    out.append((start, i - start))
                start = i
            prev = fr
        if start is not None:
            out.append((start, self.count - start))
        return out


def game_desc(t, g):
    s, n = g
    _, v0, p0 = t.rec(s)
    _, v1, p1 = t.rec(s + n - 1)
    return (f"records {s}..{s + n - 1}  frames {t.word(p0, 'frame')}..{t.word(p1, 'frame')}  "
            f"vblank {v0}..{v1}  CameraCnt {t.word(p0, 'CameraCnt')}..{t.word(p1, 'CameraCnt')}")


def cmd_info(a):
    t = Trace(a.trace)
    print(f"{a.trace}: {t.count} records of {t.recsize} bytes; regions:",
          ", ".join(f"{r.label}@{r.addr:05X}+{r.size}" for r in t.regions))
    for i, g in enumerate(t.games()):
        print(f"  game {i}: {game_desc(t, g)}")


# ---- field names for the structures we know (offsets from include/structs.h) ----
CAR_FIELDS = [
    (0x000, 4, "id/f1/model/f3"), (0x004, 18, "where.m"), (0x016, 2, "where.pad"), (0x018, 12, "where.t"),
    (0x024, 12, "fposition"), (0x030, 16, "orientation"), (0x040, 12, "linearVelocity"),
    (0x04C, 12, "angularVelocity"), (0x058, 12, "acc"), (0x064, 4, "f64"), (0x068, 12, "aacc"), (0x074, 4, "f74"),
    (0x078, 16, "wheel[4]"), (0x088, 64, "shadowPoints"), (0x0C8, 16, "pos"), (0x0D8, 16, "vel"),
    (0x0E8, 8, "angle"), (0x0F0, 4, "fF0"), (0x0F4, 4, "fF4"), (0x0F8, 4, "direction"), (0x0FC, 4, "fFC"),
    (0x100, 4, "wheelspin"), (0x104, 4, "wheel_speed"), (0x108, 4, "speed"), (0x10C, 4, "f10C"),
    (0x110, 4, "f110"), (0x114, 24, "damage[6]"), (0x12C, 2, "f12C"), (0x12E, 1, "hndType"), (0x12F, 1, "hit"),
    (0x130, 4, "f130..flash..f133"), (0x134, 64, "trails"), (0x174, 8, "old_clock"), (0x17C, 2, "totalDamage"),
    (0x17E, 2, "autoBrake/f17F"), (0x180, 4, "f180/f182"), (0x184, 4, "gear/f185/pad"), (0x188, 4, "lastPad"),
]


_PS1SYMS = None


def ps1_symbol(addr):
    """name+offset of the PS1 map symbol at or below addr"""
    global _PS1SYMS
    import bisect
    import re
    if _PS1SYMS is None:
        syms = {}
        try:
            for line in open(__file__.rsplit("/", 1)[0] + "/../../../build/slus_008.42.map"):
                mm = re.match(r"\s+0x([0-9a-f]{8})\s+([A-Za-z_]\w*)\s*(=.*)?$", line)
                if mm:
                    syms.setdefault(int(mm.group(1), 16), mm.group(2))
        except OSError:
            pass
        _PS1SYMS = sorted(syms.items())
    i = bisect.bisect_right(_PS1SYMS, (addr, "~")) - 1
    if i < 0:
        return "?"
    a, n = _PS1SYMS[i]
    return n if a == addr else f"{n}+{addr - a:#x}"


def describe(region, off):
    if region.label == "car_data":
        car, o = divmod(off, 0x18C)
        for fo, fs, name in CAR_FIELDS:
            if fo <= o < fo + fs:
                return f"car_data[{car}].{name}+{o - fo:#x} (car +{o:#x})"
        return f"car_data[{car}]+{o:#x}"
    return f"{region.label}+{off:#x} {ps1_symbol(region.addr + off)}"


def ps1_pointer_equiv(pcv, emv, pcmap):
    """True when a differing word is the same pointer on both sides: the PS1 address of an object (Driver keeps
    physical addresses, 0x000xxxxx, in its pointers; KSEG0 0x80xxxxxx accepted too) against the PC address of that
    object when the PC build owns it in C (PS1-image data sits at its PS1 address on PC and compares equal anyway)."""
    if pcmap is None:
        return False
    t = pcmap.get(pcv)
    return t is not None and t == (emv & 0x7FFFFFFF if emv & 0x80000000 else emv)


def load_pcmap(path):
    """PC address -> PS1 address for C-owned objects (and every word inside them), from `nm -S` of the PC build:
    D_xxxxxxxx and named objects whose PS1 address the PS1 map gives."""
    if not path:
        return None
    import re
    import subprocess
    ps1 = {}
    mapfile = __file__.rsplit("/", 1)[0] + "/../../../build/slus_008.42.map"
    try:
        for line in open(mapfile):
            m = re.match(r"\s+0x([0-9a-f]{8})\s+([A-Za-z_]\w*)\s*(=.*)?$", line)
            if m:
                ps1.setdefault(m.group(2), int(m.group(1), 16))
    except OSError:
        pass
    m = {}
    out = subprocess.run(["nm", "-S", path], capture_output=True, text=True).stdout
    for line in out.splitlines():   # the scratchpad: PS1 0x1F800000, PC pc_scratchpad (pc_main.c)
        p = line.split()
        if len(p) == 4 and p[3] == "pc_scratchpad":
            for o in range(0x400):
                m[int(p[0], 16) + o] = 0x1F800000 + o
    for line in out.splitlines():
        p = line.split()
        if len(p) != 4 or p[2] in "aAuUwWvV":
            continue
        name = p[3]
        if p[2] in "tT":   # a function: its entry is enough (function pointers)
            mm = re.match(r"func_([0-9A-F]{8})$", name)
            base = int(mm.group(1), 16) if mm else ps1.get(name)
            if base is not None:
                m.setdefault(int(p[0], 16), base)
            continue
        mm = re.match(r"D_([0-9A-F]{8})$", name)
        base = int(mm.group(1), 16) if mm else ps1.get(name)
        if base is None:
            continue
        addr, size = int(p[0], 16), int(p[1], 16)
        for o in range(0, max(size, 1)):
            m.setdefault(addr + o, base + o)
    return m


def load_masks(path):
    """spec "mask label offset bytes [stride count]" lines -> {label: set(word offsets)}"""
    masks = {}
    if not path:
        return masks
    for line in open(path):
        p = line.split("#")[0].split()
        if len(p) >= 4 and p[0] == "mask":
            off, size = int(p[2], 16), int(p[3])
            stride, count = (int(p[4], 16), int(p[5])) if len(p) >= 6 else (0, 1)
            for i in range(count):
                for o in range(off + i * stride, off + i * stride + size, 4):
                    masks.setdefault(p[1], set()).add(o & ~3)
    return masks


def cmd_diff(a):
    pc, em = Trace(a.pc), Trace(a.emu)
    masks = load_masks(a.spec)
    if [(r.label, r.size) for r in pc.regions] != [(r.label, r.size) for r in em.regions]:
        sys.exit("the two traces were made with different specs")
    ignore = set(a.ignore.split(",")) if a.ignore else set()
    pcmap = load_pcmap(a.pc_exe)
    pg, eg = pc.games(), em.games()
    print(f"PC : {len(pg)} games;  emulator: {len(eg)} games")
    pairs = []
    if a.pc_game is not None or a.emu_game is not None:
        pairs.append((a.pc_game or 0, a.emu_game or 0))
    else:
        pairs = list(zip(range(len(pg)), range(len(eg))))
    worst = 0
    for gi, gj in pairs:
        if gi >= len(pg) or gj >= len(eg):
            continue
        print(f"\nPC game {gi}: {game_desc(pc, pg[gi])}\nemu game {gj}: {game_desc(em, eg[gj])}")
        (ps, pn), (es, en) = pg[gi], eg[gj]
        n = min(pn, en)
        first = None
        diffs_per_region = {}
        for k in range(n):
            _, _, pp = pc.rec(ps + k)
            _, _, ep = em.rec(es + k)
            if pp == ep:
                continue
            found = []
            for r in pc.regions:
                if r.label in ignore:
                    continue
                a0, b0 = bytes(pp[r.off:r.off + r.size]), bytes(ep[r.off:r.off + r.size])
                if a0 == b0:
                    continue
                rm = masks.get(r.label, ())
                for o in range(0, r.size, 4):
                    if a0[o:o + 4] != b0[o:o + 4] and o not in rm:
                        pv = struct.unpack_from("<I", a0, o)[0]
                        ev = struct.unpack_from("<I", b0, o)[0]
                        if ps1_pointer_equiv(pv, ev, pcmap):
                            continue
                        found.append((r, o, pv, ev))
            if not found:
                continue
            for r, o, pv, ev in found:
                diffs_per_region.setdefault(r.label, k)
            if first is None:
                first = (k, found)
                if not a.all:
                    break
        if first is None:
            print(f"  lockstep: {n} frames identical"
                  + (f" (ignoring {','.join(sorted(ignore))})" if ignore else ""))
            continue
        k, found = first
        fr = pc.word(pc.rec(ps + k)[2], "frame")
        worst = max(worst, 1)
        print(f"  FIRST DIVERGENCE at frame {fr} (record pair {ps + k} / {es + k}), {k} frames identical before it;"
              f" {len(found)} differing words:")
        for r, o, pv, ev in found[:a.max]:
            print(f"    {describe(r, o):46s} @PS1 {r.addr + o:05X}   PC {pv:08X} ({pv if pv < 0x80000000 else pv - (1 << 32)})"
                  f"   PS1 {ev:08X} ({ev if ev < 0x80000000 else ev - (1 << 32)})")
        if len(found) > a.max:
            print(f"    ... {len(found) - a.max} more")
        if a.all:
            print("  first divergent frame per region:",
                  ", ".join(f"{lab}@{kk}" for lab, kk in sorted(diffs_per_region.items(), key=lambda x: x[1])))
    return worst


def cmd_show(a):
    t = Trace(a.trace)
    s, n = t.games()[a.game]
    for k in range(n):
        seq, vbl, p = t.rec(s + k)
        if t.word(p, "frame") == a.frame:
            for r in t.regions:
                if a.label and r.label != a.label:
                    continue
                print(f"{r.label} @{r.addr:05X} (+{r.size}):  [seq {seq} vblank {vbl}]")
                b = bytes(p[r.off:r.off + r.size])
                for o in range(0, r.size, 32):
                    ws = struct.unpack_from(f"<{min(8, (r.size - o) // 4)}I", b, o)
                    print(f"  +{o:04X}: " + " ".join(f"{w:08X}" for w in ws))
            return
    sys.exit("frame not found")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("info")
    p.add_argument("trace")
    p = sub.add_parser("diff")
    p.add_argument("pc")
    p.add_argument("emu")
    p.add_argument("--pc-game", type=int)
    p.add_argument("--emu-game", type=int)
    p.add_argument("--ignore", help="comma-separated region labels to leave out")
    p.add_argument("--all", action="store_true", help="scan every frame (first divergence per region)")
    p.add_argument("--max", type=int, default=40, help="differing words to list")
    p.add_argument("--pc-exe", help="the PC driver1 (nm) to recognise equal pointers")
    p.add_argument("--spec", default=__file__.rsplit("/", 1)[0] + "/trace.spec", help="spec with the mask lines")
    p = sub.add_parser("show")
    p.add_argument("trace")
    p.add_argument("game", type=int)
    p.add_argument("frame", type=int)
    p.add_argument("label", nargs="?")
    a = ap.parse_args()
    sys.exit({"info": cmd_info, "diff": cmd_diff, "show": cmd_show}[a.cmd](a) or 0)


if __name__ == "__main__":
    main()
