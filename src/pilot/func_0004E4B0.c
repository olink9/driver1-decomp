typedef struct { int key; char a[0x3C]; unsigned char b40, b41; unsigned short h42; char pad[0x84 - 0x44]; } CH;
extern CH D_000BD498[];
extern void SpuSetKey(), func_0004D4D4();

static inline void ch_off(int i)
{
    if ((unsigned)i < 16) {
        SpuSetKey(0, D_000BD498[i].key);
        func_0004D4D4(i);
    }
}
int func_0004E4B0(void)
{
    int i, best, low;
    unsigned lo;
    low = 0;
    for (i = low; i < 16; i++) {
        if (D_000BD498[i].b41 == 0 && D_000BD498[i].h42 == 0) {
            ch_off(i);
            return i;
        }
    }
    best = -1;
    for (i = 0; i < 16; i++) {
        if (D_000BD498[i].b41 == 0 && D_000BD498[i].b40 == 0 &&
            (best == -1 || D_000BD498[i].h42 < low)) {
            best = i;
            low = D_000BD498[i].h42;
        }
    }
    if (best != -1)
        ch_off(best);
    return best;
}
