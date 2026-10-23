/* func_00017DE8 - no Driver 2 counterpart */
typedef struct {
    int x;
    int pad4;
    int z;
} XZ;

typedef struct {
    char pad0[0xC8];
    XZ pos;
    int xD4;
    int xD8[(0x108 - 0xD8) / 4];
    int x108;
    char pad10C[0x18C - 0x10C];
} CAR;

typedef struct {
    unsigned char id;
    unsigned char x1;
    unsigned char state;
    unsigned char x3;
    char pad4[0x9 - 0x4];
    unsigned char x9;
    char padA[0xC - 0xA];
    unsigned char xC;
    unsigned char xD;
    char padE[0x12 - 0xE];
    short x12;
    unsigned char x14;
    char pad15[0x17 - 0x15];
    unsigned char flags;
    int x18;
    int x1C;
    int x20;
    int x24;
    char pad28[0x2C - 0x28];
    int x2C;
    int x30;
    int x34;
    int x38;
    char pad3C[0x40 - 0x3C];
    int x40;
    int x44;
} OBJ;

typedef struct {
    int x;
    int z;
    int pad8;
    short y;
    short padE;
    short dir;
    short pad12;
    int pad14;
} SEG;

typedef struct {
    int a;
    int b;
} PAIR;

extern CAR car_data[];
extern int func_00021820();
extern int func_00018100();
extern int func_00017780(OBJ *p, XZ *a1, int *a2, unsigned char mode);
extern void func_00014FC4();
extern unsigned char func_00015338();
extern int func_0001A1BC();
extern void func_0004D740();

void func_00017DE8(OBJ *c)
{
    SEG s1;
    SEG s2;
    PAIR pr;
    XZ *p;
    int *pz;
    int ok;
    int r;
    int t;

    if (c->x38 == 0)
        return;
    if (c->state == 1) {
        p = &car_data[c->id + 1].pos;
        if (func_00021820(p->x, p->z, c->x24, &s1) == 0) {
            c->x18 = s1.x;
            c->x1C = s1.z;
            c->x20 = s1.y;
            c->x12 = s1.dir;
        }
        if ((unsigned)(c->xD - 1) < 0x7C)
            c->xD++;
        return;
    }
    p = &car_data[c->id + 1].pos;
    pz = &p->z;
    if (func_00018100(c, c->x24 << 10) < 64)
        ok = func_00017780(c, p, pz, c->x24);
    else
        ok = 0;
    r = func_00021820(p->x, *pz, c->x24, &s2);
    if ((ok != 0 && s2.y == -1) || s2.y == 0x3FFF) {
        c->state = 1;
        return;
    }
    if (r == 1 && c->x3 != 0) {
        func_00014FC4(c->x2C, c->x30, c->x34, 2, c->x9, &pr);
        c->flags &= ~4;
        c->x40 = pr.a;
        c->x44 = pr.b;
        c->x1 = 3;
        c->x14 = 0;
        return;
    }
    if (r != 0)
        return;
    c->x18 = s2.x;
    c->x1C = s2.z;
    c->x20 = s2.y;
    c->x12 = s2.dir;
    c->x9 = func_00015338(c);
    if (c->xC != 0)
        c->xC--;
    if (car_data[0].x108 < 31)
        return;
    if (c->x38 < 11)
        return;
    if (func_0001A1BC(&car_data[c->id + 1]) == 0)
        return;
    if (c->xC != 0)
        return;
    t = c->id & 3;
    if (t == 3)
        t = 2;
    c->xC = 13;
    func_0004D740(-1, 0, t + 6, &car_data[c->id + 1].pos, car_data[c->id + 1].xD8);
}
