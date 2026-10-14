typedef struct { unsigned char p0, f1, f2, p3, p4, f5, p6, f7, f8; char pad[71]; } E80;
extern E80 D_00098900[];
extern unsigned char D_00096898[];
extern char D_000C71E7[];
extern char D_000988C8[];
extern void func_0004C8F8();
extern void func_000650E4();

void func_00013B80(int a, int b, int c)
{
    E80 *p = &D_00098900[a];
    E80 *q = &D_00098900[b];

    if (D_00096898[0] == 3 && ((a == 0 && b == 23) || (b == 0 && a == 23))) {
        D_000C71E7[0] = 15;
        return;
    }
    if (b == 0 || a == 0) {
        if (p->f7) func_0004C8F8();
        p->f2 = 1;
        p->f8 = 0;
        if (p->f5) {
            if (p->f1 == 4) return;
            if (c >= 16) func_000650E4(D_000988C8, 5, 0x1000);
            else func_000650E4(D_000988C8, 5, (c << 24) >> 16);
        } else {
            if (c >= 32) func_000650E4(D_000988C8, 4, 0x1000);
            else func_000650E4(D_000988C8, 4, (c << 23) >> 16);
        }
    } else {
        if (p->f2 == 1 || q->f2 == 1 || p->f5 || q->f5) {
            if (p->f7) func_0004C8F8();
            if (q->f7) func_0004C8F8(b);
            p->f2 = 1; p->f8 = 0;
            q->f2 = 1; q->f8 = 0;
        }
        if (p->f1 == 1) { p->f2 = 1; p->f8 = 0; }
        if (q->f1 == 1) { q->f2 = 1; q->f8 = 0; }
    }
}
