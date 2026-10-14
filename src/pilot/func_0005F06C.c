struct O { char p0[0x18]; short a, b, c; char p1[0x1C]; int d; char p2[2]; short e; char p3[0xF]; unsigned char f; char p4[2]; unsigned char g; };
struct Q { char p[0xA]; unsigned char f; char q; };
extern int D_000976C0;
extern int D_000C0340[];
extern struct Q *D_000976CC;
void func_0005F06C(struct O *o) {
    D_000976C0 = D_000976C0 - 1;
    o->d = 0;
    o->c = 0;
    o->b = 0;
    o->a = 0;
    D_000C0340[D_000976C0] = o->e;
    if (o->g == 6) {
        struct Q *q = &D_000976CC[o->f];
        if (q->f) q->f = 0;
    }
}
