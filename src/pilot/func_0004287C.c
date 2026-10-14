/* func_0004287C: draws up to 8 spinning models (no Driver 2 counterpart identified) */
#include "common.h"
#include "gte.h"

typedef struct {
    int *obj;
    unsigned char b4;
    char pad5[3];
    int f8;
    int pad[2];
    int x;
} ENT;

typedef struct { unsigned short flags; char pad[6]; short s8; } MODEL;

typedef struct { int pad0; int y; int z; char pad[2]; unsigned short type; } OBJ;

extern ENT D_000B7F3C[];
extern MATRIX D_0001064C;
extern MATRIX D_000AF9C0;
extern MODEL *D_000A0E20[];
extern VECTOR D_0008A8F0;
extern long long D_000971C0;
extern long long D_00097128;
extern long long D_00096850;
extern unsigned char D_00096AA8;
extern long long D_00097530;
#define current (*(int **)&D_00097530)

extern void func_0002C564();
extern void func_00079D20();
extern void func_0002DBB4();
extern int func_0002C5F0();
extern int func_0002C6DC();
extern void func_0002DCAC();

void func_0004287C(void)
{
    VECTOR pos;
    MATRIX m;
    MATRIX m2;
    OBJ *obj;
    MODEL *model;
    int c;
    ENT *e = D_000B7F3C;
    int i = 0;
    MATRIX *mp = &m;
    short a, b, c2;
    MODEL **mt = D_000A0E20;
    int *px;

    for (; i < 8; i++, e++) {
        obj = (OBJ *)e->obj; px = &e->x;
        if (obj == 0 || (unsigned)obj > 0x1FFFFFF)
            continue;
        m = D_0001064C;
        m2 = D_000AF9C0;
        model = mt[obj->type];
        if (!(model->flags & 0x4000))
            RotMatrixY((px[-3] * ((unsigned char*)px)[-16] * 3) & 0xfff, mp);
        RotMatrixZ((px[-3] * ((unsigned char*)px)[-16]) & 0xfff, mp);
        pos.vx = *px - D_0008A8F0.vx;
        *(int *)&D_000971C0 = 0;
        pos.vy = obj->y - D_0008A8F0.vy;
        pos.vz = obj->z - D_0008A8F0.vz;
        m.m[0][0] = a = m.m[0][0] * 3;
        mp->m[0][1] = b = mp->m[0][1] * 3;
        mp->m[0][2] = c2 = mp->m[0][2] * 3;
        m.m[0][0] = a >> 1;
        mp->m[0][1] = b >> 1;
        mp->m[0][2] = c2 >> 1;
        func_0002C564(&pos);
        func_00079D20(mp);
        gte_SetTransVec0(&pos);
        pos.vx = *px;
        pos.vy = obj->y;
        pos.vz = obj->z;
        func_0002DBB4();
        if (func_0002C5F0(&pos, model->s8) == -1)
            continue;
        if (model->flags & 0x4000) {
            int *sp = (int *)0x1F8001F0;
            int lv;
            lv = *(int *)&D_00097128;
            c = lv << 16 | lv << 8 | lv;
            if (D_00096AA8 == 0 && *(int *)&D_00096850 == 0)
                *sp = 0x2C808080;
            else
                *sp = c | 0x2C000000;
            *current = func_0002C6DC(model, current[0], current[1], 0);
        } else {
            func_0002DCAC(model);
        }
    }
}
