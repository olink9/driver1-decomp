extern int D_00096C88;
extern void func_0006FA5C();
extern void SpuSetReverb();
extern void func_0007C7D4();

void func_0004E398(int reverb) {
    int ct;
    func_0006FA5C(D_00096C88, -1);
    SpuSetReverb(1);
    for (ct = 16; ct < 24; ct++) {
        if (reverb) func_0007C7D4(1, 1 << ct); else func_0007C7D4(0, 1 << ct);
    }
}
