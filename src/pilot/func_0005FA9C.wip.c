typedef struct { int vx, vy, vz; } V3;
typedef struct {
    int x, y, z;
    char pad0c[0x1A - 0xC];
    short dir;
    char pad1c[0x3C - 0x1C];
    int flags;
    short out;
    char pad42[4];
    short dist;
} OBJ;
typedef struct { char pad[0xC8]; int x; int y; int z; } CAM;
extern CAM D_0009B0A4;
extern int D_000976BC;
extern int ratan2();

void func_0005FA9C(OBJ *o)
{
    V3 d, a;
    int s0, ang, dist;
    unsigned t;
    CAM *c = &D_0009B0A4;

    d.vx = o->x - c->x;
    d.vz = o->z - c->z;
    a.vx = abs(d.vx);
    a.vz = abs(d.vz);
    dist = a.vx + a.vz;
    if (dist > 0x1770)
        s0 = 0;
    else
        s0 = 0x1770 - dist;
    s0 += D_000976BC;
    if (o->flags & 0x1000) {
        o->out = -ratan2(d.vz, d.vx) - 0x400;
    } else if (s0 >= 0xBB8 || (o->flags & 0x40)) {
        o->dist = s0;
        o->out = ang = -ratan2(d.vz, d.vx) - 0x400;
        t = ((o->dir - ang + 0x800) & 0xFFF) - 0x4B1;
        if (s0 == 100000 && t < 0x69FU)
            return;
        o->out = (unsigned short)o->dir + 0x800;
    } else {
        o->out = (unsigned short)o->dir + 0x800;
    }
}
