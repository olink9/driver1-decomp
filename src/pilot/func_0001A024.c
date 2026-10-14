typedef struct { char p0; unsigned char t; char p2[3]; unsigned char id; char p6[2]; unsigned char f8; char p9[71]; } S80;
typedef struct { char p[1960]; } S1960;
typedef struct { char p0[0x1A]; short f1a; char p1c[2]; short f1e; char p20[4]; unsigned char f24, f25, f26, p27, f28; unsigned char list[1]; } P;
extern S80 D_00098950[];
extern S1960 D_000C4690[];
extern unsigned char D_00099AF0[];
extern char D_00099120[];
extern int D_0009677C;
extern long long D_000970D8;
extern int D_00096FD0;
extern int D_00096FF0;
extern unsigned char D_00096FC4;
extern void func_00063AB4();
extern void func_0001A008();

void func_0001A024(P *p)
{
    unsigned char *q;
    unsigned char *end = (unsigned char *)p + (p->f26 + 0x29);
    for (q = p->list; q < end;) {
        int i = *q;
        int t;
        S80 *s = &D_00098950[i];
        int a = s->id;
        if (s->t != 0 && D_0009677C != 0) {
            if (*(int *)&D_000970D8 != D_00096FD0 || i != D_00096FF0) {
                D_00096FF0 = i;
                D_00096FD0 = *(volatile int *)&D_000970D8;
            }
        }
        if (a != 0)
            func_00063AB4(&D_000C4690[s->id]);
        if (s->t == 9)
            func_0001A008(D_00099120);
        s->t = 0;
        s->f8 = 0;
        t = D_00096FC4 - 1;
        D_00096FC4 = t;
        q++;
        D_00099AF0[(unsigned char)t] = i;
    }
    p->f1e = 0;
    p->f28 = 0;
    p->f1a = -1;
    p->f24 = 0;
    p->f25 = 0;
    p->f26 = 0;
}
