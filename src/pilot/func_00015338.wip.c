typedef struct { unsigned char idx; char p0[0x23]; int dir; int w28; } P;
typedef struct { char p0[0xC8]; int x; char p1[4]; int z; char p2[0xB8]; } Car;
typedef struct { char p0[4]; short s4; char p1[4]; short sA; short sC; char p2[4]; short s12; } Rd;
extern Car D_0009B230[];
extern Rd D_0009E744[];

unsigned char func_00015338(P *p)
{
    Car *c = &D_0009B230[p->idx];
    int w = p->w28;
    Rd *r;
    int d, a3, q;
    unsigned char t0;

    a3 = 0;
    if (w & 0xC000) return 0;
    r = &D_0009E744[w];
    d = p->dir;
    t0 = ((unsigned char *)r)[d];
    if (t0 == 1) return 0;
    switch (d) {
    case 0: q = (c->x + 750) / 1500; a3 = q - r->s4; break;
    case 1: q = (c->z - 750) / 1500; a3 = r->sA - q; break;
    case 2: q = (c->x - 750) / 1500; a3 = r->sC - q; break;
    case 3: q = (c->z + 750) / 1500; a3 = q - r->s12; break;
    }
    return t0 - a3 - 1;
}
