/* Driver 2: Long2DDistance */
typedef struct { int vx, vy, vz, pad; } VECTOR;
extern short D_0008EAA8[];
extern short D_0008EAAA[];
extern int ratan2();

int func_00067EE0(VECTOR *pPoint1, VECTOR *pPoint2)
{
    int theta;
    int result;
    VECTOR delta;

    int t, u;
    t = pPoint1->vx - pPoint2->vx;
    if (t < 0) t = pPoint2->vx - pPoint1->vx;
    delta.vx = t;
    u = pPoint1->vz - pPoint2->vz;
    if (u < 0) u = pPoint2->vz - pPoint1->vz;
    delta.vz = u;

    theta = ratan2(delta.vz, delta.vx);

    if ((theta & 2047) - 512U > 1024) {
        int cos = D_0008EAAA[(theta & 4095) * 2];
        if (delta.vx > 0x7FFFF)
            result = (delta.vx << 9) / cos << 3;
        else
            result = (delta.vx << 12) / cos;
    } else {
        int sin = D_0008EAA8[(theta & 4095) * 2];
        if (delta.vz > 0x7FFFF)
            result = (delta.vz << 9) / sin << 3;
        else
            result = (delta.vz << 12) / sin;
    }

    return result;
}
