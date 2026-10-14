/* find the car ahead on the same road/lane within maxdist */
typedef struct {
    unsigned char id;
    unsigned char active;
    unsigned char pad2[7];
    unsigned char f9;
    unsigned char padA[10];
    unsigned char f14;
    unsigned char pad15[15];
    int dir;
    int road;
    unsigned char pad2C[28];
    int x;
    int z;
} CAR;

typedef struct {
    char pad[200];
    int x;
    int pad2;
    int z;
    char pad3[396 - 212];
} CARDATA;

extern CAR D_00098950[];
extern CARDATA D_0009B230[];

int func_00018F44(CAR *cp, int maxdist, int mode)
{
    CAR *c;
    int dist;
    int i;

    dist = 1000;
    c = D_00098950;
    if (cp->f14 != 0)
        return -1;
    cp->x = D_0009B230[cp->id].x / 1500;
    cp->z = D_0009B230[cp->id].z / 1500;
    if (mode) {
        for (i = 0; i < 22; i++, c++) {
            if (cp->id != c->id && c->active != 0 && cp->road == c->road && cp->dir == c->dir && cp->f9 == c->f9) {
                switch (cp->dir) {
                case 0:
                    if (cp->z > c->z)
                        dist = cp->z - c->z;
                    break;
                case 2:
                    if (c->z > cp->z)
                        dist = c->z - cp->z;
                    break;
                case 1:
                    if (c->x > cp->x)
                        dist = c->x - cp->x;
                    break;
                case 3:
                    if (cp->x > c->x)
                        dist = cp->x - c->x;
                    break;
                }
                if (dist >= 0 && dist <= maxdist)
                    return i;
            }
        }
        return -1;
    }
    for (i = 0; i < 22; i++, c++) {
        if (cp->id != c->id && c->active != 1 && c->active != 0 && cp->road == c->road && cp->dir == c->dir && cp->f9 == c->f9) {
            c->x = D_0009B230[c->id].x / 1500;
            c->z = D_0009B230[c->id].z / 1500;
            switch (cp->dir) {
            case 0:
                if (c->z > cp->z)
                    dist = c->z - cp->z;
                break;
            case 2:
                if (cp->z > c->z)
                    dist = cp->z - c->z;
                break;
            case 1:
                if (c->x > cp->x)
                    dist = c->x - cp->x;
                break;
            case 3:
                if (cp->x > c->x)
                    dist = cp->x - c->x;
                break;
            }
            if (dist >= 0 && dist <= maxdist)
                return i;
        }
    }
    return -1;
}
