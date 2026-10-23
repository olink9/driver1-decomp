struct E { unsigned short a; unsigned char b; unsigned char c; };
extern struct E *D_00096DC8;
extern int D_00096DCC;
extern long long CameraCnt;
void func_000589B4(int a, int b) {
    struct E *e = D_00096DC8 + D_00096DCC;
    if (D_00096DC8 != 0 && (unsigned)D_00096DCC < 0x190) {
        unsigned short s = *(unsigned short *)&CameraCnt;
        int n;
        e->c = a;
        e->b = b;
        n = D_00096DCC + 1;
        e->a = s;
        D_00096DCC = n;
    }
}
