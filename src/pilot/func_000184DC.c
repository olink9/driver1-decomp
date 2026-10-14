typedef struct { char pad[0xF4]; int f; char pad2[0x18C-0xF8]; } S18C;
extern S18C D_0009B230[];
void func_000184DC(unsigned char *p)
{
    S18C *s = &D_0009B230[p[0]];
    if (p[0xB] != 0) {
        s->f = 0x23F;
        p[0xB]--;
    } else {
        p[1] = 2;
        p[2] = 1;
    }
}
