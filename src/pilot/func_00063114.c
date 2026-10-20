/* (run 2 match)
 * func_00063114: A* search over the 64x64 cop/path map from map cell `start`
 * (offset x,z from the centre) towards the centre cell D_000C241C. */
typedef struct {
    unsigned short dist;
    unsigned char heapidx;
    unsigned char type;
} CELL;

typedef struct NODE {
    struct NODE *next;
    int data[6];
} NODE;

typedef struct {
    char pad0[0x14];
    int count;          /* 0x14 */
    char pad18[0x40];
    NODE first;         /* 0x58 */
    char pad74[0x790 - 0x74];
    int flag;           /* 0x790 */
} OBJ;

extern int D_000976EC;
extern NODE D_000C4B8C[];
extern int D_0008BA0C[];
extern int D_0008BB4C[];
extern CELL D_000C039C[];
extern unsigned char D_000C241F[];
extern CELL D_000C241C[];
extern CELL *D_000C43B4[];
extern CELL D_000976E0;
extern CELL D_000976E4;
extern int D_000976F4;
extern CELL *func_00061B9C(void);
extern int func_00061F5C();

static inline void push(CELL *c, unsigned short dist)
{
    unsigned int i;
    unsigned int parent;
    CELL *p;

    i = ++D_000976F4;
    for (;;) {
        parent = i >> 1;
        p = D_000C43B4[parent];
        if (dist < p->dist) {
            D_000C43B4[i] = p;
            i = parent;
        } else break;
    }
    D_000C43B4[i] = c;
    c->heapidx = i;
    c->dist = dist;
}

static inline int hdist(CELL *c)
{
    unsigned int idx;
    int dx;
    int dz;

    idx = c - (D_000C241C - 0x820);
    dx = (idx & 0x3F) - 32;
    dz = (idx >> 6) - 32;
    dx = abs(dx);
    dz = abs(dz);
    return (dx + dz) * 2;
}

/* pushSeedNode */
static inline void seed(CELL *c, unsigned short dist)
{
    c->dist = dist;
    push(c, dist);
}

int func_00063114(OBJ *obj, int x, int z, int start)
{
    NODE *p;
    NODE *dst;
    int n;
    int i;
    int k;
    CELL *base;
    CELL *goal;
    CELL *cell;
    CELL *nb;
    unsigned int idx;
    int dx;
    int dz;
    int d;
    int mask;
    int cost;
    int inheap;
    int g;
    unsigned char b;

    if (abs(x) >= 28) return 0;
    if (abs(z) >= 28) return 0;
    if (start != x + z * 64 + 0x820) return 0;
    if ((unsigned)(x + 32) >= 64) return 0;
    if ((unsigned)(z + 32) >= 64) return 0;

    if (obj->flag == 1) {
        n = obj->count;
        dst = D_000C4B8C;
        p = &obj->first;
        while (n--)
            p = p->next;
        for (n = 0; n < D_000976EC; n++) {
            *dst = *p;
            p = p->next;
            dst++;
        }
    }
    if (D_0008BA0C[D_000C241F[0]] == 0x1000)
        return 0;

    D_000C43B4[0] = &D_000976E4;
    D_000976F4 = 0;
    D_000976E4.dist = 0;
    D_000976E0.dist = 0xFFFF;
    for (k = 1; k < 0x1F6; k++)
        D_000C43B4[k] = &D_000976E0;

    d = (abs(x) + abs(z)) * 2;
    seed(&D_000C039C[start], d);

    goal = D_000C241C;
    while (D_000976F4 != 0) {
        cell = func_00061B9C();
        if (cell == goal)
            return func_00061F5C(obj, x, z);
        mask = 0;
        g = cell->dist - hdist(cell);
        if (cell[-1].type & 0x1E) mask = 0x101;
        if (cell[-64].type & 0x1E) mask |= 4;
        if (cell[1].type & 0x1E) mask |= 0x10;
        if (cell[64].type & 0x1E) mask |= 0x40;
        for (i = 7; i != -1; i--) {
            nb = &cell[D_0008BB4C[i]];
            cost = D_0008BA0C[nb->type];
            if (cost >= 0x1000)
                continue;
            if (!(i & 1)) {
                if ((mask >> i) & 1)
                    continue;
                cost += 2;
            }
            idx = nb - (D_000C241C - 0x820);
            dx = (idx & 0x3F) - 32;
            dz = (idx >> 6) - 32;
            cost = g + cost + (abs(dx) + abs(dz)) * 2;
            b = nb->heapidx;
            inheap = b != 0xFF && b != 0;
            if (b != 0 && nb->dist < cost)
                continue;
            nb->dist = cost;
            if (!inheap)
                seed(nb, cost);
        }
    }
    return 0;
}
