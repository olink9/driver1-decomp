#!/usr/bin/env python3
"""mips2c.py - mechanical MIPS (R3000 + GTE) -> C translator for Driver 1's hand-written asm modules.

Usage: mips2c.py in.s module_name [--entries func:off,...] > out.c

Model: one global register file R (asm_pc.h). Each asm function F becomes `void F_<addr>(int entry)` running on R;
`jr $ra` = return, `jal f` = call, j/branch into another function = tail call, fall-through past endlabel = tail
call of the next function. Delay slots and the R3000 load delay are honoured (load-use hazards are reported).
lui %hi(sym) yields the full address of sym (+offset) and %lo(...) yields 0; `lui 0x1F80` yields the PC
scratchpad base.
"""
import re, sys

REGN = ['zero','at','v0','v1','a0','a1','a2','a3','t0','t1','t2','t3','t4','t5','t6','t7',
        's0','s1','s2','s3','s4','s5','s6','s7','t8','t9','k0','k1','gp','sp','fp','ra']
ALIAS = {'s8': 'fp'}

def reg(tok):
    tok = tok.strip()
    assert tok.startswith('$'), tok
    t = tok[1:]
    if t.isdigit():
        return int(t)
    t = ALIAS.get(t, t)
    return REGN.index(t)

def R(n):
    return '0u' if n == 0 else 'R.' + REGN[n]

# known raw words
WORDS = {
    0x4A4DE012: ('cop2', 0x04DE012),
    0x4A4BE012: ('cop2', 0x04BE012),
    0x4A280030: ('cop2', 0x0280030),
    0x4B400006: ('cop2', 0x1400006),
    0x4B58002D: ('cop2', 0x158002D),
    0x4AA00428: ('cop2', 0x0A00428),
    0x0148001A: ('div', (10, 8)),
    0x0001000D: ('break', 1),
}
GTEOPS = {
    'rtps': 0x0180001, 'rtpt': 0x0280030, 'nclip': 0x1400006, 'avsz3': 0x158002D, 'avsz4': 0x168002E,
}

def cop2op(fake, op, gbg=0, sf=1, mx=0, v=0, cv=0, lm=0):
    return (fake << 20) | (gbg << 20) | (sf << 19) | (mx << 17) | (v << 15) | (cv << 13) | (lm << 10) | op

class Insn:
    def __init__(s, addr, op, args, raw):
        s.addr, s.op, s.args, s.raw = addr, op, args, raw
        s.labels = []

def strip_comments(text):
    text = re.sub(r'/\*.*?\*/', lambda m: '\n' * m.group(0).count('\n'), text, flags=re.S)
    return text

def split_args(a):
    out, depth, cur = [], 0, ''
    for ch in a:
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip()); cur = ''
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out

def parse(path):
    text = strip_comments(open(path).read())
    funcs = []  # list of dict(name, addr, insns)
    cur = None
    pend_labels = []
    pend_reloc = None
    for line in text.split('\n'):
        line = line.split('#')[0].rstrip()
        s = line.strip()
        if not s:
            continue
        m = re.match(r'glabel\s+(\w+)', s)
        if m:
            name = m.group(1)
            addr = int(name.split('_')[1], 16)
            cur = {'name': name, 'addr': addr, 'insns': [], 'pc': addr, 'labels': {}}
            funcs.append(cur)
            continue
        if s.startswith('endlabel'):
            assert not pend_labels, pend_labels
            cur = None
            continue
        if cur is None:
            continue
        # labels (possibly several on a line)
        while True:
            m = re.match(r'^(\.?\w+):\s*(.*)$', s)
            if not m:
                break
            pend_labels.append(m.group(1))
            cur['labels'][m.group(1)] = cur['pc']
            s = m.group(2).strip()
        if not s:
            continue
        if s.startswith('.set') or s.startswith('.purgem'):
            continue
        m = re.match(r'\.reloc\s+\.\s*,\s*R_MIPS_26\s*,\s*(\w+)', s)
        if m:
            pend_reloc = m.group(1)
            continue
        if s.startswith('.fill'):
            n = int(split_args(s[5:])[0], 0)
            cur['pc'] += 4 * n
            ins = Insn(cur['pc'], '.data', [], s)
            continue
        if s.startswith('.word'):
            w = int(s.split()[1], 0)
            if pend_reloc:
                assert (w >> 26) == 3
                ins = Insn(cur['pc'], 'jal', [pend_reloc], s)
                pend_reloc = None
            elif w == 0:
                ins = Insn(cur['pc'], '.data', [], s)
            elif w == 0x04010003:
                ins = Insn(cur['pc'], 'b', ['@%x' % (cur['pc'] + 4 + 12)], s)
            else:
                k, v = WORDS[w]
                if k == 'cop2':
                    ins = Insn(cur['pc'], 'cop2', [v], s)
                elif k == 'div':
                    ins = Insn(cur['pc'], 'div', ['$%d' % v[0], '$%d' % v[1]], s)
                elif k == 'break':
                    ins = Insn(cur['pc'], 'break', [v], s)
        else:
            parts = s.split(None, 1)
            op = parts[0]
            args = split_args(parts[1]) if len(parts) > 1 else []
            ins = Insn(cur['pc'], op, args, s)
        ins.labels = pend_labels
        pend_labels = []
        cur['insns'].append(ins)
        cur['pc'] += 4
    return funcs

def evalc(expr):
    """evaluate a numeric constant expression"""
    e = expr.strip()
    assert re.fullmatch(r'[0-9a-fA-FxX\s\+\-\*\(\)<>&|~]+', e), expr
    return eval(e)

SYMS = set()

def hi_expr(expr):
    """value for lui: returns C expression"""
    e = expr.strip()
    m = re.fullmatch(r'%hi\(\s*(\w+)\s*(?:\+\s*(\w+))?\s*\)', e)
    if m:
        sym, off = m.group(1), m.group(2)
        SYMS.add(sym)
        c = '(uint32_t)(uintptr_t)&%s' % sym
        if off:
            c = '(%s + %s)' % (c, off)
        return c
    v = evalc(e) & 0xFFFF
    if v == 0x1F80:
        return 'PC_SCRATCHPAD_BASE'
    return '0x%Xu' % ((v << 16) & 0xFFFFFFFF)

def imm(expr, signed=True):
    e = expr.strip()
    if e.startswith('%lo('):
        return 0
    v = evalc(e)
    if signed:
        v &= 0xFFFF
        if v >= 0x8000:
            v -= 0x10000
    else:
        v &= 0xFFFF
    return v

def memarg(a):
    m = re.fullmatch(r'(.*)\((\$\w+)\)', a.strip())
    off = m.group(1).strip() or '0'
    base = reg(m.group(2))
    o = imm(off)
    if base == 0:
        raise Exception('absolute address ' + a)
    if o == 0x100:
        # texture_cluts read as texture_pages + 0x100 (adjacent on PS1); layout-independent on PC
        return 'TEXCLUT(%s)' % R(base)
    if o == 0:
        return R(base)
    return '%s %s %s' % (R(base), '+' if o >= 0 else '-', ('0x%X' % abs(o)) if abs(o) >= 10 else str(abs(o)))

def cimm(v):
    return ('%d' % v) if -0x10000 < v < 0x10000 and abs(v) < 256 else ('0x%X' % v if v >= 0 else '-0x%X' % -v)

LOADS = {'lw', 'lh', 'lhu', 'lb', 'lbu', 'mfc2', 'cfc2'}

def dest_src(ins):
    """(dest regs, src regs) for hazard analysis"""
    op, a = ins.op, ins.args
    try:
        if op in ('lw', 'lh', 'lhu', 'lb', 'lbu'):
            return {reg(a[0])}, {reg(re.search(r'\((\$\w+)\)', a[1]).group(1))}
        if op in ('sw', 'sh', 'sb'):
            return set(), {reg(a[0]), reg(re.search(r'\((\$\w+)\)', a[1]).group(1))}
        if op in ('lwc2', 'swc2'):
            return set(), {reg(re.search(r'\((\$\w+)\)', a[1]).group(1))}
        if op in ('mfc2', 'cfc2'):
            return {reg(a[0])}, set()
        if op in ('mtc2', 'ctc2'):
            return set(), {reg(a[0])}
        if op in ('addu', 'add', 'subu', 'sub', 'and', 'or', 'xor', 'nor', 'slt', 'sltu', 'sllv', 'srlv', 'srav'):
            return {reg(a[0])}, {reg(a[1]), reg(a[2])}
        if op in ('addiu', 'addi', 'andi', 'ori', 'xori', 'slti', 'sltiu', 'sll', 'srl', 'sra'):
            return {reg(a[0])}, {reg(a[1])}
        if op in ('neg', 'not', 'move'):
            return {reg(a[0])}, {reg(a[1])}
        if op == 'lui':
            return {reg(a[0])}, set()
        if op in ('beq', 'bne'):
            return set(), {reg(a[0]), reg(a[1])}
        if op in ('beqz', 'bnez', 'bgez', 'bgtz', 'blez', 'bltz', 'bgezal', 'jr', 'jalr'):
            return set(), {reg(a[0])}
        if op == 'div':
            return set(), {reg(a[0]), reg(a[1])}
        if op in ('mflo', 'mfhi'):
            return {reg(a[0])}, set()
    except Exception:
        pass
    return set(), set()

class Gen:
    def __init__(s, funcs, entries, module):
        s.funcs = funcs
        s.module = module
        s.byaddr = {f['addr']: f for f in funcs}
        s.label_addr = {}
        for f in funcs:
            for l, a in f['labels'].items():
                if l.startswith('.L'):
                    s.label_addr[l] = a
            s.label_addr[f['name']] = f['addr']
        s.entries = {f['addr']: set() for f in funcs}
        for (fn, off) in entries:
            s.entries[s.byaddr[int(fn.split('_')[1], 16)]['addr']].add(off)
        s.warn = []
        s.extern_funcs = set()
        s.need_dispatch = False

    def func_of(s, addr):
        best = None
        for f in s.funcs:
            if f['addr'] <= addr < f['pc'] or (f['addr'] <= addr and addr == f['pc']):
                best = f
        return best

    def target(s, tok, f, ins):
        tok = tok.strip()
        if tok.startswith('@'):
            return int(tok[1:], 16)
        m = re.fullmatch(r'(\d+)([fb])', tok)
        if m:
            n, d = m.group(1), m.group(2)
            # find nearest numeric label
            cands = [(i2.addr, i2) for i2 in f['insns'] if n in i2.labels]
            if d == 'f':
                c = [a for a, _ in cands if a > ins.addr]
                return min(c)
            c = [a for a, _ in cands if a <= ins.addr]
            return max(c)
        if tok in s.label_addr:
            return s.label_addr[tok]
        if tok.startswith('func_'):
            s.extern_funcs.add(tok)
            return ('ext', tok)
        raise Exception('unknown target ' + tok)

    def lab(s, addr):
        return 'L_%05X' % addr

    def jump(s, f, addr, kind):
        """C code transferring control to addr. kind: 'goto' or 'call'"""
        if isinstance(addr, tuple):
            name = addr[1]
            return ('%s();' % name) if kind == 'call' else ('%s(); return;' % name)
        g = s.func_of(addr)
        assert g is not None, hex(addr)
        if kind == 'call':
            assert addr == g['addr'] or addr - g['addr'] in s.entries[g['addr']], (hex(addr), g['name'])
            off = addr - g['addr']
            if off and off not in s.entries[g['addr']]:
                s.entries[g['addr']].add(off)
            return 'F_%05X(%s);' % (g['addr'], '0x%X' % off if off else '0')
        if g is f and not (addr == f['addr']):
            return 'goto %s;' % s.lab(addr)
        if g is f and addr == f['addr']:
            return 'goto %s;' % s.lab(addr)
        off = addr - g['addr']
        if off:
            s.entries[g['addr']].add(off)
        return 'F_%05X(%s); return;' % (g['addr'], '0x%X' % off if off else '0')

    def simple(s, ins, f):
        """C statement for a non-control instruction"""
        op, a = ins.op, ins.args
        if op == 'nop' or op == '.data':
            return ''
        d = None
        def setd(n, expr):
            if n == 0:
                return '/* write to $zero dropped: %s */' % ins.raw
            return '%s = %s;' % (R(n), expr)
        if op in ('addu', 'add', 'or') and reg(a[1]) == 0 and reg(a[2]) == 0:
            return setd(reg(a[0]), '0')
        if op in ('addu', 'add', 'or') and reg(a[2]) == 0:
            return setd(reg(a[0]), R(reg(a[1])))
        if op in ('addu', 'add', 'or') and reg(a[1]) == 0:
            return setd(reg(a[0]), R(reg(a[2])))
        if op in ('addu', 'add'):
            return setd(reg(a[0]), '%s + %s' % (R(reg(a[1])), R(reg(a[2]))))
        if op in ('subu', 'sub'):
            return setd(reg(a[0]), '%s - %s' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'and':
            return setd(reg(a[0]), '%s & %s' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'or':
            return setd(reg(a[0]), '%s | %s' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'xor':
            return setd(reg(a[0]), '%s ^ %s' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'nor':
            return setd(reg(a[0]), '~(%s | %s)' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'not':
            return setd(reg(a[0]), '~%s' % R(reg(a[1])))
        if op == 'neg':
            return setd(reg(a[0]), '0u - %s' % R(reg(a[1])))
        if op == 'move':
            return setd(reg(a[0]), R(reg(a[1])))
        if op in ('addiu', 'addi'):
            v = imm(a[2])
            src = reg(a[1])
            if src == 0:
                return setd(reg(a[0]), '(uint32_t)%s' % cimm(v))
            if v == 0:
                if src == reg(a[0]):
                    return ''
                return setd(reg(a[0]), R(src))
            return setd(reg(a[0]), '%s %s %s' % (R(src), '+' if v >= 0 else '-', cimm(abs(v))))
        if op == 'andi':
            return setd(reg(a[0]), '%s & 0x%X' % (R(reg(a[1])), imm(a[2], False)))
        if op == 'ori':
            src = reg(a[1])
            v = imm(a[2], False)
            if a[2].strip().startswith('%lo('):
                return '' if src == reg(a[0]) else setd(reg(a[0]), R(src))
            if src == 0:
                return setd(reg(a[0]), '0x%Xu' % v)
            return setd(reg(a[0]), '%s | 0x%X' % (R(src), v))
        if op == 'xori':
            return setd(reg(a[0]), '%s ^ 0x%X' % (R(reg(a[1])), imm(a[2], False)))
        if op == 'slt':
            return setd(reg(a[0]), '(int32_t)%s < (int32_t)%s' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'sltu':
            return setd(reg(a[0]), '%s < %s' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'slti':
            return setd(reg(a[0]), '(int32_t)%s < %d' % (R(reg(a[1])), imm(a[2])))
        if op == 'sll':
            return setd(reg(a[0]), '%s << %d' % (R(reg(a[1])), evalc(a[2])))
        if op == 'srl':
            return setd(reg(a[0]), '%s >> %d' % (R(reg(a[1])), evalc(a[2])))
        if op == 'sra':
            return setd(reg(a[0]), '(uint32_t)((int32_t)%s >> %d)' % (R(reg(a[1])), evalc(a[2])))
        if op == 'sllv':
            return setd(reg(a[0]), '%s << (%s & 31)' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'srlv':
            return setd(reg(a[0]), '%s >> (%s & 31)' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'srav':
            return setd(reg(a[0]), '(uint32_t)((int32_t)%s >> (%s & 31))' % (R(reg(a[1])), R(reg(a[2]))))
        if op == 'lui':
            return setd(reg(a[0]), hi_expr(a[1]))
        if op == 'lw':
            return setd(reg(a[0]), 'LW(%s)' % memarg(a[1]))
        if op == 'lh':
            return setd(reg(a[0]), 'LH(%s)' % memarg(a[1]))
        if op == 'lhu':
            return setd(reg(a[0]), 'LHU(%s)' % memarg(a[1]))
        if op == 'lb':
            return setd(reg(a[0]), 'LB(%s)' % memarg(a[1]))
        if op == 'lbu':
            return setd(reg(a[0]), 'LBU(%s)' % memarg(a[1]))
        if op == 'sw':
            return 'SW(%s, %s);' % (memarg(a[1]), R(reg(a[0])))
        if op == 'sh':
            return 'SH(%s, %s);' % (memarg(a[1]), R(reg(a[0])))
        if op == 'sb':
            return 'SB(%s, %s);' % (memarg(a[1]), R(reg(a[0])))
        if op == 'mtc2':
            return 'MTC2(%s, %d);' % (R(reg(a[0])), reg(a[1]))
        if op == 'ctc2':
            return 'CTC2(%s, %d);' % (R(reg(a[0])), reg(a[1]))
        if op == 'mfc2':
            return setd(reg(a[0]), 'MFC2(%d)' % reg(a[1]))
        if op == 'cfc2':
            return setd(reg(a[0]), 'CFC2(%d)' % reg(a[1]))
        if op == 'lwc2':
            return 'MTC2(LW(%s), %d);' % (memarg(a[1]), reg(a[0]))
        if op == 'swc2':
            return 'SW(%s, MFC2(%d));' % (memarg(a[1]), reg(a[0]))
        if op in GTEOPS:
            return 'doCOP2(0x%07X); /* %s */' % (GTEOPS[op], op)
        if op == 'mvmva':
            sf, mx, v, cv, lm = [int(x) for x in a]
            return 'doCOP2(0x%07X); /* mvmva %s */' % (cop2op(4, 0x12, sf=sf, mx=mx, v=v, cv=cv, lm=lm), ','.join(a))
        if op == 'gpl':
            return 'doCOP2(0x%07X); /* gpl %s */' % (cop2op(0x1A, 0x3E, sf=int(a[0])), a[0])
        if op == 'cop2':
            return 'doCOP2(0x%07X); /* %s */' % (a[0], ins.raw)
        if op == 'div':
            return 'pc_div(%s, %s);' % (R(reg(a[0])), R(reg(a[1])))
        if op == 'mflo':
            return setd(reg(a[0]), 'R.lo')
        if op == 'mfhi':
            return setd(reg(a[0]), 'R.hi')
        if op == 'break':
            return 'pc_asm_break(0x%05X, %d);' % (ins.addr, a[0])
        raise Exception('unhandled op %s in %s' % (ins.raw, f['name']))

    def cond(s, ins):
        op, a = ins.op, ins.args
        def sr(n):
            return '(int32_t)%s' % R(n) if n else '0'
        if op == 'beq':
            return '%s == %s' % (R(reg(a[0])), R(reg(a[1]))), a[2]
        if op == 'bne':
            return '%s != %s' % (R(reg(a[0])), R(reg(a[1]))), a[2]
        if op == 'beqz':
            return '%s == 0' % R(reg(a[0])), a[1]
        if op == 'bnez':
            return '%s != 0' % R(reg(a[0])), a[1]
        if op in ('bgez', 'bgezal'):
            return '%s >= 0' % sr(reg(a[0])), a[1]
        if op == 'bgtz':
            return '%s > 0' % sr(reg(a[0])), a[1]
        if op == 'blez':
            return '%s <= 0' % sr(reg(a[0])), a[1]
        if op == 'bltz':
            return '%s < 0' % sr(reg(a[0])), a[1]
        raise Exception(op)

    def gen_func(s, f):
        out = []
        ins_list = f['insns']
        # labels that are branch targets inside this function
        used = set()
        # first pass: hazards
        for i in range(len(ins_list) - 1):
            a, b = ins_list[i], ins_list[i + 1]
            if a.op in LOADS:
                dd, _ = dest_src(a)
                _, ss = dest_src(b)
                if dd & ss - {0}:
                    s.warn.append('load-use hazard %s @%05X: "%s" then "%s"' % (f['name'], a.addr, a.raw, b.raw))
            if a.op in ('mtc2', 'lwc2') and b.op in ('rtps', 'rtpt', 'nclip', 'avsz3', 'avsz4', 'mvmva', 'gpl', 'cop2', 'sqr'):
                s.warn.append('gte write->cmd hazard %s @%05X: "%s" then "%s"' % (f['name'], a.addr, a.raw, b.raw))
        body = []
        i = 0
        n = len(ins_list)
        skip_natural = set()
        while i < n:
            ins = ins_list[i]
            for l in ins.labels:
                pass
            body.append(('label', ins.addr))
            op = ins.op
            if op in ('j', 'b', 'jr', 'jal', 'jalr', 'beq', 'bne', 'beqz', 'bnez', 'bgez', 'bgtz', 'blez', 'bltz', 'bgezal'):
                dl = ins_list[i + 1] if i + 1 < n else None
                dcode = s.simple(dl, f) if dl else ''
                if dl and dl.op in ('j', 'b', 'jr', 'jal', 'jalr', 'beq', 'bne', 'beqz', 'bnez', 'bgez', 'bgtz', 'blez', 'bltz', 'bgezal'):
                    raise Exception('branch in delay slot @%x' % dl.addr)
                # delay-slot write hazards: if the delay insn writes a register read by the jump target expr, evaluate first
                if op == 'jr':
                    r = reg(ins.args[0])
                    assert r == 31, ins.raw
                    body.append(('code', '%s return;' % (dcode + ' ' if dcode else '')))
                    i += 1 if (dl and dl.labels) else 2
                    continue  # delay slot also emitted naturally (only reachable via a label)
                if op == 'jalr':
                    r = reg(ins.args[0])
                    s.need_dispatch = True
                    body.append(('code', '{ uint32_t tgt = %s; %s pc_asm_dispatch(tgt); }' % (R(r), dcode)))
                    if dl and dl.labels:
                        raise Exception('labelled jalr delay slot')
                    i += 2
                    continue
                if op == 'jal':
                    t = s.target(ins.args[0], f, ins)
                    body.append(('code', '%s %s' % (dcode, s.jump(f, t, 'call'))))
                    if dl and dl.labels:
                        raise Exception('labelled jal delay slot')
                    i += 2
                    continue
                if op in ('j', 'b'):
                    t = s.target(ins.args[0], f, ins)
                    body.append(('code', '%s %s' % (dcode, s.jump(f, t, 'goto'))))
                    i += 1 if (dl and dl.labels) else 2
                    continue
                c, tok = s.cond(ins)
                t = s.target(tok, f, ins)
                if op == 'bgezal':
                    if dl and dl.labels:
                        raise Exception('labelled bgezal delay slot')
                    body.append(('code', '{ int c = %s; %s if (c) %s }' % (c, dcode, s.jump(f, t, 'call'))))
                    i += 2
                    continue
                if c in ('0 >= 0',) or (op == 'beq' and ins.args[0].strip() in ('$0', '$zero') and ins.args[1].strip() in ('$0', '$zero')):
                    body.append(('code', '%s %s' % (dcode, s.jump(f, t, 'goto'))))
                    i += 1 if (dl and dl.labels) else 2
                    continue
                if dcode:
                    body.append(('code', 'if (%s) { %s %s }' % (c, dcode, s.jump(f, t, 'goto'))))
                else:
                    body.append(('code', 'if (%s) { %s }' % (c, s.jump(f, t, 'goto'))))
                i += 1
                continue
            code = s.simple(ins, f)
            if code:
                body.append(('code', code))
            i += 1
        return body

    def emit(s):
        out = []
        bodies = {}
        for f in s.funcs:
            bodies[f['addr']] = s.gen_func(f)
        # collect goto-targets per function
        for idx, f in enumerate(s.funcs):
            body = bodies[f['addr']]
            text = '\n'.join(c for k, c in body if k == 'code')
            targets = set(int(x, 16) for x in re.findall(r'goto L_([0-9A-F]+);', text))
            ent = sorted(s.entries[f['addr']])
            ent = [e for e in ent if e]
            out.append('/* %s (0x%05X) */' % (f['name'], f['addr']))
            out.append('static void F_%05X(int entry)' % f['addr'])
            out.append('{')
            if ent:
                out.append('    switch (entry) {')
                for e in ent:
                    out.append('    case 0x%X: goto L_%05X;' % (e, f['addr'] + e))
                    targets.add(f['addr'] + e)
                out.append('    }')
            else:
                out.append('    (void)entry;')
            for k, c in body:
                if k == 'label':
                    if c in targets:
                        out.append('L_%05X:' % c)
                else:
                    out.append('    ' + c.strip())
            # fall-through past the end
            last = f['insns'][-1] if f['insns'] else None
            lastreal = [x for x in f['insns'] if x.op != '.data']
            if lastreal:
                lr = lastreal[-1]
                prev = f['insns'][f['insns'].index(lr) - 1] if f['insns'].index(lr) > 0 else None
                ends = lr.op in ('jr', 'j', 'b') or (prev is not None and prev.op in ('jr', 'j', 'b'))
                if not ends:
                    nxt = s.byaddr.get(f['pc'])
                    assert nxt, f['name']
                    out.append('    F_%05X(0); return; /* falls through into %s */' % (nxt['addr'], nxt['name']))
            out.append('}')
            out.append('')
        return '\n'.join(out)

def descriptions(path):
    raw = open(path).read()
    d = {}
    for m in re.finditer(r'((?:/\*.*?\*/\s*)+)glabel\s+(\w+)', raw, flags=re.S):
        coms = re.findall(r'/\*(.*?)\*/', m.group(1), flags=re.S)
        # only the notes between the '---- func ----' marker and the glabel
        idx = max([i for i, c in enumerate(coms) if '----' in c], default=-1)
        coms = [c.strip() for c in coms[idx + 1:]]
        if idx >= 0 and coms:
            d[m.group(2)] = coms[-1]
    return d

def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument('src'); ap.add_argument('module')
    ap.add_argument('--entries', default='')
    ap.add_argument('--abi', default='', help='functions whose C wrapper is hand-written')
    ap.add_argument('--head'); ap.add_argument('--foot')
    ap.add_argument('--alias', default='', help='name=func:off exported alias entries')
    o = ap.parse_args()
    entries = []
    for e in filter(None, o.entries.split(',')):
        fn, off = e.split(':')
        entries.append((fn, int(off, 0)))
    aliases = []
    for e in filter(None, o.alias.split(',')):
        nm, rest = e.split('=')
        fn, off = rest.split(':')
        entries.append((fn, int(off, 0)))
        aliases.append((nm, fn, int(off, 0)))
    abi = set(filter(None, o.abi.split(',')))
    funcs = parse(o.src)
    for f in funcs:
        for l, a in f['labels'].items():
            m = re.fullmatch(r'\.L([0-9A-F]+)', l)
            if m and int(m.group(1), 16) != a:
                sys.stderr.write('LABEL MISMATCH %s: %s computed %X\n' % (f['name'], l, a))
    for f1, f2 in zip(funcs, funcs[1:]):
        if f1['pc'] != f2['addr']:
            sys.stderr.write('GAP %s ends %X, %s starts %X\n' % (f1['name'], f1['pc'], f2['name'], f2['addr']))
    g = Gen(funcs, entries, o.module)
    desc = descriptions(o.src)
    bodies = g.emit()
    w = sys.stdout.write
    if o.head:
        w(open(o.head).read())
    w('\n/* ======== generated by mips2c.py from src/%s - do not hand-edit, regenerate ======== */\n\n' % o.src.split('/')[-1])
    for f in funcs:
        w('static void F_%05X(int entry);\n' % f['addr'])
    w('\n')
    # add descriptions before each function
    for f in funcs:
        if f['name'] in desc:
            dtext = ' '.join(w for w in desc[f['name']].split() if w != '*')
            bodies = bodies.replace('/* %s (0x%05X) */' % (f['name'], f['addr']), '/* %s: %s */' % (f['name'], dtext))
    w(bodies)
    w('/* ---- register-convention entry points (run on R) ---- */\n')
    for f in funcs:
        if f['name'] not in abi:
            w('void %s(void) { F_%05X(0); }\n' % (f['name'], f['addr']))
    for nm, fn, off in aliases:
        w('void %s(void) { F_%05X(0x%X); }\n' % (nm, int(fn.split('_')[1], 16), off))
    w('\n/* jalr targets: the routine entry points as code addresses */\n')
    w('int pc_asm_%s_dispatch(uint32_t tgt)\n{\n' % o.module)
    for f in funcs:
        w('    if (tgt == (uint32_t)(uintptr_t)&%s) { F_%05X(0); return 1; }\n' % (f['name'], f['addr']))
        for off in sorted(g.entries[f['addr']]):
            if off:
                w('    if (tgt == (uint32_t)(uintptr_t)&%s + 0x%X) { F_%05X(0x%X); return 1; }\n' % (f['name'], off, f['addr'], off))
    for nm, fn, off in aliases:
        w('    if (tgt == (uint32_t)(uintptr_t)&%s) { F_%05X(0x%X); return 1; }\n' % (nm, int(fn.split('_')[1], 16), off))
    w('    return 0;\n}\n')
    if o.foot:
        w('\n' + open(o.foot).read())
    for wn in g.warn:
        sys.stderr.write('WARN ' + wn + '\n')
    sys.stderr.write('SYMS ' + repr(sorted(SYMS)) + '\n')

main()
