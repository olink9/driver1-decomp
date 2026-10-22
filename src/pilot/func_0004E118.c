extern void SpuSetKey(int, int);
extern void ClearChannelFields(int);
struct V { int a[33]; };
extern struct V D_000BD498[];
void func_0004E118(unsigned int n) {
    if (n < 16) {
        SpuSetKey(0, D_000BD498[n].a[0]);
        ClearChannelFields(n);
    }
}
