typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF468;
extern T_001CF468 D_001CF468[];

void func_001CD320(void)
{
    T_001CF468 *p = D_001CF468;
    p += 4;
    p->r = 255;
    p->g = 255;
    p->b = 255;
}
