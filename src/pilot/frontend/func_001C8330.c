typedef struct { char pad[0x18]; char *str; } MENU_8330;
extern MENU_8330 D_001CEE00;
extern int D_001CEE30;
extern unsigned char D_001CEE37;
extern int D_001D30D8, D_001D30E8;
extern char D_001D30E0[];
extern char D_001C0914[];
extern int func_001C8640();
extern void func_000514E0(), func_001C1BD4(), func_001C84C0(), func_001C19C4(), func_001C11E4();
extern int sprintf();
extern char *strcpy();

int func_001C8330(char *dst)
{
    char buf[128];
    int blink = 0;
    int r;

    D_001D30D8 = 0x21;
    D_001D30E8 = 0;
    for (;;) {
        func_000514E0();
        r = func_001C8640();
        if (r != 0)
            break;
        sprintf(buf, D_001C0914, D_001D30E0);
        if (++blink > 32) {
            D_001CEE37 ^= 1;
            blink = 0;
        }
        if (D_001CEE30 < 6) {
            if (D_001CEE37 == 1) {
                D_001D30E0[D_001CEE30] = '-';
                D_001D30E0[D_001CEE30 + 1] = 0;
            } else {
                D_001D30E0[D_001CEE30] = 0;
                D_001D30E0[D_001CEE30 + 1] = 0;
            }
        }
        D_001CEE00.str = buf;
        func_001C1BD4(&D_001CEE00);
        func_001C84C0(D_001D30D8);
        blink++;
        func_001C19C4();
        func_001C11E4();
    }
    if (r == -1)
        return 0;
    D_001D30E0[D_001CEE30] = 0;
    strcpy(dst, D_001D30E0);
    return r;
}
