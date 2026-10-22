/* LongQuaternion2Matrix */
typedef struct { short m[3][3]; int t[3]; } MATRIX;
void LongQuaternion2Matrix(int (*qua)[4], MATRIX *m)
{
    int xx, xy, xz, xw, yy, yz, yw, zz, zw;
    xx = ((*qua)[0] * (*qua)[0] + 0x400) >> 11;
    xy = ((*qua)[0] * (*qua)[1] + 0x400) >> 11;
    xz = ((*qua)[0] * (*qua)[2] + 0x400) >> 11;
    yy = ((*qua)[1] * (*qua)[1] + 0x400) >> 11;
    xw = ((*qua)[0] * (*qua)[3] + 0x400) >> 11;
    yw = ((*qua)[1] * (*qua)[3] + 0x400) >> 11;
    zz = ((*qua)[2] * (*qua)[2] + 0x400) >> 11;
    zw = ((*qua)[2] * (*qua)[3] + 0x400) >> 11;
    yz = ((*qua)[1] * (*qua)[2] + 0x400) >> 11;
    m->m[0][0] = 4096 - (yy + zz);
    m->m[0][1] = xy - zw;
    m->m[0][2] = xz + yw;
    m->m[1][0] = xy + zw;
    m->m[1][1] = 4096 - (xx + zz);
    m->m[1][2] = yz - xw;
    m->m[2][1] = yz + xw;
    m->m[2][0] = xz - yw;
    m->m[2][2] = 4096 - (xx + yy);
}
