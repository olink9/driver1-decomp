#!/bin/sh
# cmpfn.sh FILE.c BASE.o : compile FILE.c, compare per-function disasm to BASE.o
cd "$(dirname "$0")/../.."
S=$(mktemp -d); trap 'rm -rf "$S"' EXIT
n=$(basename $1 .c); rm -f $S/$n.new.o
.venv/bin/python build/tu-xmplay/cc.py "$1" $S/$n.new.o | grep -v '^OK$' && true
[ -f $S/$n.new.o ] || exit 1
for o in $S/$n.new.o $2; do mips-linux-gnu-objdump -d --no-show-raw-insn -r $o | sed -E 's/^ *[0-9a-f]+:\s*//;s/^\s*[0-9a-f]+: (R_MIPS)/\1/' > $o.d; done
python3 - $S/$n.new.o.d $2.d <<'P'
import sys,re
def parse(f):
    d={};cur=None
    for l in open(f):
        m=re.match(r'^[0-9a-f]+ <(\w+)>:',l)
        if m: cur=m.group(1); d[cur]=[]; continue
        if cur and l.strip(): d[cur].append(re.sub(r'\s+',' ',re.sub(r'[0-9a-f]+ <[^>]*>','X',l.strip())))
    return d
a=parse(sys.argv[1]); b=parse(sys.argv[2]); bad=0
for k in b:
    if a.get(k)!=b[k]: bad+=1; print("DIFF",k,len(a.get(k,[])),len(b[k]))
print("ok" if not bad else "%d diffs"%bad)
P
