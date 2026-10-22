typedef struct {
    unsigned char type; char p0[0x7B]; unsigned char b7c; char p1[7]; unsigned char b84; char p2[0x6B];
    int f0; char p3[0xC]; int i100; char p4[4]; int i108; char p5[0x74];
    unsigned short h180; short h182; unsigned char b184, b185;
} CAR;
extern long long D_000967F4, D_00096858;
extern int D_000C7004[];
extern int D_00096C68, D_00096C6C, D_00096C64, D_00096C60;
extern int GetEngineRevs();

/* Driver 2 name: unknown */
void func_0004C72C(CAR *p)
{
    int s2;
    unsigned int ang;
    int tgt;
    int a0;

    s2 = p->f0;
    ang = p->h180;
    p->b185 = 0;
    if (p->i100 == 0) {
        int v;
        if (p->type == 0x17 && *(int *)&D_000967F4 != 0x16)
            goto b0;
        v = D_000C7004[0];
        if (v == 0x7f) goto b0;
        if ((unsigned)(v - 0xAA) < 10) goto b0;
        if (*(int *)&D_00096858 == 0) goto call;
b0:
        if (*(int *)&D_00096858 != 0 || p->b7c != 0)
            goto call;
        if (p->b84 != 0) {
call:
            tgt = GetEngineRevs(p);
            goto mid;
        }
    }
    tgt = 0x4E20;
    p->b184 = 0;
    if (ang < 0x1F40) ang = 0x1F40;
mid:
    a0 = tgt;
    if ((unsigned short)ang - (a0 & 0xFFFF) > D_00096C68) {
        s2 = 0;
        a0 = ang - (unsigned short)D_00096C68;
        p->b185 = 1;
    }
    if ((a0 & 0xFFFF) - (unsigned short)ang > D_00096C6C)
        a0 = ang + (unsigned short)D_00096C6C;
    p->h180 = a0;
    if (s2 != 0 || p->i108 >= 0x1F) {
        int m = s2 ? 0 : D_00096C64;
        p->h182 -= (unsigned short)D_00096C60;
        if (p->h182 < m) p->h182 = m;
    } else {
        p->h182 += 200;
        if (p->h182 > 10000) p->h182 = 10000;
    }
}
