/* spawn ground debris/dust particles around a car */
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { long vx, vy, vz, pad; } VECTOR;

typedef struct {
    char pad0[0xC8];
    long t[3];
    char pad1[0x18C - 0xD4];
} CAR;

typedef struct {
    VECTOR pos;
    char pad0[0x38 - 0x10];
    short s38, s3A, s3C, s3E;
    int i40;
    unsigned char b44;
    char pad1[3];
    int flags;
    int pad2;
} PARTICLE;

typedef struct { short type; short pad[3]; } SURF;

extern long long pauseflag;
extern CAR car_data[];
extern unsigned short *modelpointers[];
extern MATRIX D_000AF6A0[];
extern PARTICLE D_000B92A0[];
extern int func_000776E4();
extern void SetRotMatrix();
extern void func_0004EAC0();
extern int func_00042678();
extern int ROADS_GetRouteData();

void func_000412D4(int car, short height, int type)
{
    VECTOR pos;
    SURF surf;
    int count;
    int idx;
    PARTICLE *p;

    if (*(int *)&pauseflag)
        return;

    count = 15;
    if (type)
        count = 20;

    while (count-- != 0) {
        if (type) {
            pos.vx = (func_000776E4() & 0x1FF) - 256;
            pos.vz = func_000776E4() % 1024 - 512;
        } else {
            pos.vx = (func_000776E4() & 0x1FF) - 256;
            pos.vz = (func_000776E4() & 0x400) - 512;
        }
        SetRotMatrix(&D_000AF6A0[car]);
        func_0004EAC0(&pos);
        pos.vx += car_data[car].t[0];
        pos.vy = -height;
        pos.vz += car_data[car].t[2];

        idx = func_00042678();
        if (idx < 0)
            return;

        if (type) {
            if (ROADS_GetRouteData(pos.vx, pos.vz, &surf) && (*modelpointers[surf.type] & 0x80)) {
                p = &D_000B92A0[idx];
                p->pos = pos;
                p->flags = 0x8006;
                p->s3C = 200;
                p->s3E = (func_000776E4() & 7) + 5;
                p->s38 = 300;
                p->s3A = 700;
                p->i40 = 10;
                p->b44 = 100;
            }
        } else {
            p = &D_000B92A0[idx];
            p->pos = pos;
            p->flags = 6;
            p->s3C = 200;
            p->s3E = (func_000776E4() & 15) + 10;
            p->s38 = 100;
            p->i40 = 10;
            p->s3A = 200;
            p->b44 = 50;
        }
    }
}
