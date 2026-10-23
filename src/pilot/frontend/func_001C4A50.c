typedef struct { char pad[4]; char f; char pad2[11]; } T_001CE0CC;
extern T_001CE0CC D_001CE0CC[];
extern int D_001D2DB8;

void func_001C4A50(void)
{
    int i = D_001D2DB8;
    if (i != -1)
        D_001CE0CC[i].f = 0;
}
