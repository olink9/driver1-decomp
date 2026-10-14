/* camera update (user-controlled orbit camera) */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short s, c; } CS;

extern long long D_0009687C;
extern long long D_00096DA8;
extern long long D_0009752C;
extern long long D_0008A1E0;
extern long long D_00096C48, D_00096C4A, D_00096C4C;
extern long long D_000970E4;
extern long long D_00096A48;
extern int D_00096A7C;
extern int D_00096A74;
extern int D_00096A78;
extern int D_00096A60;
extern int D_00096A58;
extern int D_00096A6C;
extern VECTOR D_0008A8F0;
extern VECTOR D_000BE9B4;
extern short D_0008EAA8[];
extern int SquareRoot0();
extern int ratan2();
extern void func_00077D6C();
extern void func_0004EB7C();

void func_0003DC00(void)
{
    int dx, dy, dz, d, ang;
    unsigned short pad;

    if (*(int *)&D_0009687C == 0 || *(int *)&D_00096DA8 == 2) {
        pad = *(unsigned short *)&D_0009752C;
        if ((pad & 3) != 3) {
            if (pad & 1) {
                if (D_00096A7C < 2)
                    D_00096A7C++;
                else
                    D_00096A74 = 0x4000000;
            } else if (pad & 2) {
                if (D_00096A7C < 2)
                    D_00096A7C++;
                else
                    D_00096A74 = 0xFC000000;
            } else {
                D_00096A7C = 0;
                D_00096A74 = 0;
            }
        }
        D_00096A78 += (D_00096A74 - D_00096A78) >> 1;
        if ((*(unsigned short *)&D_0009752C & 3) == 3) {
            D_0008A8F0.vx = D_000BE9B4.vx;
            D_0008A8F0.vy = *(int *)&D_0008A1E0 - D_000BE9B4.vy;
            D_0008A8F0.vz = D_000BE9B4.vz + D_00096A60;
        } else {
            D_00096A58 += ((((D_00096A6C - D_00096A58) + 0x800) & 0xfff) - 0x800) / 8;
            D_00096A58 &= 0xfff;
            ang = (D_00096A58 - (D_00096A78 >> 16)) & 0xfff;
            D_0008A8F0.vx = D_000BE9B4.vx + D_0008EAA8[ang * 2] * D_00096A60 / 4096;
            D_0008A8F0.vy = *(int *)&D_0008A1E0 - D_000BE9B4.vy;
            D_0008A8F0.vz = D_000BE9B4.vz + D_0008EAA8[ang * 2 + 1] * D_00096A60 / 4096;
        }
        dx = D_000BE9B4.vx - D_0008A8F0.vx;
        dz = D_000BE9B4.vz - D_0008A8F0.vz;
        dy = 600;
        d = SquareRoot0(dx * dx + dy * dy + dz * dz);
        *(short *)&D_00096C48 = (ratan2(d, D_0008A8F0.vy > 0 ? 600 : -600) - 1200) & 0xfff;
        *(short *)&D_00096C4A = -ratan2(dx, dz) & 0xfff;
        *(short *)&D_00096C4C = 0;
    }
    func_00077D6C(0x100);
    *(int *)&D_000970E4 = 0x100;
    *(int *)&D_00096A48 = 10000;
    func_0004EB7C();
}
