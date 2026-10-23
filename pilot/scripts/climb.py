# run inside distrobox: python climb.py base.c outprefix
import sys, os, re, io, contextlib
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools"))
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
import fnscore
fnscore.B = "build/r4-13/fsm"; os.makedirs(fnscore.B, exist_ok=True)
def score(path):
    sys.argv=["fnscore.py","func_00029448",path]
    f=io.StringIO()
    with contextlib.redirect_stdout(f):
        try: fnscore.main()
        except SystemExit: pass
    m=re.search(r'MATCH ([\d.]+)',f.getvalue()); return float(m.group(1)) if m else 0
def parse(l):
    m=re.match(r'\s*([A-Za-z_]\w*(?:\[\d\])?)\s*=(.*);',l)
    if not m: return None,set()
    d=m.group(1); rhs=m.group(2)
    uses=set(re.findall(r'[A-Za-z_]\w*(?:\[\d\])?',rhs))
    # struct field access f->x : treat 'f' as use
    uses|=set(re.findall(r'([A-Za-z_]\w*)->',rhs))
    return d,uses
def ok(block):
    defs={}
    for i,l in enumerate(block):
        d,_=parse(l)
        if d: defs[d]=i
    for i,l in enumerate(block):
        d,u=parse(l)
        for v in u:
            if v in defs and defs[v]>i: return False
    return True
ARGS=sys.argv[:]
src=open(sys.argv[1]).read().split('\n')
start=[i for i,l in enumerate(src) if l.startswith('    s = (SV')][0]
end=[i for i,l in enumerate(src) if l.startswith('    for (k')][0]
best=score(ARGS[1]); print('start',best,flush=True)
improved=True
while improved:
    improved=False
    cands=[]
    for a in range(start,end):
        for p in range(start,end):
            if p==a: continue
            t=src[:]; l=t.pop(a); t.insert(p,l)
            if not ok(t[start:end]): continue
            open('build/r4-13/cl.c','w').write('\n'.join(t))
            sc=score('build/r4-13/cl.c')
            if sc>best:
                best=sc; src=t; improved=True
                open(ARGS[2]+'_best.c','w').write('\n'.join(t))
                print(best,a,p,l.strip(),flush=True)
                break
        if improved: break
print('done',best)
