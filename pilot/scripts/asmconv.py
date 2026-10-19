import sys,re
sym=sys.argv[1]; lines=open(sys.argv[2]).read().splitlines()
out=[]; insns=[]
for l in lines:
    m=re.match(r'\s*/\* \w+ \w+ \w+ \*/\s+(\S+)\s*(.*?)\s*(/\*.*\*/)?\s*$',l)
    if m:
        op,args=m.group(1),m.group(2)
        # replace (0x1F8001F0 >> 16) style
        args=re.sub(r'\((0x[0-9A-Fa-f]+) >> 16\)',lambda g:hex(int(g.group(1),16)>>16),args)
        args=re.sub(r'\((0x[0-9A-Fa-f]+) & 0xFFFF\)',lambda g:hex(int(g.group(1),16)&0xFFFF),args)
        insns.append([op,args]); continue
    m=re.match(r'\s*(\.L\w+):',l)
    if m: insns.append([m.group(1)+':',None])
macros=set(); res=[]
br={"bgezal",'j','jal','jalr','beq','bne','bnez','beqz','bgtz','bgez','bltz','blez'}
for i,(op,a) in enumerate(insns):
    if a is None: res.append(op); continue
    if op in br:
        nxt=insns[i+1]
        if nxt[0]=='nop' and op in ('j','jal'):
            res.append(f"{op} {a}"); insns[i+1]=['__skip__',''] ; continue
        M='X'+op.upper(); n=len(a.split(','))
        ps=','.join('p%d'%k for k in range(n)); body=', '.join('\\\\p%d'%k for k in range(n))
        macros.add(f'.macro {M} {ps}; {op} {body}; .endm')
        res.append(f"{M} {a}")
    elif op=='__skip__': continue
    elif op=='rtpt': res.append('.word 0x4A280030')
    elif op=='nclip': res.append('.word 0x4B400006')
    elif op=='avsz3': res.append('.word 0x4B58002D')
    else: res.append(f"{op} {a}".strip())
print('/* HANDWRITTEN ASM */')
print('__asm__(\n    ".set noreorder\\n"')
for m in sorted(macros): print(f'    "{m}\\n"')
print(f'    ".globl {sym}\\n"\n    "{sym}:\\n"')
for r in res: print(f'    "{r}\\n"')
print('    ".set reorder\\n"\n);')
