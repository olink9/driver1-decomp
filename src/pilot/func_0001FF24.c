/* ProcessCarPad-like: per-car pad handling (no Driver 2 match) */
typedef struct {
    unsigned char controlType;      /* 0x000 */
    char pad0[0xC8 - 1];
    int x;                          /* 0x0C8 */
    int y;                          /* 0x0CC */
    int z;                          /* 0x0D0 */
    char pad1[0xF0 - 0xD4];
    int thrust;                     /* 0x0F0 */
    int wheel_angle;                /* 0x0F4 */
    int direction;                  /* 0x0F8 */
    int handbrake;                  /* 0x0FC */
    int wheelspin;                  /* 0x100 */
    int wheel_speed;                /* 0x104 */
    char pad2[0x17E - 0x108];
    signed char autoBrake;          /* 0x17E */
    char pad3[0x185 - 0x17F];
    unsigned char f185;             /* 0x185 */
    char pad4[2];
    unsigned int lastPad;           /* 0x188 */
} CAR;

extern CAR car_data[];
extern long long D_00096780;
extern int D_00096788;
extern long long D_000967F4;
extern long long D_00096834;
extern long long D_0009683C;
extern long long D_00096840;
extern long long D_00096858;
extern long long D_00096898;
extern int D_00097004;
extern int D_0009700C;
extern int D_00097018;
extern int D_00097034;
extern long long D_00097080;
extern int D_000C7004[];
extern int ratan2();

void func_0001FF24(int n, unsigned int pad, signed char PadSteer, unsigned char use_analogue)
{
    CAR *cp;
    int v;
    int int_steer;

    cp = &car_data[n];

    if (n == 0) {
        if (*(int *)&D_00096840 == 0)
            *(int *)&D_00097080 = (pad >> 3) & 1;
        else if (!(cp->lastPad & 8) && (pad & 8))
            *(int *)&D_00097080 ^= 8;
    }

    if (*(int *)&D_000967F4 != 0x16 && n == 0 && D_00096788 > 10000) {
        v = D_00096788 - 10000;
        if (v > 10000)
            v = 10000;
        D_0009700C = D_00097018 * v / 10000;
        D_00097034 = D_0009700C;
    } else {
        D_0009700C = 0;
        D_00097034 = 0;
    }

    if (cp->autoBrake > 90)
        cp->autoBrake = 90;

    if (pad & 0x10) {
        cp->handbrake = 1;
    } else {
        cp->handbrake = 0;
        if (pad & 0x20)
            cp->wheelspin = 1;
        else
            cp->wheelspin = 0;
        if (cp->wheelspin && cp->wheel_speed > 0xA2000) {
            cp->wheelspin = 0;
            pad |= 0x40;
        }
    }

    if (use_analogue) {
        if (pad & 4)
            int_steer = PadSteer * (PadSteer * PadSteer / 60) / 60;
        else
            int_steer = PadSteer * (PadSteer * PadSteer / 80) / 80;
        cp->wheel_angle = int_steer & ~3;
        if (int_steer < -270 || int_steer > 270)
            cp->autoBrake++;
        else
            cp->autoBrake = 0;
    } else {
        if (pad & 4) {
            if (pad & 0x2000) {
                cp->wheel_angle += 0x40;
                if (cp->wheel_angle > 0x1FF)
                    cp->wheel_angle = 0x1FF;
            }
            if (pad & 0x8000) {
                cp->wheel_angle -= 0x40;
                if (cp->wheel_angle < -0x1FF)
                    cp->wheel_angle = -0x1FF;
            }
        } else {
            if (pad & 0x2000) {
                cp->wheel_angle += 0x20;
                if (cp->wheel_angle > 0x160)
                    cp->wheel_angle = 0x160;
            }
            if (pad & 0x8000) {
                cp->wheel_angle -= 0x20;
                if (cp->wheel_angle < -0x160)
                    cp->wheel_angle = -0x160;
            }
        }
        if (pad & 0xA000)
            cp->autoBrake++;
        else
            cp->autoBrake = 0;
    }

    if (!(pad & 0xA000)) {
        if (cp->wheel_angle < -64)
            cp->wheel_angle += 64;
        else if (cp->wheel_angle > 64)
            cp->wheel_angle -= 64;
        else
            cp->wheel_angle = 0;
    }

    if (D_00097004 && (pad & 0x80)) {
        int_steer = -((cp->wheel_speed + 0x800) >> 12);
        if (int_steer > 22)
            cp->thrust = ((278 - int_steer) * -7000) >> 8;
        else
            cp->thrust = -7000;
        if (n == 0)
            cp->thrust += D_00097034;
    } else if (D_00097004 && (pad & 0x40)) {
        if (n == 23 && (*(unsigned char *)&D_00096898 == 3 || *(unsigned char *)&D_00096898 == 6))
            cp->thrust = 7000;
        else
            cp->thrust = 7200;

        if (*(int *)&D_00096780 && n == 0) {
            int dx, dz, angle, diff, dist;
            dx = car_data[0].x - car_data[23].x;
            dz = car_data[0].z - car_data[23].z;
            angle = ratan2(dx, dz);
            dx >>= 10;
            dz >>= 10;
            dist = dx * dx + dz * dz;
            diff = ((car_data[0].direction - angle + 0x800) & 0xFFF) - 0x800;
            if (dist > 30)
                cp->thrust = 8000;
            else if (dist > 9)
                cp->thrust = 7400;
            else
                cp->thrust = 6700;
            if (diff > -800 && diff < 800)
                cp->thrust = 6000;
        }

        if ((n == 0 || n == 23) && cp->f185) {
            int t = 0;
            if (cp->controlType != 23 || *(int *)&D_000967F4 == 0x16) {
                if (D_000C7004[0] != 0x7F && !((unsigned)(D_000C7004[0] - 170) < 10) &&
                    *(int *)&D_00096858 == 0)
                    t = 1;
            }
            cp->thrust = t;
        }

        if (n == 0 || n == 23)
            cp->thrust += D_0009700C;
    } else {
        cp->thrust = 0;
    }

    if (n == 0 && *(int *)&D_0009683C == 3 && *(int *)&D_00096834 == 0)
        cp->thrust = (cp->thrust * 19 + 8) >> 4;

    cp->lastPad = pad;
}
