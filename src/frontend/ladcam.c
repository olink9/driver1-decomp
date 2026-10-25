/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * camera path points / transitions (func_001C9A18, D_001CEF70), ratan2/SquareRoot0 math
 * func_001C9C0C-func_001C9E58, DATA\%d.PBN loader func_001C9848. START: .rodata pad 0x1C0A14 (exact first function
 * medium). END: text pad at 0x1CA26C, .rodata pad 0x1C0A24.
 */
/* ladcam.c: 11 functions func_001C96C8..func_001C9E58 (0x1C96C8-0x1CA270)
 * Step 3: common.h (gte types) + include/frontend.h. camera_position is main's VECTOR, the other camera vectors
 * are VECTOR / VECTOR_NOPAD, D_001D3190 a table of CAMPT paths (D_001D31B8 walks one). CAMPT/TRANS/HALF are
 * file-local. No views.
 */

#include "common.h"

/* ---- types ---- */
#include "frontend.h"
typedef struct { VECTOR_NOPAD pos; VECTOR_NOPAD dir; int angle; } CAMPT; /* camera path point, 0x1C */
typedef struct { unsigned char from, to; } TRANS;
typedef struct { short lo, hi; } HALF;

/* ---- externs ---- */
extern int D_001CEF84;
extern void func_001CA270();
extern int D_001CEF64;
extern int D_001CEF68;
extern char D_001D30F0[];
extern char D_001D3100[];
extern void func_001CA408();
extern int D_00097098;
extern int D_001D3150;
extern int *D_001D3160[];
extern CAMPT *D_001D3190[];
extern char D_001C0A18[];
extern int sprintf();
extern int Loadfile();
extern int D_001D3210;
extern int D_001D3214;
extern int D_001D3218;
extern int D_001CEF88;
extern int D_001D3208;
extern int D_001D321C;
extern int D_001D320C;
extern int D_001D3200;
extern int D_001D3204;
extern CAMPT *D_001D31B8;
extern TRANS D_001CEF70[];
extern VECTOR camera_position;
extern VECTOR_NOPAD D_001D31C0;
extern VECTOR D_001D31D0;
extern VECTOR D_001D31F0;
extern VECTOR D_001D31E0;
extern SVECTOR D_00096C48;
extern short rcossin_tbl[];
extern int ratan2();
extern MATRIX inv_camera_matrix;
extern VECTOR D_001D3120;
extern VECTOR D_001D3130;
extern VECTOR D_001D3140;
extern int SquareRoot0();

/* ---- functions defined in this file ---- */
void func_001C96C8();
void func_001C9724();
void func_001C977C();
void func_001C9848();
void func_001C98F4();
void func_001C9928();
void func_001C9A0C();
int func_001C9A18();
void func_001C9C0C();
void func_001C9CF0();
void func_001C9E58();

/* ---------------- func_001C96C8 (src/pilot/frontend/func_001C96C8.c) ---------------- */
void func_001C96C8(void)
{
    func_001CA270(0, 0, 0, 0x40);
    D_001CEF84++;
    if (D_001CEF84 >= 5)
        D_001CEF84 = 0;
    func_001C9A18(D_001CEF84);
}

/* ---------------- func_001C9724 (src/pilot/frontend/func_001C9724.c) ---------------- */
void func_001C9724(void){ func_001CA270(0, 0, 0, 0x40); D_001CEF84 -= 1; if (D_001CEF84 < 0) D_001CEF84 = 4; func_001C9A18(D_001CEF84); }

/* ---------------- func_001C977C (src/pilot/frontend/func_001C977C.c) ---------------- */
void func_001C977C(void)
{
    D_001CEF64 += D_001CEF68;
    if (D_001CEF64 > 255) {
        D_001CEF64 = 255;
        D_001CEF68 = -D_001CEF68;
    }
    if (D_001CEF64 < 0) {
        D_001CEF64 = 0;
        D_001CEF68 = -D_001CEF68;
    }
    func_001CA408(30, 194, D_001D30F0, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, 0, 1);
    func_001CA408(452, 194, D_001D3100, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, 0, 1);
}

/* ---------------- func_001C9848 (src/pilot/frontend/func_001C9848.c) ---------------- */
void func_001C9848(int a)
{
    char buf[32];
    int *p;
    int i;
    sprintf(buf, D_001C0A18, a);
    D_001D3150 = D_00097098;
    D_00097098 += Loadfile(buf, D_00097098);
    p = (int *)D_001D3150;
    for (i = 0; i < 10; i++) {
        int n = *p++;
        D_001D3160[i] = p;
        D_001D3190[i] = p + 11;
        p += n >> 2;
    }
}

/* ---------------- func_001C98F4 (src/pilot/frontend/func_001C98F4.c) ---------------- */
void func_001C98F4(void){ D_001D3210 = 0; D_001D3214 = 0; D_001D3218 = 0; D_001CEF88 = 0; D_001D3208 = 0; D_001D321C = 0; }

/* ---------------- func_001C9928 (src/pilot/frontend/func_001C9928.c) ---------------- */
void func_001C9928(int a0)
{
    D_001CEF88 = 0;
    if (D_001D320C == 0) {
        if (a0 != 0) {
            D_001CEF84++;
            if (D_001CEF84 >= 5) D_001CEF84 = 0;
            D_001D31B8 = D_001D3190[D_001D3200];
        } else {
            D_001D31B8 = D_001D3190[D_001D3204];
        }
    } else if (D_001D320C == 1) {
        if (a0 == 0) {
            D_001CEF84--;
            if (D_001CEF84 < 0) D_001CEF84 = 4;
            D_001D31B8 = D_001D3190[D_001D3200];
        } else {
            D_001D31B8 = D_001D3190[D_001D3204];
        }
    }
    D_001D3204 = D_001CEF84;
    D_001D3200 = D_001CEF84;
    func_001C9CF0();
}

/* ---------------- func_001C9A0C (src/pilot/frontend/func_001C9A0C.c) ---------------- */
void func_001C9A0C(int a0) { D_001D3200 = a0; }

/* ---------------- func_001C9A18 (src/pilot/frontend/func_001C9A18.c) ---------------- */
int func_001C9A18(int to)
{
    int found = 0;
    int i;

    D_001D3204 = to;
    for (i = 0; i < 10; i++) {
        if (D_001D3200 == D_001CEF70[i].from && D_001D3204 == D_001CEF70[i].to) {
            D_001D320C = 1;
            D_001D321C = D_001D3160[i][10];
            D_001D31B8 = D_001D3190[i];
            D_001D3200 = D_001CEF70[i].from;
            D_001D3204 = D_001CEF70[i].to;
            found = 1;
            break;
        }
        if (D_001D3200 == D_001CEF70[i].to && D_001D3204 == D_001CEF70[i].from) {
            D_001D320C = 0;
            D_001D321C = D_001D3160[i][10];
            D_001D31B8 = &D_001D3190[i][D_001D321C - 1];
            D_001D3200 = D_001CEF70[i].to;
            D_001D3204 = D_001CEF70[i].from;
            found = 1;
            break;
        }
    }
    if (!found) {
        D_001D320C = 0;
        D_001D321C = D_001D3160[D_001D3204][10];
        D_001D31B8 = &D_001D3190[D_001D3204][D_001D321C - 1];
        D_001D3200 = D_001CEF70[D_001D3204].to;
        D_001D3204 = D_001CEF70[D_001D3204].from;
    } else {
        D_001CEF88 = 1;
        D_001D3208 = 0;
    }
    return D_001D321C;
}

/* ---------------- func_001C9C0C (src/pilot/frontend/func_001C9C0C.c) ---------------- */
void func_001C9C0C(void)
{
    if (D_001CEF88 == 0) {
        camera_position.vx = D_001D31C0.vx;
        camera_position.vy = D_001D31C0.vy;
        camera_position.vz = D_001D31C0.vz;
        func_001C9E58(&D_001D31D0, &D_001D31F0);
    } else if (D_001D3208 >= D_001D321C) {
        int t;
        D_001D321C = 0;
        t = D_001D3204;
        D_001D3210 = 0;
        D_001D3214 = 0;
        D_001D3218 = 0;
        D_001D3204 = -1;
        D_001CEF88 = 0;
        D_001D3208 = 0;
        D_001D3200 = t;
    } else {
        func_001C9CF0();
        D_001D3208++;
    }
}

/* ---------------- func_001C9CF0 (src/pilot/frontend/func_001C9CF0.c) ---------------- */
void func_001C9CF0(void)
{
    VECTOR a, b;
    CAMPT *p = D_001D31B8;
    int ang;

    D_001D31C0.vx = camera_position.vx = p->pos.vx;
    D_001D31C0.vy = camera_position.vy = p->pos.vy;
    D_001D31C0.vz = camera_position.vz = p->pos.vz;
    D_001D31D0.vx = a.vx = p->dir.vx;
    D_001D31D0.vy = a.vy = p->dir.vy;
    D_001D31D0.vz = a.vz = p->dir.vz;
    ang = p->angle & 0xfff;
    D_001D31F0.vx = b.vx = rcossin_tbl[ang * 2];
    D_001D31F0.vy = b.vy = -rcossin_tbl[ang * 2 + 1];
    D_001D31F0.vz = b.vz = 0;
    func_001C9E58(&a, &b);
    D_001D31E0.vx = D_00096C48.vx = 0;
    D_001D31E0.vy = D_00096C48.vy = ratan2(camera_position.vx - a.vx, a.vz - camera_position.vz) & 0xfff;
    if (D_001D320C == 0)
        D_001D31B8--;
    else
        D_001D31B8++;
}

/* ---------------- func_001C9E58 (src/pilot/frontend/func_001C9E58.c) ---------------- */
/* Camera look-at: builds inv_camera_matrix from camera_position, a target and an up vector (fwd, right = fwd x up, up = fwd x right).
   The matrix rows read each int back as its low short through a struct cast (HALF): a MEM_IN_STRUCT load keeps it below the stores. */
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

