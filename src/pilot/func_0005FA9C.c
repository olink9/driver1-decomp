/* FAKEMATCH: one int temp `k` holds the camera (car_data[0]) address and later the heading difference.
 * Reusing the variable makes the address pseudo live in two blocks (global-alloc), which is the only way
 * found to get the split `lui v0 / addiu a0,v0,%lo` prologue (a block-local pointer is always tied to its
 * HIGH part by local-alloc). The int cast of the array address is steering, not likely original style.
 * run3-r4 (r4-02). */
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
extern CAM car_data[];
extern int D_000976BC;
extern int ratan2();

void func_0005FA9C(OBJ *o)
{
    V3 d, a;
    int s0, ang, dist;
    unsigned t;
    int k;

    k = (int)car_data;
    d.vx = o->x - ((CAM *)k)->x;
    d.vz = o->z - ((CAM *)k)->z;
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
        k = o->dir - ang + 0x800;
        t = k & 0xFFF;
        if (s0 == 100000 && t - 0x4B1 >= 0x69FU)
            return;
        o->out = (unsigned short)o->dir + 0x800;
    } else {
        o->out = (unsigned short)o->dir + 0x800;
    }
}
