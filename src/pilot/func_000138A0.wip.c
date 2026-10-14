typedef struct {
    unsigned char id;
    char p1[2];
    char f3;
    char p4[4];
    char f8;
    char p9[0x0F];
    int f18;
    char p1c[4];
    int f20;
    int f24;
    int f28;
    char p2c[4];
    int f30;
    int f34;
    char p38[0x50 - 0x38];
} ENT;

typedef struct {
    short a;
    short h;
    int b;
} REC;

typedef struct {
    char p0[3];
    char f3;
    char p4[4];
    REC rec[1];
} PATH;

typedef struct {
    unsigned char id;
    char p1[0x1F];
    int f20;
    int f24;
} CAR;

typedef struct {
    int v;
    unsigned char b;
    char pad[3];
} FOUND;

extern ENT D_00098950[];
extern long long D_00097040;
extern int func_00012E98();

int func_000138A0(CAR *c)
{
    FOUND list[22];
    ENT *e;
    FOUND *f;
    FOUND *end;
    FOUND *g;
    PATH *path;
    REC *r;
    int n;
    int t0;
    int t1;
    int t2;
    int t3;

    t1 = c->f20;
    path = (PATH *)(*(char **)&D_00097040 + t1 * 44);
    r = (REC *)((char *)path + 8);
    if (path->f3 != 0)
        return 1;
    t2 = r[c->f24].h;
    t0 = (c->f24 + 2) & 3;
    if (t2 != -1) {
        f = list;
        t3 = c->id;
        n = 0;
        for (e = D_00098950; e < D_00098950 + 22; e++) {
            if (e->f8 && e->id != t3) {
                if (e->f28 == t2) {
                    if (e->f24 == t0 || e->f30 == t0) {
                        f->v = e->f18;
                        f->b = e->id;
                        n++;
                        f++;
                    }
                } else if (e->f3 && e->f20 == t1) {
                    f->v = -1;
                    f->b = e->id | 0x80;
                    n++;
                    f++;
                }
            }
        }
        g = &list[0];
        end = g + n;
        for (f = g; f < end; f++) {
            int v = f->v;
            int b;
            if (v < 0x4074) {
                b = f->b;
                if (b & 0x80)
                    return 0;
                if (D_00098950[b].f34 != -1) {
                    if (D_00098950[b].f34 == 0)
                        return 0;
                    if (v < 0x2904)
                        return 0;
                }
            }
        }
    }
    return func_00012E98(c, 1) == 0;
}
