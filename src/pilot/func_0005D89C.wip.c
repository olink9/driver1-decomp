extern long long D_00097098;
extern long long D_00097534;
extern int D_000976AC;
extern int D_000C015C[];
extern char D_0008B704[];
extern void func_00079DB0();

void func_0005D89C(int a, int n)
{
    char *p;
    func_00079DB0(*(int *)&D_00097098, a, n);
    D_000C015C[D_000976AC] = *(int *)&D_00097098;
    p = (char *)*(int *)&D_00097098;
    p[1] = D_0008B704[D_000976AC];
    D_000976AC++;
    if (D_000976AC == 8) {
        int t = *(int *)&D_00097534;
        if (t != 0x78654321) {
            do {
                __asm__ volatile("break 1024");
            } while (*(int *)&D_00097534 != 0x78654321);
        }
    }
    *(int *)&D_00097098 = *(int *)&D_00097098 + n;
}
