# exhaustive permutation of lines between //PERM{ and //PERM} (single group)
import itertools, subprocess, sys, re, os
import os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
tmpl, sym, D = sys.argv[1], sys.argv[2], sys.argv[3]
os.makedirs(D, exist_ok=True)
src=open(tmpl).read().split('\n')
a=src.index([l for l in src if '//PERM{' in l][0]); b=src.index([l for l in src if '//PERM}' in l][0])
pre, grp, post = src[:a], src[a+1:b], src[b+1:]
res=[]
for p in itertools.permutations(grp):
    f=D+'/v.c'; open(f,'w').write('\n'.join(pre+list(p)+post)+'\n')
    r=subprocess.run(['.venv/bin/python','pilot/scripts/fs.py',D+'/b',sym,f,'--quiet'],capture_output=True,text=True,cwd=ROOT)
    m=re.search(r'MATCH ([\d.]+)',r.stdout); s=float(m.group(1)) if m else 0
    res.append((s,p))
    if s==100: break
res.sort(key=lambda x:-x[0])
for s,p in res[:5]: print(s, ' | '.join(x.strip() for x in p))
open(D+'/best.c','w').write('\n'.join(pre+list(res[0][1])+post)+'\n')
