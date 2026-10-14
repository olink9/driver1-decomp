typedef struct { int vx, vy, vz; } V3;
typedef struct { char c[4]; } C4 __attribute__((aligned(1)));
typedef struct { unsigned char r, g, b, a; } RGBA;

extern long long D_000970D8;
extern V3 D_0008A8F0;
extern void func_000774C4();
extern int func_000776E4();
extern void func_00047A90();
extern void func_000466D4();
extern void func_000451A8();

void func_0004A83C(V3 *a0)
{
    V3 pos;
    V3 vel;
    RGBA col;
    RGBA tmp;

    func_000774C4(&tmp, 0, 4);
    tmp.r = (func_000776E4() & 0xF) + 0x10;
    tmp.g = func_000776E4() & 0xF;
    col = tmp;
    if (*(int *)&D_000970D8 & 1) {
        pos.vx = a0->vx - 0x40;
        pos.vz = a0->vz - 0x40;
        pos.vy = a0->vy - 0x15E;
        pos.vx += func_000776E4() & 0x7F;
        pos.vz += func_000776E4() & 0x7F;
        vel.vy = -10;
        vel.vx = 0;
        vel.vz = 0;
        func_00047A90(&pos, 80, 100, 4, 0, &vel, 0);
        if ((*(int *)&D_000970D8 & 3) == 3) {
            func_000466D4(&vel);
            pos.vy -= 100;
            func_00047A90(&pos, 100, 500, 1, 1, &vel, 0);
        }
    }
    pos.vx = a0->vx - D_0008A8F0.vx;
    pos.vy = -a0->vy - D_0008A8F0.vy;
    pos.vz = a0->vz - D_0008A8F0.vz;
    func_000451A8(&pos, &col, (func_000776E4() & 0x7F) + 0x1F4);
}
