/* SwirlLeaves */
typedef struct { int vx, vy, vz; } V3;
typedef struct { V3 position; int pad0; short dir[4]; short pad[1]; unsigned short flags; char pad3[0x2C-0x1C]; } LEAF;
typedef struct { V3 t; char pad[0x30]; int wheel_speed; char pad2[0x18C - 0x40]; } CAR;
extern CAR D_0009B16C[];
extern int D_00096AC0;
extern long long D_0009687C;
extern LEAF D_000B7584[];
extern int func_000776E4();

void func_00041CE4(int car) {
    int ZDiff;
    LEAF *lpLeaf;
    int count;
    CAR *cp = &D_0009B16C[car];

    if (cp->wheel_speed + 39999U <= 0x1d4be || D_00096AC0 == 0 || *(int *)&D_0009687C != 0)
        return;
    lpLeaf = D_000B7584;
    for (count = 0; count < 50; count++) {
        ZDiff = cp->t.vz - lpLeaf->position.vz;
        if ((lpLeaf->flags & 2) && lpLeaf->position.vy + cp->t.vy > -200 &&
            (unsigned)(cp->t.vx - lpLeaf->position.vx + 359) < 719 && ZDiff < 360 && ZDiff > -360) {
            lpLeaf->dir[1] = -30 - (func_000776E4() & 0x3f);
            lpLeaf->position.vy -= 1;
        }
        lpLeaf++;
    }
}
