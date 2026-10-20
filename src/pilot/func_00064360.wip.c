/* WIP 97.07 (run2-r3 r3-cell). func_00064360 - no Driver 2 counterpart.
   Found: `cx = sum; cx /= 1500;` (sum lands in the cx s-reg) and in GetCellType `xq = x / 60` placed between
   zoff and sel (95.49 with one shared cz). This file uses a separate top-level `tz` (97.07, but the target keeps the
   top cz in s3 = same pseudo as the tail cz, so tz is probably not the original). Remaining with shared cz:
   global-alloc priority - cz (21 refs / 75 insns) outranks cx (21 / 94) and takes s2; target has cx in s2, cz in s3.
   Tail statement order, decl order, extra refs (cx = a; cx += b) and obj->cz re-reads did not flip it.
   Also missing: `andi 0xffff` on the D_0008BA0C index (ushort local / ushort-returning inline / short *c all fold it). */
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
    xq = x / 60;
    sel = ((*(int *)&D_00097374 - zq) << 1) & 2;
    xoff = (x % 60) * 4;
    c = (unsigned short *)((char *)D_0009D8F4[sel | (xq & 1)] + zoff + xoff);
    return D_0008BA0C[(unsigned short)(((c[1] & 0x3C00) >> 9) | (c[2] & 1))];
}

void func_00064360(OBJ *obj, int *dx, int *dz, int force)
{
    int tx, tz;
    int cz;
    int cx;
    int px;
    int pz;
    int flag;
    int v;
    int *car;

    car = D_0009B0A4;
    tz = D_0009E714[3] - (car[52] - 750);
    tz /= 1500;
    cx = car[50] + D_0009E714[2];
    cx /= 1500;
    v = GetCellType(cx, tz);
    flag = 0;
    obj->x = D_00097738;
    obj->z = D_0009773C;
    if (force == 0) {
        if (D_0009770C > 0 && (D_00097724 == 0 || v != 0x1000)) {
            D_00097724 = 1;
            D_00097738 = obj->x = car[50];
            D_0009773C = obj->z = car[52];
        } else {
            flag = 1;
        }
        if (func_000641C0(obj))
            flag = 1;
    } else {
        obj->x = car[50];
        obj->z = car[52];
    }
    if (flag) {
        cx = obj->x + D_0009E714[2];
        cz = D_0009E714[3] - (obj->z - 750);
        cx = obj->cx = cx / 1500;
        cz = obj->cz = cz / 1500;
    } else {
        cx = obj->x + D_0009E714[2];
        cz = D_0009E714[3] - (obj->z - 750);
        cx = obj->cx = cx / 1500;
        cz = obj->cz = cz / 1500;
    }
    px = (obj->px + D_0009E714[2]) / 1500;
    pz = (D_0009E714[3] - (obj->pz - 750)) / 1500;
    D_00097748 = cx;
    D_0009774C = cz;
    func_00061EDC(cx, cz, obj);
    *dx = cx - px;
    *dz = cz - pz;
}
