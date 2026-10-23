/* WIP 96.38: only the else arm of the (i == 0) store differs. Target computes 10*i (from the CSEd i<<2) first, then n+60 into a0 (i's dead reg), then adds; a separate lui per arm. Plain `i*10 + (n+60)` is reassociated by fold to (10i+60)+n (96.26); separating n+60 (temp, inline helper, store-then-+=) fixes the op order but shares/hoists the D_00096808 address pseudo into a0 and shifts i/n to a1/a2. Jump table checked: 0->0x14/+110, 1->0x15/+70, 2->0x12, 3->0x11, 4->none, 5->0x16/+155|+151. */
extern int D_001CE200, D_001CE3C4[], D_001CE3BC, D_001CDE64, D_001CDE5C;
extern int D_00096808, D_000967F4, D_000967D4;
extern void func_001C2114();
extern void SetDispMask();
extern void func_001C4BC8();

int func_001C56A0(void)
{
    int i;
    int n;
    func_001C2114(0, 0, 0);
    SetDispMask(0);
    i = D_001CE200;
    if (D_001CE3C4[i] != 0) {
        D_001CDE64 = 0;
        n = D_001CE3BC;
        if (i == 0) {
            D_00096808 = n + 110;
        } else {
            D_00096808 = n + 60;
            D_00096808 += i * 10;
        }
        switch (D_001CE200) {
        case 0:
            D_000967F4 = 0x14;
            D_00096808 = n + 110;
            break;
        case 1:
            D_000967F4 = 0x15;
            D_00096808 = n + 70;
            break;
        case 2:
            D_000967F4 = 0x12;
            break;
        case 3:
            D_000967F4 = 0x11;
            break;
        case 4:
            break;
        case 5:
            D_000967F4 = 0x16;
            if (D_000967D4)
                D_00096808 = n + 155;
            else
                D_00096808 = n + 151;
            break;
        }
        func_001C4BC8(n);
        D_001CDE5C = 6;
    }
    return 1;
}
