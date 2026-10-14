typedef struct { short m[3][3]; int t[3]; } MAT;

extern long long D_00096C48, D_00096C4A, D_00096C4C;
extern MAT D_0008AA38, D_000BBE84, D_000BBEA4, D_0008A1EC, D_000AF9C0;
extern struct { int a[4]; int cur; int prev; } D_0009D8DC;
extern void RotMatrixX();
extern void RotMatrixY();
extern void RotMatrixZ();
extern void MulMatrix0();

void func_0004EB7C(void)
{
    MAT r;
    MAT *p = &r;

    r.m[0][0] = 0x1000;
    r.m[1][0] = 0;
    r.m[2][0] = 0;
    r.m[0][1] = 0;
    r.m[1][1] = 0x1000;
    r.m[2][1] = 0;
    r.m[0][2] = 0;
    r.m[1][2] = 0;
    r.m[2][2] = 0x1000;
    RotMatrixY(*(short *)&D_00096C4A, p);
    RotMatrixZ(*(short *)&D_00096C4C, p);
    RotMatrixX(*(short *)&D_00096C48, p);
    MulMatrix0(&D_0008AA38, p, &D_000BBE84);
    if (D_0009D8DC.cur) {
        D_000BBE84.m[1][0] = -D_000BBE84.m[1][0];
        D_000BBE84.m[1][1] = -D_000BBE84.m[1][1];
        D_000BBE84.m[1][2] = -D_000BBE84.m[1][2];
    }
    if (D_0009D8DC.cur != D_0009D8DC.prev) {
        D_000BBE84.m[0][0] = -D_000BBE84.m[0][0];
        D_000BBE84.m[0][1] = -D_000BBE84.m[0][1];
        D_000BBE84.m[0][2] = -D_000BBE84.m[0][2];
    }
    D_0008A1EC.m[0][0] = 0x1000;
    D_0008A1EC.m[0][2] = 0;
    D_0008A1EC.m[2][0] = 0;
    D_0008A1EC.m[2][2] = 0x1000;
    D_000BBEA4.m[0][0] = D_000BBE84.m[0][0];
    D_000BBEA4.m[0][1] = D_000BBE84.m[1][0];
    D_000BBEA4.m[0][2] = D_000BBE84.m[2][0];
    D_000BBEA4.m[1][0] = D_000BBE84.m[0][1];
    D_000BBEA4.m[1][1] = D_000BBE84.m[1][1];
    D_000BBEA4.m[1][2] = D_000BBE84.m[2][1];
    D_000BBEA4.m[2][0] = D_000BBE84.m[0][2];
    D_000BBEA4.m[2][1] = D_000BBE84.m[1][2];
    D_000BBEA4.m[2][2] = D_000BBE84.m[2][2];
    RotMatrixY(-*(short *)&D_00096C4A & 0xFFF, &D_0008A1EC);
    MulMatrix0(&D_000BBE84, &D_0008A1EC, &D_000AF9C0);
}
