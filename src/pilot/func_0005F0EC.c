typedef struct {
    int x, y, z;
    char pad0C[0x1A - 0x0C];
    unsigned short f1A;
    char pad1C[0x3C - 0x1C];
    int flags;
    char pad40[0x50 - 0x40];
    unsigned char f50, f51;
    char pad52[4];
    unsigned char f56;
    char pad57;
} ENT;
typedef struct { int x, z; } POS;
typedef struct { short f0; unsigned short f2; } SURF;

extern long long D_00096FD8;
extern int D_000976C0;
extern POS D_00099720[];
extern ENT D_000BFA7C[];
extern int D_000C0340[];
extern short D_000C0390[];
extern SURF *D_000A0E20[];
extern void func_00060E4C(void);
extern short func_00021558(int, int);
extern void func_000217C8(int, int, short *);
extern int func_0004FFE8(int);
extern void func_0005FFB0(ENT *);

static inline int AllocEnt(void)
{
    int i;

    if (D_000976C0 >= 20)
        return -1;
    i = D_000C0340[D_000976C0++];
    D_000BFA7C[i].flags = 0x10;
    return i;
}

void func_0005F0EC(void)
{
    char pad[0x20];
    short surf;
    int n;
    POS *pos;
    ENT *e;
    ENT *p;
    int i;
    int dist;
    int idx;

    n = *(int *)&D_00096FD8;
    pos = D_00099720;
    func_00060E4C();
    while (n--) {
        if (D_000976C0 >= 20)
            return;
        p = D_000BFA7C;
        dist = 0x3D10D0;
        for (i = 19; i != -1; p++, i--) {
            if (p->flags & 0x10) {
                dist = (p->x - pos->x) * (p->x - pos->x) + (p->z - pos->z) * (p->z - pos->z);
                if (p->flags & 0x200) {
                    if (dist <= 0x22550FF) {
                        dist = 1;
                        break;
                    }
                } else if (dist <= 0x3D08FF) {
                    break;
                }
            }
        }
        if (dist <= 0x3D08FF) {
            pos++;
            continue;
        }
        {
            idx = AllocEnt();
            if (idx == -1)
                return;
            e = &D_000BFA7C[idx];
            e->f51 = 0;
            e->x = pos->x;
            e->z = pos->z;
            e->y = 2000;
            D_000C0390[0] = func_00021558(e->x, e->z);
            D_000C0390[1] = func_00021558(e->x, e->z - 1500);
            D_000C0390[2] = func_00021558(e->x, e->z + 1500);
            D_000C0390[3] = func_00021558(e->x + 1500, e->z);
            D_000C0390[4] = func_00021558(e->x - 1500, e->z);
            func_000217C8(e->x, e->z, &surf);
            if ((func_0004FFE8(1) & 1) || !(D_000A0E20[surf]->f2 & 0x4000)) {
                e->f56 = 2;
                e->f50 = 10;
            } else {
                e->f56 = 3;
                e->f50 = 30;
            }
            if (!(idx & 3))
                e->flags |= 0x4000;
            func_0005FFB0(e);
            pos++;
            e->f1A &= 0x3FF;
        }
    }
}
