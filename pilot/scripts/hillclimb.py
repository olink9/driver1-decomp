import random, subprocess, sys, re, itertools
import os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
D=os.path.join(ROOT, 'build', 'sweep')
form=sys.argv[1]; iters=int(sys.argv[2]); seed=int(sys.argv[3])
random.seed(seed)
idx={'x':0,'y':1,'z':2,'w':3}
prods=['xx','xy','xz','yy','yw','zz','zw','xw','yz']
stores=[('[0][0]','4096 - (F(yy) + F(zz))'),('[0][1]','F(xy) - F(zw)'),('[0][2]','F(xz) + F(yw)'),('[1][0]','F(xy) + F(zw)'),
 ('[1][1]','4096 - (F(xx) + F(zz))'),('[2][0]','F(xz) - F(yw)'),('[2][2]','4096 - (F(xx) + F(yy))'),('[1][2]','F(yz) - F(xw)'),('[2][1]','F(yz) + F(xw)')]
def gen(P,S,form):
    o=['typedef struct { short m[3][3]; int t[3]; } MATRIX;','#define F(v) (((v) + 0x400) >> 11)' if form=='B' else '#define F(v) (v)',
       'void func_0001B380(int (*qua)[4], MATRIX *m)','{','    int xx, xy, xz, xw, yy, yz, yw, zz, zw;']
    for p in P:
        e='(*qua)[%d] * (*qua)[%d]'%(idx[p[0]],idx[p[1]])
        if form=='A': e='(%s + 0x400) >> 11'%e
        o.append('    %s = %s;'%(p,e))
    for k,e in S: o.append('    m->m%s = %s;'%(k,e))
    o.append('}')
    return '\n'.join(o)+'\n'
def score(P,S):
    f=D+'/v%d.c'%seed
    open(f,'w').write(gen(P,S,form))
    r=subprocess.run(['.venv/bin/python','pilot/scripts/fs.py',D+'/b%d'%seed,'func_0001B380',f,'--quiet'],capture_output=True,text=True,cwd=ROOT)
    m=re.search(r'MATCH ([\d.]+)',r.stdout)
    return float(m.group(1)) if m else 0
P=prods[:]; S=stores[:]
best=score(P,S); print('start',best,flush=True)
for i in range(iters):
    P2=P[:]; S2=S[:]
    L=P2 if random.random()<0.5 else S2
    a,b=random.sample(range(9),2); L[a],L[b]=L[b],L[a]
    s=score(P2,S2)
    if s>=best:
        if s>best: print(i,s,P2,[k for k,_ in S2],flush=True)
        best=s;P=P2;S=S2
    if best==100: break
open(D+'/best_%s_%d.c'%(form,seed),'w').write(gen(P,S,form))
print('best',best)
