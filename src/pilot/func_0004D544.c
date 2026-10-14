/* ResetSound */
struct Ch { int a; int pad[2]; short vl, vr; char rest[0x84 - 0x10]; };
extern int D_00096C80, D_00096C74, D_00096C78;
extern struct { int mask; short l, r; int pad[8]; } D_000BCA4C;
extern int D_000BCA0C[];
extern struct Ch D_000BD498[];
extern void SpuSetCommonAttr();
extern void func_000774C4();
extern void func_0004D4D4();

void func_0004D544(void) {
    int ct, x;
    D_00096C80 = 1;
    D_000BCA4C.l = 0x3FFF;
    D_000BCA4C.r = 0x3FFF;
    D_00096C74 = 0;
    D_00096C78 = 0;
    D_000BCA4C.mask = 3;
    SpuSetCommonAttr(&D_000BCA4C);
    for (ct = 0; ct < 16; ct++) {
        func_000774C4(&D_000BD498[ct], 0, 0x84);
        x = D_000BCA0C[ct];
        D_000BD498[ct].vl = 0;
        D_000BD498[ct].vr = 0;
        D_000BD498[ct].a = x;
        func_0004D4D4(ct);
    }
    D_00096C80 = 0;
}
