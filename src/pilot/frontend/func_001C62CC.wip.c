/* WIP 69%: all 8 stores + 2 shorts right; only scheduling/store order differs (target stores 28,48,88,a8,68,108,128,c8 with lw 794 after t0). 8 source orders tried. */
extern int D_00096A88, D_00096ABC, D_00096AC0;
extern int D_001CE534[];
extern int D_001CE784[];
extern int D_001CE794;
extern unsigned short D_001CE79C, D_001CE7A0;
extern short D_001CE4D0[];

void func_001C62CC(void)
{
    D_001CE534[10] = D_001CE784[1];
    D_001CE534[18] = D_001CE784[D_00096ABC];
    D_001CE534[26] = D_001CE784[D_00096AC0];
    D_001CE534[34] = D_001CE784[1];
    D_001CE534[42] = D_001CE794;
    D_001CE534[50] = D_001CE784[D_00096A88];
    D_001CE534[66] = D_001CE784[1];
    D_001CE534[74] = D_001CE784[1];
    D_001CE4D0[10] = D_001CE79C + 0x1C6;
    D_001CE4D0[30] = D_001CE7A0 + 0x1C6;
}
