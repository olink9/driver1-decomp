#!/usr/bin/env python3
"""Write a target's work queue (pilot/queue.tsv for main, pilot/queue-<target>.tsv for an
overlay): game functions (main: below the PsyQ libraries, which start at __main 0x738F0)
smallest first, with the Driver 2 counterpart hint from pilot/d2map.tsv when there is one.

    tools/mkqueue.py [--target main|frontend|mcard|cred|iview|nfmv]
"""
import argparse, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import targets  # noqa: E402

ROOT = targets.ROOT
ap = argparse.ArgumentParser()
ap.add_argument("--target", choices=targets.NAMES, default="main")
T = targets.get(ap.parse_args().target)
LIB_START = T["lib_start"]
GAME_START = T.get("game_start")
FN = targets.path(T, "fn")
hint = {}
p = os.path.join(ROOT, "pilot/d2map.tsv")
if os.path.exists(p):
    for l in open(p).read().splitlines()[1:]:
        c = l.split("\t")
        hint[c[0]] = (c[2], c[4])
rows = []
for f in os.listdir(FN):
    t = open(os.path.join(FN, f), errors="replace").read()
    m = re.search(r"nonmatching (\w+), (0x[0-9A-F]+)", t)
    a = re.search(r"/\* \w+ ([0-9A-F]{8}) ", t)
    if not m or not a or "glabel" not in t:
        continue
    addr = int(a.group(1), 16)
    if LIB_START is not None and addr >= LIB_START:
        continue
    if GAME_START is not None and addr < GAME_START:
        continue
    h = hint.get(m.group(1), ("", ""))
    rows.append((int(m.group(2), 16), addr, m.group(1), h[0], h[1]))
rows.sort()
with open(targets.path(T, "queue"), "w") as f:
    f.write("size\taddr\tsymbol\td2_name\td2_score\n")
    for s, a, n, h, sc in rows:
        f.write(f"{s}\t0x{a:08X}\t{n}\t{h}\t{sc}\n")
print(len(rows), "game functions,", sum(r[0] for r in rows), "bytes")
