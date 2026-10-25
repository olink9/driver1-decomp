#!/usr/bin/env python3
"""Make a translation unit: concatenate the src/pilot files of an address range.

    tools/mktu.py OUT.c FIRST LAST [--target main] [--wip] [--report FILE]

FIRST and LAST are function names or addresses (0x...) of asm blocks in the target's
.text asm (asm/2C8C.s for main); the range is inclusive and every block in it needs a
matched src/pilot/<name>.c (--wip also accepts <name>.wip.c). Nothing under src/ is
changed: OUT is a new file (put scratch output under build/).

What goes where:
  * a file-local prelude at the top: every #include, typedef, struct/union/enum tag
    definition and extern/prototype declaration of the pilot files, deduplicated when
    the token text is identical. Declarations of a symbol that differ between files are
    CONFLICTS: they are listed (stdout and the head comment of OUT) and not resolved -
    the first spelling stays active, the others are kept as comments next to it.
  * each function the TU defines gets one forward declaration `RET name();` (static if
    its definition is) instead of the pilot files' extern lines for it; pilot spellings
    of those that were not plain `RET name();` are listed as notes (a prototype, or a
    different return type, can change codegen at call sites).
  * then one section per pilot file, in address order: its comments, data definitions
    and helper functions in their original order, then the function. #define lines stay
    in place and are #undef'd at the end of their section; a typedef/extern that uses
    such a macro stays in place too ("kept local").
  * helper functions (static inline) and data definitions that appear in several files
    are kept once if identical, otherwise listed as conflicts (and left in, so the
    compiler stops on them).
Exit status 0 even with conflicts; the report says how many.
"""
import argparse, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import targets  # noqa: E402

KEYWORDS = {"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "const",
            "volatile", "struct", "union", "enum", "static", "extern", "register", "inline", "__inline__",
            "__inline", "typedef", "auto", "__attribute__", "u_char", "u_short", "u_int", "u_long"}
TOKEN_RE = re.compile(r"""\s*(?:(0[xX][0-9A-Fa-f]+[uUlL]*|\d+[uUlL]*|[A-Za-z_]\w*|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|\.\.\.|->|<<=|>>=|[-+*/%&|^!<>=]=|&&|\|\||<<|>>|\+\+|--|\S))""")


# ------------------------------------------------------------------ lexing
def strip_comments(s):
    out, i, n = [], 0, len(s)
    while i < n:
        c = s[i]
        if s.startswith("/*", i):
            j = s.find("*/", i + 2)
            i = n if j < 0 else j + 2
            out.append(" ")
        elif s.startswith("//", i):
            j = s.find("\n", i)
            i = n if j < 0 else j
        elif c in "\"'":
            j = i + 1
            while j < n and s[j] != c:
                j += 2 if s[j] == "\\" else 1
            out.append(s[i:j + 1])
            i = j + 1
        else:
            out.append(c)
            i += 1
    return "".join(out)


def tokens(s):
    s = strip_comments(s)
    out, i = [], 0
    while True:
        m = TOKEN_RE.match(s, i)
        if not m:
            break
        out.append(m.group(1))
        i = m.end()
    return out


def join(toks):
    """Readable C from a token list."""
    out = ""
    for t in toks:
        if not out:
            out = t
        elif t in (",", ";", ")", "]", "[") or out[-1] in "([" or (t == "(" and out[-1] not in ",=") or out.endswith("*") and t != "*" and not re.match(r"\w", t):
            out += t
        elif out.endswith("*") and out[-2:-1] in ("*", "(", " ") and re.match(r"\w", t):
            out += t
        else:
            out += " " + t
    return out


def top_items(src):
    """Split a C file into top-level items: (kind, text, is_function_definition) with kind
    'pp' (preprocessor line), 'comment' or 'code'."""
    items, i, n = [], 0, len(src)
    while i < n:
        while i < n and src[i] in " \t\r\n":
            i += 1
        if i >= n:
            break
        if src[i] == "#":
            j = i
            while True:
                k = src.find("\n", j)
                if k < 0:
                    k = n
                    break
                if src[k - 1] == "\\":
                    j = k + 1
                    continue
                break
            items.append(("pp", src[i:k], False))
            i = k
            continue
        if src.startswith("/*", i):
            j = src.find("*/", i + 2)
            j = n if j < 0 else j + 2
            items.append(("comment", src[i:j], False))
            i = j
            continue
        if src.startswith("//", i):
            j = src.find("\n", i)
            j = n if j < 0 else j
            items.append(("comment", src[i:j], False))
            i = j
            continue
        # code up to ';' at depth 0, or the '}' closing a function body
        j, depth, func, last_sig, knr = i, 0, False, "", False
        while j < n:
            c = src[j]
            if src.startswith("/*", j):
                k = src.find("*/", j + 2)
                j = n if k < 0 else k + 2
                continue
            if src.startswith("//", j):
                k = src.find("\n", j)
                j = n if k < 0 else k
                continue
            if c in "\"'":
                k = j + 1
                while k < n and src[k] != c:
                    k += 2 if src[k] == "\\" else 1
                j = k + 1
                last_sig = c
                continue
            if c == "#" and (j == 0 or src[src.rfind("\n", 0, j) + 1:j].strip() == ""):
                k = src.find("\n", j)       # preprocessor line inside an item: keep as text
                j = n if k < 0 else k
                continue
            if c in "([{":
                if c == "{" and depth == 0 and (last_sig == ")" or knr):
                    func = True
                depth += 1
            elif c in ")]}":
                depth -= 1
                if c == "}" and depth == 0 and func:
                    j += 1
                    break
            elif c == ";" and depth == 0:
                if knr or is_knr_head(src[i:j]):
                    knr = True                  # K&R parameter declarations: the body follows
                    j += 1
                    continue
                j += 1
                break
            if not c.isspace():
                last_sig = c
            j += 1
        items.append(("code", src[i:j], func))
        i = j
    return items


def is_knr_head(text):
    """`RET name(a, b) int a;` - the start of a K&R function definition."""
    toks = tokens(text)
    if not toks or toks[0] in ("typedef", "extern") or "=" in toks or "{" in toks:
        return False
    for k, t in enumerate(toks):
        if t == "(" and k and re.match(r"[A-Za-z_]\w*$", toks[k - 1]):
            try:
                e = toks.index(")", k)
            except ValueError:
                return False
            inner = toks[k + 1:e]
            if inner and all(re.match(r"[A-Za-z_]\w*$", x) or x == "," for x in inner[0::1]) \
                    and not set(inner) & KEYWORDS and e + 1 < len(toks):
                return True
            return False
    return False


def split_top(toks, sep=","):
    parts, cur, d = [], [], 0
    for t in toks:
        if t in "([{":
            d += 1
        elif t in ")]}":
            d -= 1
        if t == sep and d == 0:
            parts.append(cur)
            cur = []
        else:
            cur.append(t)
    parts.append(cur)
    return parts


def decl_name(toks):
    """(index, name) of the declarator identifier in a declaration's tokens."""
    d = 0
    for k, t in enumerate(toks):
        if t == "{":
            d += 1
        elif t == "}":
            d -= 1
        if d or not re.match(r"[A-Za-z_]\w*$", t) or t in KEYWORDS:
            continue
        nxt = toks[k + 1] if k + 1 < len(toks) else None
        if nxt == "(" and k + 2 < len(toks) and toks[k + 2] == "*":
            continue                                   # TYPE (*fp)(...): the type
        if nxt in (None, "(", "[", ")", ",", ";", "=", ":"):
            return k, t
    return None, None


def type_prefix(toks):
    k, _ = decl_name(toks)
    if k is None:
        return toks
    j = k
    while j > 0 and toks[j - 1] in ("*", "("):
        j -= 1
    return toks[:j]


# ------------------------------------------------------------------ classification
class Item:
    def __init__(self, kind, text, src, toks=None, names=(), key=None):
        self.kind, self.text, self.src, self.toks = kind, text, src, toks
        self.names, self.key = list(names), key


def classify(text, isfunc, src):
    """Hoistable items: ('typedef'|'tag'|'decl') with names; others: 'func'|'data'."""
    toks = tokens(text)
    if not toks:
        return []
    if toks[-1] == ";":
        toks = toks[:-1]
    if isfunc:
        body = toks.index("{")
        head = toks[:body]
        k, name = decl_name(head)
        it = Item("func", text, src, head, [name])
        it.static = "static" in head[:k]
        it.inline = any(t in ("inline", "__inline__", "__inline") for t in head[:k])
        it.ret = [t for t in head[:k] if t not in ("static", "extern", "inline", "__inline__", "__inline")]
        return [it]
    if toks[0] in ("__asm__", "asm", "__asm"):
        return [Item("data", text, src, toks, [])]           # a hand-written asm function: in place
    if toks[0] == "typedef":
        rest = toks[1:]
        # names: declarators after the type (after the closing brace if there is a body)
        if "{" in rest:
            d, end = 0, None
            for k, t in enumerate(rest):
                if t == "{":
                    d += 1
                elif t == "}":
                    d -= 1
                    if d == 0:
                        end = k
                        break
            decls = split_top(rest[end + 1:])
        else:
            parts = split_top(rest)
            decls = [parts[0][len(type_prefix(parts[0])):]] + parts[1:]
        names = [decl_name(p)[1] for p in decls]
        tag = None
        if rest[0] in ("struct", "union", "enum") and len(rest) > 1 and re.match(r"\w+$", rest[1]) and rest[1] not in KEYWORDS:
            tag = f"{rest[0]} {rest[1]}"
        return [Item("typedef", text, src, toks, [n for n in names if n] + ([tag] if tag and "{" in rest else []))]
    if toks[0] in ("struct", "union", "enum") and "{" in toks and toks[-1] == "}":
        return [Item("tag", text, src, toks, [f"{toks[0]} {toks[1]}"] if re.match(r"\w+$", toks[1]) else [])]
    storage = [t for t in toks if t in ("extern", "static")]
    is_static = "static" in storage
    parts = split_top(toks)
    pre = type_prefix(parts[0])
    out = []
    for k, p in enumerate(parts):
        full = p if k == 0 else pre + p
        _, name = decl_name(full)
        ki, _ = decl_name(full)
        isproto = ki is not None and ki + 1 < len(full) and full[ki + 1] == "("
        if "=" in full or (not isproto and "extern" not in full):
            # a definition (initialised, or a tentative definition): stays in place
            return [Item("data", text, src, toks, [decl_name(split_top(toks)[0])[1]] +
                         [decl_name(pre + q)[1] for q in split_top(toks)[1:]])]
        norm = [t for t in full if t != "extern"]
        it = Item("decl", None, src, norm, [name])
        it.static, it.proto = is_static, isproto
        out.append(it)
    return out


PROMOTABLE = {"char", "short", "float", "u_char", "u_short"}


def params(head):
    """Parameter token lists of a function definition head (None if unprototyped/K&R)."""
    k, _ = decl_name(head)
    if k is None or k + 1 >= len(head) or head[k + 1] != "(":
        return None
    d, j = 0, k + 1
    for j in range(k + 1, len(head)):
        d += head[j] == "("
        d -= head[j] == ")"
        if d == 0:
            break
    inner = head[k + 2:j]
    if not inner or inner == ["void"]:
        return []
    return split_top(inner)


def promotable(head):
    """True if a prototyped parameter has a type that default promotions change (char,
    short, float): an unprototyped forward declaration would then conflict."""
    ps = params(head)
    if not ps:
        return False
    for p in ps:
        if "*" in p or "[" in p or ("(" in p):
            continue
        if PROMOTABLE & set(p):
            return True
    return False


def decl_text(it):
    if it.kind == "decl":
        return join(it.toks if it.static else ["extern"] + it.toks) + ";"
    return it.text.strip()


def shape(toks):
    """A declaration with array bounds and parameter lists removed (conflict hint)."""
    out, d = [], 0
    for t in toks:
        if t in "([":
            if d == 0:
                out.append(t)
            d += 1
            continue
        if t in ")]":
            d -= 1
            if d == 0:
                out.append(t)
            continue
        if d == 0:
            out.append(t)
    return out


INCDIRS = ["include", "include/shim", "include/psyq", "src"]       # tools/fnscore.py's -I order


def header_types(includes):
    """({typedef/tag name: header}, {declared name: (decl Item, header)}) over the #include
    closure of the TU's includes."""
    out, decls, seen = {}, {}, set()
    todo = [re.search(r'[<"]([^>"]+)[>"]', i).group(1) for i in includes if re.search(r'[<"]([^>"]+)[>"]', i)]
    while todo:
        h = todo.pop()
        p = next((os.path.join(ROOT, d, h) for d in INCDIRS if os.path.exists(os.path.join(ROOT, d, h))), None)
        if p is None or p in seen:
            continue
        seen.add(p)
        txt = open(p, errors="replace").read()
        # C++ linkage wrappers (`extern "C" {` ... `}` under #if __cplusplus) are not C
        txt = re.sub(r'^\s*extern\s+"C"\s*\{\s*$|^\s*\}\s*$', "", txt, flags=re.M)
        todo += re.findall(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', txt, re.M)
        for kind, text, isfunc in top_items(txt):
            if kind != "code" or isfunc:
                continue
            try:
                its = classify(text, False, h)
            except Exception:
                continue
            for it in its:
                if it.kind in ("typedef", "tag"):
                    for n in it.names:
                        if n:
                            out.setdefault(n, os.path.relpath(p, ROOT))
                elif it.kind == "decl" and it.names[0]:
                    decls.setdefault(it.names[0], (it, os.path.relpath(p, ROOT)))
    return out, decls


def decl_clash(a, b):
    """Why two declarations of one symbol cannot both be in scope (None if they can):
    a different type before the name, a different parameter count, or an unprototyped one
    against a prototype with char/short/float parameters."""
    ta, tb = [t for t in type_prefix(a.toks) if t not in ("extern", "static")], \
             [t for t in type_prefix(b.toks) if t not in ("extern", "static")]
    if ta != tb:
        return f"type {join(ta)} vs {join(tb)}"
    pa, pb = params(a.toks), params(b.toks)
    ua, ub = a.proto and not a.toks[a.toks.index("(") + 1:a.toks.index("(") + 2] == [")"], \
             b.proto and not b.toks[b.toks.index("(") + 1:b.toks.index("(") + 2] == [")"]
    if pa is not None and pb is not None and ua and ub and len(pa) != len(pb):
        return f"{len(pa)} vs {len(pb)} parameters"
    if ua != ub and promotable(a.toks if ua else b.toks):
        return "unprototyped vs a prototype with promoted parameter types"
    return None


def call_arity(text, name):
    """[number of arguments] of each call to `name` in the C text."""
    toks = tokens(text)
    out = []
    for k, t in enumerate(toks):
        if t != name or k + 1 >= len(toks) or toks[k + 1] != "(":
            continue
        if k and (re.match(r"[A-Za-z_]\w*$", toks[k - 1]) and toks[k - 1] not in ("return", "else", "do")
                  or toks[k - 1] in ("*",) and k > 1 and re.match(r"\w", toks[k - 2]) and toks[k - 2] in KEYWORDS):
            continue                                   # a declaration or the definition, not a call
        d, n, j = 0, 0, k + 1
        empty = toks[k + 2] == ")" if k + 2 < len(toks) else True
        for j in range(k + 1, len(toks)):
            if toks[j] in "([{":
                d += 1
            elif toks[j] in ")]}":
                d -= 1
                if d == 0:
                    break
            elif toks[j] == "," and d == 1:
                n += 1
        out.append(0 if empty else n + 1)
    return out


# ------------------------------------------------------------------ main
def asm_blocks(path):
    """[(name, address)] of the nonmatching blocks of a .text asm file, in order."""
    out, cur = [], None
    for l in open(os.path.join(ROOT, path)):
        if l.startswith("nonmatching "):
            cur = l.split()[1].rstrip(",")
            out.append([cur, None])
        elif cur and out[-1][1] is None:
            m = re.search(r"/\*\s+[0-9A-F]+\s+([0-9A-F]{8})\b", l)
            if m:
                out[-1][1] = int(m.group(1), 16)
    return [(n, a) for n, a in out]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("first")
    ap.add_argument("last")
    ap.add_argument("--target", choices=targets.NAMES, default="main")
    ap.add_argument("--wip", action="store_true", help="accept <name>.wip.c when there is no <name>.c")
    ap.add_argument("--report", help="also write the report to this file")
    a = ap.parse_args()
    t = targets.get(a.target)
    srcdir = targets.path(t, "src")
    blocks = asm_blocks(t["text_asm"])

    def find(x):
        if x.lower().startswith("0x"):
            v = int(x, 16)
            k = next((i for i, (_, ad) in enumerate(blocks) if ad == v), None)
        else:
            k = next((i for i, (n, _) in enumerate(blocks) if n == x), None)
        if k is None:
            sys.exit(f"mktu: no asm block {x} in {t['text_asm']}")
        return k
    lo, hi = find(a.first), find(a.last)
    if hi < lo:
        sys.exit("mktu: LAST is before FIRST")
    rng = blocks[lo:hi + 1]
    end = blocks[hi + 1][1] if hi + 1 < len(blocks) else None
    files, missing = [], []
    for n, _ in rng:
        p = os.path.join(srcdir, n + ".c")
        if not os.path.exists(p) and a.wip and os.path.exists(os.path.join(srcdir, n + ".wip.c")):
            p = os.path.join(srcdir, n + ".wip.c")
        (files if os.path.exists(p) else missing).append((n, p))
    if missing:
        sys.exit("mktu: no matched C for " + ", ".join(n for n, _ in missing))

    includes, hoisted, sections = [], [], []          # hoisted: Items in first-seen order
    seen_inc = set()
    defined = {}                                       # TU function name -> func Item
    notes, kept_local, dropped, conflicts = [], [], 0, []
    for n, p in files:
        src = open(p).read()
        rel = os.path.relpath(p, ROOT)
        macros = set(re.findall(r"^\s*#\s*define\s+(\w+)", src, re.M))
        sec = []
        for kind, text, isfunc in top_items(src):
            if kind == "pp":
                if re.match(r"\s*#\s*include\b", text):
                    k = " ".join(text.split())
                    if k not in seen_inc:
                        seen_inc.add(k)
                        includes.append(text.strip())
                    continue
                sec.append(text)
                continue
            if kind == "comment":
                sec.append(text)
                continue
            its = classify(text, isfunc, rel)
            if not its:
                continue
            if its[0].kind == "data" and its[0].toks and its[0].toks[0] in ("__asm__", "asm", "__asm"):
                conflicts.append(f"asm {n}: {rel} is a hand-written asm function (top-level __asm__); "
                                 "TU-PLAN keeps those as their own .s objects")
            if its[0].kind in ("typedef", "tag", "decl"):
                if macros & set(its[0].toks or tokens(text)):
                    kept_local.append(f"{', '.join(x for i in its for x in i.names if x)} in {rel} (uses a #define of that file)")
                    sec.append(text)
                    for i in its:
                        i.local = True
                    hoisted += its
                    continue
                hoisted += its
                continue
            it = its[0]
            if it.kind == "func" and it.names[0] == n:
                defined[n] = it
            it.text = text
            sec.append(it)
        sections.append((n, rel, sec, macros))

    # --- dedupe hoisted items per name
    by = {}                                            # key -> [Item variants]
    order = []
    keyof = {}                                         # each typedef/tag name -> its group key
    for it in hoisted:
        if it.kind == "decl":
            key = ("decl", it.names[0])
        else:
            nm = [x for x in it.names if x]
            key = next((keyof[x] for x in nm if x in keyof), None) or ("typedef", tuple(nm))
            for x in nm:
                keyof.setdefault(x, key)
        if it.kind == "decl" and it.names[0] in defined:
            own = defined[it.names[0]]
            plain = ["static"] * own.static + own.ret + [it.names[0], "(", ")"]
            if it.toks != [x for x in plain if x != "static"]:
                notes.append(f"{it.src}: `{decl_text(it)}` replaced by the forward declaration of {it.names[0]}")
            continue
        txt = tuple(it.toks if it.kind == "decl" else tokens(it.text))
        if key not in by:
            by[key] = []
            order.append(key)
        v = next((x for x in by[key] if x[0] == txt), None)
        if v:
            v[1].append(it)
            dropped += 0 if getattr(it, "local", False) else 1
        else:
            by[key].append((txt, [it]))
    out_typedefs, out_decls = [], []
    for key in order:
        vs = by[key]
        dst = out_typedefs if key[0] == "typedef" else out_decls
        act = [(t_, [i for i in its if not getattr(i, "local", False)]) for t_, its in vs]
        act = [(t_, its) for t_, its in act if its]
        if act:
            dst.append(decl_text(act[0][1][0]))
        for t_, its in act[1:]:
            dst.append(f"/* MKTU CONFLICT ({', '.join(os.path.basename(i.src)[:-2] for i in its)}): "
                       f"{decl_text(its[0]).replace('*/', '* /')} */")
        if len(vs) > 1:
            name = key[1] if key[0] == "decl" else ", ".join(key[1])
            hint = ""
            if key[0] == "decl":
                shapes = {tuple(shape(list(t))) for t, _ in vs}
                hint = " [array bound / parameter list only]" if len(shapes) == 1 else ""
            lines = []
            for txt, its in vs:
                loc = " (kept local)" if all(getattr(i, "local", False) for i in its) else ""
                lines.append(f"    {decl_text(its[0])}    <- {', '.join(os.path.basename(i.src)[:-2] for i in its)}{loc}")
            conflicts.append(f"{key[0]} {name}{hint}:\n" + "\n".join(lines))

    # --- typedefs/tags that an included header defines as well
    if includes:
        ht, hd = header_types(includes)
        for key in order:
            if key[0] != "decl" or key[1] not in hd:
                continue
            h, hp = hd[key[1]]
            for _, its in by[key]:
                why = decl_clash(its[0], h) if its[0].proto == h.proto else (
                    f"type {join(type_prefix(its[0].toks))} vs {join(type_prefix(h.toks))}"
                    if [t for t in type_prefix(its[0].toks) if t not in ("extern", "static")] !=
                       [t for t in type_prefix(h.toks) if t not in ("extern", "static")] else None)
                if why:
                    conflicts.append(f"decl {key[1]} clashes with {hp} ({why}):\n    {decl_text(its[0])}    <- "
                                     f"{', '.join(os.path.basename(i.src)[:-2] for i in its)}\n    {decl_text(h)}    <- {hp}")
        for key in order:
            if key[0] != "typedef":
                continue
            clash = [n for n in key[1] if n in ht]
            if clash:
                vs = by[key]
                conflicts.append(f"typedef {', '.join(clash)} is also defined by {ht[clash[0]]} (included by the TU):\n" +
                                 "\n".join(f"    {decl_text(its[0])}    <- {', '.join(os.path.basename(i.src)[:-2] for i in its)}"
                                           for _, its in vs))

    # --- calls whose argument count differs from the TU's own definition
    for n, _ in files:
        f = defined.get(n)
        ps = params(f.toks) if f else None
        if ps is None or any("..." in p for p in ps):
            continue
        for m, rel, sec, _ in sections:
            body = "\n".join(x if isinstance(x, str) else x.text for x in sec)
            bad = sorted({c for c in call_arity(body, n) if c != len(ps)})
            if bad:
                where = "after" if [x for x, _ in files].index(m) > [x for x, _ in files].index(n) else "before"
                conflicts.append(f"call {n}: {m} passes {'/'.join(map(str, bad))} argument(s), the definition "
                                 f"takes {len(ps)} (call {where} the definition" +
                                 (": a compile error)" if where == "after" or promotable(f.toks) else ")"))

    # --- forward declarations of the TU's own functions (address order)
    fwd = []
    for n, _ in files:
        f = defined.get(n)
        if f is None:
            if not any(c.startswith(f"asm {n}:") for c in conflicts):
                notes.append(f"{n}: no definition of {n} found in its pilot file")
            continue
        if promotable(f.toks):
            fwd.append(join([t_ for t_ in f.toks if t_ not in ("inline", "__inline__", "__inline")]) + ";")
        else:
            fwd.append(join((["static"] if f.static else []) + f.ret + [n, "(", ")"]) + ";")

    # --- helper functions / data defined in several files
    seen_def = {}
    body_out = []
    for n, rel, sec, macros in sections:
        chunk = [f"/* ---------------- {n} ({rel}) ---------------- */"]
        for x in sec:
            if isinstance(x, str):
                chunk.append(x.rstrip())
                continue
            if x.kind == "func" and x.names[0] == n:
                chunk.append(x.text.strip())
                continue
            k = tuple(nm for nm in x.names if nm)
            txt = tuple(tokens(x.text))
            prev = [seen_def[nm] for nm in k if nm in seen_def]
            if prev and all(p[0] == txt for p in prev):
                chunk.append(f"/* mktu: {', '.join(k)} - identical to the definition in {prev[0][1]} */")
                dropped += 1
                continue
            if prev:
                conflicts.append(f"definition {', '.join(k)}: {prev[0][1]} and {rel} differ (both left in)")
            for nm in k:
                seen_def.setdefault(nm, (txt, rel))
            chunk.append(x.text.strip())
        if macros:
            chunk.append("\n".join(f"#undef {m}" for m in sorted(macros)))
        body_out.append("\n".join(chunk))

    first_a, last_n = rng[0][1], rng[-1][0]
    head = (f"{os.path.basename(a.out)}: {len(files)} functions {rng[0][0]}..{last_n} "
            f"(0x{first_a:X}-0x{end:X})" if end else f"{len(files)} functions")
    report = [f"mktu: {head}",
              f"prelude: {len(includes)} includes, {len(out_typedefs)} typedef/tag lines, {len(out_decls)} declarations, "
              f"{len(fwd)} forward declarations; {dropped} duplicates dropped",
              f"conflicts: {len(conflicts)}"] + ["  " + c.replace("\n", "\n  ") for c in conflicts]
    if kept_local:
        report += [f"kept local: {len(kept_local)}"] + ["  " + k for k in kept_local]
    if notes:
        report += [f"notes: {len(notes)}"] + ["  " + k for k in notes]
    text = ["/* " + head, " * generated by tools/mktu.py from src/pilot; " +
            (f"{len(conflicts)} CONFLICTS listed below, unresolved" if conflicts else "no conflicts"), " */"]
    if conflicts:
        text += ["/* MKTU CONFLICTS (first spelling active, others commented at their place below):"]
        text += [" * " + l for c in conflicts for l in c.split("\n")] + [" */"]
    text += [""] + includes + ([""] if includes else [])
    text += ["/* ---- types ---- */"] + out_typedefs + ["", "/* ---- externs ---- */"] + out_decls
    text += ["", "/* ---- functions defined in this file ---- */"] + fwd + [""]
    text += ["\n\n".join(body_out), ""]
    os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
    open(a.out, "w").write("\n".join(text))
    rep = "\n".join(report)
    print(rep)
    if a.report:
        open(a.report, "w").write(rep + "\n")


if __name__ == "__main__":
    main()
