/* PlaceCameraAtLocation */
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { char a[0xC8]; int bx, by, bz; char b[0x18C - 0xD4]; } PL;
extern long long D_00097658, D_00096897, D_000970E4, D_00096A48;
extern int D_000973C8;
#define B(x) (*(unsigned char *)&x)
#define I(x) (*(int *)&x)
extern VEC D_0008A8F0;
typedef struct { short vx, vy, vz, pad; } SVEC;
extern SVEC D_00096C48;
extern PL D_0009B0A4[];
extern int func_0003D600();
extern void func_00077D6C(), func_0004EB7C();

void func_0003D4D0(int idx, int zoom)
{
    int d;
    VEC temp;
    PL *lp;
    d = 0;
    lp = &D_0009B0A4[idx];
    if (B(D_00097658)) {
        temp.vx = lp->bx;
        temp.vy = -lp->by;
        temp.vz = lp->bz;
        d = func_0003D600(&D_0008A8F0, &temp, &D_00096C48);
        D_000973C8 = d;
        if (d > 0x6978) {
            B(D_00096897) = 0;
            return;
        }
        if (zoom) {
            I(D_000970E4) = (d >> 4) + 0x100;
            if (I(D_000970E4) > 800) I(D_000970E4) = 800;
        } else
            I(D_000970E4) = 0x100;
    }
    if (I(D_000970E4) < 0x100)
        I(D_000970E4) = 0x100;
    func_00077D6C(I(D_000970E4));
    I(D_00096A48) = 10000 + (d << 2);
    func_0004EB7C();
}
