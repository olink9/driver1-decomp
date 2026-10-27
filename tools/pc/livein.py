#!/usr/bin/env python3
"""Live-in argument registers (a0-a3) of every asm function (CFG + delay slots + callee fixpoint),
compared with the parameter count of the function's C definition in src/ and port/pc/."""
import re, glob, os, sys
os.chdir(os.path.expanduser('~/dev/driver1'))
ARGS = ['$a0', '$a1', '$a2', '$a3']
CLOB = set(ARGS + ['$v0', '$v1', '$at'] + ['$t%d' % i for i in range(10)])
STORES = {'sb', 'sh', 'sw', 'swl', 'swr', 'swc2', 'sdc2'}
LOADS = {'lb', 'lbu', 'lh', 'lhu', 'lw', 'lwl', 'lwr', 'lwc2'}
BR2 = {'beq', 'bne', 'beql', 'bnel'}
BR1 = {'beqz', 'bnez', 'blez', 'bgtz', 'bltz', 'bgez', 'blezl', 'bgtzl', 'bltzl', 'bgezl', 'bnezl', 'beqzl'}
USEALL = {'mult', 'multu', 'div', 'divu', 'mthi', 'mtlo', 'mtc2', 'ctc2', 'mtc0', 'teq', 'tne'}
funcs = {}
for f in glob.glob('asm_fn/nonmatchings/**/*.s', recursive=True):
    name = None; ins = []; labels = {}
    for line in open(f):
        m = re.match(r'\s*glabel\s+(\w+)', line)
        if m and name is None: name = m.group(1); labels[name] = 0; continue
        m = re.match(r'\s*(\.?L?\w+):\s*$', line)
        if m: labels[m.group(1)] = len(ins); continue
        m = re.match(r'\s*/\*.*?\*/\s+(\S+)\s*(.*)', line)
        if m:
            op = m.group(1); ops = [o.strip() for o in m.group(2).split(',')] if m.group(2).strip() else []
            ins.append((op, ops))
    if name: funcs[name] = (ins, labels, f)
def regs(s): return re.findall(r'\$\w+', s)
def analyse(name, livein):
    ins, labels, _ = funcs[name]; n = len(ins)
    nodes = []  # (uses, defs, succs)
    for i, (op, ops) in enumerate(ins):
        uses, defs = set(), set(); succ = [i + 1]
        allr = [regs(o) for o in ops]
        flat = [r for rr in allr for r in rr]
        if op in STORES: uses |= set(flat)
        elif op in LOADS: defs |= set(allr[0]) if op != 'lwc2' else set(); uses |= set(r for rr in allr[1:] for r in rr)
        elif op in BR2 or op in BR1 or op in ('b', 'j'):
            uses |= set(flat) - {o for o in ops if not o.startswith('$')}
        elif op in ('jal', 'bal', 'jalr'):
            if op == 'jalr': uses |= set(flat)
        elif op == 'jr': uses |= set(flat)
        elif op in USEALL: uses |= set(flat)
        elif op in ('mfc2', 'cfc2', 'mfc0', 'mfhi', 'mflo', 'lui', 'li', 'la'): defs |= set(flat[:1])
        elif op in ('nop', 'syscall', 'break') or op.startswith('.') or not flat: pass
        else: defs |= set(flat[:1]); uses |= set(flat[1:])
        nodes.append([uses, defs, succ, op, ops])
    # control flow with delay slots
    extra = {}  # delay-slot index -> (call uses, call defs)
    for i, (op, ops) in enumerate(ins):
        tgt = ops[-1] if ops else None
        if op in BR2 | BR1 or op in ('b', 'j', 'bal'):
            if i + 1 < n:
                t = labels.get(tgt)
                s = []
                if t is not None: s.append(t)
                elif op == 'j' and tgt in funcs: extra[i + 1] = (livein.get(tgt, set()), set()); s = []  # tail call
                if op not in ('b', 'j'): s.append(i + 2)
                nodes[i + 1][2] = s
        elif op in ('jal', 'jalr'):
            if i + 1 < n:
                cu = livein.get(tgt, set()) if op == 'jal' else set()
                extra[i + 1] = (cu, CLOB)
        elif op == 'jr':
            if i + 1 < n:
                if ops and ops[0] == '$ra': nodes[i + 1][2] = []
                else: nodes[i + 1][2] = list(labels.values())  # jump table: any label
    live = [set() for _ in range(n + 1)]
    changed = True
    while changed:
        changed = False
        for i in range(n - 1, -1, -1):
            uses, defs, succ = nodes[i][0], nodes[i][1], nodes[i][2]
            out = set()
            for s in succ:
                if s <= n: out |= live[s] if s < n else set()
            if i in extra:
                cu, cd = extra[i]
                out = (out - cd) | cu
            inn = (out - defs) | uses
            if inn != live[i]: live[i] = inn; changed = True
    return live[0] & set(ARGS) if n else set()
livein = {}
VARIADIC = {"sprintf": {"$a0", "$a1"}, "printf": {"$a0"}, "FntPrint": {"$a0"}}
for it in range(20):
    ch = False
    for nm in funcs:
        r = VARIADIC.get(nm) or analyse(nm, livein)
        if r != livein.get(nm): livein[nm] = r; ch = True
    if not ch: break
# C definitions
files = [f for f in glob.glob('src/**/*.c', recursive=True) if '/pilot/' not in f and not f.endswith('.wip.c')] + glob.glob('port/pc/*.c')
cdef = {}
for f in files:
    s = open(f, errors='replace').read()
    s = re.sub(r'/\*.*?\*/', '', s, flags=re.S)
    for m in re.finditer(r'^[A-Za-z_][\w\s\*]*?\b(\w+)\s*\(([^;{)]*)\)\s*\{', s, re.M):
        nm, a = m.group(1), m.group(2).strip()
        if '...' in a: n = 99
        else: n = 0 if a in ('', 'void') else a.count(',') + 1
        cdef.setdefault(nm, []).append((n, f))
    for m in re.finditer(r'^[A-Za-z_][\w\s\*]*?\b(\w+)\s*\(([\w\s,]*)\)\s*\n(?=[^{]*\{)', s, re.M):
        nm, a = m.group(1), m.group(2).strip()
        if a: cdef.setdefault(nm, []).append((a.count(',') + 1, f))
for nm in sorted(funcs):
    li = livein[nm]
    if not li: continue
    need = max(ARGS.index(r) for r in li) + 1
    for n, f in cdef.get(nm, []):
        if n < need:
            print(f'{nm:16s} asm reads {",".join(sorted(li))}  C def takes {n}  {f}')
if '-a' in sys.argv:
    for nm in sorted(funcs): print(nm, sorted(livein[nm]))
if '-d' in sys.argv:
    print('--- direct (own instructions, callees ignored; jr-table funcs marked *):')
    for nm in sorted(funcs):
        d = analyse(nm, {})
        jt = any(op == 'jr' and ops and ops[0] != '$ra' for op, ops in funcs[nm][0])
        li = livein[nm]
        if not li: continue
        need = max(ARGS.index(r) for r in li) + 1
        for n, f in cdef.get(nm, []):
            if n < need: print(f'{nm:16s} direct {",".join(sorted(d))} via-callee {",".join(sorted(li - d))} {"*" if jt else ""} C {n} {f}'); break
