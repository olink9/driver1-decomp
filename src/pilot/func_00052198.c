struct B8 { char b[8]; };
extern struct B8 D_000975F0;
extern struct B8 D_00097598;
extern int D_00097580;
extern long long D_00097584;
extern int D_00097588;
extern void func_000511EC(), loadsectors(), func_000516E4();

void func_00052198(void)
{
    struct B8 a, b;
    func_000511EC(0);
    a = D_000975F0;
    b = D_00097598;
    loadsectors(0xD8600, D_00097580, *(int *)&D_00097584);
    func_000516E4(&a, &b, D_00097588, 0xD8600);
    func_000511EC(1);
}
