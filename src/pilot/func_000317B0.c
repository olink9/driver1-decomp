extern long long D_00096800;
extern char D_000969B4[], D_000969BC[], D_000969C0[];
extern char D_1F3850[], D_1F3836[];
extern void func_00052240(char *, char *);
void func_000317B0(void)
{
    if (*(int *)&D_00096800 == 1) {
        func_00052240(D_000969B4, D_1F3850);
        func_00052240(D_000969BC, D_1F3836);
        func_00052240(D_000969C0, D_1F3836 - 14);
    }
}
