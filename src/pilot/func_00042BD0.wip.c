typedef struct { int f0; int f4; int f8; } T;
typedef struct {
    T *p; unsigned char cnt; char pad[7];
    short s0C; unsigned short s0E; short s10; short pad2; int acc;
} E;
extern E D_000B7F3C[];
void func_00042BD0(void) {
    int i;
    E *e;
    for (i = 0; i < 8; i++) {
        e = &D_000B7F3C[i];
        if (D_000B7F3C[i].p != 0 && (unsigned)D_000B7F3C[i].p <= 0x1FFFFFF) {
            if (D_000B7F3C[i].p->f4 < 50) {
                e->acc += e->s0C;
                D_000B7F3C[i].p->f4 += e->s0E;
                D_000B7F3C[i].p->f8 += e->s10;
                e->cnt++;
                e->s0E += 10;
            } else D_000B7F3C[i].p = 0;
        }
    }
}
