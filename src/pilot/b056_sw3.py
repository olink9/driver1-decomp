import subprocess, re
lines=open('./src/pilot/func_0006A6C8.c').read().split('\n')
pre='\n'.join(lines[:47]); post='\n'.join(lines[51:])
D1="*(int *)&D_0009770C > 0"; D2="*(int *)&D_00097708 > 0x292"
V={
'a':f"""            play = 0;
            if (!(p->flags & 2))
                play = 1;
            else if ((p->flags & 2) && {D1} && {D2})
                play = 1;""",
'b':f"""            play = !(p->flags & 2) || ((p->flags & 2) && {D1} && {D2});""",
'c':f"""            play = 0;
            if (!(p->flags & 2))
                play = 1;
            if ((p->flags & 2) && {D1} && {D2})
                play = 1;""",
'd':f"""            play = 0;
            if ((p->flags & 2) == 0 || (p->flags & 2) != 0 && {D1} && {D2})
                play = 1;""",
'e':f"""            play = 0;
            if (!(p->flags & 2) || ((p->flags & 2) && {D1} && *(int *)&D_00097708 >= 0x293))
                play = 1;""",
'f':f"""            play = 0;
            if (!(p->flags & 2) || ((p->flags & 2) && ({D1} && {D2})))
                play = 1;""",
'g':f"""            play = 0;
            if ((p->flags & 2) ? ({D1} && {D2}) : 1)
                play = 1;""",
'h':f"""            play = 0;
            if (!(p->flags & 2)) {{
                play = 1;
            }} else if (p->flags & 2) {{
                if ({D1} && {D2})
                    play = 1;
            }}""",
}
for k,v in V.items():
    open('./src/pilot/b056_t.c','w').write(pre+'\n'+v+'\n'+post)
    o=subprocess.run(['distrobox','enter','build','--','.venv/bin/python','tools/fnscore.py','func_0006A6C8','src/pilot/b056_t.c'],capture_output=True,text=True,cwd='.').stdout
    print(k, o.split('\n')[0])
    if 'MATCH 100.0' in o:
        open('./src/pilot/b056_win.c','w').write(pre+'\n'+v+'\n'+post)
