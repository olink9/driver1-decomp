/* NOTE: D_001E4038 / D_001E40A0 are absolute addresses 0x1E4038 / 0x1E40A0 (memory past bss);
   they need linker/symbol_addrs definitions at those addresses. Code is byte-identical then. */
extern long long D_001E4038, D_001E40A0;
#define CNT (*(int *)&D_001E4038)
typedef struct { int a, b, c; } COP;
typedef struct { COP *cop; char active; char pad[3]; int rot; short vx, vy, vz; short p2; int c0; } DAM;
extern int D_0009741C, D_0009745C;
extern long long D_000967F4;
extern DAM D_000B7F3C[];
extern int func_000776E4();

/* Driver 2 name: damage_object */
int func_00043270(COP *cop, int *v)
{
    DAM *dam;

    dam = D_000B7F3C;
    dam += D_0009741C;
    D_0009741C++;
    dam->cop = cop;
    D_0009745C++;
    dam->active = 0;
    dam->c0 = cop->a;
    dam->vx = v[0] >> 10;
    dam->vz = v[2] >> 10;
    if (dam->vx < 0)
        dam->vy = v[0];
    else
        dam->vy = -v[0];
    if (dam->vz < 0)
        dam->vy += v[2];
    else
        dam->vy -= v[2];
    dam->vy = dam->vy >> 1;
    if (func_000776E4() & 1)
        dam->rot = dam->vy;
    else
        dam->rot = -dam->vy;
    if (dam->vy < -100)
        dam->vy = -100;
    if (D_0009741C >= 8)
        D_0009741C = 0;
    if (*(int *)&D_000967F4 == 0x16) {
        int n = CNT;
        COP **t = (COP **)(n * 16 + *(int *)&D_001E40A0);
        t[0] = cop;
        ((int *)t)[1] = cop->a;
        ((int *)t)[2] = cop->b;
        ((int *)t)[3] = cop->c;
        if (n < 200)
            CNT = n + 1;
    }
    cop->a = 0xFD46FEC0;
    return 0;
}
