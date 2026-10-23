/* camera update (user-controlled orbit camera); Driver 2 name unknown */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short s, c; } CS;

extern long long pauseflag;
extern long long D_00096DA8;
extern long long D_0009752C;
typedef struct { int v; int pad[3]; } I8;
extern I8 D_0008A1E0;
typedef struct { short v; short pad[3]; } S8;
extern S8 D_00096C48, D_00096C4A, D_00096C4C;
extern long long D_000970E4;
extern long long D_00096A48;
extern int D_00096A7C;
extern int D_00096A74;
extern int D_00096A78;
extern int D_00096A60;
extern int D_00096A58;
extern int D_00096A6C;
extern VECTOR camera_position;
extern VECTOR D_000BE9B4;
extern short rcossin_tbl[];
extern int SquareRoot0();
extern int ratan2();
extern void func_00077D6C();
extern void func_0004EB7C();

void func_0003DC00(void)
{
    int dx, dy, dz, d, ang, r, t, n, y;
    unsigned short pad;

    if (*(int *)&pauseflag == 0 || *(int *)&D_00096DA8 == 2) {
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
            camera_position.vx = D_000BE9B4.vx;
            camera_position.vy = -D_000BE9B4.vy + D_0008A1E0.v;
            camera_position.vz = D_000BE9B4.vz + D_00096A60;
        } else {
            n = D_00096A6C;
            t = D_00096A58;
            n = (((n - t + 0x800) & 0xfff) - 0x800) / 8;
            r = (t + n) & 0xfff;
            D_00096A58 = r;
            r -= D_00096A78 >> 16;
            ang = r & 0xfff;
            camera_position.vx = D_000BE9B4.vx + rcossin_tbl[ang * 2] * D_00096A60 / 4096;
            y = D_000BE9B4.vy;
            camera_position.vy = D_0008A1E0.v - y;
            camera_position.vz = D_000BE9B4.vz + rcossin_tbl[ang * 2 + 1] * D_00096A60 / 4096;
        }
        dx = D_000BE9B4.vx - camera_position.vx;
        dz = D_000BE9B4.vz - camera_position.vz;
        dy = 600;
        d = SquareRoot0(dx * dx + dy * dy + dz * dz);
        D_00096C48.v = (ratan2(d, camera_position.vy > 0 ? 600 : -600) - 1200) & 0xfff;
        D_00096C4A.v = -ratan2(dx, dz) & 0xfff;
        D_00096C4C.v = 0;
    }
    func_00077D6C(0x100);
    *(int *)&D_000970E4 = 0x100;
    *(int *)&D_00096A48 = 10000;
    func_0004EB7C();
}
