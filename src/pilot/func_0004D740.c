struct S84 { char pad[0x48]; int f48; char p4c[4]; short f50; char p52[10]; int f5c; char p60[0x10]; int f70; char p74[0x10]; };
extern struct S84 D_000BD498[];
extern int func_0004E4B0();
extern int func_0004D968();

int func_0004D740(int a0, int a1, int a2, int a3, int a4)
{
    int idx = a0;
    if (a0 < 0) {
        idx = func_0004E4B0();
    }
    a0 = idx;
    if ((unsigned)a0 >= 16) return -1;
    D_000BD498[a0].f5c = a3;
    D_000BD498[a0].f70 = a4;
    D_000BD498[a0].f48 = 0;
    D_000BD498[a0].f50 = 0x1000;
    return func_0004D968(a0, a1, a2, 0x1000);
}
