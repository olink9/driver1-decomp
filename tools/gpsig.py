#!/usr/bin/env python3
"""Per-function gp/absolute addressing signature (TU-PLAN.md section 0, finding 4).

    tools/gpsig.py                         # every game function: which small globals it
                                           #   reaches through $gp and which absolutely
    tools/gpsig.py --range FIRST LAST      # only these functions (names or 0x addresses)
    tools/gpsig.py --mixed                 # per symbol reached both ways: runs of functions
    tools/gpsig.py --gaps [N]              # the N gaps (default 40) where the most symbols
                                           #   change addressing mode: candidate file cuts
    tools/gpsig.py --tus FILE              # check proposed files: symbols mixed INSIDE a
                                           #   file, and the mode changes AT each boundary
    [--target main] [--window W]

Reads the committed .text asm (asm/2C8C.s for main): a `%gp_rel(sym)` operand is a gp
access, `%hi(sym)`/`%lo(sym)` an absolute one (`sym + 0x4` counts as sym). Only "small"
globals are reported: symbols reached through $gp somewhere, or whose address lies inside
the span of those (the .sdata/.sbss window around gp). An absolute access to one of them
is what the pilot files express with `extern long long sym` - the original compiler chose
it per file, so a run of functions that agree is a file, and a gap where many symbols flip
is a real boundary.

--gaps scores the gap between function i and i+1: a small symbol whose consecutive uses
j < k (at most --window functions apart, default 40) differ in mode adds 1/(k-j) to every
gap between them, so a flip pinned between two neighbours counts 1 and one that could lie
anywhere in a long stretch is spread over it.

--tus FILE: JSON from the TU plan's tus.py (a list of {name, start, ...}) or a text file
with one `name FIRSTFUNC|0xADDR` per line; each file runs up to the next one's start.
Reports, per file, the symbols it reaches both ways (wrong boundary, or a second effect
such as the maspsx .comm rule) and the gap score at its start.
"""
import argparse, json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import targets  # noqa: E402

ADDR_RE = re.compile(r"/\*\s+[0-9A-F]+\s+([0-9A-F]{8})\b")
REF_RE = re.compile(r"%(gp_rel|hi|lo)\(\s*([A-Za-z_.$][\w.$]*)")


def functions(path, lib_start=None, game_start=None):
    """[{name, addr, gp: {sym: n}, abs: {sym: n}}] for each glabel block, in order."""
    out, cur = [], None
    for l in open(os.path.join(ROOT, path)):
        w = l.split()
        if w[:1] == ["glabel"]:
            cur = dict(name=w[1].rstrip(","), addr=None, gp={}, abs={})
            out.append(cur)
            continue
        if w[:1] in (["dlabel"], ["nonmatching"]) and cur and w[:1] == ["dlabel"]:
            cur = None
            continue
        if cur is None or "*/" not in l:
            continue
        if cur["addr"] is None:
            m = ADDR_RE.search(l)
            if m:
                cur["addr"] = int(m.group(1), 16)
        for kind, sym in REF_RE.findall(l.split("*/", 1)[1]):
            if sym.startswith(".L"):
                continue
            d = cur["gp"] if kind == "gp_rel" else cur["abs"]
            d[sym] = d.get(sym, 0) + 1
    out = [f for f in out if f["addr"] is not None]
    if lib_start is not None:
        out = [f for f in out if f["addr"] < lib_start]
    if game_start is not None:
        out = [f for f in out if f["addr"] >= game_start]
    return out


def symbol_addrs(t):
    """{name: address} from the target's symbol scripts and symbol_addrs file."""
    files = [u for u in t["undef"] if os.path.exists(os.path.join(ROOT, u))]
    files += ["symbol_addrs.txt" if t["name"] == "main" else f"symbol_addrs.{t['name']}.txt"]
    out = {}
    for f in files:
        p = os.path.join(ROOT, f)
        if not os.path.exists(p):
            continue
        for l in open(p):
            m = re.match(r"\s*(?:PROVIDE\()?\s*([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+)", l)
            if m:
                out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def addr_of(sym, table):
    if sym in table:
        return table[sym]
    m = re.match(r"D_([0-9A-F]{8})$", sym)
    return int(m.group(1), 16) if m else None


def small_symbols(fns, table):
    gp = {s for f in fns for s in f["gp"]}
    addrs = [a for a in (addr_of(s, table) for s in gp) if a is not None]
    if not addrs:
        return gp, None
    lo, hi = min(addrs), max(addrs)
    allref = {s for f in fns for s in list(f["gp"]) + list(f["abs"])}
    small = gp | {s for s in allref if (addr_of(s, table) or -1) >= lo and addr_of(s, table) <= hi}
    return small, (lo, hi)


def modes(f, small):
    """{sym: 'gp'|'abs'|'both'} for the small symbols f uses."""
    out = {}
    for s in small:
        g, a = s in f["gp"], s in f["abs"]
        if g or a:
            out[s] = "both" if g and a else "gp" if g else "abs"
    return out


def gap_scores(fns, small, window):
    """[(score, i, [flipping symbols])] for the gap between fns[i] and fns[i+1]. A symbol
    whose consecutive uses j < k (at most `window` functions apart) differ in mode adds
    1/(k-j) to each gap between them: a flip pinned between two neighbours scores 1, one
    that could lie anywhere in a long stretch is spread over it."""
    M = [modes(f, small) for f in fns]
    uses = {}
    for i, m in enumerate(M):
        for s, x in m.items():
            if x != "both":
                uses.setdefault(s, []).append((i, x))
    score = [0.0] * (len(fns) - 1)
    syms = [[] for _ in score]
    for s, u in uses.items():
        for (j, m), (k, m2) in zip(u, u[1:]):
            if m == m2 or k - j > window:
                continue
            for g in range(j, k):
                score[g] += 1.0 / (k - j)
                syms[g].append(f"{s}:{m}>{m2}" + ("" if k - j == 1 else f"({j}-{k})"))
    return [(round(score[g], 2), g, sorted(syms[g])) for g in range(len(score))]


def load_tus(path, fns):
    idx = {f["name"]: i for i, f in enumerate(fns)}
    byaddr = {f["addr"]: i for i, f in enumerate(fns)}
    starts = []
    if path.endswith(".json"):
        for t in json.load(open(path)):
            a = t["start"] if isinstance(t["start"], int) else int(t["start"], 16)
            starts.append((t["name"], byaddr[a]))
    else:
        for l in open(path):
            w = l.split("#")[0].split()
            if not w:
                continue
            x = w[1]
            starts.append((w[0], byaddr[int(x, 16)] if x.lower().startswith("0x") else idx[x]))
    starts.sort(key=lambda x: x[1])
    return [(n, s, starts[k + 1][1] if k + 1 < len(starts) else len(fns)) for k, (n, s) in enumerate(starts)]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--target", choices=targets.NAMES, default="main")
    ap.add_argument("--range", nargs=2, metavar=("FIRST", "LAST"))
    ap.add_argument("--mixed", action="store_true")
    ap.add_argument("--gaps", nargs="?", type=int, const=40)
    ap.add_argument("--tus")
    ap.add_argument("--window", type=int, default=40)
    a = ap.parse_args()
    t = targets.get(a.target)
    fns = functions(t["text_asm"], t.get("lib_start"), t.get("game_start"))
    table = symbol_addrs(t)
    small, win = small_symbols(fns, table)
    if win is None:
        sys.exit(f"gpsig: no $gp access at all in {t['text_asm']} (built -G0)")
    hexs = lambda f: f"0x{f['addr']:X}"
    print(f"# {len(fns)} functions, {len(small)} small globals "
          f"(gp window 0x{win[0]:X}-0x{win[1]:X})")

    if a.mixed:
        for s in sorted(small, key=lambda s: addr_of(s, table) or 0):
            runs = []
            for i, f in enumerate(fns):
                m = modes(f, {s}).get(s)
                if not m:
                    continue
                if runs and runs[-1][0] == m:
                    runs[-1][2] = i
                    runs[-1][3] += 1
                else:
                    runs.append([m, i, i, 1])
            if len({r[0] for r in runs}) < 2:
                continue
            print(f"{s}\t" + "  ".join(f"{m} {fns[i]['name']}..{fns[j]['name']} ({i}-{j}, {n} fns)"
                                       for m, i, j, n in runs))
        return
    if a.gaps is not None:
        sc = sorted(gap_scores(fns, small, a.window), key=lambda x: (-x[0], x[1]))
        for n, i, fl in sc[:a.gaps]:
            print(f"{n:g}\tafter {i} {fns[i]['name']} | {i + 1} {fns[i + 1]['name']} {hexs(fns[i + 1])}\t{' '.join(fl)}")
        return
    if a.tus:
        tus = load_tus(a.tus, fns)
        score = {i + 1: (n, fl) for n, i, fl in gap_scores(fns, small, a.window)}
        tot = 0
        for name, s, e in tus:
            gp, ab = {}, {}
            for f in fns[s:e]:
                for x, m in modes(f, small).items():
                    if m in ("gp", "both"):
                        gp.setdefault(x, []).append(f["name"])
                    if m in ("abs", "both"):
                        ab.setdefault(x, []).append(f["name"])
            mixed = sorted(set(gp) & set(ab))
            tot += len(mixed)
            n, fl = score.get(s, (0, []))
            print(f"{name}\t{s}-{e - 1}\t{hexs(fns[s])}\tstart flips {n:g}\tmixed inside {len(mixed)}")
            for x in mixed:
                print(f"    {x}: gp {','.join(gp[x])} | abs {','.join(ab[x])}")
        print(f"# {tot} symbols mixed inside a file")
        return
    lo, hi = 0, len(fns) - 1
    if a.range:
        def find(x):
            k = next((i for i, f in enumerate(fns) if (f["addr"] == int(x, 16) if x.lower().startswith("0x") else f["name"] == x)), None)
            if k is None:
                sys.exit(f"gpsig: no function {x}")
            return k
        lo, hi = find(a.range[0]), find(a.range[1])
    for i in range(lo, hi + 1):
        f = fns[i]
        m = modes(f, small)
        g = sorted(s for s, v in m.items() if v in ("gp", "both"))
        b = sorted(s for s, v in m.items() if v in ("abs", "both"))
        print(f"{i}\t{hexs(f)}\t{f['name']}\tgp: {' '.join(g) or '-'}\tabs: {' '.join(b) or '-'}")


if __name__ == "__main__":
    main()
