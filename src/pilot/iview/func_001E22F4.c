extern char D_001E3758[];
extern int D_001E3750;
extern int D_001E35B0;
extern void *memset(void *s, int c, int n);

void func_001E22F4(void)
{
    memset(D_001E3758, 0, 0x8C0);
    D_001E3750 = 0;
    D_001E35B0 = 0;
}
