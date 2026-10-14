extern int D_00097344;
extern long long D_000972C0;
extern int D_0009732C;
extern long long D_00097560;
extern unsigned char D_000BE4F8[];
extern unsigned char D_000BE278[];
extern int D_000A8024[];
extern void func_00039D94();

void func_0003A068(void) {
    int i;
    char pad[8];
    unsigned char v;
    D_00097344 = -1;
    *(int *)&D_000972C0 = -1;
    D_0009732C = -1;
    for (i = *(int *)&D_00097560; i < 15; i++) {
        v = D_000BE4F8[i];
        if (v != 0xff) D_000BE278[v] = 0;
        D_000BE4F8[i] = 0xff;
    }
    D_000A8024[3] = -1;
    D_000A8024[2] = -1;
    D_000A8024[1] = -1;
    D_000A8024[0] = -1;
    func_00039D94();
}
