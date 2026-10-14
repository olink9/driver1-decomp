typedef struct { unsigned char a, b, c, d, e, f; char pad[74]; } T;
extern T D_00098950[];
extern void func_0004AD94();

void func_00014F28(void) {
    int i;
    T *p = D_00098950;
    for (i = 0; i < 22; i++, p++) {
        if (p->b != 0) {
            if (p->c == 1 || p->f != 0)
                func_0004AD94(p->a + 1);
        }
    }
}
