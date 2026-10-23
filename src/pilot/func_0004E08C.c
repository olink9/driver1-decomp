extern int D_00096C74;
extern int D_00096C78;
extern int D_00096C88;
extern int D_000BD498[];
extern void SpuSetMute();
extern void XM_Restart();
extern void SpuSetVoiceAttr();

void func_0004E08C(void) {
    int i;
    int *p;
    int k;
    if (D_00096C78 != 0) {
        SpuSetMute(0);
        XM_Restart(D_00096C88);
        D_00096C78 = 0;
    }
    if (D_00096C74 != 0) {
        i = 0;
        k = 15;
        for (p = D_000BD498; i < 16; i++, p += 33) {
            p[1] = k;
            SpuSetVoiceAttr(p);
        }
        D_00096C74 = 0;
    }
}
