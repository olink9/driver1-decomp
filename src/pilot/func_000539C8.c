/* func_000539C8: builds a scaled top-down rotation from the player car heading and RotTrans()es a list of world points (map/radar projection). */
#include "common.h"
#include "gte.h"

typedef struct {
    char pad0[0xC8];
    int xC8;
    int pad1;
    int xD0;
    char pad2[0xF8 - 0xD4];
    int dirF8;
    char pad3[0x18C - 0xFC];
} CAR_b062;

typedef struct {
    char pad[0x14];
    int scale;
} SCALE_b062;

extern CAR_b062 car_data[];
extern short rcossin_tbl[];
extern SCALE_b062 D_0008B080[];
extern long long D_00096888;
extern long long D_00096834;
extern void RotTrans();

void func_000539C8(VECTOR *in, VECTOR *out, int count, unsigned char flag, unsigned char flag2)
{
    long f;
    int scale;

    {
    MATRIX m;
    int s, c;
    int a;
    a = car_data[*(int *)&D_00096888].dirF8 & 0xFFF;
    s = rcossin_tbl[a*2];
    c = rcossin_tbl[a*2+1];
    m.m[2][0] = -s;
    m.m[2][2] = c;
    m.m[0][0] = (c << 9) / 320;
    m.m[0][2] = (s << 9) / 320;
    m.m[0][1] = 0;
    m.m[1][0] = 0;
    m.m[1][1] = 0x1000;
    m.m[1][2] = 0;
    m.m[2][1] = 0;
    m.t[1] = 0;
    if (flag2) {
        m.t[0] = 0;
        m.t[2] = 0;
    } else {
        m.t[0] = 0x1C0;
        m.t[2] = 0xBE;
    }
    gte_SetRotMatrix(&m);
    gte_SetTransVec0(&m.t);
    }
    scale = D_0008B080[*(int *)&D_00096834].scale;
    while (count--) {
        SVECTOR vv;
        vv.vy = 0;
        if (flag) {
            vv.vx = in->vx / scale;
            vv.vz = -in->vz / scale;
        } else {
            vv.vx = (in->vx - car_data[0].xC8) / scale;
            vv.vz = (car_data[0].xD0 - in->vz) / scale;
        }
        RotTrans(&vv, out++, &f);
        in++;
    }
}
