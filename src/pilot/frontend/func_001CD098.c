typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF368;
extern T_001CF368 D_001CF368[];

void func_001CD098(void)
{
    T_001CF368 *p = D_001CF368;
    p += 6;
    p->r = 255;
    p->g = 255;
    p->b = 255;
}
