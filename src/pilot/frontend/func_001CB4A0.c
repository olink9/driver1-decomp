/* FAKEMATCH: pad[8] only sizes the frame (0x60, matrix at sp+0x18); the identical-arm restore at the end only adds refs to 'saved' so it outranks 'dir' in global-alloc (s4/s5). */
typedef struct { short m[3][3]; int t[3]; } MAT;
extern MAT inv_camera_matrix;
extern int D_001CF2F8, D_001CF2FC, D_001CF300, D_00096C90;
extern volatile int D_00097534;
extern char D_001CF2D8[];
extern void SetDispMask(), func_0002DBEC(), func_0002C53C(), func_001C9C0C(), MulMatrix0();
extern void InitFrustrumMatrix(), func_0002DBB4(), DrawMapPSX(), DrawSync(), VSync(), func_000513D4();

void func_001CB4A0(int dir, int steps)
{
    int saved, step, i;
    int s0, s1, s2;
    char pad[8]; /* FAKEMATCH: frame size */
    MAT m;

    D_001CF2F8 = 1;
    D_001CF2FC = 255 / steps;
    if (dir == 0)
        D_001CF300 = 0;
    else
        D_001CF300 = 255;
    if (dir == 0) {
        saved = D_00096C90;
        step = (saved - 10000) / steps;
    } else {
        saved = D_00096C90;
        D_00096C90 = -10000;
        step = (saved - 10000) / steps;
    }
    i = 0;
    if (dir != 0)
        SetDispMask(0);
    while (D_001CF2F8 != 0) {
        if (dir == 0)
            D_00096C90 += step;
        else
            D_00096C90 -= step;
        if (i == 3)
            SetDispMask(1);
        func_0002DBEC();
        func_0002C53C();
        func_001C9C0C();
        s0 = inv_camera_matrix.m[0][0] * inv_camera_matrix.m[0][0];
        s1 = inv_camera_matrix.m[0][1] * inv_camera_matrix.m[0][1];
        s2 = inv_camera_matrix.m[0][2] * inv_camera_matrix.m[0][2];
        if (s0 + s1 + s2 <= 0x10D2A3F) {
            m = inv_camera_matrix;
            MulMatrix0(D_001CF2D8, &m, &inv_camera_matrix);
        }
        InitFrustrumMatrix();
        func_0002DBB4();
        DrawMapPSX();
        D_00097534++;
        DrawSync(0);
        VSync(0);
        func_000513D4();
        if (dir == 0) {
            D_001CF300 += D_001CF2FC;
            if (D_001CF300 >= 255) {
                D_001CF300 = 255;
                D_001CF2F8 = 0;
            }
        } else {
            D_001CF300 -= D_001CF2FC;
            if (D_001CF300 <= 0) {
                D_001CF2F8 = 0;
                D_001CF300 = 0;
            }
        }
        i++;
    }
    if (dir == 0)
        D_00096C90 = saved;
    else
        D_00096C90 = saved;
}
