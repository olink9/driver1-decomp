#!/usr/bin/env python3
"""Print the next N untried functions from pilot/queue.tsv (smallest first),
skipping ones already in src/pilot or logged in pilot/results.tsv and ones
that are hand-written GTE assembly.

    tools/nextbatch.py N [--sample LO HI] [--seeded]

--sample LO HI picks evenly from sizes LO..HI; --seeded keeps only functions
with a Driver 2 counterpart (pilot/d2map.tsv score >= 0.4)."""
import os, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    a = sys.argv[1:]
    n = int(a[0])
    lo, hi = (int(a[a.index("--sample") + 1]), int(a[a.index("--sample") + 2])) if "--sample" in a else (0, 1 << 30)
    done = {f.split(".")[0] for f in os.listdir(os.path.join(ROOT, "src/pilot"))}
    res = os.path.join(ROOT, "pilot/results.tsv")
    if os.path.exists(res):
        done |= {l.split("\t")[1] for l in open(res).read().splitlines()[1:] if "\t" in l}
    rows = []
    for l in open(os.path.join(ROOT, "pilot/queue.tsv")).read().splitlines()[1:]:
        c = l.split("\t")
        size, sym = int(c[0]), c[2]
        if sym in done or not (lo <= size <= hi):
            continue
        if "--seeded" in a and not (c[4] and float(c[4]) >= 0.4):
            continue
        s = open(os.path.join(ROOT, "asm_fn/nonmatchings/main", sym + ".s"), errors="replace").read()
        if "handwritten instruction" in s and "ctc2" not in s and "mtc2" not in s:
            continue
        rows.append(l)
    if "--sample" in a and len(rows) > n:
        step = len(rows) / n
        rows = [rows[int(i * step)] for i in range(n)]
    print("\n".join(rows[:n]))


if __name__ == "__main__":
    main()
