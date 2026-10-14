/* Driver 2 name unknown. Note: the doubled 0x80 test reproduces the target's duplicate beqz; maybe a 1-bit bitfield in the original. */
typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    VEC pos;
    VEC *src;
    unsigned char flags;
} OBJ;

typedef struct {
    char pad0[0xC8];
    VEC pos0;
    char pad1[0x245C - 0xD8];
    VEC pos1;
} CARS;

typedef struct {
    char pad0[0x84];
    VEC v;
} S17C;

extern CARS D_0009B0A4;
extern S17C D_000C017C;
extern VEC D_0009D500;
extern int D_000AF6A0[];
extern int D_000AF980[];
extern void func_00079D20();
extern void func_0004EAC0();
extern int SquareRoot0();

void func_000614DC(OBJ *obj)
{
    VEC a;
    VEC b;
    VEC c;
    VEC d;
    VEC *src;
    unsigned char flags;
    int i;
    int d1;
    int d2;

    a = D_0009B0A4.pos0;
    flags = obj->flags;
    src = obj->src;
    obj->pos.vx = src->vx;
    obj->pos.vy = src->vy;
    obj->pos.vz = src->vz;

    switch (flags & 7) {
    case 0:
    default:
        break;
    case 1:
        obj->pos.vx += a.vx;
        obj->pos.vy -= a.vy;
        obj->pos.vz += a.vz;
        break;
    case 2:
        obj->pos.vx += D_000C017C.v.vx;
        obj->pos.vy += D_000C017C.v.vy;
        obj->pos.vz += D_000C017C.v.vz;
        break;
    case 4:
        func_00079D20(D_000AF6A0);
        func_0004EAC0(obj);
        obj->pos.vx += a.vx;
        obj->pos.vy -= a.vy;
        obj->pos.vz += a.vz;
        break;
    case 6:
        func_00079D20(D_000AF980);
        func_0004EAC0(obj);
    case 3:
        obj->pos.vx += D_0009D500.vx;
        obj->pos.vy -= D_0009D500.vy;
        obj->pos.vz += D_0009D500.vz;
        break;
    }

    b.vz = 0;
    b.vy = 0;
    switch (flags & 0x60) {
    case 0:
        b.vx = -250;
        break;
    case 0x20:
        b.vx = -750;
        break;
    case 0x40:
        b.vx = -1500;
        break;
    default:
        b.vx = -250;
        break;
    }

    func_00079D20(D_000AF6A0);
    func_0004EAC0(&b);

    if ((flags & 0x80) && ((flags & 0x80) >> 7)) a = D_0009B0A4.pos1;

    c.vx = a.vx - b.vx;
    c.vy = -a.vy - b.vy;
    c.vz = a.vz - b.vz;
    b.vx = b.vx + a.vx;
    b.vy = b.vy - a.vy;
    b.vz = b.vz + a.vz;

    d.vx = b.vx - obj->pos.vx;
    d.vy = b.vy - obj->pos.vy;
    d.vz = b.vz - obj->pos.vz;
    d1 = SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz);

    d.vx = c.vx - obj->pos.vx;
    d.vy = c.vy - obj->pos.vy;
    d.vz = c.vz - obj->pos.vz;
    d2 = SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz);

    switch (flags & 0x18) {
    case 0:
        break;
    case 8:
        if (d1 < d2)
            obj->pos = b;
        else
            obj->pos = c;
        break;
    case 0x10:
        if (d2 < d1)
            obj->pos = b;
        else
            obj->pos = c;
        break;
    }
}
