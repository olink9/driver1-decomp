typedef struct { char pad[0x34]; unsigned char f; } V;
extern short D_0008D218[];
extern V *D_000C7888[];
extern V *D_00097810;
extern void func_00073324();
extern void func_000737CC();

void func_00073738(int a) {
    if (D_0008D218[a] != -1) {
        D_00097810 = D_000C7888[a];
        if (D_00097810->f) {
            D_00097810->f = 0;
            func_00073324();
        }
    }
    D_0008D218[a] = -1;
    func_000737CC(a + 1);
}
