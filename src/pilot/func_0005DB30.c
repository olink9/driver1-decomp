typedef struct { char c[4]; } S4;
struct Q { char pad[0x3C]; int f; char pad2[0x58 - 0x40]; };
extern struct Q D_000BFA7C[];
extern S4 D_00096E44[];
extern void func_0002DBB4();
extern int func_000398B0();
extern int func_0002C5F0();
extern void DrawCiv();

void func_0005DB30(void) {
    S4 s;
    int i;
    struct Q *p = D_000BFA7C;
    s = D_00096E44[0];
    func_0002DBB4();
    for (i = 0; i < 20; i++, p++) {
        if ((p->f & 0x10) && func_000398B0(p) && func_0002C5F0(p, 0x12C) != -1)
            DrawCiv(p, i);
    }
}
