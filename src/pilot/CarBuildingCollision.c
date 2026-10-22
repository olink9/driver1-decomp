/* FAKEMATCH: the do { } while (0) around the velocity step (non-camera branch) only exists to give cp
 * loop-weighted refs so global-alloc puts cp in s2 ahead of the collisionResult base (s3); probably a
 * macro in the original. hit.vx / surfNormal.vx are read as non-struct (*(int *)&...) to get the
 * scheduler/CSE behaviour of the target; LeafPosition uses a (short) cast to stop reassociation. */
/* Driver 2: CarBuildingCollision (DamageCar inlined) */
typedef struct { int vx, vy, vz, pad; } VECTOR;

typedef struct {
    VECTOR x;          /* 0x00 */
    VECTOR axis[2];    /* 0x10 */
    VECTOR vel;        /* 0x30 */
    int theta;         /* 0x40 */
    int length[2];     /* 0x44 */
    int dist[2];       /* 0x4C */
    int limit[2];      /* 0x54 */
    int avel;          /* 0x5C */
} CDATA2D;

typedef struct {
    VECTOR hit;        /* 0x00 */
    VECTOR surfNormal; /* 0x10 */
    int penetration;   /* 0x20 */
    int neverfree;
} CRET2D;

typedef struct {
    unsigned char controlType;  /* 0x00 */
    char pad0[0x17];
    int where[3];               /* 0x18 */
    char pad1[0x40 - 0x24];
    int linearVelocity[3];      /* 0x40 */
    int angularVelocity[3];     /* 0x4C */
    char pad2[0x68 - 0x58];
    int aacc[3];                /* 0x68 */
    char pad3[0xF8 - 0x74];
    int direction;              /* 0xF8 */
    char pad4[0x104 - 0xFC];
    int wheel_speed;            /* 0x104 */
    int speed;                  /* 0x108 */
    char pad5[0x114 - 0x10C];
    int damage[6];              /* 0x114 */
    char pad6[0x12E - 0x12C];
    unsigned char hndType;      /* 0x12E */
} CAR_DATA;

typedef struct {
    int vx, vy, vz;
    int pad;
    int xsize, zsize;
    int theta;
    int height;
} BUILDING_BOX;

typedef struct {
    int vx, vy, vz;
    unsigned short pad;
    unsigned short type;
} CELL_OBJECT;

typedef struct {
    unsigned short shape_flags;
    unsigned short flags2;
} MODEL;

typedef struct {
    MODEL *model;
    char *name;
    int sound;
    int volume;
    int pitch;
} SMASHABLE_OBJECT;

extern MODEL *modelpointers[];
extern CDATA2D D_000B70C4[2];
extern CRET2D D_000B7184;
extern VECTOR D_000B7194;
extern long long D_00096A60;
extern long long gNight;
extern long long D_000974A8;
extern SMASHABLE_OBJECT D_0008AE8C[];
extern SMASHABLE_OBJECT D_0008AEB4;
extern char D_000988C8[];
extern char D_000C6F04[];

extern int bcollided2d();
extern void bFindCollisionTime();
extern void bFindCollisionPoint();
extern void func_000650E4();
extern void damage_object();
extern void func_0006821C();
extern void func_0004D8B0();
extern void Start3DSoundVolPitch();
extern void func_00047FA4();
extern void Setup_Debris();
extern void func_00035B50();
extern int func_000776E4();
extern void func_00041AC4();
extern void func_0004157C();
extern int damage_lamp();
extern void memset();
extern void func_0003FAE8();

#define FIXEDH(x) (((x) + 2048) >> 12)

static inline void DamageCar(CAR_DATA *cp, CDATA2D *cd, CRET2D *collisionResult, int strikeVel)
{
    VECTOR d;
    int nose, door, region, value;

    if (strikeVel < 0 || cp->controlType == 0x17 || cp->speed < 10)
        return;
    d.vx = collisionResult->hit.vx - cd->x.vx;
    d.vz = collisionResult->hit.vz - cd->x.vz;

    nose = FIXEDH(cd->axis[0].vx * d.vx + cd->axis[0].vz * d.vz);
    door = FIXEDH(cd->axis[1].vx * d.vx + cd->axis[1].vz * d.vz);

    if (door > 0) {
        if (nose < 0x105) {
            if (nose >= -0x103)
                region = 2;
            else
                region = 3;
        } else
            region = 1;
    } else {
        if (nose < 0x105) {
            if (nose >= -0x103)
                region = 5;
            else
                region = 4;
        } else
            region = 0;
    }

    strikeVel = strikeVel / 300;
    if (strikeVel > 0x800)
        strikeVel = 0x800;

    func_0003FAE8(cp, region, strikeVel - (strikeVel * cp->damage[region] >> 13), cp->controlType == 0x17);
}

int CarBuildingCollision(CAR_DATA *cp, BUILDING_BOX *building, CELL_OBJECT *cop)
{
    VECTOR tempwhere;
    VECTOR velocity;
    int pointVel[4];
    int reaction[4];
    int lever[4];
    MODEL *model;
    int boxDiffY;
    int collided;
    int strikeVel;
    int displacement;
    int denom;
    int lever_dot_n;
    int ret;

    model = modelpointers[cop->type];

    boxDiffY = abs(cp->where[1] + building->vy);

    if (building->height / 2 < boxDiffY)
        return 0;
    if (cop->vx == 0xFD46FEC0)
        return 0;
    if (model->shape_flags & 0x10)
        return 0;

    tempwhere.vx = cp->where[0];
    tempwhere.vz = cp->where[2];

    D_000B70C4[0].x.vx = cp->where[0];
    D_000B70C4[0].x.vz = cp->where[2];
    D_000B70C4[0].theta = cp->direction;

    if (cp->controlType == 0x19) {
        D_000B70C4[0].vel.vx = 0;
        D_000B70C4[0].vel.vz = 0;
        D_000B70C4[0].length[1] = -1;
        D_000B70C4[0].length[0] = *(int *)&D_00096A60 / 2;
    } else {
        do {
            D_000B70C4[0].vel.vx = FIXEDH(cp->linearVelocity[0]);
            D_000B70C4[0].vel.vz = FIXEDH(cp->linearVelocity[2]);
            cp->where[0] += D_000B70C4[0].vel.vx;
            cp->where[2] += D_000B70C4[0].vel.vz;
        } while (0);
        D_000B70C4[0].length[0] = 0x208;
        D_000B70C4[0].length[1] = (cp->hndType == 1) ? 0x8A : 0xA9;
    }

    D_000B70C4[0].avel = FIXEDH(cp->angularVelocity[1]) * 5 >> 5;

    D_000B70C4[1].x.vx = building->vx;
    D_000B70C4[1].x.vz = building->vz;
    D_000B70C4[1].theta = building->theta;
    D_000B70C4[1].length[0] = building->xsize;
    D_000B70C4[1].length[1] = building->zsize;
    D_000B70C4[1].vel.vx = 0;
    D_000B70C4[1].vel.vz = 0;
    D_000B70C4[1].avel = 0;

    collided = bcollided2d(D_000B70C4);

    if (cp->controlType == 0x19)
        return collided != 0;

    if (collided) {
        bFindCollisionTime(D_000B70C4, &D_000B7184);
        bFindCollisionPoint(D_000B70C4, &D_000B7184);

        *(int *)&D_000B7184.surfNormal = -*(int *)&D_000B7184.surfNormal;
        D_000B7184.surfNormal.vy = 0;
        D_000B7184.hit.vy = cp->where[1] + 60;
        D_000B7184.surfNormal.vy = 0;
        D_000B7184.surfNormal.vz = -D_000B7184.surfNormal.vz;

        cp->where[0] += FIXEDH(D_000B7184.penetration * *(int *)&D_000B7184.surfNormal);
        cp->where[2] += FIXEDH(D_000B7184.penetration * D_000B7184.surfNormal.vz);
        lever[0] = *(int *)&D_000B7184 - cp->where[0];

        lever[1] = D_000B7184.hit.vy - cp->where[1];
        lever[2] = D_000B7184.hit.vz - cp->where[2];

        pointVel[0] = FIXEDH(cp->angularVelocity[1] * lever[2] - cp->angularVelocity[2] * lever[1]);
        pointVel[1] = FIXEDH(cp->angularVelocity[2] * lever[0] - cp->angularVelocity[0] * lever[2]);
        pointVel[2] = FIXEDH(cp->angularVelocity[0] * lever[1] - cp->angularVelocity[1] * lever[0]);
        pointVel[0] += cp->linearVelocity[0];
        pointVel[1] += cp->linearVelocity[1];
        pointVel[2] += cp->linearVelocity[2];

        strikeVel = -((pointVel[0] / 256) * (*(int *)&D_000B7184.surfNormal / 16) +
                      (pointVel[1] / 256) * (D_000B7184.surfNormal.vy / 16) +
                      (pointVel[2] / 256) * (D_000B7184.surfNormal.vz / 16));

        if (strikeVel > 0) {
            if (cp->controlType == 0) {
                short scale;

                if (strikeVel < 32)
                    scale = strikeVel << 7;
                else
                    scale = 4096;

                if (model->flags2 & 0x800)
                    func_000650E4(D_000988C8, 7, scale);
                else
                    func_000650E4(D_000988C8, 6, scale);
            }

            D_000B7184.hit.vy = -D_000B7184.hit.vy;

            velocity.vx = cp->linearVelocity[0] / 4096;
            velocity.vy = -17;
            velocity.vz = cp->linearVelocity[2] / 4096;

            if (model->flags2 & 0x800) {
                damage_object(cop, &velocity);
                func_0006821C(D_000C6F04);

                if (model->shape_flags & 0x8) {
                    func_0004D8B0(2, 0, 0, D_000B7184.hit.vx, -D_000B7184.hit.vy, D_000B7184.hit.vz);
                } else {
                    SMASHABLE_OBJECT *sip;
                    SMASHABLE_OBJECT *match;

                    match = 0;
                    for (sip = D_0008AE8C; sip->name; sip++) {
                        if (sip->model == model) {
                            match = sip;
                            break;
                        }
                    }
                    if (match == 0)
                        match = &D_0008AEB4;

                    Start3DSoundVolPitch(-1, 0, match->sound, D_000B7184.hit.vx, -D_000B7184.hit.vy, D_000B7184.hit.vz,
                                  match->volume, match->pitch);
                }

                cp->where[0] = tempwhere.vx;
                cp->where[2] = tempwhere.vz;

                D_000B7184.hit.vy += 30;

                func_00047FA4(&D_000B7184, &velocity, 10, 0);
                Setup_Debris(&D_000B7184, &velocity, 5, 0);
                Setup_Debris(&D_000B7184, &velocity, 5, 0x10000);

                if (cp->controlType == 0)
                    func_00035B50(3);

                return 0;
            }

            if (strikeVel > 0x5000 && cp->wheel_speed + 16000U > 32000) {
                if (model->flags2 & 0x2000) {
                    int rnd;
                    VECTOR LeafPosition;

                    rnd = func_000776E4();
                    LeafPosition.vx = D_000B7184.hit.vx;
                    LeafPosition.vy = -D_000B7184.hit.vy - (short)((rnd & 0xFE) + 600);
                    LeafPosition.vz = D_000B7184.hit.vz;

                    func_00041AC4(&LeafPosition, 3, 1);

                    if ((rnd & 0xF) == 0) {
                        LeafPosition.vy += 100;
                        func_0004157C(&LeafPosition, (rnd >> 8) & 3);
                    }
                } else {
                    if (*(int *)&gNight && (modelpointers[*(unsigned short *)&D_000974A8]->flags2 & 0x1000)) {
                        if (damage_lamp(cop)) {
                            VECTOR lamp_velocity;

                            memset(&lamp_velocity, 0, 16);
                            D_000B7184.hit.vy = -730;
                            func_00047FA4(&D_000B7184, &lamp_velocity, 20, 0);
                            D_000B7184.hit.vy = 730;
                        }
                    }

                    velocity.vy -= 20;
                    D_000B7184.hit.vy += 30;

                    func_00047FA4(&D_000B7184, &velocity, 4, 0);

                    D_000B7184.hit.vy -= 30;
                    velocity.vy += 20;
                }

                if (strikeVel > 0x28000) {
                    Setup_Debris(&D_000B7184, &velocity, 6, (cp->controlType + 1) << 16);

                    if (cp->controlType == 0)
                        func_00035B50(1);
                }

                func_0004D8B0(2, 0, 0, D_000B7184.hit.vx, -D_000B7184.hit.vy, D_000B7184.hit.vz);
            }

            DamageCar(cp, D_000B70C4, &D_000B7184, strikeVel);

            lever_dot_n = FIXEDH(lever[0] * D_000B7194.vx + lever[1] * D_000B7194.vy + lever[2] * D_000B7194.vz);
            displacement = FIXEDH(((lever[0] * lever[0] + lever[2] * lever[2]) - lever_dot_n * lever_dot_n) * 110) + 4096;

            if (strikeVel > 0x7F000)
                strikeVel = (strikeVel / displacement) * 4096;
            else
                strikeVel = (strikeVel * 4096) / displacement;

            reaction[0] = (strikeVel / 64) * (D_000B7194.vx / 64);
            reaction[1] = (strikeVel / 64) * (D_000B7194.vy / 64);
            reaction[2] = (strikeVel / 64) * (D_000B7194.vz / 64);

            cp->aacc[0] += FIXEDH(lever[1] * reaction[2]);
            cp->aacc[0] -= FIXEDH(lever[2] * reaction[1]);
            cp->aacc[1] += FIXEDH(lever[2] * reaction[0]);
            cp->aacc[1] -= FIXEDH(lever[0] * reaction[2]);
            cp->aacc[2] += FIXEDH(lever[0] * reaction[1]);
            cp->aacc[2] -= FIXEDH(lever[1] * reaction[0]);

            cp->linearVelocity[0] += reaction[0];
            cp->linearVelocity[1] += reaction[1];
            cp->linearVelocity[2] += reaction[2];
        }
        ret = 1;
    } else {
        ret = 0;
    }

    cp->where[0] -= FIXEDH(cp->linearVelocity[0]);
    cp->where[2] -= FIXEDH(cp->linearVelocity[2]);

    return ret;
}
