typedef struct { char pad0[0xC8]; int x; char pad1[4]; int z; char pad2[0x24]; int rot; } S;
extern S D_0009B0A4;
extern short D_0008EAA8[];
#define RC(a) D_0008EAA8[((a) & 0xfff) * 2]
#define RS(a) D_0008EAA8[((a) & 0xfff) * 2 + 1]
static inline int dot(int dx, int dz, int a) { return dx * RC(a) + dz * RS(a); }
int func_0001A1BC(S *p)
{
    int dx, dz, r;
    r = D_0009B0A4.rot;
    if ((unsigned)(((r - p->rot) & 0xfff) - 0x69c) >= 0x2c9) return 0;
    dx = p->x - D_0009B0A4.x;
    dz = p->z - D_0009B0A4.z;
    if ((unsigned)(dx + 0x2328) >= 0x4651) return 0;
    if (dz < -0x2328) return 0;
    if (dz >= 0x2329) return 0;
    if (dot(dx, dz, r) < -0x384000) return 0;
    if (dot(dx, dz, r + 0x3ce) < -0x258000) return 0;
    return !(dot(dx, dz, r - 0x3ce) < -0x258000);
}
