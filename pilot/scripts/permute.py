# generic: permute lines between '//PERM{' and '//PERM}' markers (multiple groups)
import random, subprocess, sys, re
import os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
tmpl, sym, iters, seed = sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4])
D=os.path.join(ROOT, 'build', 'sweep')
random.seed(seed)
src=open(tmpl).read().split('\n')
groups=[]; out=[]; cur=None
for l in src:
    if '//PERM{' in l: cur=[]; continue
    if '//PERM}' in l: groups.append(cur); out.append(('G',len(groups)-1)); cur=None; continue
    if cur is not None: cur.append(l)
    else: out.append(('L',l))
def gen(G):
    r=[]
    for k,v in out:
        if k=='L': r.append(v)
        else: r.extend(G[v])
    return '\n'.join(r)+'\n'
def score(G):
    f=D+'/v%d.c'%seed
    open(f,'w').write(gen(G))
    r=subprocess.run(['.venv/bin/python','pilot/scripts/fs.py',D+'/b%d'%seed,sym,f,'--quiet'],capture_output=True,text=True,cwd=ROOT)
    m=re.search(r'MATCH ([\d.]+)',r.stdout)
    return float(m.group(1)) if m else 0
G=[g[:] for g in groups]
best=score(G); print('start',best,flush=True)
for i in range(iters):
    G2=[g[:] for g in G]
    L=random.choice([g for g in G2 if len(g)>1])
    if random.random()<0.5:
        a,b=random.sample(range(len(L)),2); L[a],L[b]=L[b],L[a]
    else:
        a=random.randrange(len(L)); x=L.pop(a); L.insert(random.randrange(len(L)+1),x)
    s=score(G2)
    if s>=best:
        if s>best: print(i,s,flush=True)
        best=s;G=G2
    if best==100: break
open(D+'/best_%d.c'%seed,'w').write(gen(G))
print('best',best)
