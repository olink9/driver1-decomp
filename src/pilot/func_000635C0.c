/* cop sight / pursuit check (Driver 2 name unknown) */
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct {
    unsigned char id;
    char a[0x17];
    int px, py, pz;
    char b[0xC8 - 0x24];
    int x, y, z;
    char c[0x18C - 0xD4];
} CAR;
typedef struct {
    char p0[0xC];
    int ang;        /* 0x0C */
    int dist;       /* 0x10 */
    char p1[0x1C - 0x14];
    int f1C;        /* 0x1C */
    char p2[0x44 - 0x20];
    int timer;      /* 0x44 */
    char p3[0x75C - 0x48];
    int x;          /* 0x75C */
    int y;
    int z;          /* 0x764 */
    char p4[0x78C - 0x768];
    int active;     /* 0x78C */
    char p5[0x798 - 0x790];
    int f798;       /* 0x798 */
    char p6[0x7A0 - 0x79C];
    int f7A0;       /* 0x7A0 */
    CAR *car;       /* 0x7A4 */
} COP;
typedef struct {
    unsigned char b0;
    unsigned char f1;
    unsigned char f2;
    char pad[80 - 3];
} SLOT;
typedef struct { char p[0x28]; unsigned char f28; } PLY;

extern CAR car_data[];
extern COP D_000C4E38[];
extern char D_000C4CA8[];
extern SLOT D_00098950[];
extern SLOT D_00098900[];
extern PLY D_00099120;
extern char D_000BC9BC[];
extern long long D_00096FBC;
extern int D_000C6FFC[];
extern int D_00097704, D_00097708, D_0009770C, D_00097744;
typedef struct { short a, b, c; } LIM;
extern short D_00097758;
extern LIM Lm __asm__("D_00097758");
extern long long D_0009775A, D_0009775C;
extern void func_0003A100();
extern int func_0001B198();
extern int func_000397F4();
extern int ratan2();
extern int SquareRoot0();
extern void PlaySpeech();

#define ABS(x) ((x) >= 0 ? (x) : -(x))

void func_000635C0(void)
{
    VEC pos;
    int a;
    int b;
    char *tgt;
    int found;
    int i;
    int dx, dz;
    unsigned char flag;
    COP *cop;
    COP *cp2;
    LIM *lim;
    CAR *car;
    short s;

    tgt = D_000C4CA8;
    pos.vx = car_data[0].x;
    pos.vz = car_data[0].z;
    cop = D_000C4E38;
    func_0003A100(&pos, tgt, &a, &b);
    found = 0;
    if (*(unsigned char *)&D_00096FBC)
        D_000C6FFC[0] = func_0001B198(tgt, a, b);
    lim = &Lm;
    if (D_00097708 > 0x292) {
        D_00097758 = 8000;
        lim->b = 25000;
        lim->c = 1024;
    }
    if (D_0009770C > 0)
        D_0009770C = 2;
    else
        D_0009770C = 1;
    D_00097704 = 0;
    if (D_00099120.f28 == 2 && func_000397F4(&D_00099120, tgt, a, b)) {
        found = 1;
        D_00097704 = found;
    }
    if (!found) {
        for (i = 0; i < 4; i++, cop++) {
            if (cop->active == 0 || cop->f798 != 0)
                continue;
            D_00098950[cop->car->id - 1].f2 = 0;
            dx = cop->x - car_data[0].x >= 0 ? cop->x - car_data[0].x : car_data[0].x - cop->x;
            dz = cop->z - car_data[0].z >= 0 ? cop->z - car_data[0].z : car_data[0].z - cop->z;
            if (dz < dx)
                cop->dist = dx + dz / 2;
            else
                cop->dist = dz + dx / 2;
            pos.vx = cop->x;
            pos.vz = cop->z;
            if (cop->f7A0 == 0) {
                if (!func_000397F4(&pos, tgt, a, b))
                    continue;
                flag = 0;
                if (cop->dist < D_00097758) {
                    flag = 1;
                } else {
                    VEC d;
                    CAR *c = cop->car;
                    d.vx = car_data[0].px - c->px;
                    d.vz = car_data[0].pz - c->pz;
                    s = *(short *)&D_0009775C;
                    if (ABS(ratan2(d.vx, d.vz) - cop->ang) < s
                        && SquareRoot0(d.vx * d.vx + d.vz * d.vz) < *(short *)&D_0009775A)
                        flag = 1;
                }
                if (!flag)
                    continue;
            }
            found = 1;
            D_00097704 = found;
            break;
        }
    }
    if (found == 1) {
        D_00097744 = 0;
    } else if (D_00097744 >= 256) {
        if (D_00097744 == 256) {
            D_0009770C = -1;
            D_00097744 = 257;
            if (D_00097708 > 0x292)
                PlaySpeech(D_000BC9BC, 8);
        } else {
            D_0009770C = 0;
        }
    } else {
        D_00097744++;
    }
    if (D_0009770C <= 0) {
        for (cp2 = D_000C4E38; cp2 < &D_000C4E38[4]; cp2++) {
            if (cp2->active == 0)
                continue;
            car = cp2->car;
            pos.vx = car->x;
            pos.vz = car->z;
            if (cp2->f1C >= 5 || cp2->f798 != 0)
                continue;
            if (func_000397F4(&pos, tgt, a, b))
                continue;
            if (++cp2->timer > 50) {
                cp2->timer = 0;
                if (D_00098900[car->id].f1 == 4)
                    D_00098900[car->id].f1 = 2;
            }
        }
    }
}
