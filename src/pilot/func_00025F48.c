extern int D_00096884;
extern int D_00096888;
extern char D_00096898;
extern int D_00096858;
extern char D_000967BF;
extern long long ReplayParameterPtr;
extern int D_000BE9DC[];
extern long long D_00096780;
extern long long D_0009677C;
extern int D_00096834;
extern char D_00096895;
extern int CameraCnt;
extern void func_00016CE4();
extern void func_00026040();
extern void func_00058814();
extern void func_0001AF2C();
extern void func_0001AECC();
extern void func_000148C4();
extern void func_0001FA1C();
extern void func_1E1D30();
extern void func_1E1008();
extern void FindNextChange();
extern void func_00059698();

void func_00025F48(void)
{
    func_00016CE4();
    D_00096884 = 0;
    func_00026040();
    D_00096898 = 8;
    func_00058814();
    if (D_00096858 != 0)
        D_000967BF = 0;
    func_0001AF2C(0x17, (char *)*(int *)&ReplayParameterPtr + 4, *(short *)((char *)*(int *)&ReplayParameterPtr + 0x10), D_000967BF, 4);
    D_00096884 = 0x17;
    D_00096888 = 0;
    *(int *)&D_00096780 = 1;
    *(int *)&D_0009677C = 0;
    D_000BE9DC[0] = 1;
    func_0001AECC();
    func_000148C4();
    func_0001FA1C();
    func_1E1D30();
    if (D_00096834 == 2)
        func_1E1008();
    if (D_00096895)
        FindNextChange(CameraCnt);
    else
        func_00059698();
}
