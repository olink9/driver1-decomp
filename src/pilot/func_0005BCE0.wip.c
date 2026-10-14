typedef struct { char p0; unsigned char f1; char pad[0x4e]; } S50;

extern S50 D_00098900[];
extern long long D_00096897;
extern long long D_00096898;
extern long long D_0009688C;
extern int D_0008A1E0[];
extern char D_0009B0A4[];
extern char D_0009D438[];
extern int D_0008A8F0[];
extern int func_00067EE0();
extern int func_0005BDF8();

int func_0005BCE0(int a)
{
    S50 *p = &D_00098900[a];
    char *base;

    if (*(unsigned char *)&D_00096897 == 0) return 0;
    if (*(unsigned char *)&D_00096897 == 2 && a != 0 && p->f1 == 0) {
        *(int *)&D_0009688C = 0;
        D_0008A1E0[0] = -300;
        return 1;
    }
    if (*(unsigned char *)&D_00096898 != 3)
        base = D_0009B0A4;
    else
        base = D_0009D438;
    if (func_00067EE0(base + 0xC8, D_0008A8F0) >= 0x55F1) {
        *(unsigned char *)&D_00096897 = 0;
        D_0008A1E0[0] = -300;
    } else {
        if (func_0005BDF8(D_0008A8F0[0], D_0008A8F0[2])) return 0;
        *(unsigned char *)&D_00096897 = 2;
    }
    *(int *)&D_0009688C = 0;
    return 1;
}
