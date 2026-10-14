struct T { char p[3]; char v; };
struct U { char p[3]; char v; char q[0xFC]; };
extern struct T D_000C039C[];
extern char D_000C429B[];
extern void func_00061C58();
extern void func_00062DB0();
void func_00061EDC(int a, int b, int c) {
    int i;
    struct T *p;
    func_00061C58();
    p = D_000C039C;
    for (i = 0; i < 64; i++) {
        p[i].v = 30;
        *((char *)p + 0x3F00 + i * 4 + 3) = 30;
        ((struct U *)p)[i].v = 30;
    }
    D_000C429B[0] = 30;
    func_00062DB0(c);
}
