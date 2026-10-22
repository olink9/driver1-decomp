/* Setup_Smoke (Driver 2 name; AllocateSmoke inlined) */
typedef struct {
    int vx, vy, vz, pad;
} VEC;

typedef struct {
    int position[4];
    int drift[4];
    short drift_change[4];
    char pad28[0x10];
    short start_w;
    short final_w;
    short transparency;
    short t_step;
    int step;
    unsigned char life;
    unsigned char halflife;
    char pad46[2];
    int flags;
    char pad4C[4];
} SMOKE;

extern int D_0009742C;
extern short D_000BB530[];
extern SMOKE D_000B92A0[];
extern int func_000776E4(void);

static inline int AllocateSmoke(void)
{
    if (D_0009742C < 80)
        return D_000BB530[D_0009742C++];
    return -1;
}

void Setup_Smoke(VEC *ipos, int start_w, int end_w, int SmokeType, int WheelSpeed, VEC *Drift, int Exhaust)
{
    int num;
    SMOKE *mysmoke;

    num = AllocateSmoke();
    if (num == -1)
        return;

    mysmoke = &D_000B92A0[num];

    if (SmokeType == 4) {
        mysmoke->position[0] = ipos->vx;
        mysmoke->position[1] = ipos->vy;
        mysmoke->position[2] = ipos->vz;
        mysmoke->start_w = start_w;
        mysmoke->final_w = end_w;
        mysmoke->step = (end_w - start_w) / 128 * 4;
        mysmoke->flags = 0x1006;
        mysmoke->life = 20;
        mysmoke->halflife = 10;
        if (WheelSpeed > 200000)
            mysmoke->transparency = (800000 - WheelSpeed) >> 11;
        else
            mysmoke->transparency = 255;
        mysmoke->t_step = (end_w - start_w) >> 2;
    } else if (SmokeType == 1) {
        if (Exhaust) {
            mysmoke->position[0] = ipos->vx + (func_000776E4() & 0x3f);
            mysmoke->position[1] = ipos->vy;
            mysmoke->position[2] = ipos->vz + (func_000776E4() & 0x3f);
            mysmoke->flags = 0x2006;
            mysmoke->transparency = 140;
            mysmoke->t_step = 2;
        } else {
            mysmoke->position[0] = ipos->vx;
            mysmoke->position[1] = ipos->vy;
            mysmoke->position[2] = ipos->vz;
            if (WheelSpeed > 200000)
                mysmoke->transparency = (800000 - WheelSpeed) >> 11;
            else
                mysmoke->transparency = 255;
            mysmoke->t_step = (end_w - start_w) >> 5;
        }
        mysmoke->start_w = start_w;
        mysmoke->final_w = end_w;
        mysmoke->step = (end_w - start_w) / 128 * 4;
        mysmoke->flags = 0x2006;
        mysmoke->life = 40;
        mysmoke->halflife = 20;
    } else if (SmokeType == 2 || SmokeType == 5) {
        if (Exhaust) {
            mysmoke->position[0] = ipos->vx + (func_000776E4() & 7);
            mysmoke->position[1] = ipos->vy;
            mysmoke->position[2] = ipos->vz + (func_000776E4() & 7);
            mysmoke->flags = 0x4006;
            mysmoke->transparency = 70;
            mysmoke->t_step = 2;
        } else {
            mysmoke->position[0] = ipos->vx + (func_000776E4() & 0x3f);
            mysmoke->position[1] = ipos->vy;
            mysmoke->position[2] = ipos->vz + (func_000776E4() & 0x3f);
            mysmoke->flags = 0x16;
        }
        mysmoke->start_w = start_w;
        mysmoke->final_w = end_w;
        if (SmokeType == 5) {
            mysmoke->life = 32;
            mysmoke->step = (end_w - start_w) / 32 * 4;
            mysmoke->t_step = (end_w - start_w) >> 5;
            mysmoke->transparency = WheelSpeed;
        } else {
            mysmoke->life = 128;
            mysmoke->step = (end_w - start_w) / mysmoke->life * 8;
            mysmoke->t_step = (end_w - start_w) >> 6;
            mysmoke->transparency = 80;
        }
        mysmoke->halflife = 64;
    } else if (SmokeType == 6) {
        mysmoke->position[0] = ipos->vx;
        mysmoke->position[1] = ipos->vy;
        mysmoke->position[2] = ipos->vz;
        mysmoke->flags = 0x20006;
        mysmoke->transparency = 160;
        mysmoke->step = 20;
        mysmoke->t_step = 5;
        mysmoke->start_w = start_w;
        mysmoke->final_w = end_w;
        mysmoke->life = 178;
        mysmoke->halflife = 32;
    } else {
        mysmoke->position[0] = ipos->vx + (func_000776E4() & 0x3f);
        mysmoke->position[1] = ipos->vy;
        mysmoke->position[2] = ipos->vz + (func_000776E4() & 0x3f);
        mysmoke->flags = 0x10006;
        mysmoke->transparency = 60;
        mysmoke->t_step = 5;
        mysmoke->start_w = start_w;
        mysmoke->final_w = end_w;
        mysmoke->step = (end_w - start_w) / 128 * 8;
        mysmoke->life = 128;
        mysmoke->halflife = 64;
    }

    if (Exhaust) {
        mysmoke->drift[0] = Drift->vx;
        mysmoke->drift[1] = Drift->vy;
        mysmoke->drift[2] = Drift->vz;
        mysmoke->drift_change[0] = 0;
        mysmoke->drift_change[1] = 0;
        mysmoke->drift_change[2] = 0;
    } else {
        mysmoke->drift[0] = 0;
        mysmoke->drift[1] = 0;
        mysmoke->drift[2] = 0;
        if (mysmoke->life < 40 || SmokeType == 4) {
            mysmoke->drift_change[0] = 0;
            mysmoke->drift_change[1] = 1;
            mysmoke->drift_change[2] = 0;
        } else {
            mysmoke->drift_change[0] = Drift->vx;
            mysmoke->drift_change[1] = Drift->vy;
            mysmoke->drift_change[2] = Drift->vz;
        }
    }
}
