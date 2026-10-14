/* no Driver 2 counterpart */
typedef struct {
    unsigned char c0;
    unsigned char c1;
    unsigned char c2;
    unsigned char c3;
    int pad4;
    unsigned char c8;
    char pad9[0x1C - 9];
    int i1C;
    int i20;
    int i24;
    int i28;
    int i2C;
    int pad30;
    int i34;
    int i38;
    int pad3C[5];
} Entry;

typedef struct {
    unsigned char c0;
    char pad1[0x1F];
    int i20;
    int pad24[2];
    int i2C;
    unsigned char c30;
} Actor;

typedef struct {
    int pad0[2];
    short s8;
    short padA;
    int padC;
} Node;

extern Entry D_00098950[];
extern long long D_00097044;
#define nodes (*(Node **)&D_00097044)

static inline int check1(Actor *a)
{
    int i;
    Entry *p;
    unsigned char id = a->c0;
    int road = a->i20;

    for (p = D_00098950, i = 0; i < 22; i++, p++) {
        if (p->c8 && p->c0 != id && p->c3 && p->i20 == road)
            return 0;
    }
    return 1;
}
static inline int check2(Actor *a, int idx)
{
    int i;
    Entry *p;
    int id = a->c0;
    int road = a->i20;
    unsigned char lane = a->c30;

    for (p = D_00098950, i = 0; i < 22; i++, p++) {
        if (!p->c8 || p->c0 == id)
            continue;
        if (p->i28 == idx) {
            if (p->i24 == lane)
                return 1;
        } else if (p->c3 && p->i20 == road && p->i2C == idx) {
            return 1;
        }
    }
    return 0;
}
static inline int check3(Actor *a)
{
    int i;
    Entry *p;
    int id = a->c0;
    int idx = a->i2C;
    unsigned char lane = a->c30;
    int lim;

    p = D_00098950;
    if (p[id].i34)
        lim = 6000;
    else
        lim = 3000;
    for (i = 0; i < 22; i++, p++) {
        if (p->c1 >= 2 && p->c0 != id
            && p->i28 == idx && p->i24 == lane
            && p->i1C < lim && p->i38 < 30)
            return 1;
    }
    return 0;
}
int func_00012E98(Actor *a, int mode)
{
    if (mode == 1 && !check1(a))
        return 1;
    if (nodes[a->i2C].s8 >= 4 || !check2(a, a->i2C))
        return check3(a);
    return 1;
}
