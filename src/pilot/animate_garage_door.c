/* animate_garage_door */
typedef struct { int x, y, z; } V3;
typedef struct { V3 *cop; V3 old; int pad; int cy; int q; unsigned short rot; } GAR;
extern GAR D_000BE754;
extern long long D_00096808, D_00097704, D_00097708, D_00096878;
extern struct { char a[0xC8]; int x, y, z; } car_data;
#define I(x) (*(int *)&x)

void animate_garage_door(void)
{
    int dx, dz, d;
    if (D_000BE754.cop == 0) return;
    if (I(D_00096808) != 53 && I(D_00097704) != 0 && I(D_00097708) > 0x292) {
        *D_000BE754.cop = D_000BE754.old;
        D_000BE754.cop = 0;
        return;
    }
    dx = D_000BE754.old.x - car_data.x;
    dz = D_000BE754.old.z - car_data.z;
    if (dx < 0) dx = -dx;
    if (dz < 0) dz = -dz;
    d = dx + dz;
    if (d > 0x1770 || I(D_00096878) != 0) {
        if (D_000BE754.cy < D_000BE754.old.y) {
            D_000BE754.cy += 12;
            D_000BE754.rot -= 25;
        }
    } else {
        if (d < 4000 && D_000BE754.cy > D_000BE754.old.y - 400) {
            D_000BE754.cy -= 12;
            D_000BE754.rot += 25;
        }
    }
}
