extern long long D_00096834;
extern long long D_00096978;
extern int D_000973AC;
extern void SetRotMatrix();
extern int func_00079AF0();
extern void func_0003A45C();

void func_0003B168(int a0, int a1, int *a2)
{
    register int t4 __asm__("$12") = a2[0];
    register int t5 __asm__("$13") = a2[1];
    register int t6 __asm__("$14") = a2[2];
    __asm__ volatile("ctc2 %0, $5; ctc2 %1, $6; ctc2 %2, $7" : : "r"(t4), "r"(t5), "r"(t6));
    SetRotMatrix(a1);
    if (*(int *)&D_00096834 != 3) {
        D_000973AC = func_00079AF0(0x1F800308);
    }
    func_0003A45C(a0, *(int *)&D_00096978);
    if (*(int *)&D_00096834 != 3) {
        func_00079AF0(D_000973AC);
    }
}
