#!/usr/bin/env python3
"""Everything an agent needs to start on one function.

    tools/fnasm.py SYMBOL [--no-d2]

Prints the target asm, the symbols it references, and - when pilot/d2map.tsv
pairs it with a Driver 2 function - that function's debug-symbol prototype
(source file, parameters with registers, locals, stack frame; from the Driver 2
symbol dump) and its REDRIVER2 C source (MIT, a reconstruction of Driver 2 -
same engine, different game: treat it as a strong hint, not the answer).
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FN = os.path.join(ROOT, "asm_fn/nonmatchings/main")
sys.path.insert(0, os.path.join(ROOT, "tools"))
import targets  # noqa: E402  (overlay symbols: asm_fn/nonmatchings/<target>/)
DUMP = os.environ.get("D2_SYMBOL_DUMP", os.path.expanduser("~/dev/redriver2-old/symbols/spanish_dump.h"))
R2 = os.environ.get("REDRIVER2_GAME", os.path.expanduser("~/dev/redriver2-ref/src_rebuild/Game"))


def d2_hint(sym):
    p = os.path.join(ROOT, "pilot/d2map.tsv")
    if not os.path.exists(p):
        return None
    for l in open(p).read().splitlines()[1:]:
        c = l.split("\t")
        if c[0] == sym:
            return c[2], float(c[4])
    return None


def dump_entry(name):
    d = open(DUMP, errors="replace").read()
    for e in d.split("\n/*\n * Offset ")[1:]:
        head = e.split(" */\n", 1)
        if len(head) == 2 and re.match(r"[^\n(]*\b" + re.escape(name) + r"\s*\(", head[1]):
            return ("/*\n * Offset " + e)[:6000]
    return None


def r2_source(name):
    for dp, _, fs in os.walk(R2):
        for f in fs:
            if not f.endswith(".c"):
                continue
            t = open(os.path.join(dp, f), errors="replace").read()
            m = re.search(r"^[\w \*]*\b" + re.escape(name) + r"\s*\([^;{]*\)\s*\{", t, re.M)
            if m:
                i, depth = t.index("{", m.start()), 0
                for j in range(i, len(t)):
                    depth += {"{": 1, "}": -1}.get(t[j], 0)
                    if depth == 0:
                        return os.path.relpath(os.path.join(dp, f), R2), t[m.start():j + 1][:8000]
    return None


def main():
    sym = sys.argv[1]
    p = os.path.join(FN, sym + ".s")
    if not os.path.exists(p):
        p = os.path.join(targets.path(targets.get(targets.of_symbol(sym)), "fn"), sym + ".s")
    if not os.path.exists(p):
        sys.exit(f"{sym}: not found")
    text = open(p, errors="replace").read()
    print(text)
    refs = sorted(set(re.findall(r"\b(func_[0-9A-F]{8}|D_[0-9A-F]{8}|jtbl_[0-9A-F]{8}|[A-Za-z_]\w*)\b(?=\)|$|,)", text))
                  - {sym}) if False else sorted(set(re.findall(r"\b(?:func|D|jtbl)_[0-9A-F]{8}\b", text)) - {sym})
    if refs:
        print("---- references: " + " ".join(refs))
    if "--no-d2" in sys.argv:
        return
    h = d2_hint(sym)
    if not h:
        return
    name, score = h
    print(f"---- Driver 2 counterpart (similarity {score:.2f}; >=0.6 strong, 0.4-0.6 weak): {name}")
    e = dump_entry(name)
    if e:
        print("---- Driver 2 debug symbols\n" + e)
    s = r2_source(name)
    if s:
        print(f"---- REDRIVER2 source ({s[0]})\n{s[1]}")


if __name__ == "__main__":
    main()
