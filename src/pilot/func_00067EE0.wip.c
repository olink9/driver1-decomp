/* Driver 2: Long2DDistance */
typedef struct { int vx, vy, vz, pad; } VECTOR;
extern short D_0008EAA8[];
extern short D_0008EAAA[];
extern int ratan2();

int func_00067EE0(VECTOR *pPoint1, VECTOR *pPoint2)
{
    int theta, tempTheta;
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
        tempTheta = D_0008EAAA[(theta & 4095) * 2];
        result = delta.vx;
    } else {
        tempTheta = D_0008EAA8[(theta & 4095) * 2];
        result = delta.vz;
    }

    if (result > 0x7FFFF)
        result = (result << 9) / tempTheta << 3;
    else
        result = (result << 12) / tempTheta;

    return result;
}
