/* Camera look-at: builds inv_camera_matrix from camera_position, a target and an up vector (fwd, right = fwd x up, up = fwd x right).
   The matrix rows read each int back as its low short through a struct cast (HALF): a MEM_IN_STRUCT load keeps it below the stores. */
#include "common.h"

typedef struct { short lo, hi; } HALF;
extern VECTOR camera_position;
extern MATRIX inv_camera_matrix;
extern VECTOR D_001D3120, D_001D3130, D_001D3140;
extern int SquareRoot0();

void func_001C9E58(VECTOR *target, VECTOR *up)
{
    MATRIX m;
    int fx, fy, fz;
    int rx, ry, rz;
    int len;
    int len2;

    m.m[0][0] = 0x1000;
    m.m[1][0] = 0;
    m.m[2][0] = 0;
    m.m[0][1] = 0;
    m.m[1][1] = 0x1000;
    m.m[2][1] = 0;
    m.m[0][2] = 0;
    m.m[1][2] = 0;
    m.m[2][2] = 0x1000;

    fx = target->vx - camera_position.vx;
    D_001D3120.vx = fx;
    fy = target->vy - camera_position.vy;
    D_001D3120.vy = fy;
    fz = target->vz - camera_position.vz;
    D_001D3120.vz = fz;
    len = SquareRoot0(fx * fx + fy * fy + fz * fz);
    fx = (fx << 12) / len;
    fy = (fy << 12) / len;
    fz = (fz << 12) / len;
    D_001D3120.vx = fx;
    D_001D3120.vy = fy;
    D_001D3120.vz = fz;
    m.m[2][0] = ((HALF *)&D_001D3120.vx)->lo;
    m.m[2][1] = ((HALF *)&D_001D3120.vy)->lo;
    m.m[2][2] = ((HALF *)&D_001D3120.vz)->lo;

    D_001D3130.vx = up->vx;
    D_001D3130.vy = up->vy;
    D_001D3130.vz = up->vz;
    rx = (fy * D_001D3130.vz - fz * D_001D3130.vy) >> 12;
    ry = (fz * D_001D3130.vx - fx * D_001D3130.vz) >> 12;
    rz = (fx * D_001D3130.vy - fy * D_001D3130.vx) >> 12;
    D_001D3140.vx = rx;
    D_001D3140.vy = ry;
    D_001D3140.vz = rz;
    len2 = SquareRoot0(rx * rx + ry * ry + rz * rz);
    rx = (rx << 12) / len2;
    ry = (ry << 12) / len2;
    rz = (rz << 12) / len2;
    D_001D3140.vx = rx;
    D_001D3140.vy = ry;
    D_001D3140.vz = rz;

    D_001D3130.vx = (D_001D3120.vy * rz - D_001D3120.vz * ry) >> 12;
    D_001D3130.vy = (D_001D3120.vz * rx - D_001D3120.vx * rz) >> 12;
    D_001D3130.vz = (D_001D3120.vx * ry - D_001D3120.vy * rx) >> 12;

    m.m[0][0] = ((HALF *)&D_001D3140.vx)->lo;
    m.m[0][1] = ((HALF *)&D_001D3140.vy)->lo;
    m.m[0][2] = ((HALF *)&D_001D3140.vz)->lo;
    m.m[1][0] = ((HALF *)&D_001D3130.vx)->lo;
    m.m[1][1] = ((HALF *)&D_001D3130.vy)->lo;
    m.m[1][2] = ((HALF *)&D_001D3130.vz)->lo;

    inv_camera_matrix.m[0][0] = m.m[0][0];
    inv_camera_matrix.m[0][1] = m.m[0][1];
    inv_camera_matrix.m[0][2] = m.m[0][2];
    inv_camera_matrix.m[1][0] = m.m[1][0];
    inv_camera_matrix.m[1][1] = m.m[1][1];
    inv_camera_matrix.m[1][2] = m.m[1][2];
    inv_camera_matrix.m[2][0] = m.m[2][0];
    inv_camera_matrix.m[2][1] = m.m[2][1];
    inv_camera_matrix.m[2][2] = m.m[2][2];
}
