typedef struct { char pad[12]; char r, g, b, pad2; } T_001CF198;
extern T_001CF198 D_001CF198[];

void func_001CB104(void)
{
    D_001CF198[1].r = 255;
    D_001CF198[1].g = 255;
    D_001CF198[1].b = 255;
}
