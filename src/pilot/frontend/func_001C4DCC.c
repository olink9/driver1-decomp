extern int D_001CDE5C;
extern int D_001CDE64;
extern int D_001CE200;
extern char D_001CE1A0[];
extern char D_001CE120[];
extern void func_001C2114();
extern void func_001C2644();
extern void func_001C5268();

int func_001C4DCC(void)
{
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 6;
    func_001C5268();
    D_001CDE5C = 6;
    D_001CE200 = 0;
    func_001C2644(D_001CE1A0, 6, 0);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}
