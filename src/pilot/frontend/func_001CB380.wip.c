/* WIP 99.38: only register choice (matrix base lui a3/addiu a2 vs a2/v1, 3rd product t1 vs t0) differs. FAKEMATCH-style: char pad[32] sizes the 0x58 frame. D_00097534 is volatile like main FrameCnt. */
/* Diagnosis (ov1-wip pass): the 3 products are global pseudos (pref LO_REG); reload then spills a3 for the mult (.greg
   'Spilling reg 7') and moves s2 to t0. Target has the matrix address pseudo in a2 (ours v1, local-alloc) so the high part
   lands in a3 and the spill hits t0 -> s2 in t1. Tried with no effect: one-expression sum, s += accumulation (98.19), sum
   temp, reversed compare, goto skip, lim variable at every position (const-propagated), MAT pointer local, all 6 decl orders
   and statement orders. */
typedef struct { short m[3][3]; int t[3]; } MAT;
extern MAT inv_camera_matrix;
extern int D_001CF2D4;
extern volatile int D_00097534;
extern char D_001CF2D8[];
extern void func_00077D6C();
extern void func_001C9C0C();
extern void MulMatrix0();
extern void func_0002DBEC();
extern void func_0002C53C();
extern void InitFrustrumMatrix();
extern void func_0002DBB4();
extern void DrawMapPSX();
extern void func_000244B0();

void func_001CB380(void)
{
    char pad[32];
    int s0, s1, s2;
    MAT m;
    func_00077D6C(D_001CF2D4);
    func_001C9C0C();
    s0 = inv_camera_matrix.m[0][0] * inv_camera_matrix.m[0][0];
    s1 = inv_camera_matrix.m[0][1] * inv_camera_matrix.m[0][1];
    s2 = inv_camera_matrix.m[0][2] * inv_camera_matrix.m[0][2];
    if (s0 + s1 + s2 <= 0x10D2A3F) {
        m = inv_camera_matrix;
        MulMatrix0(D_001CF2D8, &m);
    }
    func_0002DBEC();
    func_0002C53C();
    InitFrustrumMatrix();
    func_0002DBB4();
    DrawMapPSX();
    func_000244B0(0xB4, 0xD7);
    D_00097534++;
}
