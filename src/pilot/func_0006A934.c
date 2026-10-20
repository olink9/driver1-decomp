/* cop/pursuit controller update (per-frame) */
typedef struct { int a, b, c, d; } F;
typedef struct { int a, b, c, d; } SUB;
typedef struct {
    int f0, f4, f8, f0C;
    F *f10;
    short f14;
    short pad;
    int f18;
    SUB sub;
} E;
typedef struct { int flags; int val; } T29C;
typedef struct {
    char p0[0x104];
    int f104;
    int f108;
    int f10C;
    int f110;
    int f114;
    int f118;
    int f11C;
    char p1[0x130 - 0x120];
    E e[6];
    F f[6];
    int f298;
    T29C f29C;
    char p2[0x2B4 - 0x2A4];
    int *f2B4;
    int *f2B8;
    char p3[0x2D0 - 0x2BC];
    int f2D0;
    char p4[0x2E1 - 0x2D4];
    unsigned char b2E1;
    unsigned char b2E2;
    unsigned char b2E3;
    unsigned char b2E4;
} CAR;

extern CAR D_000C6F04;
extern long long D_0009687C;
extern long long D_00096F80;
extern long long D_000976F8;
extern long long D_000975EC;
extern long long D_0009709C;
extern long long D_00096878;
extern short D_0009B220[];
extern int D_00096E7C;
extern void func_0006A6C8();
extern void func_00068C6C(CAR *);
extern void func_0006A318(CAR *);

static inline unsigned char AddTarget(CAR *car, int a1, int a2, int a3)
{
    E *e;
    F *f = car->f;
    SUB *s;

    for (e = car->e; e < (E *)car->f; e++, f++) {
        if (!(e->f0C & 1)) {
            f->a = a2;
            f->c = a3;
            e->f0 = 0x2EE;
            e->f4 = 0x5DC0;
            e->f8 = 0x1C2;
            e->f14 = 10;
            e->sub.a = 1000;
            e->f0C = 0;
            e->f10 = 0;
            e->f18 = 0;
            s = &e->sub;
            s->b = 0;
            s->c = 0;
            s->d = 0;
            e->f0 = a1;
            e->f10 = f;
            e->f0C = 1;
            car->f298++;
            return 1;
        }
    }
    return 0;
}

void func_0006A934(CAR *car)
{
    int *p;
    T29C *t;

    if (D_000C6F04.f2B8 == 0 || D_000C6F04.f2B4 == 0 || *(int *)&D_0009687C != 0)
        return;

    func_0006A6C8(car);

    if (car->b2E2)
        car->b2E2--;
    else
        car->b2E1 = 0;

    if (car->f2B8 < car->f2B4) {
        if (car->f104 == 0xFF0)
            goto end;
        switch (car->f2D0) {
        case 0:
            func_00068C6C(car);
            break;
        case 1:
            car->f2B8++;
            car->f2D0 = 2;
            break;
        case 2:
            p = car->f2B8;
            if (car->f298 < 6) {
                car->f104 = 2;
                { int *q; if (p[0] == 0x7FFFFFFF) q = p; else if (AddTarget(car, 0x2EE, p[0], p[1])) q = p + 2; else q = p; p = q; }
            }
            if (car->f298 == 0) {
                car->f2D0 = 0;
                p++;
            }
            car->f2B8 = p;
            break;
        }
    }

    if (car->f104 == 0xFF0)
        goto end;

    if (*(int *)&D_00096F80 == 5 && ((t = &car->f29C)->flags & 8)) {
        if (t->flags & 0x80) {
            t->val += 0x222;
            if (t->val > 0x3840000)
                t->val = 0x3840000;
        } else if (t->val == 0) {
            D_000C6F04.f104 = 0xFF0;
            D_000C6F04.f10C = 1;
            car->f29C.flags &= ~8;
        } else {
            t->val -= 0x222;
            if (t->val < 0)
                t->val = 0;
        }
    }

    func_0006A318(car);

    if (car->f104 & 0x8000) {
        *(int *)&D_000976F8 = 0;
        car->f104 = 0x7FFF;
    }
    if (car->f114)
        car->f114--;
    if ((car->f11C & 4) && D_0009B220[0] >= D_00096E7C) {
        car->f104 = 0xFF0;
        car->f10C = 2;
    }
    if (car->f104 != 0xFF0)
        return;
end:
    *(int *)&D_000976F8 = 0;
    *(int *)&D_000975EC = 0;
    if (car->f110 == 0) {
        car->f110 = 0x3C;
    } else if (--car->f110 == 0) {
        *(int *)&D_0009709C = 1;
        car->b2E4 = 0;
    }
    *(int *)&D_00096878 = 1;
}
