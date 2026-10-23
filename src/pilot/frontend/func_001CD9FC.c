typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF710;
extern T_001CF710 D_001CF710[];

void func_001CD9FC(int i)
{
    T_001CF710 *p = &D_001CF710[i];
    p->r = 255;
    p->g = 255;
    p->b = 255;
}
