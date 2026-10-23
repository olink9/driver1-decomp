#!/usr/bin/env python3
"""Print the progress table for README.md from a target's queue, linkcheck TSV and C
files (tools/targets.py: pilot/queue.tsv, pilot/linkcheck.tsv, src/pilot for main;
pilot/queue-<t>.tsv, pilot/linkcheck-<t>.tsv, src/pilot/<t>/ for an overlay). Only
functions that pass tools/linkcheck.py count as matched.

    tools/progress.py                 # main's table, then one row per binary + total
    tools/progress.py --target T      # T's table only
"""
import os, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import targets  # noqa: E402


def stats(name):
    T = targets.get(name)
    q = {}
    qp = targets.path(T, "queue")
    if not os.path.exists(qp):
        return None
    for l in open(qp).read().splitlines()[1:]:
        c = l.split("\t")
        q[c[2]] = int(c[0])
    lc = {}
    p = targets.path(T, "linkcheck")
    if os.path.exists(p):
        for l in open(p).read().splitlines():
            c = l.split("\t")
            if len(c) >= 2:
                lc[c[0]] = c[1]
    ok = [s for s in q if lc.get(s) == "pass"]
    src = targets.path(T, "src")

    def tagged(s, tag):
        with open(os.path.join(src, s + ".c")) as f:
            return tag in f.read(400)
    hw = [s for s in ok if tagged(s, "HANDWRITTEN ASM")]
    fake = [s for s in ok if tagged(s, "FAKEMATCH")]
    wip = [f[:-6] for f in os.listdir(src) if f.endswith(".wip.c") and f[:-6] not in ok] if os.path.isdir(src) else []
    return dict(q=q, ok=ok, hw=hw, fake=fake, wip=wip)


def table(st):
    q, ok, hw, fake, wip = st["q"], st["ok"], st["hw"], st["fake"], st["wip"]
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


def main():
    if "--target" in sys.argv:
        st = stats(sys.argv[sys.argv.index("--target") + 1])
        if st is None:
            sys.exit("no queue for that target (tools/mkqueue.py --target ...)")
        table(st)
        return
    table(stats("main"))
    print()
    print("| Binary | Functions matched | Code bytes matched |")
    print("|---|---|---|")
    F = B = nf = nb = 0
    for name in targets.NAMES:
        st = stats(name)
        if st is None:
            continue
        q, ok = st["q"], st["ok"]
        f, b, tb = len(ok), sum(q[s] for s in ok), sum(q.values())
        label = targets.get(name)["orig"].split("/")[-1]
        print(f"| {label} | {f} / {len(q)} ({100*f/len(q):.1f}%) | {b:,} / {tb:,} ({100*b/tb:.1f}%) |")
        F, B, nf, nb = F + f, B + b, nf + len(q), nb + tb
    print(f"| **Total** | **{F} / {nf}** ({100*F/nf:.1f}%) | **{B:,} / {nb:,}** ({100*B/nb:.1f}%) |")


if __name__ == "__main__":
    main()
