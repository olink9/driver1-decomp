typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF710;
extern T_001CF710 D_001CF710[];

void func_001CD9FC(int i)
{
    D_001CF710[i].r = 255;
    D_001CF710[i].g = 255;
    D_001CF710[i].b = 255;
}
