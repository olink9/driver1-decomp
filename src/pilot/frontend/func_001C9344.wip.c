/* WIP 96.46: 0x40 branch result: target tests v0 directly (beqz v0; slot -1; j end), ternary makes a v1 temp. */
extern unsigned short D_00097538;
extern int D_001CEF88;
extern int func_001C9404();
extern void func_001C96C8();
extern void func_001C9724();
extern void func_001C9928();

int func_001C9344(void)
{
    if (D_001CEF88 == 0) {
        unsigned short b = D_00097538;
        if (b & 0x10) return 0;
        if (b & 0x8000) {
            func_001C9724();
        } else if (b & 0x2000) {
            func_001C96C8();
        } else if (b & 0x40) {
            return func_001C9404() ? -1 : 0;
        }
        return 0;
    } else {
        int a = 0;
        if (((short)D_00097538 & 0x8000) == 0) {
            if ((D_00097538 & 0x2000) == 0) return 0;
            a = 1;
        }
        func_001C9928(a);
        return 0;
    }
}
