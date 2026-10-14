extern void func_0004E170();
extern void func_0004E348();
extern void SpuQuit();
extern void func_00050E28();
extern void func_0007EC78();
extern void func_0007EBE8();
extern void func_0007EB34();
extern void func_0007EB24();
extern char D_00011630[];

void func_0006D610(int a0) {
    func_0004E170();
    func_0004E348();
    SpuQuit();
    func_00050E28(D_00011630, 0x800FF800);
    func_0007EC78();
    func_0007EBE8();
    func_0007EB34();
    func_0007EB24(0x800FF810, 1, a0);
}
