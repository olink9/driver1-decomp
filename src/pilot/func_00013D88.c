/* func_00013D88: car AI per-frame state update (no Driver 2 counterpart) */

typedef struct {
    unsigned char id;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    unsigned char x4;
    char pad5[3];
    unsigned char x8;
    char pad9[2];
    unsigned char xB;
    char padC[6];
    unsigned short x12;
    char pad14;
    unsigned char x15;
    unsigned char x16;
    unsigned char x17;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
    int x3C;
    int x40;
    int x44;
} OBJ_b095;

typedef struct {
    char pad[0xF0];
    int xF0;
    char padF4[0x108 - 0xF4];
    int x108;
} CTRL_b095;

typedef struct {
    int x;
    int y;
    int z;
    char pad[0x18C - 12];
} POS_b095;

typedef struct {
    char pad[3];
    unsigned char x3;
    char pad4[0x2C - 4];
} NODE_b095;

typedef struct {
    int a, b, c, d, e, f;
} SEG_b095;

extern POS_b095 D_0009B2F8[20];
extern long long D_00097040;
extern long long D_00097050;
extern long long D_00097054;
extern long long CameraCnt;
extern long long D_00097038;
extern int func_000142CC();
extern void func_00021820();
extern void func_00016F0C();
extern int func_000130D4();
extern int func_000138A0();
extern int func_00012E98();
extern unsigned char func_0001262C();

static inline void reroute(OBJ_b095 *p)
{
    SEG_b095 seg;
    POS_b095 *px = &D_0009B2F8[p->id];
    func_00021820(px->x, px->z, p->x24, &seg);
    func_00016F0C(p, &seg);
}

int func_00013D88(OBJ_b095 *p, CTRL_b095 *c)
{
    int mode;
    SEG_b095 seg;
    POS_b095 *px;
    int d;
    unsigned char v;
    int r;

    c->xF0 = 0;
    p->x15 = 0;
    if (p->x2 == 1 || p->x1 == 1)
        return 0;
    if (p->x1 == 9)
        return 0;
    if (p->x3 == 0)
        p->x3C = func_000142CC(p);
    if ((p->x17 & 8) && c->x108 == 0 && p->x3C == 0) {
        p->x17 &= ~8;
        p->x1 = 1;
        p->x8 = 0;
        p->xB = 0xFF;
        return 0;
    }
    if (p->x18 < 3000 && (p->x17 & 2)) {
        if (!(p->x17 & 0x20))
            p->x40 = p->x44 = 0x7FFFFFFF;
        px = &D_0009B2F8[p->id];
        p->x17 &= ~2;
        func_00021820(px->x, px->z, p->x24, &seg);
        func_00016F0C(p, &seg);
    }

    if ((*(NODE_b095 **)&D_00097040)[p->x20].x3 == 0) {
        if (p->x34 == 1) { mode = 1; goto sw; }
        if (p->x12 & 1) { mode = 1; goto sw; }
        if (p->x12 & 4) {
            if (p->x24 & 1)
                v = *(unsigned char *)&D_00097050;
            else
                v = *(unsigned char *)&D_00097054;
            if (v == 3) { mode = 3; goto sw; }
            if (p->x3 == 0) { mode = 2; goto sw; }
        }
    }
    mode = 0;
sw:
    switch (mode) {
    case 1:
        if (p->x3 == 0 && p->x18 < 650 && func_000130D4(p)) {
            p->x3 = mode;
            if (p->x34)
                p->x3C = 60;
            else
                p->x3C = 130;
        }
        break;
    case 0:
    case 3:
        if (p->x3 == 0 && p->x18 < 650 && (*(int *)&CameraCnt & 3) == p->x24) {
            mode = p->x34;
            switch (mode) {
            case -1:
                r = func_000138A0(p);
                if (r == 1) {
                    p->x3C = 60;
                    p->x3 = r;
                    break;
                }
                goto retry;
            case 1:
                if (func_00012E98(p, 0) == 0) {
                    p->x3C = 60;
                    p->x3 = mode;
                    break;
                }
                goto retry;
            case 0:
                if (func_00012E98(p, 0) == 0) {
                    p->x3C = 130;
                    p->x3 = 1;
                    break;
                }
            retry:
                if ((p->x12 & 4) && p->x4 == *(unsigned char *)&D_00097038 + 1) {
                    px = &D_0009B2F8[p->id];
                    func_00021820(px->x, px->z, p->x24, &seg);
                    func_00016F0C(p, &seg);
                }
                break;
            }
        }
        break;
    case 2:
        if (p->x1 != 3 && (p->x34 != 0 || p->x3 == 0)) {
            p->x3 = 0;
            if (p->x3C > 90)
                p->x3C = 90;
            if (p->x18 < 650) {
                p->x3C = 0;
                p->x17 &= 0xDE;
            }
        }
        break;
    }

    mode = p->x3C;
    if ((p->id & 3) == (*(int *)&CameraCnt & 3))
        p->x16 = func_0001262C(p);
    if (p->x16)
        mode = 0;
    d = mode - p->x38;
    if (d > 0) {
        if (d > 32)
            d = 32;
        c->xF0 = (d * 5000) / 32;
    } else if (d < 0) {
        if (d < -32)
            d = -32;
        if (d < -20)
            p->x15 = 1;
        c->xF0 = (d * 7000) / 32;
    }
    return 0;
}
