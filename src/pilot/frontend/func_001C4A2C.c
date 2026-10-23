typedef struct { char pad[4]; char f; char pad2[11]; } T_001CE0CC;
extern T_001CE0CC D_001CE0CC[];
extern int D_001D2DC0;

void func_001C4A2C(void)
{
    D_001CE0CC[D_001D2DC0].f = 1;
}
