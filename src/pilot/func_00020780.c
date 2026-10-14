typedef struct { char pad[0x100]; int a; int pad2[2]; int b; char pad3[396 - 0x110]; } Car;
extern Car D_0009B0A4[];
extern int D_00096794;
extern unsigned char D_0009702C, D_0009701C;
extern void func_000299B8(), func_00029C00(), func_00029B88();
void func_00020780(int i)
{
    Car *c = &D_0009B0A4[i];
    int v, w = c->b;
    func_000299B8();
    if (c->a == 0 && abs(w) <= 0x3E80) {
        v = -1;
    } else {
        if (D_00096794 == D_0009702C) D_0009701C = 1;
        else D_0009701C = 0;
        func_00029C00(2);
        func_00029C00(3);
        v = D_0009702C;
    }
    D_00096794 = v;
    func_00029B88(2);
}
