typedef struct {
    char pad[0xF0];
    int a;
    int b;
    char pad2[0x104 - 0xF8];
    int c;
} S;
extern void func_00043E8C();
extern void func_00043D0C();
extern void func_0004388C();
extern void func_000439E4();

void func_0003B994(S *s, int x) {
    int c = s->c;
    func_00043E8C(0);
    if ((c > 0 && s->a < 0) || (c < 0 && s->a > 0))
        func_00043D0C(x);
    if (c < 0 && s->a < 0)
        func_0004388C(x);
    if ((unsigned)(c + 0xC7) <= 0x4B0C6) {
        int b = s->b;
        if (b > 200) func_000439E4(x, 1);
        else if (b < -200) func_000439E4(x, 0);
    }
}
