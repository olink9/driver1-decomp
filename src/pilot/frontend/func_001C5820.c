typedef struct { char pad[4]; char f; char pad2[11]; } T_001CE3D8;
extern T_001CE3D8 D_001CE3D8[];
extern int D_001CE3D4;

void func_001C5820(void)
{
    int i = D_001CE3D4;
    if (i != -1)
        D_001CE3D8[i].f = 0;
}
