typedef struct { char p0; unsigned char f1; char p2, p3, p4; unsigned char f5; char p6, p7; unsigned char f8; char pad[0x47]; } S50;
typedef struct { char pad[1960]; } S1960;

extern S50 D_00098950[];
extern S1960 D_000C4690[];
extern int D_0009677C;
extern volatile long long D_000970D8;
extern int D_00096FD0;
extern int D_00096FF0;
extern unsigned char D_00096FC4;
extern unsigned char D_00099AF0[];
extern char D_00099120[];
extern void func_00063AB4();
extern void func_0001A008();

void func_00017C50(int a)
{
    S50 *p = &D_00098950[a];
    unsigned char x = p->f5;

    if (p->f1 != 0 && D_0009677C != 0) {
        if (*(volatile int *)&D_000970D8 != D_00096FD0 || a != D_00096FF0) {
            D_00096FF0 = a;
            D_00096FD0 = *(volatile int *)&D_000970D8;
        }
    }
    if (x != 0) {
        func_00063AB4(&D_000C4690[p->f5]);
    }
    if (p->f1 == 9) {
        func_0001A008(D_00099120);
    }
    p->f1 = 0;
    p->f8 = 0;
    D_00099AF0[--D_00096FC4] = a;
}
