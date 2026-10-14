struct B { char pad[0x41]; char f; char pad2[0x42]; };
extern struct B D_000BD498[];
void func_0004E1DC(unsigned a)
{
    char pad[16];
    if (a < 16)
        D_000BD498[a].f = 1;
}
