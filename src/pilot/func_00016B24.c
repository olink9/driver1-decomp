typedef struct { short s0; unsigned char c2; char pad3; int i4; int i8; } ENT;
typedef struct { char pad0; char c1; char pad2[6]; char c8; char pad9[0x47]; } SLOT;
extern int D_00096FD4, D_00096780, D_0009677C;
extern long long D_00096834;
extern char D_00099120[];
extern SLOT D_00098950[];
extern ENT D_00099180[];
extern void func_0001943C();
extern int func_00017A64();
extern int func_0004FFE8();
extern int func_00058A00();
extern int func_000154A8();
extern void func_000589B4();
/* Driver 2 name: unknown */
void func_00016B24(void)
{
    int local;
    unsigned char idx;
    ENT *e;
    int a0;

    if (D_00096FD4 == 0)
        return;
    if (D_00096780 == 0) {
        func_0001943C(D_00099120);
        local = func_00017A64();
        if (local == -1)
            return;
    }
    idx = func_0004FFE8(local) % D_00096FD4;
    if (D_00096780 != 0) {
        idx = func_00058A00(idx, &local);
        if (idx == 0xFF)
            return;
        if (idx == 0xFE) {
            SLOT *sl = &D_00098950[local];
            sl->c1 = 0;
            sl->c8 = 0;
            return;
        }
    }
    e = &D_00099180[idx];
    a0 = 0;
    if (*(int *)&D_00096834 != 2 && *(int *)&D_00096834 != 6) {
        if (e->s0 != -1)
            a0 = func_000154A8(local, e->s0, func_0004FFE8(local) & 1, e->i4, e->i8);
        else
            a0 = func_000154A8(local, -1, e->c2 & 3, e->i4, e->i8);
    }
    if (D_0009677C && a0)
        func_000589B4(idx, (unsigned char)local);
}
