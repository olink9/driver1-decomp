typedef struct OBJ {
    int pad0;
    short pad4;
    short ref;
    char pad8[8];
    int f10;
    int f14;
    int f18;
    int f1c;
    int f20;
} OBJ;
extern OBJ *D_00097238;
extern int D_00097248;
extern unsigned short *D_000972D0;
extern int D_000969E8;
extern OBJ *D_000A0E20[];

void func_00033E54(void)
{
    OBJ *p;
    OBJ *q;
    OBJ *o;
    int i, n, hdr;

    D_00097248 = 0;
    n = *D_000972D0++;
    p = D_00097238;
    for (i = 0; i < n; i++) {
        hdr = *(int *)p;
        p = (OBJ *)((int *)p + 1);
        D_000A0E20[D_000972D0[i]] = p;
        o = p;
        if (p->pad4 == -1) {
            if (p->f20)
                p->f20 += (int)p;
            p->f10 += (int)p;
            p->f18 += (int)p;
            p->f1c += (int)p;
        } else {
            q = D_000A0E20[p->pad4];
            if (q->f20)
                p->f20 = q->f20;
            p->f10 = q->f10;
            p->f18 = q->f18;
            p->f1c = q->f1c;
        }
        o->f14 += (int)o;
        p = (OBJ *)((char *)p + hdr);
    }
    D_000969E8 = 0;
}
