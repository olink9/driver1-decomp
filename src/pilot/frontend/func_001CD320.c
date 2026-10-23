typedef struct { char pad[12]; char r, g, b, pad2; } T_001CF468;
extern T_001CF468 D_001CF468[];

void func_001CD320(void)
{
    D_001CF468[4].r = 255;
    D_001CF468[4].g = 255;
    D_001CF468[4].b = 255;
}
