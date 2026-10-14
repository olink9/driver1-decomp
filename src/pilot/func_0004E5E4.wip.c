/* Driver 2 name unknown */
typedef struct { char pad[8]; short a; short b; char pad2[0x78]; } E_T;
extern int D_00096C7C;
extern E_T D_000BD498[];
extern int func_0004E67C();

void func_0004E5E4(int idx) {
    int t = func_0004E67C() + 10000;
    int v;
    int r;
    if (t > 10000) v = 10000;
    else {
        v = t;
        if (v < 0) v = 0;
    }
    r = v + (v >> 1) + (v >> 3) + (v >> 7);
    r = (r * D_00096C7C) >> 14;
    D_000BD498[idx].a = r;
    D_000BD498[idx].b = r;
}
