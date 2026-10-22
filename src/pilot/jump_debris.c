typedef struct { int vx, vy, vz, pad; } V;
typedef struct { unsigned char c; char p[3]; } Wh;
typedef struct { char p00[0x78]; Wh wh[4]; char p0[0x40]; int x, y, z; } Car;
extern unsigned char D_0009678C;
extern int D_00097010;
extern long long D_00096834;
extern void func_00020998();
extern void memset();
extern int func_000776E4();
extern void Setup_Debris();

static inline int jit(void) { int r = func_000776E4() & 0x1ff; return r - 256; }
/* Driver 2: jump_debris */
void jump_debris(Car *cp)
{
    int count;
    V position, velocity;

    for (count = 0; count < 4; count++) {
        if (cp->wh[count].c != 0) {
            D_0009678C = 0;
            D_00097010 = 1;
            return;
        }
    }
    if (D_00097010) {
        D_0009678C = 80;
        D_00097010 = 0;
        if (*(int *)&D_00096834 != 6)
            func_00020998(cp);
    }
    if (D_0009678C != 0 && --D_0009678C < 75) {
        memset(&velocity, 0, 16);
        velocity.vx = cp->x + jit();
        velocity.vy = 200 - cp->y;
        velocity.vz = cp->z + jit();
        position = velocity;
        memset(&velocity, 0, 16);
        Setup_Debris(&position, &velocity, 5, 0xb);
    }
}
