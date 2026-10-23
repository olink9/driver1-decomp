typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF198;
extern T_001CF198 D_001CF198[];

void func_001CB14C(void)
{
    T_001CF198 *p = D_001CF198;
    p += 2;
    p->r = 255;
    p->g = 255;
    p->b = 255;
}
