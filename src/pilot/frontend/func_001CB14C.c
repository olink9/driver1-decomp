typedef struct { char pad[12]; char r, g, b, pad2; } T_001CF198;
extern T_001CF198 D_001CF198[];

void func_001CB14C(void)
{
    D_001CF198[2].r = 255;
    D_001CF198[2].g = 255;
    D_001CF198[2].b = 255;
}
