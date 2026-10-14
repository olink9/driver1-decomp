struct O { int a; short w4, h6, w8, h10, c12, d14; int e16; int f20; };
extern void func_00012498();
void func_00056B5C(struct O *o, int a, int b, int c) {
    func_00012498(o, 0x1C);
    o->w4 = 0x96; o->h6 = 10; o->w8 = 0xA4; o->h10 = 10; o->c12 = 0; o->d14 = a;
    o->e16 = b; o->f20 = 0; o->a = c;
}
