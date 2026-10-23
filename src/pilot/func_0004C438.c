/* no Driver 2 counterpart */
typedef struct { int vx, vy, vz, pad; } VECTOR;

extern VECTOR D_0009B16C;
extern int D_000BC9AC[];
extern int D_000BCA8C[];
extern int D_000974C8;
extern int D_000974C0;
extern long long D_00096840;
extern void Start3DSoundVolPitch();

void func_0004C438(void)
{
    D_000974C8 = 0;
    D_000BC9AC[0] = -1;
    D_000BCA8C[0] = 1;
    D_000974C0 = 0;
    Start3DSoundVolPitch(1, 0, 2, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
    Start3DSoundVolPitch(0, 0, 1, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
    Start3DSoundVolPitch(3, 0, 3, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
    Start3DSoundVolPitch(5, 0, 4, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
    Start3DSoundVolPitch(6, 0, 4, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
    Start3DSoundVolPitch(7, 0, 1, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
    Start3DSoundVolPitch(8, 0, 1, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
    if (*(int *)&D_00096840)
        Start3DSoundVolPitch(4, 0, 4, D_0009B16C.vx, D_0009B16C.vy, D_0009B16C.vz, -10000, 4096);
}
