typedef struct { int f0, f4, f8, f0C, f10; short f14; short pad; int f18, f1C, f20, f24, f28; } E;
typedef struct { int a, b, c, d; } F;
typedef struct { char pad[0x130]; E e[6]; F f[6]; int cnt; } B;
int func_0006A60C(B *b, int a1, int a2, int a3) {
    E *e;
    F *f = b->f;
    for (e = b->e; e < (E *)b->f; e++, f++) {
        if (!(e->f0C & 1)) {
            f->a = a2;
            f->c = a3;
            e->f0 = 0x2EE;
            e->f4 = 0x5DC0;
            e->f8 = 0x1C2;
            e->f0C = 0;
            e->f10 = 0;
            e->f14 = 10;
            e->f18 = 0;
            e->f1C = 0x3E8;
            e->f20 = 0;
            e->f24 = 0;
            e->f28 = 0;
            e->f0 = a1;
            e->f10 = (int)f;
            e->f0C = 1;
            b->cnt++;
            return 1;
        }
    }
    return 0;
}
