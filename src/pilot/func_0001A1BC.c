/* func_0001A1BC */
typedef struct { char pad0[0xC8]; int x; char pad1[4]; int z; char pad2[0x24]; int rot; } S;
extern S car_data;
extern short rcossin_tbl[];
int func_0001A1BC(S *p)
{
    int dx, dz, r, c, s, a, b;
    r = car_data.rot;
    if ((unsigned)(((r - p->rot) & 0xfff) - 0x69c) >= 0x2c9) return 0;
    dx = p->x - car_data.x;
    dz = p->z - car_data.z;
    if ((unsigned)(dx + 0x2328) >= 0x4651) return 0;
    if (dz < -0x2328) return 0;
    if (dz >= 0x2329) return 0;
    c = rcossin_tbl[(r & 0xfff) * 2];
    s = rcossin_tbl[(r & 0xfff) * 2 + 1];
    if (dx * c + dz * s < -0x384000) return 0;
    c = rcossin_tbl[((r + 0x3ce) & 0xfff) * 2];
    s = rcossin_tbl[((r + 0x3ce) & 0xfff) * 2 + 1];
    if (dx * c + dz * s < -0x258000) return 0;
    c = rcossin_tbl[((r - 0x3ce) & 0xfff) * 2];
    s = rcossin_tbl[((r - 0x3ce) & 0xfff) * 2 + 1];
    return !(dx * c + dz * s < -0x258000);
}
