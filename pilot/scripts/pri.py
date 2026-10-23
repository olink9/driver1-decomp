import re,sys,math
txt=open(sys.argv[1]).read()
used={int(a):int(b) for a,b in re.findall(r'Register (\d+) used (\d+) times',txt)}
chunks=re.split(r'\n(?=\((?:insn|jump_insn|call_insn|note|code_label|barrier) )',txt)
n=0; first={}; last={}
for c in chunks:
    if not re.match(r'\((insn|jump_insn|call_insn) ',c): continue
    n+=1
    for r in re.findall(r'\(reg(?:/[a-z]+)?:[A-Z]+ (\d+)\)',c.split('(expr_list')[0]):
        r=int(r)
        if r<64: continue
        first.setdefault(r,n); last[r]=n
g=open(sys.argv[1].replace('.lreg','.greg')).read()
disp={int(a):int(b) for a,b in re.findall(r'(\d+) in (\d+)',g.split(';; Register dispositions:')[1].split(';; Hard')[0])}
nm={2:'v0',3:'v1',4:'a0',5:'a1',6:'a2',7:'a3',8:'t0',9:'t1',10:'t2',11:'t3',12:'t4',13:'t5',14:'t6'}
rows=[]
for r in first:
    L=max(1,last[r]-first[r]); N=used.get(r,0)
    p=(int(math.log2(N)) if N else 0)*N/L
    rows.append((p,r,first[r],last[r],N,nm.get(disp.get(r),disp.get(r))))
rows.sort(reverse=True)
for x in rows:
    if x[3]-x[2]>8: print("%.4f"%x[0],*x[1:])
