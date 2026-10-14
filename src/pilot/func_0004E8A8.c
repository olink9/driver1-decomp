struct S84 { char pad[0x40]; unsigned char f40; char p41; unsigned short f42; char pad2[0x40]; };
extern int D_00096C88;
extern int D_00096C80;
extern int D_00096C74;
extern long long D_00097540;
extern struct S84 D_000BD498[];
extern void func_0006FA0C();

void func_0004E8A8(void)
{
    int i;
    *(int *)&D_00097540 = *(int *)&D_00097540 + 1;
    if (D_00096C88 != -1) func_0006FA0C();
    if (D_00096C80 == 0 && D_00096C74 == 0) {
        for (i = 0; i < 16; i++) {
            if (D_000BD498[i].f40 == 0 && D_000BD498[i].f42 != 0)
                D_000BD498[i].f42--;
        }
    }
}
