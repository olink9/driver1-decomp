extern void Loadfile();
extern char D_000EFC08[];

void func_000EC508(char *name)
{
    int i;
    int *p;

    Loadfile(name, D_000EFC08);
    i = 31;
    p = (int *)(D_000EFC08 + 0x2D0C);
    for (; i >= 0; i--) {
        *p = 0;
        p = (int *)((char *)p - 0x174);
    }
}
