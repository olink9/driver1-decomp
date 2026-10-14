extern long long D_000967BC;
#define D_000967BC_ (*(char *)&D_000967BC)
extern long long D_00096834;
#define D_00096834_ (*(int *)&D_00096834)
extern int D_00096C70;
extern long long D_00096C78;
#define D_00096C78_ (*(int *)&D_00096C78)
extern long long D_00096C88;
#define D_00096C88_ (*(int *)&D_00096C88)
extern long long D_00096C8C;
#define D_00096C8C_ (*(int *)&D_00096C8C)
extern long long D_00097098;
extern long long D_00097298;
extern long long D_000974CC;
extern int D_000974BC;
extern int D_000974C4;
extern char D_000106C0[];
extern void func_0004E214();
extern void func_0004E840();
extern void func_00050EC4();
extern void func_0006FB14();
extern int func_0006FDF0();
extern void func_00073408();
extern int func_00073804();
extern int func_00073880();
extern int func_00073888();
extern int func_000738CC();
extern void func_000738D4();
#define P (*(int *)&D_00097098)

void func_0004D208(int a0)
{
    int buf[3];
    int x, y;
    char *s0;
    int w;
    int v;
    int t;

    D_00096C70 = 0;
    s0 = D_000106C0;
    if (D_000967BC_) {
        if (D_00096834_ == 2) {
            x = 5;
            y = 0x4000;
        } else {
            x = 3;
            y = 0x2000;
        }
        func_0004E840(x, y);
        *(int *)&D_00097298 = a0;
        func_00050EC4(s0, buf, a0 << 3, 12);
        t = P;
        v = buf[1] - buf[0];
        w = buf[2] - buf[1];
        *(int *)&D_000974CC = t;
        D_000974C4 = t;
        P = t + ((v + 3) & -4);
        D_000974BC = P;
        func_00050EC4(s0, t, buf[0], v + w);
        if (D_00096C88_ == -1) {
            D_00096C8C_ = func_00073804();
            func_0004E214(D_000974BC, w, 2);
            w = func_000738CC();
            func_000738D4(P);
            P = P + w;
            w = func_00073880();
            func_00073888(P);
            P = P + w;
        }
    }
    func_0006FB14(D_000974C4, 0, 0);
    D_00096C88_ = func_0006FDF0(D_00096C8C_, 0, 0, 16, 1, -1, 0, 0);
    if (D_00096C78_)
        func_00073408(D_00096C88_);
}
