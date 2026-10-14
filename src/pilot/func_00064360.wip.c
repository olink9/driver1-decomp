/* func_00064360 - no Driver 2 counterpart */
extern int D_0008BA0C[];
extern long long D_00097374;
extern int D_0009770C;
extern int D_00097724;
extern int D_00097738;
extern int D_0009773C;
extern int D_00097748;
extern int D_0009774C;
extern int D_0009B0A4[];
extern int (*D_0009D8F4[])[60];
extern int D_0009E714[];
extern int func_000641C0();
extern void func_00061EDC();

typedef struct {
    char pad0[0x48];
    int x;
    int z;
    int cx;
    int cz;
    char pad58[0x75C - 0x58];
    int px;
    int py;
    int pz;
} OBJ;

static inline int GetCellType(int x, int z)
{
    int zq, zoff, sel, xq, xoff;
    unsigned short *c;

    zq = z / 60;
    zoff = (z % 60) * 240;
    sel = ((*(int *)&D_00097374 - zq) << 1) & 2;
    xq = x / 60;
    xoff = (x % 60) * 4;
    c = (unsigned short *)((char *)D_0009D8F4[sel | (xq & 1)] + zoff + xoff);
    return D_0008BA0C[(unsigned short)(((c[1] & 0x3C00) >> 9) | (c[2] & 1))];
}

void func_00064360(OBJ *obj, int *dx, int *dz, int force)
{
    int cz;
    int cx;
    int px;
    int pz;
    int flag;
    int v;

    cz = (D_0009E714[3] - (D_0009B0A4[52] - 750)) / 1500;
    cx = (D_0009B0A4[50] + D_0009E714[2]) / 1500;
    v = GetCellType(cx, cz);
    flag = 0;
    obj->x = D_00097738;
    obj->z = D_0009773C;
    if (force == 0) {
        if (D_0009770C > 0 && (D_00097724 == 0 || v != 0x1000)) {
            obj->x = D_0009B0A4[50];
            D_00097724 = 1;
            D_00097738 = D_0009B0A4[50];
            obj->z = D_0009B0A4[52];
            D_0009773C = D_0009B0A4[52];
        } else {
            flag = 1;
        }
        if (func_000641C0(obj))
            flag = 1;
    } else {
        obj->x = D_0009B0A4[50];
        obj->z = D_0009B0A4[52];
    }
    if (flag) {
        cx = (obj->x + D_0009E714[2]) / 1500;
        obj->cx = cx;
        cz = (D_0009E714[3] - (obj->z - 750)) / 1500;
        obj->cz = cz;
    } else {
        cx = (obj->x + D_0009E714[2]) / 1500;
        obj->cx = cx;
        cz = (D_0009E714[3] - (obj->z - 750)) / 1500;
        obj->cz = cz;
    }
    px = (obj->px + D_0009E714[2]) / 1500;
    pz = (D_0009E714[3] - (obj->pz - 750)) / 1500;
    D_00097748 = cx;
    D_0009774C = cz;
    func_00061EDC(cx, cz, obj);
    *dx = cx - px;
    *dz = cz - pz;
}
