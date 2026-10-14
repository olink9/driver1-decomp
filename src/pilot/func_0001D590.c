typedef struct { char pad[0xA]; short x; short z; short y; char pad2[0xD8]; short f_e8; unsigned short f_ea; short f_ec; char pad3[0xA]; unsigned short f_f8; } S;
extern int SquareRoot0();
extern int ratan2();

void func_0001D590(S *p) {
    int d = SquareRoot0(p->x * p->x + p->z * p->z);
    p->f_e8 = ratan2(-p->y, d);
    p->f_ea = p->f_f8;
    p->f_ec = ratan2(p->x, p->z);
}
