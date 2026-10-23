typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF468;
extern T_001CF468 D_001CF468[];
extern int D_001CDECC;
void func_001CD514(void)
{
    D_001CDECC = 0;
    T_001CF468 *p = D_001CF468;
    p += 10;
    p->r = 255;
    p->g = 255;
    p->b = 255;
}
