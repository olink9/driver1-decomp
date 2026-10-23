typedef struct { char pad[4]; char f; char pad2[11]; } T_001CE2AC;
extern T_001CE2AC D_001CE2AC[];
extern int D_001CE3BC;

void func_001C5650(void)
{
    D_001CE2AC[D_001CE3BC].f = 1;
}
