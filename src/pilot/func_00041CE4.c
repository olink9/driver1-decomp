/* SwirlLeaves (run2-r3 r3-11): wheel_speed read through an int-cast address so CSE keeps the two
   index*396+base computations separate (target computes it twice); pauseflag test split after lpLeaf init. */
typedef struct { int vx, vy, vz; } V3;
typedef struct { V3 position; int pad0; short dir[4]; short pad[1]; unsigned short flags; char pad3[0x2C-0x1C]; } LEAF;
typedef struct { V3 t; char pad[0x30]; int wheel_speed; char pad2[0x18C - 0x40]; } CAR;
extern CAR D_0009B16C[];
extern int D_00096AC0;
extern long long D_0009687C;
extern LEAF D_000B7584[];
extern int func_000776E4();

void func_00041CE4(int car) {
    int XDiff;
    int ZDiff;
    LEAF *lpLeaf;
    int count;
    V3 *CarPos;
    int WheelSpeed;

    CarPos = &D_0009B16C[car].t;
    WheelSpeed = *(int *)((int)D_0009B16C + car * sizeof(CAR) + 0x3c);
    if (WheelSpeed + 39999U <= 0x1d4be || D_00096AC0 == 0)
        return;
    lpLeaf = D_000B7584;
    if (*(int *)&D_0009687C != 0)
        return;
    for (count = 0; count < 50; count++, lpLeaf++) {
        if ((lpLeaf->flags & 2) && lpLeaf->position.vy + CarPos->vy > -200) {
            XDiff = CarPos->vx - lpLeaf->position.vx;
            ZDiff = CarPos->vz - lpLeaf->position.vz;
            if ((unsigned)(XDiff + 359) < 719 && ZDiff < 360 && ZDiff > -360) {
                lpLeaf->dir[1] = -30 - (func_000776E4() & 0x3f);
                lpLeaf->position.vy -= 1;
            }
        }
    }
}
