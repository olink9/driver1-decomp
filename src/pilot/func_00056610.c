typedef struct { int a, b, c, d, e[4]; } ENT;
extern ENT D_0008ABAC[];
extern long long D_00096834;
extern long long D_00097550, D_00097552;
extern int D_000975CC;
extern int D_000BE9FC[][30][5];
extern void func_00074404(), func_000516AC();
extern unsigned func_0007A7F4();

void func_00056610(int *p)
{
    int i, n, a, b, c, d;

    D_000975CC = ((ENT *)((char *)D_0008ABAC + (*(int *)&D_00096834 << 5)))->d;
    n = *p++;
    for (i = 0; i < n;) {
        a = *p++;
        b = *p++;
        c = *p++;
        d = c - D_000975CC;
        func_00074404(&D_00097550, p);
        if (b >= 30)
            b = 30;
        p += 8;
        i++;
        D_000BE9FC[d][b][a] = (unsigned short)func_0007A7F4(*(short *)&D_00097550, *(short *)&D_00097552);
        func_000516AC(&D_00097550);
    }
}
