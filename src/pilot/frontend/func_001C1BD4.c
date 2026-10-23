typedef struct { short a, b; char c, d; short e; int f; } E_001C1BD4;
extern void func_000E7350();
extern void func_000E6F14();
extern void func_000E6F98();

void func_001C1BD4(E_001C1BD4 *p)
{
    int *q;
    func_000E7350();
    q = &p->f;
    if (*q != 0) {
        do {
            func_000E6F14(p->a, p->b, p->c, p->d, (char *)p + 0xC);
            func_000E6F98(*q);
            q += 4;
            p++;
        } while (*q != 0);
    }
}
