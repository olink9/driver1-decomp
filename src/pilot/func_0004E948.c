typedef struct { short m[3][3]; short pad; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
extern void RotMatrixX(); extern void RotMatrixY(); extern void RotMatrixZ();
void func_0004E948(MATRIX *mat, SVECTOR *a) {
    mat->m[0][0] = 0x1000; mat->m[0][1] = 0; mat->m[0][2] = 0;
    mat->m[1][0] = 0; mat->m[1][1] = 0x1000; mat->m[1][2] = 0;
    mat->m[2][0] = 0; mat->m[2][1] = 0; mat->m[2][2] = 0x1000;
    RotMatrixX(a->vx, mat); RotMatrixZ(a->vz, mat); RotMatrixY(a->vy, mat);
}
