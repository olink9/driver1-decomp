extern char D_000BD498[];
extern void SpuSetKey(int, int);
extern void VSync(int);
extern void ClearChannelFields(int);
void func_0004E170(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if ((unsigned)i < 16) {
            SpuSetKey(0, *(int *)(D_000BD498 + i * 0x84));
            ClearChannelFields(i);
        }
        VSync(0);
    }
}
