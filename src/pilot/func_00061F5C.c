/* (run 2 match, b125)
 * func_00061F5C: turn the A* result of func_00063114 into the cop route node list:
 * walk back from the centre cell along falling dist values, build NODEs (prepending
 * the D_000C4B8C nodes when obj->flag == 1), convert to world coords, add corner
 * nodes at diagonal moves, drop collinear/duplicate nodes, compute angles (ratan2)
 * and reject routes with a turn >= 90 degrees. Returns count <= 64.
 * Notes: `mode = obj->flag;` (a user variable) is what stops jump threading of the
 * repeated flag test; `cur = D_000C241C;` gives cse the register base for the
 * `- 0x2080` address. */
typedef struct {
    unsigned short dist;
    unsigned char heapidx;
    unsigned char type;
} CELL;

typedef struct NODE {
    struct NODE *next;   /* 0x00 */
    struct NODE *prev;   /* 0x04 */
    int x;               /* 0x08 */
    int z;               /* 0x0C */
    CELL *cell;          /* 0x10 */
    short cost;          /* 0x14 */
    unsigned char f16;   /* 0x16 */
    unsigned char f17;
    short angle;         /* 0x18 */
    unsigned char dir;   /* 0x1A */
    unsigned char extra; /* 0x1B */
} NODE;

typedef struct {
    char pad0[0x48];
    int x;              /* 0x48 */
    int z;              /* 0x4C */
    char pad50[8];
    NODE first[66];     /* 0x58 */
    int flag;           /* 0x790 */
} OBJ;

extern int D_000976E8;
#define count D_000976E8
extern int D_000976EC;
extern NODE D_000C4B8C[];
extern int D_0008BA0C[];
extern int D_0008BB4C[];
extern CELL D_000C241C[];
extern int ratan2();

#define ABS(x) ((x) < 0 ? -(x) : (x))

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

#define INSERT(DX, DZ)                          \
    do {                                        \
        n = &nodes[count];            \
        if (++count > 64) return 0;        \
        *n = *p;                                \
        p->next = n;                            \
        n->extra = 1;                           \
        n->f16 = 0;                             \
        n->cost = 0;                            \
        n->next = next;                         \
        n->prev = p;                            \
        next->prev = n;                         \
        n->x += DX;                             \
        n->z += DZ;                             \
    } while (0)

int func_00061F5C(OBJ *obj, int x, int z)
{
    CELL *path[64];
    CELL *goal;
    CELL *end;
    CELL *cur;
    CELL *best;
    CELL *nb;
    CELL *c;
    NODE *nodes;
    NODE *p;
    NODE *next;
    NODE *prev;
    NODE *nn;
    NODE *n;
    unsigned int idx;
    int ox;
    int oz;
    int dist;
    int bestd;
    int d;
    int i;
    int dx;
    int dz;
    int dir;
    int mode;
    int pa;
    int na;

    nodes = obj->first;
    if (obj->x >= 0) ox = obj->x + 750; else ox = obj->x - 750;
    oz = obj->z + 750;
    if (obj->z < 0) oz = obj->z - 750;
    goal = D_000C241C;
    dist = goal->dist;
    count = 0;
    ox = (ox / 1500) * 1500;
    oz = (oz / 1500) * 1500;
    path[count++] = goal;
    cur = goal;
    end = &goal[x + z * 64];
    while (cur != end && count < 64) {
        best = cur;
        bestd = dist;
        for (i = 7; i >= 0; i--) {
            nb = &cur[D_0008BB4C[i]];
            d = nb->dist - hdist(nb);
            if (d < bestd) {
                best = nb;
                bestd = d;
            }
        }
        cur = best;
        path[count++] = cur;
        dist = bestd;
    }
    if (cur != end)
        return 0;
    if (count > 64)
        return 0;
    cur = D_000C241C;

    p = nodes + count - 1;
    if (obj->flag == 1)
        p += D_000976EC;
    if (count + D_000976EC > 64)
        return 0;
    for (i = 0; i < count; i++, p--) {
        cur = path[i];
        idx = cur - (D_000C241C - 0x820);
        p->x = (idx & 0x3F) - 32;
        p->z = 32 - (idx >> 6);
        p->cell = cur;
        p->cost = D_0008BA0C[cur->type];
        cur->heapidx = 0xAB;
        if (i != 0) p->next = p + 1; else p->next = 0;
        if (i == count - 1) p->prev = 0; else p->prev = p - 1;
        p->f16 = 0;
        p->extra = 0;
    }

    p = nodes;
    if (obj->flag == 1) {
        for (i = 0; i < D_000976EC; i++, p++) {
            *p = D_000C4B8C[i];
            if (i == 0) p->prev = 0; else p->prev = p - 1;
            p->f16 = 1;
            p->next = p + 1;
        }
        count += D_000976EC;
        p->prev = p - 1;
    }

    mode = obj->flag;
    if (mode == 1)
        p = &nodes[D_000976EC];
    else
        p = nodes;
    for (; p; p = next) {
        next = p->next;
        if (p->extra)
            continue;
        if (next != 0) {
        dx = p->x - next->x;
        dz = next->z - p->z;
        if (dz < 0) {
            if (dx < 0) dir = 7;
            else if (dx > 0) dir = 1;
            else dir = 0;
        } else if (dz > 0) {
            if (dx < 0) dir = 5;
            else if (dx > 0) dir = 3;
            else dir = 4;
        } else {
            if (dx < 0) dir = 6;
            else dir = 2;
        }
        p->dir = dir;
        if (p->f16 == 0) {
            p->x = p->x * 1500 + ox;
            p->z = p->z * 1500 + oz;
        }
        if (p->cost != 9)
        switch (dir) {
        case 5:
            if (p->cell[-64].type & 0x1E) INSERT(0x465, 0x177);
            else if (p->cell[1].type & 0x1E) INSERT(0x177, 0x465);
            break;
        case 3:
            if (p->cell[-64].type & 0x1E) INSERT(-0x465, 0x177);
            else if (p->cell[-1].type & 0x1E) INSERT(-0x177, 0x465);
            break;
        case 1:
            if (p->cell[64].type & 0x1E) INSERT(-0x465, -0x177);
            else if (p->cell[-1].type & 0x1E) INSERT(-0x177, -0x465);
            break;
        case 7:
            if (p->cell[64].type & 0x1E) INSERT(0x465, -0x177);
            else if (p->cell[1].type & 0x1E) INSERT(0x177, -0x465);
            break;
        }
        } else {
            p->x = p->x * 1500 + ox;
            p->z = p->z * 1500 + oz;
        }
    }

    mode = obj->flag;
    if (mode == 1)
        p = &nodes[D_000976EC];
    else
        p = nodes;
    for (; p; p = next) {
        prev = p->prev;
        next = p->next;
        if (prev == 0 || prev->extra || next == 0)
            continue;
        if (next->extra) {
            prev->next = next;
            next->prev = prev;
            count--;
            continue;
        }
        nn = next->next;
        if (nn == 0)
            continue;
        if (p->extra) {
            if (nn->extra == 0 || p->dir == nn->dir)
                goto remove;
            continue;
        } else {
            pa = p->dir;
            na = next->dir;
            if ((pa & 1) ? (na & 1) : !(na & 1))
                continue;
            if (p->dir == nn->dir && nn->extra == 0) {
                next = nn->next;
                p->next = next;
                if (next)
                    next->prev = p;
                count -= 2;
                continue;
            }
        }
    remove:
        next = nn;
        p->next = next;
        next->prev = p;
        count--;
    }

    for (p = nodes; p; p = next) {
        next = p->next;
        if (p->cost == 9 && p->prev) {
            switch (p->prev->dir) {
            case 2:
            case 6:
                if (p->cell->type & 6) p->z -= 0x177;
                else p->z += 0x177;
                break;
            case 0:
            case 4:
                if (p->cell->type & 0xA) p->x += 0x177;
                else p->x -= 0x177;
                break;
            case 1:
            case 3:
            case 5:
            case 7:
                break;
            }
        }
    }

    for (p = nodes; p; p = next) {
        next = p->next;
        if (next && p->x == next->x && p->z == next->z) {
            next = next->next;
            p->next = next;
            if (next)
                next->prev = p;
            count--;
            next = p;
        }
    }

    if (count <= 0)
        return 0;

    for (p = nodes; p; p = next) {
        next = p->next;
        if (next)
            p->angle = ratan2(next->x - p->x, next->z - p->z) & 0xFFF;
        else
            p->angle = -1;
    }

    for (p = nodes; p; p = next) {
        next = p->next;
        if (next && next->angle != -1) {
            if (ABS(((p->angle - next->angle + 0x800) & 0xFFF) - 0x800) >= 0x400)
                return 0;
        }
    }
    return count <= 64;
}
