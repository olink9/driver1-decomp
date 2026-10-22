typedef struct { char pad[0x34]; unsigned char f; } V;
extern short D_0008D218[];
extern V *D_000C7888[];
extern V *D_00097810;
extern void SilenceXM();
extern void JPClearSPUFlags();

void func_00073738(int a) {
    if (D_0008D218[a] != -1) {
        D_00097810 = D_000C7888[a];
        if (D_00097810->f) {
            D_00097810->f = 0;
            SilenceXM();
        }
    }
    D_0008D218[a] = -1;
    JPClearSPUFlags(a + 1);
}
