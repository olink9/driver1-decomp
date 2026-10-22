typedef struct { char pad[0x18C]; } CAR;
typedef struct { unsigned char c0, c1, p[3], c5; char pad[0x4A]; } ENT;
extern CAR car_data[];
extern ENT D_00098950[];
extern char D_00099120[];
extern long long D_00096884;
extern unsigned char func_000192D0();
extern int func_000612FC();
extern void func_00017C50();

int func_00017AA0(unsigned char type)
{
    int best = -1;
    int bestidx = -1;
    CAR *base;
    CAR *car;
    CAR *cur;
    ENT *e;
    int i;
    unsigned char ok;
    int r;

    base = car_data;
    car = base + 1;
    cur = base + *(int *)&D_00096884;
    e = D_00098950;
    i = 0;
    for (; car < &car_data[23]; car++, e++, i++) {
        ok = 0;
        switch (type) {
        case 0:
            ok = 1;
            break;
        case 1:
            if (e->c5 != 0)
                ok = 1;
            break;
        case 2:
            if (e->c5 == 0)
                ok = 1;
            break;
        }
        if (ok) {
            if (e->c1 == 0) {
                if (func_000192D0(D_00099120, e->c0) == 0)
                    return 1;
            } else {
                r = func_000612FC((char *)car + 0xC8, (char *)cur + 0xC8);
                if (best < r) {
                    best = r;
                    bestidx = i;
                }
            }
        }
    }
    if (bestidx == -1)
        return 0;
    func_00017C50(bestidx);
    return 1;
}
