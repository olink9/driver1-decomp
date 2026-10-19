/* Driver 2 name unknown. Remaining diff: target tests the MAX temp copy (v0=v1; bgez v0) instead of v. */
typedef struct { char pad[8]; short a; short b; char pad2[0x78]; } E_T;
extern int D_00096C7C;
extern E_T D_000BD498[];
extern int func_0004E67C();
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void func_0004E5E4(int idx) {
int v, r; v = func_0004E67C() + 10000; v = MIN(10000,MAX(0,v));
    r = v + (v >> 1); r += v >> 3; r += v >> 7;
    r = (r * D_00096C7C) >> 14;
    D_000BD498[idx].a = r;
    D_000BD498[idx].b = r;
}
