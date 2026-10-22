/* no Driver 2 counterpart */
typedef struct {
    char pad0[0xF8];
    int fF8;
    int pad1[2];
    int f104;
    int f108;
} S9B0A4;

typedef struct {
    short v;
    short pad;
} Tick;

typedef struct {
    char pad[0x2C];
    Tick t[11];
} Obj;

extern int D_00097708;
extern int D_00097704;
extern int D_00097730;
extern int D_00097750;
extern int D_00097734;
extern int D_00097754;
extern int D_00097728;
extern long long D_000974C0;
extern long long D_00096834;
extern int D_000C4C90[];
extern S9B0A4 car_data;
extern int D_0008BBC8[];
extern char D_000BC9BC[];
extern char D_00099120[];
extern int func_000635B0();
extern void func_0004C9EC();
extern void func_00019314();

void func_000651F0(Obj *obj)
{
    Tick *p;
    int v;
    int s;
    int buf[2];

    if (D_00097708 != 0 && D_00097708 < 0x293) {
        if (D_00097730 == D_00097750) {
            D_00097708 = D_00097708 - 1;
            D_00097730 = 0;
        } else {
            D_00097730++;
        }
    } else if (D_00097704 != 0 && D_00097708 >= 0x293) {
        if (*(int *)&D_000974C0 >= 0xF1 && car_data.f108 >= 0x15) {
            v = func_000635B0(car_data.fF8);
            if (car_data.f104 < 0)
                v = (v + 2) & 3;
            s = v & 0xFF;
            switch (*(int *)&D_00096834) {
            case 0:
                s += 1;
                break;
            case 1:
                s += 2;
                break;
            }
            if (s != D_00097728) {
                buf[0] = 0;
                buf[1] = D_0008BBC8[s & 3];
                func_0004C9EC(D_000BC9BC, buf, 2);
            }
            D_00097728 = s;
        }
        if (D_00097734 == D_00097754) {
            D_00097708++;
            if (D_00097708 > 0x1000)
                D_00097708 = 0x1000;
            D_00097734 = 0;
        } else {
            D_00097734++;
        }
        if (D_00097708 >= D_000C4C90[0]) {
            v = func_000635B0(car_data.fF8);
            if (car_data.f104 < 0)
                v = (v + 2) & 3;
            func_00019314(D_00099120, v & 0xFF);
        }
    }
    for (p = obj->t; p < &obj->t[11]; p++) {
        if (p->v)
            p->v--;
    }
}
