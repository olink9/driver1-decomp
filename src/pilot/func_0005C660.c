struct S { char pad[0x22]; unsigned char prev; };
extern struct S *D_00096DB4;
int func_0005C660(void)
{
    if (!D_00096DB4 || D_00096DB4->prev == 255)
        return 1;
    return 0;
}
