/* func_000143CC - no Driver 2 counterpart: gather the objects of the cells around the player (offset table D_00099B08) */
typedef struct {
    short model;
    char flag;
    char pad;
    int x;
    int z;
} CELLOBJ;

typedef struct {
    int x;
    int z;
} CELLPOS;

extern int D_00096780;
extern int D_0009677C;
extern int D_00096FD4;
extern int D_00096FD8;
extern long long D_000967A8;
extern long long D_00097044;
extern long long D_00097374;
extern int D_0009B0A4[];
extern int D_0009E714[];
extern short *D_0009E724[];
extern int *D_0009D8F4[];
extern short D_00099B08[];
extern CELLOBJ D_00099180[];
extern CELLPOS D_00099720[];

static inline int cellmid(int c)
{
    return c * 1500 + 750;
}

void func_000143CC(void)
{
    short *ofs;
    CELLOBJ *obj;
    CELLPOS *pos;
    int cx, cz;
    int i;
    int x, z;
    int idx;
    int val;
    int cell;
    int tx, tz;
    int qx, qz;
    unsigned short uval;
    int wx;

    ofs = D_00099B08;
    obj = D_00099180;
    pos = D_00099720;
    D_00096FD4 = 0;
    D_00096FD8 = 0;
    if (D_00096780 != 0 || D_0009677C != 0) {
        cx = (D_0009B0A4[0x245C / 4] + D_0009E714[0] * 750) / 1500;
        cz = (D_0009E714[1] * 750 - D_0009B0A4[0x2464 / 4]) / 1500;
    } else {
        cx = (D_0009B0A4[0xC8 / 4] + D_0009E714[0] * 750) / 1500;
        cz = (D_0009E714[1] * 750 - D_0009B0A4[0xD0 / 4]) / 1500;
    }
    for (i = 0; i < 120; i++, ofs += 2) {
        x = cx + ofs[0];
        z = cz + ofs[1];
        if (x < 0 || x >= D_0009E714[0] || z < 0 || z >= D_0009E714[1])
            continue;
        tz = z;
        tx = x;
        qz = tz / 60;
        qx = tx / 60;
        x = tx - qx * 60;
        z = tz - qz * 60;
        qz = *(int *)&D_00097374 - qz;
        idx = (qx & 1) + ((qz & 1) << 1);
        val = ((short (*)[60])D_0009E724[idx])[z][x];
        uval = ((unsigned short (*)[60])D_0009E724[idx])[z][x];
        if (val == -0x8000) {
            pos->x = cellmid(tx) - D_0009E714[2];
            D_00096FD8++;
            pos->z = D_0009E714[3] - tz * 1500;
            pos++;
            continue;
        }
        cell = D_0009D8F4[idx][z * 60 + x];
        obj->flag = cell >> 30;
        if (cell & 2) {
            obj->model = -1;
        } else {
            if (val < 0)
                continue;
            if (*(int *)&D_000967A8 < val)
                continue;
            if (*(short *)(*(char **)&D_00097044 + val * 16 + 8) < 4)
                continue;
        }
        if (!(cell & 2))
            obj->model = uval;
        obj->x = cellmid(tx) - D_0009E714[2];
        D_00096FD4++;
        obj->z = D_0009E714[3] - tz * 1500;
        obj++;
    }
}
