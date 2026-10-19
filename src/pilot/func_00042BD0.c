typedef struct { int f0; int f4; int f8; } T;
typedef struct {
    T *p; unsigned char cnt; char pad[7];
    short s0C; short s0E; short s10; short pad2; int acc;
} E;
extern E D_000B7F3C[];
void func_00042BD0(void) {
    int i;
    T *t;
    E *e = D_000B7F3C;
    for (i = 0; i < 8; i++, e++) {
        t = e->p;
        if (t != 0 && (unsigned)t <= 0x1FFFFFF) {
            if (t->f4 < 50) {
                e->acc += e->s0C;
                t->f4 += e->s0E;
                t->f8 += e->s10;
                e->cnt++;
                e->s0E += 10;
            } else e->p = 0;
        }
    }
}
