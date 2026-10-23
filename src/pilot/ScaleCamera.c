/* ScaleCamera */
typedef struct { short m[3][3]; int t[3]; } MAT;
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
extern long long D_00096C4A;
extern char inv_camera_matrix[];
extern void RotMatrixYXZ();
extern void MulMatrix0();
extern void TransMatrix();
extern void SetRotMatrix();
extern void SetTransMatrix();

static inline void inv(MAT *a, MAT *b)
{
    b->m[0][0] = a->m[0][0];
    b->m[0][1] = a->m[1][0];
    b->m[0][2] = a->m[2][0];
    b->m[1][0] = a->m[0][1];
    b->m[1][1] = a->m[1][1];
    b->m[1][2] = a->m[2][1];
    b->m[2][0] = a->m[0][2];
    b->m[2][1] = a->m[1][2];
    b->m[2][2] = a->m[2][2];
}

void ScaleCamera(void)
{
    MAT temp;
    MAT temp2;
    MAT scale;
    MAT scaledcammat;
    VEC pos;
    SVEC tempang;

    tempang.vx = 0;
    tempang.vz = 0;
    pos.vz = 0;
    pos.vy = 0;
    pos.vx = 0;
    tempang.vy = *(unsigned short *)&D_00096C4A;
    RotMatrixYXZ(&tempang, &temp);

    inv(&temp, &temp2);

    scale.m[0][0] = 0x1000;
    scale.m[0][1] = 0;
    scale.m[0][2] = 0;
    scale.m[1][0] = 0;
    scale.m[1][1] = 0x1000;
    scale.m[1][2] = 0;
    scale.m[2][0] = 0;
    scale.m[2][1] = 0;
    scale.m[2][2] = 0x1000;

    MulMatrix0(&scale, inv_camera_matrix, &scaledcammat);
    TransMatrix(&scaledcammat, &pos);
    SetRotMatrix(&scaledcammat);
    SetTransMatrix(&scaledcammat);
}
