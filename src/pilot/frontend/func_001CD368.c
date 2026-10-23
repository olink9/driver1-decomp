typedef struct { char pad[12]; char r, g, b, pad2; } T_001CF468;
extern T_001CF468 D_001CF468[];

void func_001CD368(void)
{
    D_001CF468[5].r = 255;
    D_001CF468[5].g = 255;
    D_001CF468[5].b = 255;
}
