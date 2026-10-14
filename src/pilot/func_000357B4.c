extern char D_000A7FD0[];
extern int D_000972EC;
extern char D_000972E0;
extern void PadInitDirect(char *a, char *b);
extern void func_0007AEB4(void);
void func_000357B4(void) {
    PadInitDirect(D_000A7FD0, D_000A7FD0 + 0x22);
    func_0007AEB4();
    D_000972EC = 0;
    D_000972E0 = 0x63;
}
