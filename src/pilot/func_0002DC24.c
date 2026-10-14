struct M { short m[3][3]; short pad; int t[3]; };
extern struct M D_000A6E8C[];
extern struct M D_000A668C[];
extern void RotMatrix();
extern void func_0004EB10();
void func_0002DC24(void) {
    short rot[4];
    char pad[0x20];
    int i = 0;
    struct M *a = D_000A668C;
    struct M *b = D_000A6E8C;
    rot[2] = 0; rot[1] = 0; rot[0] = 0;
    for (; i < 64; i++) {
        RotMatrix(rot, b);
        func_0004EB10(b, a);
        a++; b++;
        rot[1] += 0x40;
    }
}
