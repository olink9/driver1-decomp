import itertools,subprocess,re,sys
import os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
vals=['-1','0','1','3','7','8','9','0xFF0','0xFF1','0x7FFF','0x7FFE','0x8000']
combos=[]
for n in range(1,7):
  for c in itertools.combinations(vals,n):
    if '7' not in c: continue
    for mode in (0,1):
        combos.append((c,mode))
out=["extern void f0(),f1(),f2(),f3();","int D;"]
for k,(c,mode) in enumerate(combos):
    out.append(f"void t{k}(int x){{ switch(x){{ case 2: f0(); break; case 4: f1(); break; case 5: f2(); break; case 6: f3(); break;")
    if mode==0: out.append("".join(f" case {v}:" for v in c)+" break;")
    else:
        out.append(" case 7: return;")
        rest=[v for v in c if v!='7']
        if rest: out.append("".join(f" case {v}:" for v in rest)+" break;")
    out.append(" } D=1; }")
open(os.path.join(ROOT, 'build', 'sweep'),'w').write("\n".join(out)+"\n")
subprocess.run(["distrobox","enter","build","--","bash","-c","compilers/gcc-2.7.2-cdk/cc1 -O2 -G8 -quiet build/sweep/swb.c -o build/sweep/swb.s"],check=True)
s=open(os.path.join(ROOT, 'build', 'sweep')).read()
from collections import Counter
cnt=Counter()
for k,(c,mode) in enumerate(combos):
    m=re.search(rf"\nt{k}:\n(.*?)\n\t\.end", s, re.S)
    seq=" ".join(a+b for a,b in re.findall(r"\t(li|slt|sltu)\t(\S+)",m.group(1)) if not (a=='li' and b in('$2,1',)))
    cnt[seq]+=1
    if seq.startswith("li$2,4 slt$2,$4,5 li$2,2 li$2,7"): print(c,mode,seq)
for k,v in cnt.most_common(15): print(v,k,file=sys.stderr)
