/* Driver 2 name unknown (skid/tyre mark pair) */
typedef struct { short x, y, z, pad; } SV;
typedef struct { char pad0[2]; unsigned char model; char pad1[0x11d]; int wl0; int wl1; char pad2[0x64]; } CAR; /* 0x18C */
typedef struct { char pad[0x20]; SV v; char pad2[0x3c]; } MODEL; /* 100 */
extern CAR D_0009B0A4[];
extern MODEL D_0009AF04[];
extern char D_000B9170[];
extern void func_00043FDC();

void func_0004388C(int idx)
{
    SV v;
    char col[4];
    MODEL *m;
    col[0] = 100;
    col[1] = 90;
    col[2] = 90;
    if (idx == 0)
        m = D_0009AF04;
    else
        m = &D_0009AF04[D_0009B0A4[idx].model];
    v = m->v;
    if (D_0009B0A4[idx].wl1 < 500)
        func_00043FDC(&v, idx, col, 20, D_000B9170, 0);
    if (D_0009B0A4[idx].wl0 < 500) {
        v.x = -v.x;
        func_00043FDC(&v, idx, col, 20, D_000B9170, 0);
    }
}
