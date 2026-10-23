extern int D_001CEF84;
extern void func_001CA270();
extern void func_001C9A18();

void func_001C96C8(void)
{
    func_001CA270(0, 0, 0, 0x40);
    D_001CEF84++;
    if (D_001CEF84 >= 5)
        D_001CEF84 = 0;
    func_001C9A18(D_001CEF84);
}
