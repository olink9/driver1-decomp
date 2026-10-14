#!/usr/bin/env python3
"""Re-score every src/pilot/*.c (and *.wip.c) with tools/fnscore.py, promote a
.wip.c that now reaches 100, demote a .c that no longer does, and write
pilot/verified.tsv. Run inside distrobox `build`."""
import glob, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def score(sym, path):
    r = subprocess.run([sys.executable, os.path.join(ROOT, "tools/fnscore.py"), sym, path],
                       cwd=ROOT, capture_output=True, text=True)
    line = (r.stdout.splitlines() or ["?"])[0]
    return line, line.startswith("MATCH 100.0")


def main():
    rows = []
    for path in sorted(glob.glob(os.path.join(ROOT, "src/pilot/*.c"))):
        base = os.path.basename(path)[:-2]
        wip = base.endswith(".wip")
        sym = base[:-4] if wip else base
        line, ok = score(sym, path)
        if wip and ok:
            os.rename(path, path.replace(".wip.c", ".c")); line += "  (promoted)"
        elif not wip and not ok:
            os.rename(path, path[:-2] + ".wip.c"); line += "  (DEMOTED)"
        rows.append((sym, "exact" if ok else "wip", line))
        print(f"{sym}\t{line}")
    with open(os.path.join(ROOT, "pilot/verified.tsv"), "w") as f:
        f.write("symbol\tstatus\tscore\n")
        for r in rows:
            f.write("\t".join(r) + "\n")
    n = sum(1 for r in rows if r[1] == "exact")
    print(f"{n} exact, {len(rows) - n} wip")


if __name__ == "__main__":
    main()
