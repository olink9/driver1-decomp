typedef struct { char pad[4]; char f; char pad2[11]; } T_001CEE68;
extern T_001CEE68 D_001CEE68[];
extern int D_001CEF28;

void func_001C8DC8(void)
{
    D_001CEE68[D_001CEF28 + 1].f = 1;
}
