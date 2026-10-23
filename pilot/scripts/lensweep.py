import sys, os, re, subprocess
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
exec(open('build/r4-13/climb.py').read().split('src=open')[0].split('def parse')[1].join(['def parse','']) if False else '')
def parse(l):
    m=re.match(r'\s*([A-Za-z_]\w*(?:\[\d\])?)\s*=(.*);',l)
    if not m: return None,set()
    d=m.group(1); rhs=m.group(2)
    uses=set(re.findall(r'[A-Za-z_]\w*(?:\[\d\])?',rhs))|set(re.findall(r'([A-Za-z_]\w*)->',rhs))
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
CPP="compilers/gcc-2.7.2-psx/cpp -Iinclude -undef -lang-c -nostdinc -D__GNUC__=2 -Dmips -D_MIPSEL".split()
CC1="compilers/gcc-2.7.2-cdk/cc1 -O2 -G8 -mips1 -mcpu=3000 -fgnu-linker -mgas -msoft-float -quiet -funsigned-char -w -dl -dg".split()
D='build/r4-13/ls'; os.makedirs(D,exist_ok=True)
def lens(text):
    global BPREG
    open(D+'/x.c','w').write(text)
    subprocess.run(CPP+[D+'/x.c',D+'/x.i'],check=True)
    subprocess.run(CC1+[D+'/x.i','-o',D+'/x.s'],check=True)
    t=open(D+'/x.i.lreg').read()
    r={}
    for reg in (BPREG,'92','89'):
        m=re.search(r'^Register %s used (\d+) times across (\d+) insns'%reg,t,re.M)
        r[reg]=(int(m.group(1)),int(m.group(2)))
    return r
BPREG=sys.argv[2]
src=open(sys.argv[1]).read().split('\n')
start=[i for i,l in enumerate(src) if l.startswith('    s = (SV')][0]
end=[i for i,l in enumerate(src) if l.startswith('    for (k')][0]
print(lens('\n'.join(src)))
for a in range(start,end):
    for p in range(start,end):
        if p==a: continue
        t=src[:]; l=t.pop(a); t.insert(p,l)
        if not ok(t[start:end]): continue
        r=lens('\n'.join(t))
        print(r[BPREG][1],a,p,l.strip(),flush=True)
