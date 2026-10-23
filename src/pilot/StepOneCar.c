#include "common.h"
#include "gte.h"

/* StepOneCar - Driver 2 name: StepOneCar (WHEELFORCES.C). Driver 1 has the collision-point
 * lift, GetFrictionScalesDriver1 and AddWheelForcesDriver1 all in this one function; the second
 * argument is the aggressive-braking flag. func_0001EEB4 = FindSurface, func_0001F490 =
 * ConvertTorqueToAngularAcceleration, func_00047FA4 = Setup_Sparks, Setup_Debris = Setup_Debris,
 * func_00035B50 = SetPadVibration-like, Start3DSoundVolPitch = 3D sound start. */

#define FIXEDH(a) (((a) + 2048) >> 12)

typedef struct {
    unsigned char susCompression;
    unsigned char locked;
    unsigned char onGrass;
    unsigned char pad;
} WHEEL;

typedef struct {
    unsigned char id;           /* 0x00 */
    unsigned char pad1;
    unsigned char model;        /* 0x02 */
    unsigned char pad3;
    MATRIX where;               /* 0x04 */
    int fpos[3];                /* 0x24 */
    char pad30[0x10];
    int vel[3];                 /* 0x40 */
    int avel[3];                /* 0x4C */
    int acc[3];                 /* 0x58 */
    char pad64[4];
    int aacc[3];                /* 0x68 */
    char pad74[4];
    WHEEL wheel[4];             /* 0x78 */
    VECTOR contact[4];          /* 0x88 */
    int posX;                   /* 0xC8 */
    int posY;                   /* 0xCC */
    char padD0[0x20];
    int thrust;                 /* 0xF0 */
    int wheel_angle;            /* 0xF4 */
    int direction;              /* 0xF8 */
    int handbrake;              /* 0xFC */
    int wheelspin;              /* 0x100 */
    int wheel_speed;            /* 0x104 */
    int speed;                  /* 0x108 */
    int front_vel;              /* 0x10C */
    int rear_vel;               /* 0x110 */
    char pad114[0x18];
    short mayBeColliding;       /* 0x12C */
    unsigned char hndType;      /* 0x12E */
    char pad12F[0x4F];
    signed char autoBrake;      /* 0x17E */
    unsigned char b17F;         /* 0x17F */
} CAR;

typedef struct {
    MATRIX where;
    int vel[4];
    int acc[4];
    int avel[4];
    int aacc[4];
    int extraangulardamping;
    int aggressive;
} CL;

typedef struct {
    char pad[0x40];
    SVECTOR wheelDisp[4];
    char pad60[4];
} COS;

extern CAR car_data;
extern COS car_cosmetics[];
extern SVECTOR D_00086698[];
extern int D_000866F8[];
extern short rcossin_tbl[];
extern short D_0008EAAA[];
extern int D_0009B094[];
extern VECTOR D_00010034;
extern VECTOR D_00010044;
extern long long D_0009D8E8;
extern long long gNight;
extern int D_00097024;
extern long long D_00096834;
extern long long D_000967F4;
extern int D_00096784;
extern unsigned char D_00096FFC;
extern long long wetness;
extern long long cameraview;
extern long long D_00096870;
extern long long D_0009776C;

extern int func_0001EEB4();
extern void Start3DSoundVolPitch();
extern void func_00047FA4();
extern void Setup_Debris();
extern void func_00035B50();
extern void func_0001F490();
extern int ratan2();

static inline int CarSpeed(CL *cl)
{
    int a, b;

    a = FIXEDH(cl->vel[0]);
    b = FIXEDH(cl->vel[2]);
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;

    if (a < b)
        return b + a / 2;
    else
        return a + b / 2;
}

static inline int sub20(int v)
{
    return v - 20;
}

#define RSIN(a) rcossin_tbl[((a) & 0xFFF) * 2]
#define RCOS(a) rcossin_tbl[((a) & 0xFFF) * 2 + 1]

void StepOneCar(CAR *cp, int aggressive)
{
    CL _cl;
    long A[4];
    long B[4];
    long C[4];
    long D[4];
    long E[4];
    long F[4];
    COS *car_cos;
    int friction_coef;
    int front_vel;
    int rear_vel;
    int oldSpeed;
    int cdx;
    int cdz;
    int sdx;
    int sdz;
    int dvx;
    int dvy;
    int dvz;
    WHEEL *wheel;
    CL *cl;
    int i;
    int a, b, speed;
    int lift;
    int newLift;
    int impulse;
    int componant;
    int loss, limit;
    int lever_dot_n, displacementsquared;
    int frontFS, rearFS;
    int autoBrake;
    int dir;
    int forcefac;
    int oldCompression, newCompression;
    int susForce;
    int lfx, lfz;
    int slidevel, wheelspd;
    int fx, fy, fz;
    int angle;
    int y;
    int count;
    int ws;
    int tmp;

    cl = &_cl;
    cl->extraangulardamping = 0;

    if (cp == &car_data)
        car_cos = &car_cosmetics[0];
    else
        car_cos = &car_cosmetics[cp->model];

    for (i = 2; i >= 0; i--) {
        cp->fpos[i] &= 0xF;
        cp->fpos[i] += cp->where.t[i] << 4;
        cl->vel[i] = cp->vel[i];
        cl->where.t[i] = cp->where.t[i];
        cl->avel[i] = cp->avel[i];
        cl->aacc[i] = cp->aacc[i];
    }

    if (*(int *)&D_0009D8E8) {
        cl->acc[0] = 0;
        cl->acc[1] = -0xAAA;
        cl->acc[2] = 0;
    } else {
        cl->acc[0] = 0;
        cl->acc[1] = -0x2AAA;
        cl->acc[2] = 0;
    }

    speed = CarSpeed(cl);
    cp->speed = speed;

    gte_SetRotMatrix(&cp->where);
    gte_SetTransVec0(cl->where.t);

    lift = 0;

    if (cp->where.m[1][1] > 2048)
        count = 4;
    else
        count = 12;

    for (i = count - 1; i != -1; i--) {
        gte_ldv0(&D_00086698[i]);
        gte_rtv0tr();
        gte_stlvnl(A);

        E[0] = A[0] - cl->where.t[0];
        E[1] = A[1] - cl->where.t[1];
        E[2] = A[2] - cl->where.t[2];

        func_0001EEB4(A, D, C, cp->id);

        if (i < 4) {
            cp->contact[i].vx = C[0];
            cp->contact[i].vy = C[1];
            cp->contact[i].vz = C[2];
        }

        newLift = C[1] - A[1];

        if (newLift > 0) {
            if (lift < newLift)
                lift = newLift;

            B[0] = FIXEDH(cl->avel[1] * E[2] - cl->avel[2] * E[1]);
            B[1] = FIXEDH(cl->avel[2] * E[0] - cl->avel[0] * E[2]);
            B[2] = FIXEDH(cl->avel[0] * E[1] - cl->avel[1] * E[0]);

            B[0] += cl->vel[0];
            B[1] += cl->vel[1];
            B[2] += cl->vel[2];

            impulse = FIXEDH(B[0] * D[0] + B[1] * D[1] + B[2] * D[2]);
            lever_dot_n = FIXEDH(E[0] * D[0] + E[1] * D[1] + E[2] * D[2]);
            displacementsquared = FIXEDH(((E[0] * E[0] + E[1] * E[1] + E[2] * E[2]) - lever_dot_n * lever_dot_n) * 110) + 4096;

            impulse = (impulse / displacementsquared) * -2048;

            componant = 2;
            do {
                loss = B[componant] * 96;
                limit = D_000866F8[componant];

                if (limit < loss)
                    loss = limit;
                else {
                    limit = -limit;
                    if (loss < limit)
                        loss = limit;
                }

                F[componant] = FIXEDH(impulse * D[componant] - loss);
                componant--;
            } while (componant >= 0);

            if (impulse > 20000) {
                D_00097024 = impulse;

                if (*(int *)&gNight == 1) {
                    VECTOR direction;
                    direction = D_00010034;
                    func_00047FA4(C, &direction, 15, 1);
                } else {
                    VECTOR direction;
                    direction = D_00010044;
                    Setup_Debris(C, &direction, 10, 0);
                }

                if (cp == &car_data && impulse > 30000)
                    Start3DSoundVolPitch(2, 0, 0, C[0], -C[1], C[2], 9000, 4096);
            }

            cl->aacc[2] -= FIXEDH(E[1] * F[0]);
            cl->aacc[1] += FIXEDH(E[2] * F[0]);
            cl->acc[0] += F[0];
            cl->aacc[0] -= FIXEDH(E[2] * F[1]);
            cl->aacc[2] += FIXEDH(E[0] * F[1]);
            cl->acc[1] += F[1];
            cl->aacc[1] -= FIXEDH(E[0] * F[2]);
            cl->aacc[0] += FIXEDH(E[1] * F[2]);
            cl->acc[2] += F[2];
        }
    }

    if (lift != 0) {
        cl->where.t[1] += lift;
        cp->fpos[1] += lift << 4;
        cp->posY += lift;
        gte_SetTransVec0(cl->where.t);
        cl->extraangulardamping = 1;

        if (lift > 120)
            cp->vel[1] = 0;
    }

    if (*(int *)&D_00096834 == 5 && cl->where.t[2] < -0x6CA7A && cl->where.t[0] > -96450 &&
        cl->where.t[0] <= 0x2BB05 && cl->where.t[1] > -300) {
        cl->where.t[1] = -300;
        cp->fpos[1] = -4800;
        cp->posY = -300;
        gte_SetTransVec0(cl->where.t);
        cl->extraangulardamping = 1;

        if (cp->vel[1] > 0)
            cp->vel[1] = -cp->vel[1];
    }

    if (speed < 32)
        oldSpeed = 3696 - speed * 72;
    else
        oldSpeed = 1424 - speed;

    cdx = RSIN(cp->direction);
    cdz = RCOS(cp->direction);
    sdx = RSIN(cp->direction + cp->wheel_angle);
    sdz = RCOS(cp->direction + cp->wheel_angle);

    dvx = -(cl->vel[0] * 12);
    dvy = -(cl->vel[1] * 12);
    dvz = -(cl->vel[2] * 12);

    if (oldSpeed > 3300)
        oldSpeed = 3300;

    /* GetFrictionScales */
    if (cp->thrust < 0)
        frontFS = 1453;
    else if (cp->thrust > 0)
        frontFS = 820;
    else
        frontFS = 937;

    if (((*(int *)&D_000967F4 != 22 && cp->id == 23) || D_00096784 != 0) && cp->wheelspin == 0 &&
        (cp->id == 0 || cp->id == 23) && cp->autoBrake > 0 && cp->wheel_speed > 0) {
        autoBrake = cp->autoBrake;
        if (D_00096784 == 2)
            frontFS += 600;
        else {
            if (autoBrake > 13)
                autoBrake = 13;
            frontFS += (autoBrake * 2 + autoBrake) * 15;
        }
    }

    if (cp->thrust < 0 && cp->wheel_speed > 0xF000 && aggressive) {
        cp->wheel[3].locked = 1;
        cp->wheel[2].locked = 1;
        cp->wheel[1].locked = 1;
        cp->wheel[0].locked = 1;
    } else {
        cp->wheel[3].locked = 0;
        cp->wheel[2].locked = 0;
        cp->wheel[1].locked = 0;
        cp->wheel[0].locked = 0;
    }

    if (cp->handbrake) {
        if (cp->thrust > -1)
            cp->thrust = 0;

        if (cp->wheel_speed > 0)
            frontFS += 656;
        else
            frontFS -= 375;

        cp->wheel[1].locked = 1;
        cp->wheel[3].locked = 1;
        cp->wheelspin = 0;
    } else if (cp->wheelspin) {
        frontFS += 600;
    }

    if (cp->wheel_speed < 0 && cp->thrust > -1 && cp->handbrake == 0)
        frontFS -= 400;

    rearFS = 0x780 - frontFS;

    if (cp->wheelspin)
        cp->thrust = 7000;

    if (cp->thrust < 0 && cp->wheel_speed > 0xF000 && aggressive) {
        frontFS = (frontFS * 10) / 8;
        rearFS = (rearFS * 10) / 8;
    } else if (D_00096784 < 2) {
        if (cp->wheel[0].onGrass)
            frontFS = (frontFS * 40 - frontFS) / 32;
        else
            frontFS = (frontFS * 36 - frontFS) / 32;
    } else if (aggressive && cp->wheel[0].onGrass) {
        frontFS = (frontFS * 36) / 32;
        if (cp->b17F)
            rearFS = (rearFS * 34) / 32;
    }

    if (cp->hndType == 1) {
        switch (*(int *)&D_00096870) {
        case 2:
        default:
            frontFS = (frontFS * 68) / 32;
            rearFS = (rearFS * 68) / 32;
            break;
        case 1:
            frontFS = (frontFS * 55) / 32;
            rearFS = (rearFS * 55) / 32;
            break;
        case 0:
            frontFS = (frontFS * 45) / 32;
            rearFS = (rearFS * 45) / 32;
            break;
        }
    } else if (cp->id != 0 && cp->id != 23) {
        frontFS = (frontFS * 29) / 32;
        rearFS = (rearFS * 29) / 32;
    }

    front_vel = 0;
    rear_vel = 0;
    cp->b17F = 0;

    for (i = 3; i >= 0; i--) {
        wheel = &cp->wheel[i];

        gte_ldv0(&car_cos->wheelDisp[i]);
        gte_rtv0tr();
        gte_stlvnl(A);

        forcefac = func_0001EEB4(A, C, B, cp->id);
        friction_coef = forcefac * (32400 - *(int *)&wetness) >> 15;

        if (cp == &car_data) {
            int quiet = *(unsigned char *)&cameraview == 2;
            y = B[1];
            if (y - D_0009B094[i] > 12)
                Start3DSoundVolPitch(-1, 0, 11, B[0], y, B[2], quiet ? 0 : -7000, 400);
            D_0009B094[i] = y;
        }

        wheel->onGrass = D_00096FFC;
        oldCompression = wheel->susCompression;
        newCompression = B[1] - sub20(A[1]);

        if (newCompression < 0)
            newCompression = 0;

        if (cp->id == 0) {
            if (newCompression > 120)
                func_00035B50(2);
            else if (newCompression > 48)
                func_00035B50(3);
        }

        if (newCompression > 60)
            newCompression = 60;

        if (newCompression != 0 || oldCompression != 0) {
            A[0] -= cl->where.t[0];
            A[1] -= cl->where.t[1];
            A[2] -= cl->where.t[2];

            D[0] = FIXEDH(cl->avel[1] * A[2] - cl->avel[2] * A[1]);
            D[1] = FIXEDH(cl->avel[2] * A[0] - cl->avel[0] * A[2]);
            D[2] = FIXEDH(cl->avel[0] * A[1] - cl->avel[1] * A[0]);

            fx = fz = 0;

            D[0] += cl->vel[0];
            D[1] += cl->vel[1];
            D[2] += cl->vel[2];

            susForce = newCompression * 230 - oldCompression * 100;

            if (wheel->locked) {
                dir = ratan2(D[0] / 64, D[2] / 64);
                lfz = D_0008EAAA[(dir & 0xFFF) * 2];
                lfx = D_0008EAAA[(dir & 0xFFF) * 2 - 1];
            } else if ((i & 1) == 0) {
                lfz = -sdx;
                lfx = sdz;
            } else {
                lfz = -cdx;
                lfx = cdz;
            }

            slidevel = (D[0] / 64) * (lfx / 64) + (D[2] / 64) * (lfz / 64);
            wheelspd = abs((oldSpeed / 64) * (slidevel / 64));

            if (slidevel > 72000)
                slidevel = 18000;
            else if (slidevel < -72000)
                slidevel = -18000;
            else {
                slidevel = FIXEDH(oldSpeed * slidevel);
                if (slidevel > 18000)
                    slidevel = 18000;
                else if (slidevel < -18000)
                    slidevel = -18000;
            }

            if ((i & 1) == 0) {
                tmp = frontFS * slidevel + 2048;
                slidevel = tmp >> 12;

                if (wheel->locked) {
                    slidevel = (tmp >> 13) + slidevel >> 1;
                    a = FIXEDH(-slidevel * lfx);
                    b = FIXEDH(-slidevel * lfz);
                    forcefac = (a * sdz - b * sdx + 0x400) >> 11;
                    fx += FIXEDH(forcefac * sdz);
                    fz += FIXEDH(sdx * -forcefac);
                } else if (cp->hndType == 1) {
                    fx += FIXEDH(sdx * cp->thrust);
                    fz += FIXEDH(sdz * cp->thrust);
                }

                if (front_vel < wheelspd)
                    front_vel = wheelspd;
            } else {
                if (wheel->locked == 0) {
                    slidevel = FIXEDH(rearFS * slidevel);

                    if ((cp->id == 0 || cp->id == 23) && slidevel * cp->wheel_angle > 0) {
                        cp->autoBrake = -1;
                        cp->b17F = 0;
                    }

                    fx += FIXEDH(-lfz * cp->thrust);
                    fz += FIXEDH(lfx * cp->thrust);
                } else {
                    slidevel = (frontFS * slidevel + 0x2000) >> 14;
                }

                if (rear_vel < wheelspd)
                    rear_vel = wheelspd;
            }

            fx += FIXEDH(susForce * C[0] - slidevel * lfx + dvx);
            fz += FIXEDH(susForce * C[2] - slidevel * lfz + dvz);
            fx = fx * (friction_coef + 500) >> 12;
            fz = fz * (friction_coef + 500) >> 12;
            fy = FIXEDH(susForce * C[1] + dvy);

            if (cp->where.m[1][1] < 2048) {
                angle = 4096 - cp->where.m[1][1];

                angle = (angle <= 4096) ? 4096 - FIXEDH(angle * angle) : 0;

                fx = FIXEDH(fx * angle);
                fz = FIXEDH(fz * angle);
            }

            if (cp->hndType == 1) {
                if (*(int *)&D_00096870 == 2)
                    A[1] = (A[1] * 7) / 32;
                else
                    A[1] = A[1] / 4;
            } else {
                A[1] = (A[1] * 5) / 16;
            }

            if (*(int *)&D_0009776C != 0 && cp->id == 0) {
                if ((i & 1) == 0) {
                    cl->aacc[0] -= (A[2] * 0x23000) >> 12;
                    cl->aacc[2] += (A[0] * 0x23000) >> 12;
                    cl->acc[1] += 0x23000;
                } else {
                    cl->aacc[0] -= (A[2] * 0xB000) >> 12;
                    cl->aacc[2] += (A[0] * 0xB000) >> 12;
                    cl->acc[1] += 0xB000;
                }
            } else {
                cl->aacc[2] -= FIXEDH(A[1] * fx);
                cl->aacc[1] += FIXEDH(A[2] * fx);
                cl->acc[0] += fx;
                cl->aacc[0] -= FIXEDH(A[2] * fy);
                cl->aacc[2] += FIXEDH(A[0] * fy);
                cl->acc[1] += fy;
                cl->aacc[1] -= FIXEDH(A[0] * fz);
                cl->aacc[0] += FIXEDH(A[1] * fz);
                cl->acc[2] += fz;
            }

            wheel->susCompression = newCompression;
        } else {
            wheel->susCompression = 0;
        }
    }

    if (cp->wheel[1].susCompression != 0 || cp->wheel[3].susCompression != 0)
        cp->wheel_speed = (cl->vel[0] / 64) * (cdx / 64) + (cl->vel[2] / 64) * (cdz / 64);
    else {
        if (cp->thrust <= 0) {
            if (cp->thrust < 0)
                ws = -0x12C000;
            else
                ws = 0;
        } else
            ws = 0x1A4000;
        cp->wheel_speed = ws;
    }

    cp->front_vel = front_vel;
    cp->rear_vel = rear_vel;

    func_0001F490(cp, cl);

    cp->mayBeColliding = 0;
}
