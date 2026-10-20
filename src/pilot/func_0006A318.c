typedef struct { int a, b, c, d; } F;
typedef struct { int f0, f4, f8, f0C; F *f10; short f14; short pad; int f18, f1C, f20, f24, f28; } E;
typedef struct {
    char p0[0xF8];
    unsigned char *fF8;
    char p1[0x104 - 0xFC];
    int f104;
    int f108;
    int f10C;
    char p2[0x130 - 0x110];
    E e[6];
    char p3[0x2DC - 0x238];
    short s2DC;
} CAR;
typedef struct { unsigned char b0, b1; char pad[0x4E]; } ENT;
typedef struct { char pad[0xC]; short fC, fE; char pad2[8]; int f18; } ST;

extern int func_00069D08(CAR *, E *);
extern void func_0006A178(CAR *, E *);
extern short func_0006B9D8(char *, int *, short);
extern long long D_000975DC;
extern int D_000C4C8C[];
extern E *D_000977AC;
extern F D_000BE9B4;
extern ENT D_00098900[];
extern int D_00096E78, D_00097790;
extern ST D_000BE774;
extern char D_000C6EA8[];
extern int D_0009B0A4[];

void func_0006A318(CAR *car)
{
    E *e;
    int d;
    int best = 0x7FFFFFFF;
    int second = 0x7FFFFFFF;
    E *bestp = 0;
    unsigned char *p;
    ENT *q;
    ST *st;
    int lim;
    int n;

    for (e = car->e; e < &car->e[6]; e++) {
        d = func_00069D08(car, e);
        if (d < best) {
            second = best;
            best = d;
            bestp = e;
        } else if (d < second) {
            second = d;
        }
    }
    if (second - best > 400 || (*(int *)&D_000975DC == 0 && best < 0x7FFFFFFF)) {
        *(int *)&D_000975DC = 1;
        D_000977AC = bestp;
        D_000BE9B4 = *bestp->f10;
    }
    switch (car->f104) {
    case 2:
        if (car->f108 == 0)
            break;
    case 5:
        func_0006A178(car, car->e);
        break;
    case 6:
        p = car->fF8;
        q = &D_00098900[*p];
        if (p == 0)
            break;
        if (q->b1 == 0) {
            E *e2;
            for (e2 = car->e; e2 < &car->e[6]; e2++) {
                if (e2->f10 == (F *)(p + 0xC8))
                    e2->f0C &= ~1;
            }
            car->f10C = 3;
            car->fF8 = 0;
            car->f104 = 0xFF0;
        } else {
            if (*(short *)(p + 0x17C) < D_00096E78)
                break;
            car->f104 = 0x7FFF;
            n = *p - 1;
            if (((unsigned char *)&D_00098900[n])[0x55] == 0 || D_000C4C8C[0] == 0)
                car->fF8 = 0;
        }
        break;
    case 0:
    case 1:
    case 3:
    case 7:
    case 0xFF0:
    case 0x7FFF:
        break;
    case 4:
        st = &D_000BE774;
        lim = D_00097790;
        st->f18 = 1;
        st->fE = lim;
        st->fC = car->s2DC = func_0006B9D8(D_000C6EA8, D_0009B0A4, car->s2DC);
        if (car->s2DC >= lim)
            car->f104 = 0x7FFF;
        break;
    }
}
