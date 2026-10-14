/* func_000181B4: car AI - turn toward the target heading, then pick the next road segment */

typedef struct {
    unsigned char id;
    unsigned char x1;
    unsigned char x2;
    char pad3[6];
    unsigned char x9;
    char padA[8];
    short x12;
    unsigned char x14;
    char pad15[3];
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
    int pad3c;
    int x40;
    int x44;
} OBJ_b062;

typedef struct {
    int x;
    int y;
    int z;
    char pad[0x18C - 12];
} POS_b062;

typedef struct {
    char pad0[0xF4];
    int xF4;
    int xF8;
    char pad[0x18C - 0xFC];
} CAR2_b062;

typedef struct {
    int a;
    int b;
    int pad8;
    short c;
    short padE;
    short d;
    short pad12;
    int pad14;
} SEG_b062;

typedef struct {
    int x;
    int z;
} XZ_b062;

extern POS_b062 D_0009B2F8[];
extern CAR2_b062 D_0009B230[];
extern short func_00021558();
extern void func_00021820();
extern int func_00016F0C();
extern void func_00014FC4();
extern void func_00017780(OBJ_b062 *, POS_b062 *, int *, unsigned char);

#define ABSDIFF(a, b) (((a) - (b)) < 0 ? (b) - (a) : (a) - (b))

void func_000181B4(OBJ_b062 *p)
{
    POS_b062 *px;
    int *pz;
    int r;
    int ang, dir, d, steer;
    SEG_b062 seg;
    XZ_b062 v;

    if (p->x2 == 1 || p->x38 == 0)
        return;

    px = &D_0009B2F8[p->id];
    pz = &px->z;
    r = func_00021558(px->x, px->z);
    p->x28 = r;

    if (p->x14 == 0) {
        switch (p->x24) {
        case 0:
        case 2:
            if (ABSDIFF(*pz, p->x44) < p->x38 + 700)
                p->x14 = 1;
            else
                return;
            break;
        case 1:
        case 3:
            if (ABSDIFF(px->x, p->x40) < p->x38 + 700)
                p->x14 = 1;
            else
                return;
            break;
        }
    }

    steer = 0;
    ang = p->x30 << 10;
    dir = D_0009B230[p->id].xF8 & 0xFFF;
    d = ((ang - dir + 0x800) & 0xFFF) - 0x800;
    if (dir != ang) {
        if (abs(d) < 128)
            steer = d / 2;
        else if (d >= 0)
            steer = 512;
        else
            steer = -512;
    }
    D_0009B230[p->id].xF4 = steer;

    if (abs(d) < 64) {
        if (!(r & 0x4000)) {
        p->x1 = 2;
        p->x24 = p->x30;
        p->x28 = p->x2C;
        func_00021820(px->x, *pz, p->x24, &seg);
        p->x18 = seg.a;
        p->x1C = seg.b;
        p->x20 = seg.c;
        p->x12 = seg.d;
        if (func_00016F0C(p, &seg)) {
            func_00014FC4(p->x28, p->x24, p->x34, 2, p->x9, &v);
            p->x44 = v.z;
            p->x40 = v.x;
        }
        } else {
            func_00017780(p, px, pz, p->x30);
        }
    }
}
