extern int D_000975F8;
extern long long D_00097098;
extern long long D_000975A0;
extern long long D_000975A2;
extern short D_000975FC;
extern void func_00079DB0();
extern void func_00074404();
extern short func_0007A7F4();
void func_00054140(int a, int n) {
    int v = *(int *)&D_00097098;
    D_000975F8 = v;
    func_00079DB0(v, a, n);
    *(int *)&D_00097098 = *(int *)&D_00097098 + n;
    func_00074404(&D_000975A0, D_000975F8 + 0x148);
    D_000975FC = func_0007A7F4(*(short *)&D_000975A0, *(short *)&D_000975A2);
}
