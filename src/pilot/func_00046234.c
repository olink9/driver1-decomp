/* func_00046234: car damage smoke/sparks per player (no Driver 2 counterpart) */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;

typedef struct {
    char pad0[2];
    unsigned char model;
    char pad3[0x114 - 3];
    int dmg0;
    int dmg1;
    char pad11c[0x120 - 0x11C];
    int dmg3;
    int dmg2;
    char pad128[0x132 - 0x128];
    unsigned char flash;
    char pad133[0x18C - 0x133];
} CAR;

typedef struct {
    SVECTOR a;
    char pad8[0x18 - 8];
    SVECTOR b;
    char pad20[0x64 - 0x20];
} MODEL;

typedef struct { char pad0; unsigned char type; char pad[0x4E]; } CIV;

extern CAR car_data[];
extern MODEL car_cosmetics[];
extern CIV D_00098900[];
extern char D_000B9170[];
extern long long D_00096854;
extern int D_00096AA4;
extern int D_0009740C;
extern unsigned char D_00097458;
extern unsigned char D_00097460;
extern void func_00043FDC();
extern void PlacePoolForCar();

void func_00046234(int player)
{
    SVECTOR v;
    SVECTOR v2;
    CVECTOR col;
    int n;
    CIV *c;
    unsigned char *flash;
    MODEL *m;
    CAR *car;

    n = 0;
    flash = &car_data[player].flash;
    D_00097458 = 0;
    D_00097460 = 0;
    if (player == 0)
        m = &car_cosmetics[0];
    else
        m = &car_cosmetics[car_data[player].model];
    if (player != 0) {
        c = &D_00098900[player];
        if (c->type == 1)
            return;
    }

    v = m->a;
    v2.vx = v.vx - 30;
    v2.vy = v.vy;
    v2.vz = v.vz;

    v.vz += car_data[player].dmg0 >> 6;
    D_0009740C = player;
    v2.vz += car_data[player].dmg0 >> 6;
    if (car_data[player].dmg0 < 1000) {
        D_00097458 = 1;
        col.r = 255;
        col.b = 255;
        col.g = 255;
        func_00043FDC(&v, player, &col, 30, D_000B9170, 9);
        n++;
        if (car_data[player].dmg0 < 500) {
            n++;
            D_00097458 = 2;
            col.r = 255;
            col.b = 255;
            col.g = 255;
            func_00043FDC(&v2, player, &col, 30, D_000B9170, 1);
        }
    }

    v = m->a;
    v.vx = -v.vx;
    v2.vx = v.vx + 30;
    v2.vy = v.vy;

    v.vz += car_data[player].dmg1 >> 6;
    v2.vz = v.vz;
    if (car_data[player].dmg1 < 1000) {
        D_00097460 = 1;
        col.r = 255;
        col.b = 255;
        col.g = 255;
        func_00043FDC(&v, player, &col, 30, D_000B9170, 0x11);
        n++;
        if (car_data[player].dmg1 < 500) {
            n++;
            D_00097460 = 2;
            col.r = 255;
            col.b = 255;
            col.g = 255;
            func_00043FDC(&v2, player, &col, 30, D_000B9170, 1);
        }
    }

    col.r = 200;
    if (player > 0)
        col.r = 255;
    col.b = 0;
    col.g = 0;
    v = m->b;

    if (car_data[player].dmg2 < 500) {
        func_00043FDC(&v, player, &col, 25, D_000B9170, 2);
        *flash += 16;
    }
    if (car_data[player].dmg3 < 500) {
        v.vx = -v.vx;
        func_00043FDC(&v, player, &col, 25, D_000B9170, 4);
        *flash += 16;
    }
    if (n != 0) {
        col.r = n << 4;
        col.b = n << 4;
        col.g = n << 4;
        PlacePoolForCar(player, &col, 1);
    }
    if (*flash != 0) {
        col.r = *flash;
        col.b = 0;
        col.g = 0;
        PlacePoolForCar(player, &col, 0);
        *flash = 0;
    }
    if (player == 0) {
        if (*(int *)&D_00096854 != 0)
            D_00096AA4 = n * 2;
        else
            D_00096AA4 = n;
    }
}
