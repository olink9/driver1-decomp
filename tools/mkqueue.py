#!/usr/bin/env python3
"""Write pilot/queue.tsv: game functions (below the PsyQ libraries, which start
at __main 0x738F0) smallest first, with the Driver 2 counterpart hint from
pilot/d2map.tsv when there is one."""
import os, re
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LIB_START = 0x738F0
FN = os.path.join(ROOT, "asm_fn/nonmatchings/main")
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
    if addr >= LIB_START:
        continue
    h = hint.get(m.group(1), ("", ""))
    rows.append((int(m.group(2), 16), addr, m.group(1), h[0], h[1]))
rows.sort()
with open(os.path.join(ROOT, "pilot/queue.tsv"), "w") as f:
    f.write("size\taddr\tsymbol\td2_name\td2_score\n")
    for s, a, n, h, sc in rows:
        f.write(f"{s}\t0x{a:08X}\t{n}\t{h}\t{sc}\n")
print(len(rows), "game functions,", sum(r[0] for r in rows), "bytes")
