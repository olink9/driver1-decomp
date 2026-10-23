struct M { short m[3][3]; short pad; int t[3]; };
extern struct M matrixtable[];
extern struct M D_000A668C[];
extern void RotMatrix();
extern void InvertMatrix();
void func_0002DC24(void) {
    short rot[4];
    char pad[0x20];
    int i = 0;
    struct M *a = D_000A668C;
    struct M *b = matrixtable;
    rot[2] = 0; rot[1] = 0; rot[0] = 0;
    for (; i < 64; i++) {
        RotMatrix(rot, b);
        InvertMatrix(b, a);
        a++; b++;
        rot[1] += 0x40;
    }
}
