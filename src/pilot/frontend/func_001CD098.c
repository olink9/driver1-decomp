typedef struct { char pad[12]; char r, g, b, pad2; } T_001CF368;
extern T_001CF368 D_001CF368[];

void func_001CD098(void)
{
    D_001CF368[6].r = 255;
    D_001CF368[6].g = 255;
    D_001CF368[6].b = 255;
}
