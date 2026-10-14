extern char D_000BD498[];
extern void SpuSetKey(int, int);
extern void VSync(int);
extern void func_0004D4D4(int);
void func_0004E170(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if ((unsigned)i < 16) {
            SpuSetKey(0, *(int *)(D_000BD498 + i * 0x84));
            func_0004D4D4(i);
        }
        VSync(0);
    }
}
