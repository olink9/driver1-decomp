typedef struct {
    int f0;
    char p0[0x48 - 4];
    int t48, t4c;
    char p1[0x75C - 0x50];
    int px;
    int pad760;
    int pz;
    char p2[0x78C - 0x768];
    int f78c;
    char p3[0x79C - 0x790];
    int f79c;
    char p4[0x7A8 - 0x7A0];
} CAR;

extern int D_000976FC;
extern long long D_0009703C;
#define tab (*(int **)&D_0009703C)
extern int D_0009B0A4[];
extern CAR D_000C4E38[];

static inline int dist(CAR *car, int a, int c)
{
    int b, d;
    int dx, dz;
    b = car->px;
    dx = a - b;
    if (dx < 0)
        dx = b - a;
    d = car->pz;
    dz = c - d;
    if (dz < 0)
        dz = d - c;
    if (dz < dx)
        return dx + dz / 2;
    return dz + dx / 2;
}

int func_000641C0(CAR *car)
{
    int a, b, i, c2, c1;
    int x2, z2;
    CAR *p;

    if (D_000976FC != -1) {
        a = tab[D_000976FC * 2];
        b = -tab[D_000976FC * 2 + 1];
        x2 = D_0009B0A4[50];
        z2 = D_0009B0A4[52];
        if (dist(car, a, b) <= 0x9E33 &&
            dist(car, x2, z2) >= 0x6979) {
            p = D_000C4E38;
            c1 = 0;
            c2 = 0;
            for (i = 0; i < 4; i++, p++) {
                if (p->f78c != 0) {
                    c1++;
                    if (p->f0 != car->f0 && p->f79c != 0)
                        c2++;
                }
            }
            if (c2 < (c1 >> 1)) {
                car->t48 = a;
                car->t4c = b;
                car->f79c = 1;
                return 1;
            }
        }
    }
    car->f79c = 0;
    return 0;
}
