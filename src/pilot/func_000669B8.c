/* func_000669B8 - no Driver 2 counterpart */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { char *primptr; unsigned int *ot; } DB;

typedef struct {
    char pad0[0x10];
    int f10;
    int f14;
    int f18;
    int f1C;
    int scale;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int f38;
} OBJ;

typedef struct {
    char pad0[0xC8];
    int x;
    int y;
    int z;
} CAR;

extern OBJ D_0008BC34;
extern MATRIX D_000AF6A0;
extern CAR D_0009B0A4;
extern VECTOR D_000C6CD8;
extern VECTOR D_000C6CE8;
extern VECTOR D_0008A8F0;
extern MATRIX D_000BBE84;
extern int D_00097768;
extern int D_00097778;
extern int D_00096E6C;
extern long long D_00096897;
extern long long D_0009688C;
extern long long D_000970C8;
extern long long D_000970CC;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

extern void func_00079D20();
extern void func_0004EAC0();
extern void MulMatrix0();
extern void func_0002DCAC();
extern void func_00066FF0();
extern void func_000493CC();

#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")

void func_000669B8(void)
{
    VECTOR pos;
    MATRIX m;
    int i;
    int c;

    pos.vx = (D_0008BC34.f24 - D_0008BC34.f2C) / 2 - D_0008BC34.f10;
    pos.vy = D_0008BC34.f38;
    pos.vz = D_0008BC34.f34 + D_0008BC34.f18;
    func_00079D20(&D_000AF6A0);
    func_0004EAC0(&pos);
    pos.vx += D_0009B0A4.x;
    pos.vy -= D_0009B0A4.y;
    pos.vz += D_0009B0A4.z;
    D_000C6CD8 = pos;
    D_000C6CD8.vy -= D_0008BC34.f2C / 2;

    if (D_00097768 < 100) {
        if (*(unsigned char *)&D_00096897 == 2 && *(int *)&D_0009688C == 0)
            return;
        pos.vx -= D_0008A8F0.vx;
        pos.vy -= D_0008A8F0.vy;
        pos.vz -= D_0008A8F0.vz;
        func_00079D20(&D_000BBE84);
        func_0004EAC0(&pos);
        MulMatrix0(&D_000BBE84, &D_000AF6A0, &m);
        for (i = 8; i >= 0; i--)
            m.m[0][i] = (D_0008BC34.scale * m.m[0][i] + 0x800) >> 12;
        func_00079D20(&m);
        gte_SetTransVector(&pos);
        current->ot += D_00096E6C;
        if (*(int *)&D_000970C8 != 0 || (*(int *)&D_000970C8 = *(int *)&D_000970CC) != 0)
            func_0002DCAC(*(int *)&D_000970C8);
        current->ot -= D_00096E6C;
    } else if (D_00097778 < 32) {
        c = (D_00097778 << 7) & 0xFFF;
        if (D_00097778 == 0)
            D_000C6CE8 = D_000C6CD8;
        func_00066FF0(c);
        if (D_00097778 < 8) {
            int col = (9 - D_00097778) * 0x181818;
            func_000493CC(col, col, 7);
        }
    }
}
