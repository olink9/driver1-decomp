/* Driver 2: CheckCarToCarCollisions (HANDLING.C) */
typedef struct {
    short m[3][3];
    int t[3];
} MATRIX;

typedef struct {
    int pad0;
    MATRIX where;
    char pad24[0x40 - 0x24];
    int linearVelocity[3];
    char pad4C[0xCC - 0x4C];
    int y;
    char padD0[0x12C - 0xD0];
    short mayBeColliding;
    char pad12E[0x18C - 0x12E];
} CAR_DATA;

typedef struct {
    int x0, y0, z0;
    int x1, y1, z1;
} BOUND_BOX;

typedef struct {
    unsigned char b0, active;
    char pad[0x50 - 2];
} ENT50;

extern CAR_DATA D_0009B0A4[];
extern BOUND_BOX D_0009ACAC[];
extern ENT50 D_00098950[];
extern long long D_00096898;
extern long long D_00096880;
extern short D_0009D564[];

#define FIXEDH(a) (((a) + 0x800) >> 12)

void func_0001FAFC(void)
{
    int loop1, loop2;
    int xx, zz;
    CAR_DATA *cp;
    BOUND_BOX *bb, *bb1, *bb2;

    cp = D_0009B0A4;
    if (*(unsigned char *)&D_00096898 == 3 || *(unsigned char *)&D_00096898 == 6)
        cp[23].mayBeColliding = 1;

    if (*(unsigned char *)&D_00096880 != 0)
        return;

    bb = D_0009ACAC;
    loop1 = 0;
    do {
        xx = FIXEDH(abs(cp->where.m[0][2] * 585) + abs(cp->where.m[0][0] * 209)) + 90;
        zz = FIXEDH(abs(cp->where.m[2][2] * 585) + abs(cp->where.m[2][0] * 209)) + 90;

        bb->x0 = (cp->where.t[0] - xx) / 16;
        bb->z0 = (cp->where.t[2] - zz) / 16;
        bb->x1 = (cp->where.t[0] + xx) / 16;
        bb->z1 = (cp->where.t[2] + zz) / 16;

        if (cp->linearVelocity[0] < 0)
            bb->x0 += FIXEDH(cp->linearVelocity[0]) / 8;
        else
            bb->x1 += FIXEDH(cp->linearVelocity[0]) / 8;

        if (cp->linearVelocity[2] < 0)
            bb->z0 += FIXEDH(cp->linearVelocity[2]) / 8;
        else
            bb->z1 += FIXEDH(cp->linearVelocity[2]) / 8;

        bb->y0 = (cp->y - 2400) / 16;
        bb->y1 = (cp->y + 2400) / 16;

        loop1++;
        bb++;
        cp++;
    } while (loop1 < 25);

    bb1 = D_0009ACAC;
    loop1 = 0;
    do {
        bb2 = bb1 + 1;
        loop2 = loop1 + 1;
        while (loop2 < 25) {
            if (bb1->x1 > bb2->x0 && bb1->z1 > bb2->z0 && bb1->x0 < bb2->x1 &&
                bb1->z0 < bb2->z1 && bb1->y1 > bb2->y0 && bb1->y0 < bb2->y1 &&
                (loop1 == 0 || D_00098950[loop1 - 1].active != 0) &&
                D_00098950[loop2 - 1].active != 0) {
                D_0009B0A4[loop1].mayBeColliding = 1;
                D_0009B0A4[loop2].mayBeColliding = 1;
            }
            loop2++;
            bb2++;
        }
        loop1++;
        bb1++;
    } while (loop1 < 23);

    if (*(unsigned char *)&D_00096898 == 7 || *(unsigned char *)&D_00096898 == 8)
        D_0009D564[0] = 0;
}
