import sys
import os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
sets=[l.split(',') for l in sys.argv[1:]]
out=["extern void f(int);","int D;"]
for k,s in enumerate(sets):
    out.append(f"void t{k}(int x){{ switch(x){{")
    for j,v in enumerate(s):
        if v.endswith('!'): out.append(f" case {v[:-1]}: break;")
        else: out.append(f" case {v}: f({j}); break;")
    out.append(" } D=1; }")
open(os.path.join(ROOT, 'build', 'sweep'),'w').write("\n".join(out)+"\n")
