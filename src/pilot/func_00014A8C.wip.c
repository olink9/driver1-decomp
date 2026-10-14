/* func_00014A8C */
typedef struct {
    unsigned char f0, f1, f2, f3, f4, f5;
    char pad6[0x10 - 6];
    unsigned char f10, f11, f12, f13, f14, f15, f16, f17;
    int f18;
    char pad1c[0x24 - 0x1c];
    int f24;
    int f28;
    char pad2c[0x34 - 0x2c];
    int f34;
    char pad38[0x40 - 0x38];
    int f40;
    int f44;
    char pad48[0x50 - 0x48];
} ENT;

typedef struct { int vx, vy, vz; } VECTOR;
typedef struct {
    char pad0[0x18];
    int f18;
    char pad1c[0x20 - 0x1c];
    int f20;
    char pad24[0xC8 - 0x24];
    VECTOR pos;
    char padD4[0x18C - 0xD4];
} CAR;

extern ENT D_00098950[];
extern CAR D_0009B230[];
extern int D_0008A8F0[];
extern int D_0009784C[];
extern long long D_000973B4;
extern long long D_000973B8;
extern long long D_00096990;
extern long long D_00097530;
typedef struct { char *primptr; unsigned long *ot; char *primtab; } DB;
#define current (*(DB **)&D_00097530)

extern int func_000398B0();
extern void func_0003A698();
extern void func_00043E8C();
extern void func_00046000();
extern void func_000439E4();
extern void func_00043D0C();

void func_00014A8C(void)
{
    unsigned char order[32];
    unsigned char n;
    int i, j, k;
    int camx, camz;
    int d, dx, dz;
    int t;
    unsigned char o;
    int idx;
    ENT *e;
    CAR *car;
    int *cur;
    char *p;
    CAR *c;
    int r;
    int a1;

    n = 0;
    camx = D_0008A8F0[0];
    camz = D_0008A8F0[2];
    *(int *)&D_000973B4 = 0;
    for (i = 0; i < 25; i++) {
        if (D_00098950[i].f1 != 0 && func_000398B0(&D_0009B230[i].pos) == 1) {
            dx = camx - D_0009B230[i].pos.vx;
            if (dx < 0) dx = D_0009B230[i].pos.vx - camx;
            dz = camz - D_0009B230[i].pos.vz;
            if (dz < 0) dz = D_0009B230[i].pos.vz - camz;
            d = dx + dz;
            if (d < 27000) {
                D_0009784C[n] = d;
                order[n++] = i;
            }
        }
    }
    if (n == 0)
        return;
    *(int *)&D_000973B8 = 0;
    for (j = 1; j < n; j++) {
        o = order[j];
        t = D_0009784C[j];
        for (k = j; k != 0 && t < D_0009784C[k - 1]; k--) {
            order[k] = order[k - 1];
            D_0009784C[k] = D_0009784C[k - 1];
        }
        order[k] = o;
        D_0009784C[k] = t;
    }
    for (i = 0; i < n; i++) {
        idx = order[i];
        e = &D_00098950[idx];
        cur = *(int **)&D_00097530;
        p = current->primptr - 0x1E000; r = current->primtab - p - 3000;
        *(int *)&D_00096990 = e->f11;
        if (r < 0)
            return;
        r -= (n - i - 1) << 11;
        if (r < 4000)
            *(int *)&D_000973B8 = 1;
        idx++;
        func_0003A698(idx, e->f10);
        *(int *)&D_00096990 = 0;
        func_00043E8C(idx);
        if (e->f10 == 1 && ((e->f5 != 0 && e->f1 == 4) || e->f1 == 9))
            func_00046000(idx);
        if (e->f17 & 1) {
            car = &D_0009B230[e->f0];
            a1 = e->f24;
            if (a1 & 1) {
                if (e->f44 != 0x7FFFFFFF) {
                    if (e->f44 < car->f20) {
                        if (a1 == 3) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    } else if (car->f20 < e->f44) {
                        if (a1 == 1) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    }
                }
            } else {
                if (e->f40 != 0x7FFFFFFF) {
                    if (e->f40 < car->f18) {
                        if (a1 == 0) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    } else if (car->f18 < e->f40) {
                        if (a1 == 2) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    }
                }
            }
        } else if (e->f1 != 1 && ((e->f28 & 0x4000) || e->f1 == 3 || e->f18 < 5000)) {
            switch (e->f34) {
            case -1: func_000439E4(idx, 0); break;
            case 1: func_000439E4(idx, 1); break;
            }
        }
        if (e->f17 & 0x10)
            func_000439E4(idx, 0);
        if (e->f17 & 0x20)
            func_000439E4(idx, 1);
        if (e->f15)
            func_00043D0C(idx);
    }
}
