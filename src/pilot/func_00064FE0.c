typedef struct { char pad[0xC8]; int f_c8; int pad2; int f_d0; char pad3[0x24]; int f_f8; char pad4[0xC]; int f_108; char pad5[0x18C - 0x10C]; } CARDEF;
typedef struct { int f_0; int f_4; int f_8; int f_c; char pad[0x18]; int f_28; char pad2[0x75C - 0x2C]; int f_75c; int pad3; int f_764; char pad4[0x794 - 0x768]; int f_794; char pad5[1960 - 0x798]; } CAR;
typedef struct { unsigned char a, b, c, d, e, f; } SLOT;
extern int D_00097708;
extern int D_0009770C;
extern long long D_000970D8;
extern CARDEF D_0009B230[];
extern CAR D_000C4690[];
extern void func_00064360();

void func_00064FE0(SLOT *p)
{
    int a, b[2];
    CAR *c;
    CARDEF *d;

    if (D_00097708 >= 0x293 && D_0009770C > 0)
        p->b = 4;
    if (p->b != 4) {
        c = &D_000C4690[p->f];
        d = &D_0009B230[p->a];
        c->f_75c = d->f_c8;
        c->f_764 = d->f_d0;
        c->f_28 = d->f_108;
        a = d->f_f8;
        c->f_794 = 0;
        c->f_c = a;
        if (*(int *)&D_000970D8 % 4 == p->f - 1)
            func_00064360(c, &b[0], &b[1], 1);
    }
}
