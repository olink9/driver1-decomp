typedef struct { char c[9]; } S9 __attribute__((aligned(4)));
extern S9 D_00010920;
extern char D_00096CC8[];
extern void func_0007E8EC();
extern int func_0007E8FC();
extern int CdReadSync();

int Loadfile(char *name, int mode) {
    S9 path = D_00010920;
    char pad[0x6c];
    int ev;
    int r;
    func_0007E8EC(&path, name);
    func_0007E8EC(&path, D_00096CC8);
    r = func_0007E8FC(&path, mode, 0);
    while (CdReadSync(0, &ev)) ;
    return r;
}
