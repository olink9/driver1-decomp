typedef struct { char pad[0x78C]; int f; char pad2[0x18]; } S7A8;
extern S7A8 D_000C4E38[];
extern void func_00012498(S7A8 *p, int n);
int func_00063B14(void)
{
    int i;
    S7A8 *p = D_000C4E38;
    for (i = 0; i < 4; i++, p++) {
        if (p->f == 0) {
            func_00012498(p, 0x7A8);
            return i;
        }
    }
    return -1;
}
