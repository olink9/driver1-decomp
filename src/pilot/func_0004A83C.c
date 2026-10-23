typedef struct { int vx, vy, vz; } V3;
typedef struct { char c[4]; } C4 __attribute__((aligned(1)));
typedef struct { unsigned char r, g, b, a; } RGBA;

extern long long CameraCnt;
extern V3 camera_position;
extern void memset();
extern int func_000776E4();
extern void Setup_Smoke();
extern void GetSmokeDrift();
extern void RoundShadow();

void func_0004A83C(V3 *a0)
{
    V3 pos;
    V3 vel;
    RGBA col;
    RGBA tmp;

    memset(&tmp, 0, 4);
    tmp.r = (func_000776E4() & 0xF) + 0x10;
    tmp.g = func_000776E4() & 0xF;
    col = tmp;
    if (*(int *)&CameraCnt & 1) {
        pos.vx = a0->vx - 0x40;
        pos.vz = a0->vz - 0x40;
        pos.vy = a0->vy - 0x15E;
        pos.vx += func_000776E4() & 0x7F;
        pos.vz += func_000776E4() & 0x7F;
        vel.vy = -10;
        vel.vx = 0;
        vel.vz = 0;
        Setup_Smoke(&pos, 80, 100, 4, 0, &vel, 0);
        if ((*(int *)&CameraCnt & 3) == 3) {
            GetSmokeDrift(&vel);
            pos.vy -= 100;
            Setup_Smoke(&pos, 100, 500, 1, 1, &vel, 0);
        }
    }
    pos.vx = a0->vx - camera_position.vx;
    pos.vy = -a0->vy - camera_position.vy;
    pos.vz = a0->vz - camera_position.vz;
    RoundShadow(&pos, &col, (func_000776E4() & 0x7F) + 0x1F4);
}
