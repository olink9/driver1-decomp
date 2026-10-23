typedef struct { short a, b; char c, d; short e; int f; int g; } E_001C1BD4;
extern void func_000E7350();
extern void func_000E6F14();
extern void func_000E6F98();

void func_001C1BD4(E_001C1BD4 *p)
{
    E_001C1BD4 *q = p;
    func_000E7350();
    while (q->f != 0) {
        func_000E6F14(q->a, q->b, q->c, q->d, &q->g);
        func_000E6F98(q->f);
        q++;
    }
}
