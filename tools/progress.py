#!/usr/bin/env python3
"""Print the progress table for README.md from a target's queue, linkcheck TSV and C
files (tools/targets.py: pilot/queue.tsv, pilot/linkcheck.tsv, src/pilot for main;
pilot/queue-<t>.tsv, pilot/linkcheck-<t>.tsv, src/pilot/<t>/ for an overlay).

A function counts as matched when
  * it is defined in a committed translation unit (main: src/*.c, overlay: src/<t>/*.c;
    or an original assembler module src[/<t>]/*.s, one function per `glabel`;
    `git ls-files`, so untracked drafts don't count) - `make` links those and checks the
    sha1, and linkcheck --tu proves each one byte-identical. A TU row marked `fail` in the
    linkcheck TSV removes that TU's functions again. Functions a TU still pulls in with
    INCLUDE_ASM(..., fn) are wips, not matches; or
  * its per-function file src/pilot[/<t>]/<fn>.c exists and passes in the linkcheck TSV.
HANDWRITTEN ASM / FAKEMATCH tags are read from the function's own text: its pilot file,
or inside a TU the function's mktu section (from its
`/* ---------------- fn (...)` line to the next one; without such a line, from the end of
the previous definition to the end of this one).

    tools/progress.py                 # main's table, then one row per binary + total
    tools/progress.py --target T      # T's table only
"""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import targets  # noqa: E402


def tu_files(T):
    """Committed TUs of target T: main src/*.c, overlay src/<t>/*.c."""
    d = "src" if T["name"] == "main" else f"src/{T['name']}"
    pats = [f":(glob){d}/*.c", f":(glob){d}/*.s"]          # .s: original assembler modules
    try:
        out = subprocess.run(["git", "ls-files"] + pats, cwd=ROOT, capture_output=True, text=True, check=True).stdout
    except (OSError, subprocess.CalledProcessError):
        return []
    return [os.path.join(ROOT, l) for l in out.split()]


def _blank(src):
    """Comments, strings, char literals and preprocessor lines replaced by spaces (offsets kept)."""
    sub = lambda m: re.sub(r"[^\n]", " ", m.group(0))
    src = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', sub, src, flags=re.S)
    return re.sub(r"(?m)^[ \t]*#(?:[^\n]*\\\n)*[^\n]*", sub, src)   # preprocessor lines too


def tu_functions(path):
    """[(name, text)] for every top-level function definition of a TU, in file order
    (text = the function's section, see the module doc), and the INCLUDE_ASM names."""
    src = open(path, encoding="utf-8", errors="replace").read()
    if path.endswith(".s"):                    # an assembler module: every `glabel fn` is a function
        heads = [(m.start(), m.group(1)) for m in re.finditer(r"(?m)^/\* -{8,} (\w+) \(", src)]
        out = []
        for name in re.findall(r"(?m)^glabel\s+(\w+)", src):
            h = next((k for k, (a, n) in enumerate(heads) if n == name), None)
            if h is None:
                out.append((name, ""))
                continue
            out.append((name, src[heads[h][0]:heads[h + 1][0] if h + 1 < len(heads) else len(src)]))
        return out, []
    code = _blank(src)
    ansi = re.compile(r"(\w+)\s*\((?:[^()]|\([^()]*\))*\)\s*$")
    knr = re.compile(r"(\w+)\s*\([\w\s,]*\)\s*(?:[^;{}()=]*;\s*)+$")   # f(a, b) int a; char *b;
    defs, depth, starts = [], 0, [0]
    for m in re.finditer(r"[{};]", code):
        c = m.group(0)
        if depth == 0 and c == "{":
            found = None
            seg = code[starts[-1]:m.start()]
            if "=" not in seg:
                found = ansi.search(seg)
            if not found:
                for st in reversed(starts[:-1]):
                    seg = code[st:m.start()]
                    if "=" in seg.split("(")[0]:
                        break
                    found = knr.search(seg)
                    if found:
                        break
            if found and not re.search(r"\bstatic\s+inline\b|\binline\s+static\b", seg[:found.start()]):
                defs.append([found.group(1), m.start(), None])
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                if defs and defs[-1][2] is None:
                    defs[-1][2] = m.end()
                starts = [m.end()]
        elif depth == 0:
            starts.append(m.end())
    heads = {m.group(1): m.start() for m in re.finditer(r"(?m)^/\* -{8,} (\w+) \(", src)}
    hpos = sorted(heads.values())
    out, prev_end = [], 0
    for name, a, b in defs:
        if b is None:
            continue
        if name in heads:
            h = heads[name]
            nxt = next((x for x in hpos if x > h), len(src))
            text = src[h:max(nxt, b)]
        else:
            text = src[prev_end:b]
        out.append((name, text))
        prev_end = b
    asm = re.findall(r"(?m)^\s*INCLUDE_ASM\s*\(\s*\"[^\"]*\"\s*,\s*(\w+)\s*\)", src)
    return out, asm


def stats(name):
    T = targets.get(name)
    q = {}
    qp = targets.path(T, "queue")
    if not os.path.exists(qp):
        return None
    for l in open(qp).read().splitlines()[1:]:
        c = l.split("\t")
        q[c[2]] = int(c[0])
    lc, tu_fail = {}, set()
    p = targets.path(T, "linkcheck")
    if os.path.exists(p):
        for l in open(p).read().splitlines():
            c = l.split("\t")
            if len(c) >= 2:
                lc[c[0]] = c[1]
            if len(c) >= 3 and c[1] == "fail" and c[2].split(":")[0].endswith(".c"):
                tu_fail.add(c[2].split(":")[0])        # a TU row: detail starts with its path
    src = targets.path(T, "src")
    text, tu_asm = {}, set()
    for f in tu_files(T):
        if os.path.relpath(f, ROOT) in tu_fail:
            continue
        fns, asm = tu_functions(f)
        tu_asm |= {s for s in asm if s in q}
        for s, t in fns:
            if s in q:
                text[s] = t
    for s in q:
        if s in text or lc.get(s) != "pass":
            continue
        pf = os.path.join(src, s + ".c")
        if os.path.exists(pf):
            with open(pf) as f:
                text[s] = f.read()
    ok = [s for s in q if s in text]
    hw = [s for s in ok if "HANDWRITTEN ASM" in text[s]]
    fake = [s for s in ok if "FAKEMATCH" in text[s]]
    wip = {f[:-6] for f in os.listdir(src) if f.endswith(".wip.c")} if os.path.isdir(src) else set()
    wip = sorted((wip | tu_asm) - set(ok))
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
