import sys, subprocess, re
out=subprocess.run(['build/r4-08/sc.sh',sys.argv[1],'--asm']+sys.argv[2:],capture_output=True,text=True).stdout
head=out.splitlines()[0]
t=out.split('---- target\n')[1].split('\n---- candidate\n')
L=[x for x in t[0].splitlines()]; R=[x for x in t[1].splitlines()]
regs=r'\b(zero|v[01]|a[0-3]|t[0-9]|s[0-8]|gp|sp|fp|ra|at)\b'
def norm(x): return re.sub(regs,'R',x)
import difflib
sm=difflib.SequenceMatcher(None,[norm(x) for x in L],[norm(x) for x in R],autojunk=False)
bad=0
for op,a1,a2,b1,b2 in sm.get_opcodes():
    if op!='equal':
        bad+=max(a2-a1,b2-b1)
        if '-v' in sys.argv[2:] or True:
            print(op, a1, L[a1:a2], '|', R[b1:b2])
print(head,'shape-diff',bad)
