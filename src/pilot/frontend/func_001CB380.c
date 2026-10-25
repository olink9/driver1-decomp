/* FAKEMATCH: unused MAT local (`MAT tmp;`) only sizes the 0x58 frame (32 B below m); the rest is clean C. */
/* func_001CB380 - frontend 3D backdrop frame: if inv_camera_matrix row 0 is not degenerate, premultiply it by
   D_001CF2D8 (MulMatrix0 takes 3 args - the third, &inv_camera_matrix, is what left a2 holding its address and fixed
   the a2/a3 + t0/t1 register diffs of the 99.38 wip), then draw the map. */
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
    MAT tmp;
    int s0, s1, s2;
    MAT m;
    func_00077D6C(D_001CF2D4);
    func_001C9C0C();
    s0 = inv_camera_matrix.m[0][0] * inv_camera_matrix.m[0][0];
    s1 = inv_camera_matrix.m[0][1] * inv_camera_matrix.m[0][1];
    s2 = inv_camera_matrix.m[0][2] * inv_camera_matrix.m[0][2];
    if (s0 + s1 + s2 <= 0x10D2A3F) {
        m = inv_camera_matrix;
        MulMatrix0(D_001CF2D8, &m, &inv_camera_matrix);
    }
    func_0002DBEC();
    func_0002C53C();
    InitFrustrumMatrix();
    func_0002DBB4();
    DrawMapPSX();
    func_000244B0(0xB4, 0xD7);
    D_00097534++;
}
