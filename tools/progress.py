#!/usr/bin/env python3
"""Print the progress table for README.md from pilot/queue.tsv, pilot/linkcheck.tsv
and the files in src/pilot. Only functions that pass tools/linkcheck.py count
as matched.

    tools/progress.py
"""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    q = {}
    for l in open(os.path.join(ROOT, "pilot/queue.tsv")).read().splitlines()[1:]:
        c = l.split("\t")
        q[c[2]] = int(c[0])
    lc = {}
    p = os.path.join(ROOT, "pilot/linkcheck.tsv")
    if os.path.exists(p):
        for l in open(p).read().splitlines():
            c = l.split("\t")
            if len(c) >= 2:
                lc[c[0]] = c[1]
    ok = [s for s in q if lc.get(s) == "pass"]
    src = os.path.join(ROOT, "src/pilot")

    def tagged(s, tag):
        with open(os.path.join(src, s + ".c")) as f:
            return tag in f.read(400)
    hw = [s for s in ok if tagged(s, "HANDWRITTEN ASM")]
    fake = [s for s in ok if tagged(s, "FAKEMATCH")]
    wip = [f[:-6] for f in os.listdir(src) if f.endswith(".wip.c") and f[:-6] not in ok]
    tot, done = sum(q.values()), sum(q[s] for s in ok)

    print(f"| | Functions | Code bytes |")
    print(f"|---|---|---|")
    print(f"| **Matched (link-verified)** | **{len(ok)} / {len(q)}** ({100*len(ok)/len(q):.1f}%) "
          f"| **{done:,} / {tot:,}** ({100*done/tot:.1f}%) |")
    print(f"| of which clean C | {len(ok)-len(hw)-len(fake)} | |")
    print(f"| of which hand-written asm in the original | {len(hw)} | |")
    print(f"| of which `FAKEMATCH` | {len(fake)} | |")
    print(f"| Near-misses (`.wip.c`) | {len(wip)} | {sum(q.get(s, 0) for s in wip):,} |")
    print()
    print("| Function size | Matched |")
    print("|---|---|")
    for lo, hi, name in [(0, 32, "≤ 32 B"), (33, 128, "33–128 B"), (129, 1024, "129 B–1 KB"), (1025, 1 << 30, "> 1 KB")]:
        S = [s for s in q if lo <= q[s] <= hi]
        print(f"| {name} | {len([s for s in S if s in ok])} / {len(S)} |")


if __name__ == "__main__":
    main()
