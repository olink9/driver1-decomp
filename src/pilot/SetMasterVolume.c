/* clamp (x+10000) to [0,10000] then scale by ~1.63 */
extern int D_00096C94;
extern int D_00096C7C;

void SetMasterVolume(int a) {
    int x;
    char pad[16];
    D_00096C94 = a;
    x = a + 10000;
    if (x < 0) goto lo;
    if (x > 10000) {
        a = 10000;
    } else {
lo:
        a = x;
        if (a < 0) a = 0;
    }
    D_00096C7C = a + (a >> 1) + (a >> 3) + (a >> 7);
}
