extern int D_00096A88, D_00096ABC, D_00096AC0;
extern int D_001CE534[];
extern int D_001CE784[];
extern int D_001CE794;
extern unsigned short D_001CE79C, D_001CE7A0;
extern short D_001CE4D0[];

void func_001C62CC(void)
{
    int v = D_001CE784[1];
    D_001CE534[10] = v;
    D_001CE534[18] = D_001CE784[D_00096ABC];
    D_001CE534[26] = D_001CE784[D_00096AC0];
    D_001CE534[34] = v;
    D_001CE534[42] = D_001CE794;
    D_001CE534[50] = D_001CE784[D_00096A88];
    D_001CE534[66] = v;
    D_001CE534[74] = v;
    D_001CE4D0[10] = D_001CE79C + 0x1C6;
    D_001CE4D0[30] = D_001CE7A0 + 0x1C6;
}
