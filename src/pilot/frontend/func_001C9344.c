/* func_001C9344: menu input handler. One shared final return 0 (nested ifs, no early return 0s) - the -1 path tests\n   the call result in v0 directly; reorg copies the ret0 insn into the other branch slots. */
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
        if (!(b & 0x10)) {
            if (b & 0x8000) {
                func_001C9724();
            } else if (b & 0x2000) {
                func_001C96C8();
            } else if (b & 0x40) {
                if (func_001C9404())
                    return -1;
            }
        }
    } else {
        int a = 0;
        if (((short)D_00097538 & 0x8000) == 0) {
            if ((D_00097538 & 0x2000) == 0) return 0;
            a = 1;
        }
        func_001C9928(a);
    }
    return 0;
}
