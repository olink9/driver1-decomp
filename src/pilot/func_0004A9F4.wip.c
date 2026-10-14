/* func_0004A9F4 - cell object collision test (returns 0 if pos hits a solid object) */
typedef struct {
    int x, y, z;
    unsigned short pad;
    unsigned short type;
} CELL_OBJECT;

typedef struct {
    unsigned short num;
    unsigned short next;
} PACKED_CELL;

typedef struct {
    char pad0[8];
    short radius;
    char pad1[0x16];
    int solid;
} MODEL;

extern long long D_00097398;    /* map x offset */
extern long long D_00097368;    /* map z offset */
extern long long D_0009733C;    /* region size */
extern long long D_00096A24;    /* cells across */
extern long long D_000970E0;    /* cell ptrs */
extern long long D_000970D4;    /* cells */
extern long long D_00097108;    /* cell objects */
extern int D_0009E734[];        /* regions present */
extern MODEL *D_000A0E20[];     /* models */

static inline int GetCell(int cx, int cz)
{
    int rx, rz, rs, ra, region, idx;

    rs = *(int *)&D_0009733C;
    rx = cx / rs;
    rz = cz / rs;
    ra = *(int *)&D_00096A24 / rs;
    cx -= rx * rs;
    cz -= rz * rs;
    region = rx + rz * ra;
    idx = (rx & 1) + (rz & 1) * 2;

    if (D_0009E734[idx] == region)
        return (*(unsigned short **)&D_000970E0)[idx * rs * rs + cx + cz * rs];
    return -1;
}

int func_0004A9F4(int *pos)
{
    int cell;
    int dx, dz, r;
    unsigned short num;
    PACKED_CELL *pc;
    CELL_OBJECT *co;
    MODEL *m;

    cell = GetCell((pos[0] + *(int *)&D_00097398 - 1500) / 3000, (pos[2] + *(int *)&D_00097368 - 1500) / 3000);

    if (cell != -1) {
        num = cell;
        while (num != 0xFFFF) {
            pc = &(*(PACKED_CELL **)&D_000970D4)[num];
            co = &(*(CELL_OBJECT **)&D_00097108)[pc->num];
            m = D_000A0E20[co->type];
            if (m->solid != 0) {
                dx = co->x - pos[0];
                dz = co->z - pos[2];
                r = m->radius + 0x244;
                if (dx * dx + dz * dz < r * r)
                    return 0;
            }
            num = pc->next;
        }
    }
    return 1;
}
