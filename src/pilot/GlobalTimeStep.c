/* Driver 2: GlobalTimeStep (HANDLING.C) */
typedef struct {
    int fposition[3];       /* 0x00 */
    int orientation[4];     /* 0x0C */
    int linearVelocity[3];  /* 0x1C */
    int angularVelocity[3]; /* 0x28 */
} RBN;

typedef union {
    RBN n;
    int v[13];
} RBS;

typedef struct { short vx, vy, vz, pad; } SV;
typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    unsigned char id;       /* 0x00 */
    char pad1[3];
    short m[3][3];          /* 0x04 */
    short mpad;
    int t[3];               /* 0x18 */
    RBS st;                 /* 0x24 */
    int acc[3];             /* 0x58 */
    int pad64;
    int aacc[3];            /* 0x68 */
    char pad74[0xC8 - 0x74];
    int drawt[3];           /* 0xC8 */
    int padD4;
    int vel[3];             /* 0xD8 */
    char padE4[0xF8 - 0xE4];
    int direction;          /* 0xF8 */
    char padFC[0x108 - 0xFC];
    int speed;              /* 0x108 */
    char pad10C[0x12C - 0x10C];
    short mayBeColliding;   /* 0x12C */
    unsigned char controlType; /* 0x12E */
    unsigned char needsDenting; /* 0x12F */
} CAR;

typedef struct {
    char pad[0x40];
    SV wheelDisp[4];        /* 0x40 */
} COS;

extern long long D_00096834;
extern CAR car_data[];
extern int D_00097000;
extern int D_000C7004[];
extern long long D_000967F4;
extern CAR D_0009D438[];
extern int D_00096798;
extern int D_0009679C;
extern short D_0009B1D0[];
extern int D_0009D8DC[];
extern short D_0009AF48[];
extern COS car_cosmetics[];
extern int D_0009D8E4[];
extern int D_00097030;
extern CAR *D_0009AC48[];
extern RBS D_00099CE8[];
extern RBS D_0009A208[];
extern RBS D_0009A728[];
extern long long D_00096854;
extern long long D_00096870;
extern long long D_00096898;
extern long long CameraCnt;

extern void StepCars();
extern void CheckCarToCarCollisions();
extern void LongQuaternion2Matrix();
extern void func_0004AD94();
extern int func_00067C9C();
extern void func_00013B80();
extern int func_0003FC98();
extern void func_00047FA4();
extern void func_00035B50();
extern void Setup_Debris();
extern void Start3DSoundVolPitch();
extern void DentCar();
extern int ratan2();
extern void SetShadowPoints();

#define FIXEDH(x) (((x) + 0x800) >> 12)
#define FHR(x) (((x) + 0x1000) >> 13)

static inline void RebuildCarMatrix(RBS *st, CAR *cp)
{
    int sm;
    int osm;
    int qx;

    cp->drawt[0] = cp->t[0] = st->n.fposition[0] >> 4;
    cp->drawt[1] = cp->t[1] = st->n.fposition[1] >> 4;
    cp->drawt[2] = cp->t[2] = st->n.fposition[2] >> 4;

    qx = st->n.orientation[0];
    osm = qx * qx + st->n.orientation[1] * st->n.orientation[1] +
          st->n.orientation[2] * st->n.orientation[2] + st->n.orientation[3] * st->n.orientation[3];
    if (osm < 1024) {
        st->n.orientation[2] = 0;
        st->n.orientation[1] = 0;
        st->n.orientation[0] = 0;
        st->n.orientation[3] = 4096;
    } else {
        sm = 0x1800 - (osm >> 13);
        st->n.orientation[0] = (sm * qx + 0x800) >> 12;
        st->n.orientation[1] = (sm * st->n.orientation[1] + 0x800) >> 12;
        st->n.orientation[2] = (sm * st->n.orientation[2] + 0x800) >> 12;
        st->n.orientation[3] = (sm * st->n.orientation[3] + 0x800) >> 12;
    }
    LongQuaternion2Matrix(st->n.orientation, cp->m);
}

void GlobalTimeStep(void)
{
    long AV[4];
    long delta[4];
    long diff[4];
    long normal[4];
    long collisionpoint[4];
    int depth;
    int i;
    int RKstep;
    int subframe;
    int j;
    CAR *cp;
    CAR *c1;
    RBS *st;
    RBS *tp;
    RBS *d0;
    RBS *d1;
    RBS *thisState_i;
    RBS *thisState_j;
    RBS *thisDelta;
    int howHard;
    int strikeVel;
    int do1;
    int do2;
    int twist;
    int strength;
    int t;
    int u;

    D_00097000 = *(int *)&D_00096834 == 5 && car_data[0].t[2] < -445000 &&
                 car_data[0].t[0] > -123000;

    if (D_000C7004[0] != 127 && (D_000C7004[0] < 170 || D_000C7004[0] >= 190)) {
        if (car_data[0].st.n.angularVelocity[1] > -6000 && car_data[0].st.n.angularVelocity[1] < 6000)
            car_data[0].st.n.angularVelocity[1] = 0;
    }
    if (*(int *)&D_000967F4 == 22) {
        if (D_0009D438[0].st.n.angularVelocity[1] > -6000 && D_0009D438[0].st.n.angularVelocity[1] < 6000)
            D_0009D438[0].st.n.angularVelocity[1] = 0;
    }

    StepCars();
    if (*(int *)&D_000967F4 != 22)
        CheckCarToCarCollisions();

    if (D_00096798 && !D_0009679C)
        D_0009B1D0[0] = 0;

    if (D_0009D8DC[0] == 0 ? D_0009AF48[0] < 0 : D_0009AF48[0] > 0) {
        t = car_cosmetics[0].wheelDisp[1].vz;
        u = car_cosmetics[0].wheelDisp[0].vz;
        car_cosmetics[0].wheelDisp[0].vz = t;
        car_cosmetics[0].wheelDisp[1].vz = u;
        car_cosmetics[0].wheelDisp[2].vz = t;
        car_cosmetics[0].wheelDisp[3].vz = u;
    }

    if (D_0009D8E4[0]) {
        car_cosmetics[0].wheelDisp[0].vy = -245;
        car_cosmetics[0].wheelDisp[1].vy = -245;
        car_cosmetics[0].wheelDisp[2].vy = -245;
        car_cosmetics[0].wheelDisp[3].vy = -245;
    } else if (car_cosmetics[0].wheelDisp[0].vy < -200) {
        car_cosmetics[0].wheelDisp[0].vy = -165;
        car_cosmetics[0].wheelDisp[1].vy = -165;
        car_cosmetics[0].wheelDisp[2].vy = -165;
        car_cosmetics[0].wheelDisp[3].vy = -165;
    }

    for (i = 0; i < D_00097030; i++) {
        CAR *cp;
        RBS *st;
        cp = D_0009AC48[i];

        cp->st.n.linearVelocity[0] += cp->acc[0];
        cp->st.n.linearVelocity[1] += cp->acc[1];
        cp->st.n.linearVelocity[2] += cp->acc[2];

        cp->st.n.angularVelocity[0] += cp->aacc[0];
        cp->st.n.angularVelocity[1] += cp->aacc[1];
        cp->st.n.angularVelocity[2] += cp->aacc[2];

        cp->aacc[0] = 0;
        cp->aacc[1] = 0;
        cp->aacc[2] = 0;

        st = &cp->st;

        if (cp->speed == 0) {
            if (abs(cp->st.n.linearVelocity[0]) + abs(cp->st.n.linearVelocity[1]) + abs(cp->st.n.linearVelocity[2]) < 1000) {
                cp->st.n.linearVelocity[0] = 0;
                cp->st.n.linearVelocity[1] = 0;
                cp->st.n.linearVelocity[2] = 0;
            } else {
                cp->speed = 1;
            }
        }

        if (st->n.angularVelocity[0] > 0x800000)
            st->n.angularVelocity[0] = 0x800000;
        else if (st->n.angularVelocity[0] < -0x800000)
            st->n.angularVelocity[0] = -0x800000;

        if (st->n.angularVelocity[1] > 0x800000)
            st->n.angularVelocity[1] = 0x800000;
        else if (st->n.angularVelocity[1] < -0x800000)
            st->n.angularVelocity[1] = -0x800000;

        if (st->n.angularVelocity[2] > 0x800000)
            st->n.angularVelocity[2] = 0x800000;
        else if (st->n.angularVelocity[2] < -0x800000)
            st->n.angularVelocity[2] = -0x800000;

        if (cp->mayBeColliding)
            continue;

        st->n.fposition[0] += st->n.linearVelocity[0] >> 8;
        st->n.fposition[1] += st->n.linearVelocity[1] >> 8;
        st->n.fposition[2] += st->n.linearVelocity[2] >> 8;

        AV[0] = FHR(st->n.angularVelocity[0]);
        AV[1] = FHR(st->n.angularVelocity[1]);
        AV[2] = FHR(st->n.angularVelocity[2]);

        delta[0] = FIXEDH(-st->n.orientation[1] * AV[2] + st->n.orientation[2] * AV[1] + st->n.orientation[3] * AV[0]);
        delta[1] = FIXEDH(st->n.orientation[0] * AV[2] - st->n.orientation[2] * AV[0] + st->n.orientation[3] * AV[1]);
        delta[2] = FIXEDH(-st->n.orientation[0] * AV[1] + st->n.orientation[1] * AV[0] + st->n.orientation[3] * AV[2]);
        delta[3] = FIXEDH(-st->n.orientation[0] * AV[0] - st->n.orientation[1] * AV[1] - st->n.orientation[2] * AV[2]);

        st->n.orientation[0] += delta[0];
        st->n.orientation[1] += delta[1];
        st->n.orientation[2] += delta[2];
        st->n.orientation[3] += delta[3];

        RebuildCarMatrix(st, cp);
    }

    for (subframe = 0; subframe < 4; subframe++) {
        for (RKstep = 0; RKstep < 2; RKstep++) {
            if (RKstep != 0 && (subframe & 1) && ((D_000C7004[0] & ~3) == 100 || D_00097000))
                func_0004AD94(0);

            for (i = 0; i < D_00097030; i++) {
                cp = D_0009AC48[i];
                if (cp->mayBeColliding == 0)
                    continue;

                if (RKstep == 0) {
                    thisState_i = &cp->st;
                    thisDelta = D_0009A208;
                } else {
                    thisState_i = &D_00099CE8[i];
                    thisDelta = D_0009A728;
                }

                thisDelta[i].n.fposition[0] = thisState_i->n.linearVelocity[0] >> 8;
                thisDelta[i].n.fposition[1] = thisState_i->n.linearVelocity[1] >> 8;
                thisDelta[i].n.fposition[2] = thisState_i->n.linearVelocity[2] >> 8;

                AV[0] = FHR(thisState_i->n.angularVelocity[0]);
                AV[1] = FHR(thisState_i->n.angularVelocity[1]);
                AV[2] = FHR(thisState_i->n.angularVelocity[2]);

                thisDelta[i].n.orientation[0] = FIXEDH(-thisState_i->n.orientation[1] * AV[2] + thisState_i->n.orientation[2] * AV[1] + thisState_i->n.orientation[3] * AV[0]);
                thisDelta[i].n.orientation[1] = FIXEDH(thisState_i->n.orientation[0] * AV[2] - thisState_i->n.orientation[2] * AV[0] + thisState_i->n.orientation[3] * AV[1]);
                thisDelta[i].n.orientation[2] = FIXEDH(-thisState_i->n.orientation[0] * AV[1] + thisState_i->n.orientation[1] * AV[0] + thisState_i->n.orientation[3] * AV[2]);
                thisDelta[i].n.orientation[3] = FIXEDH(-thisState_i->n.orientation[0] * AV[0] - thisState_i->n.orientation[1] * AV[1] - thisState_i->n.orientation[2] * AV[2]);

                thisDelta[i].n.linearVelocity[0] = 0;
                thisDelta[i].n.linearVelocity[1] = 0;
                thisDelta[i].n.linearVelocity[2] = 0;
                thisDelta[i].n.angularVelocity[0] = 0;
                thisDelta[i].n.angularVelocity[1] = 0;
                thisDelta[i].n.angularVelocity[2] = 0;

                for (j = 0; j < i; j++) {
                    c1 = D_0009AC48[j];

                    thisState_j = &c1->st;
                    if (RKstep > 0)
                        thisState_j = &D_00099CE8[j];

                    if (D_00096798 && j == 0 && cp->controlType != 1)
                        continue;
                    if (c1->mayBeColliding == 0)
                        continue;
                    if (c1->speed == 0 && cp->speed == 0)
                        continue;

                    AV[0] = cp->t[0];
                    AV[1] = cp->t[1];
                    AV[2] = cp->t[2];
                    delta[0] = c1->t[0];
                    delta[1] = c1->t[1];
                    delta[2] = c1->t[2];
                    diff[0] = delta[0] - AV[0];
                    diff[1] = delta[1] - AV[1];
                    diff[2] = delta[2] - AV[2];

                    if (diff[0] >= -1200 && diff[0] <= 1200 &&
                        diff[1] <= 1200 && diff[1] >= -1200 &&
                        diff[2] <= 1200 && diff[2] >= -1200 &&
                        func_00067C9C(cp, c1, &depth, collisionpoint, normal)) {
                        long lever0[4];
                        long lever1[4];
                        long torque[4];
                        long pointVel0[4];
                        long pointVel1[4];
                        VEC velocity;

                        collisionpoint[1] += 70;

                        lever0[0] = collisionpoint[0] - AV[0];
                        lever0[1] = collisionpoint[1] - AV[1];
                        lever0[2] = collisionpoint[2] - AV[2];

                        lever1[0] = collisionpoint[0] - delta[0];
                        lever1[1] = collisionpoint[1] - delta[1];
                        lever1[2] = collisionpoint[2] - delta[2];

                        strikeVel = depth * 0xC000;

                        pointVel0[0] = FIXEDH(thisState_i->n.angularVelocity[1] * lever0[2] - thisState_i->n.angularVelocity[2] * lever0[1]);
                        pointVel0[1] = FIXEDH(thisState_i->n.angularVelocity[2] * lever0[0] - thisState_i->n.angularVelocity[0] * lever0[2]);
                        pointVel0[2] = FIXEDH(thisState_i->n.angularVelocity[0] * lever0[1] - thisState_i->n.angularVelocity[1] * lever0[0]);
                        pointVel0[0] += thisState_i->n.linearVelocity[0];
                        pointVel0[1] += thisState_i->n.linearVelocity[1];
                        pointVel0[2] += thisState_i->n.linearVelocity[2];

                        pointVel1[0] = FIXEDH(thisState_j->n.angularVelocity[1] * lever1[2] - thisState_j->n.angularVelocity[2] * lever1[1]);
                        pointVel1[1] = FIXEDH(thisState_j->n.angularVelocity[2] * lever1[0] - thisState_j->n.angularVelocity[0] * lever1[2]);
                        pointVel1[2] = FIXEDH(thisState_j->n.angularVelocity[0] * lever1[1] - thisState_j->n.angularVelocity[1] * lever1[0]);

                        pointVel1[0] += thisState_j->n.linearVelocity[0];
                        pointVel1[1] += thisState_j->n.linearVelocity[1];
                        pointVel1[2] += thisState_j->n.linearVelocity[2];

                        pointVel0[0] -= pointVel1[0];
                        pointVel0[1] -= pointVel1[1];
                        pointVel0[2] -= pointVel1[2];

                        howHard = (pointVel0[0] / 256) * (normal[0] / 32) +
                                  (pointVel0[1] / 256) * (normal[1] / 32) +
                                  (pointVel0[2] / 256) * (normal[2] / 32);

                        if (howHard > 0 && RKstep > -1) {
                            func_00013B80(cp->id, c1->id, howHard);

                            if (cp->id != 23 || c1->id != 23) {
                                if (func_0003FC98(cp, lever0, howHard >> 1, c1))
                                    cp->needsDenting = 1;
                                if (func_0003FC98(c1, lever1, howHard >> 1, cp))
                                    c1->needsDenting = 1;
                            }

                            if (howHard > 0x2800) {
                                velocity.vx = cp->st.n.linearVelocity[0] >> 12;
                                velocity.vy = -17;
                                velocity.vz = cp->st.n.linearVelocity[2] >> 12;
                                collisionpoint[1] = -collisionpoint[1];

                                if (cp->id == 0 || c1->id == 0) {
                                    if (*(int *)&D_00096854)
                                        *(int *)0x1E3358 = 1;
                                    func_00047FA4(collisionpoint, &velocity, 6, 0);
                                    func_00035B50(1);
                                }

                                if (howHard > 0x3555) {
                                    Setup_Debris(collisionpoint, &velocity, 3, 4);
                                    Setup_Debris(collisionpoint, &velocity, 6, (cp->id + 1) << 16);
                                    Setup_Debris(collisionpoint, &velocity, 2, 0x10000);
                                    Start3DSoundVolPitch(2, 0, 0, collisionpoint[0], -collisionpoint[1], collisionpoint[2], 9000, 0x1000);
                                }
                            }
                        }

                        strikeVel += (howHard * 9) / 4;

                        if (strikeVel > 0x96000)
                            strikeVel = 0x96000;

                        normal[0] = (normal[0] >> 3) * (strikeVel >> 3) >> 6;
                        normal[1] = (normal[1] >> 3) * (strikeVel >> 3) >> 6;
                        normal[2] = (normal[2] >> 3) * (strikeVel >> 3) >> 6;

                        do1 = do2 = 1;
                        twist = 55;

                        if (*(unsigned char *)&D_00096898 == 3 || *(unsigned char *)&D_00096898 == 6) {
                            if (c1->id >= 23)
                                do2 = 0;
                            if (cp->id >= 23)
                                do1 = 0;
                        }

                        if (do1) {
                            if (cp->controlType == 1 && c1->controlType != 1 && c1->id != 0) {
                                strength = 7 - *(int *)&D_00096870;
                                velocity.vx = normal[0] * strength / 8;
                                velocity.vy = normal[1] * strength / 8;
                                velocity.vz = normal[2] * strength / 8;

                                thisDelta[i].n.linearVelocity[0] -= velocity.vx;
                                thisDelta[i].n.linearVelocity[1] -= velocity.vy;
                                thisDelta[i].n.linearVelocity[2] -= velocity.vz;

                                torque[0] = FIXEDH(velocity.vy * lever0[2] - velocity.vz * lever0[1]);
                                torque[1] = FIXEDH(velocity.vz * lever0[0] - velocity.vx * lever0[2]);
                                torque[2] = FIXEDH(velocity.vx * lever0[1] - velocity.vy * lever0[0]);
                            } else {
                                thisDelta[i].n.linearVelocity[0] -= normal[0];
                                thisDelta[i].n.linearVelocity[1] -= normal[1];
                                thisDelta[i].n.linearVelocity[2] -= normal[2];

                                torque[0] = FIXEDH(normal[1] * lever0[2] - normal[2] * lever0[1]);
                                torque[1] = FIXEDH(normal[2] * lever0[0] - normal[0] * lever0[2]);
                                torque[2] = FIXEDH(normal[0] * lever0[1] - normal[1] * lever0[0]);
                            }

                            torque[0] *= twist;
                            torque[1] *= twist;
                            torque[2] *= twist;

                            thisDelta[i].n.angularVelocity[0] += torque[0];
                            thisDelta[i].n.angularVelocity[1] += torque[1];
                            thisDelta[i].n.angularVelocity[2] += torque[2];
                        }

                        if (do2) {
                            if (cp->controlType != 1 && c1->controlType == 1 && cp->id != 0) {
                                strength = 7 - *(int *)&D_00096870;
                                velocity.vx = normal[0] * strength / 8;
                                velocity.vy = normal[1] * strength / 8;
                                velocity.vz = normal[2] * strength / 8;

                                thisDelta[j].n.linearVelocity[0] += velocity.vx;
                                thisDelta[j].n.linearVelocity[1] += velocity.vy;
                                thisDelta[j].n.linearVelocity[2] += velocity.vz;

                                torque[0] = FIXEDH(lever1[1] * velocity.vz - lever1[2] * velocity.vy);
                                torque[1] = FIXEDH(lever1[2] * velocity.vx - lever1[0] * velocity.vz);
                                torque[2] = FIXEDH(lever1[0] * velocity.vy - lever1[1] * velocity.vx);
                            } else {
                                thisDelta[j].n.linearVelocity[0] += normal[0];
                                thisDelta[j].n.linearVelocity[1] += normal[1];
                                thisDelta[j].n.linearVelocity[2] += normal[2];

                                torque[0] = FIXEDH(lever1[1] * normal[2] - lever1[2] * normal[1]);
                                torque[1] = FIXEDH(lever1[2] * normal[0] - lever1[0] * normal[2]);
                                torque[2] = FIXEDH(lever1[0] * normal[1] - lever1[1] * normal[0]);
                            }

                            torque[0] *= twist;
                            torque[1] *= twist;
                            torque[2] *= twist;

                            thisDelta[j].n.angularVelocity[0] += torque[0];
                            thisDelta[j].n.angularVelocity[1] += torque[1];
                            thisDelta[j].n.angularVelocity[2] += torque[2];
                        }
                    }
                }
            }

            for (i = 0; i < D_00097030; i++) {
                CAR *cp;
                RBS *st;
                int k;
                cp = D_0009AC48[i];
                if (cp->mayBeColliding == 0)
                    continue;

                st = &cp->st;
                tp = &D_00099CE8[i];
                d0 = &D_0009A208[i];
                d1 = &D_0009A728[i];

                switch (RKstep) {
                case 0:
                    for (k = 12; k >= 0; k--)
                        tp->v[k] = st->v[k] + (d0->v[k] >> 2);
                    RebuildCarMatrix(tp, cp);
                    break;
                case 1:
                    for (k = 12; k >= 0; k--)
                        st->v[k] += (d0->v[k] + d1->v[k]) >> 3;
                    RebuildCarMatrix(st, cp);
                    break;
                }
            }
        }
    }

    for (i = 0; i < D_00097030; i++) {
        CAR *cp;
        cp = D_0009AC48[i];

        if (cp->needsDenting && ((*(int *)&CameraCnt + i) & 3) == 0) {
            DentCar(cp->id);
            cp->needsDenting = 0;
        }

        cp->direction = ratan2(cp->m[0][2], cp->m[2][2]);
        cp->drawt[0] = cp->t[0];
        cp->drawt[1] = cp->t[1];
        cp->drawt[2] = cp->t[2];
        cp->vel[0] = FIXEDH(cp->st.n.linearVelocity[0]);
        cp->vel[1] = FIXEDH(cp->st.n.linearVelocity[1]);
        cp->vel[2] = FIXEDH(cp->st.n.linearVelocity[2]);
        SetShadowPoints(cp);
    }
}
