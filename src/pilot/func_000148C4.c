#include "common.h"
typedef struct { char p0[2]; char b2; char p1[0xC5]; int c8, cc, d0; char p2[0x24]; int f8; char p3[0x90]; } A_t;
typedef struct { char p0[0x12C]; int f12c; char p1[0x2A4 - 0x130]; int a4, a8, ac; char p3[0x2DE - 0x2B0]; unsigned char b2de; char p4[0x100]; } B_t;
typedef struct { int x, y, z, w; } V3;
typedef struct { int x, y, z; } V3s;
extern A_t car_data;
extern B_t D_000C6F04;
extern long long D_00096804, D_00096834;
#define G04 (*(int *)&D_00096804)
#define G34 (*(int *)&D_00096834)
extern V3 D_0008531C[];
extern V3s camera_position;
extern void memset(), func_00020A64(), func_0001B4A4(), func_0001B720();
extern int MapHeight();

/* Driver 2 name: unknown */
void func_000148C4(void)
{
    char pad[16];
    V3s v;
    A_t *p;
    memset(&car_data, 0, 0x18C);
    if (D_000C6F04.b2de != 0 && G04 == 0) {
        int a = D_000C6F04.a4, b = D_000C6F04.a8, c = D_000C6F04.ac;
        car_data.b2 = 0;
        car_data.c8 = a;
        car_data.cc = b;
        car_data.d0 = c;
        func_00020A64(&car_data);
        v.x = car_data.c8; v.y = car_data.cc; v.z = car_data.d0;
        func_0001B4A4(&car_data, &v);
        car_data.f8 = D_000C6F04.f12c;
        func_0001B720(&car_data, 0);
    } else {
        v.x = D_0008531C[G34].x; v.y = D_0008531C[G34].y; v.z = D_0008531C[G34].z;
        p = &car_data;
        if (G34 == 2) p->f8 = 0; else p->f8 = 0x400;
        p->c8 = D_0008531C[G34].x;
        p->cc = D_0008531C[G34].y;
        p->d0 = D_0008531C[G34].z;
        p->cc = MapHeight(&p->c8) + 0xA4;
        v.y = p->cc;
        func_0001B4A4(p, &v);
        func_0001B720(&car_data, 0);
        car_data.b2 = 0;
    }
    camera_position.x = car_data.c8;
    camera_position.y = -car_data.cc - 200;
    camera_position.z = car_data.d0;
}
