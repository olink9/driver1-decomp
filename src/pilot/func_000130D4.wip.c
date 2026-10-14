/* func_000130D4 - no Driver 2 counterpart (car AI: junction clear check) */

typedef struct {
    unsigned char id;
    char p1[2];
    unsigned char f3;
    char p4[4];
    unsigned char f8;
    char p9[0x0F];
    int f18;
    char p1c[4];
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int f38;
    char p3c[0x50 - 0x3C];
} ENT;

typedef struct {
    short a;
    short h;
    int b;
} REC;

typedef struct {
    int v;
    unsigned char b;
    char pad[3];
} FOUND;

extern ENT D_00098950[];
extern long long D_00097040;
extern int func_00012E98();

static inline int collect(ENT *c, FOUND *f, int road, int lane, int jn)
{
    ENT *e;
    int n = 0;
    int id = c->id;

    for (e = D_00098950; e < D_00098950 + 22; e++) {
        if (e->f8 && e->id != id) {
            if (e->f28 == road) {
                if (e->f24 == lane || e->f30 == lane) {
                    f->v = e->f18;
                    f->b = e->id;
                    n++;
                    f++;
                }
            } else if (e->f3 && e->f20 == jn) {
                f->v = -1;
                f->b = e->id | 0x80;
                n++;
                f++;
            }
        }
    }
    return n;
}

int func_000130D4(ENT *c)
{
    FOUND l1[22];
    FOUND l2[22];
    FOUND *f;
    FOUND *end;
    REC *r;
    int n;
    int b;
    int jn;
    int prev;
    int next;
    int road1;
    int road2;

    jn = c->f20;
    r = (REC *)(*(char **)&D_00097040 + jn * 44 + 8);
    prev = (c->f24 - 1) & 3;
    next = (c->f24 + 1) & 3;
    road1 = r[prev].h;
    road2 = r[next].h;

    if (c->f34 == 0) {
        if (road1 != -1) {
            n = collect(c, l1, road1, next, jn);
            f = l1;
            end = f + n;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        return 0;
                    if (D_00098950[b].f38 && D_00098950[b].f2C != c->f28)
                        return 0;
                }
            }
        }
        if (road2 != -1) {
            n = collect(c, l2, road2, prev, jn);
            f = l2;
            end = f + n;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    if (D_00098950[f->b].f38)
                        return 0;
                }
            }
        }
        return func_00012E98(c, 1) == 0;
    } else if (c->f34 > 0) {
        if (road1 != -1) {
            n = collect(c, l1, road1, next, jn);
            f = l1;
            end = f + n;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        return 0;
                    if (D_00098950[b].f2C == road2 && D_00098950[b].f38)
                        return 0;
                }
            }
        }
        return func_00012E98(c, 1) == 0;
    } else if (c->f34 < 0) {
        if (road1 != -1) {
            n = collect(c, l1, road1, next, jn);
            f = l1;
            end = f + n;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        return 0;
                    if (D_00098950[b].f38 && D_00098950[b].f2C != c->f28)
                        return 0;
                }
            }
        }
        if (road2 != -1) {
            n = collect(c, l2, road2, prev, jn);
            f = l2;
            end = f + n;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        return 0;
                    if (D_00098950[b].f2C != r[c->f24].h && D_00098950[b].f38)
                        return 0;
                }
            }
        }
        if (func_00012E98(c, 1))
            return 0;
        return 1;
    }
    return 1;
}
