typedef struct { char pad[4]; char f; char pad2[11]; } T_001CE2AC;
extern T_001CE2AC D_001CE2AC[];
extern int D_001CE3C0;

void func_001C5674(void)
{
    int i = D_001CE3C0;
    if (i != -1)
        D_001CE2AC[i].f = 0;
}
