struct S7EA4 { short a; unsigned short b; short c[8]; };
extern int D_000972BC;
extern int D_0009727C;
extern short D_0009728C[];
extern struct S7EA4 D_000A7EA4[];
extern void func_0003575C(void);
void func_0003490C(void) {
    int i;
    func_0003575C();
    i = D_000972BC;
    D_000972BC = i + 1;
    D_0009728C[D_000A7EA4[i].b] = -1;
    if (D_000972BC == D_0009727C) {
        D_0009727C = 0;
        D_000972BC = 0;
    }
}
