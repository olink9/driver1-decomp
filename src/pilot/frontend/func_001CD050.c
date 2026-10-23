typedef struct { char pad[12]; char r, g, b, pad2; } T_001CF368;
extern T_001CF368 D_001CF368[];

void func_001CD050(void)
{
    D_001CF368[5].r = 255;
    D_001CF368[5].g = 255;
    D_001CF368[5].b = 255;
}
