typedef struct { char pad[4]; char f; char pad2[11]; } T_001CE3D8;
extern T_001CE3D8 D_001CE3D8[];
extern int D_001D2DC8;

void func_001C57FC(void)
{
    D_001CE3D8[D_001D2DC8].f = 1;
}
