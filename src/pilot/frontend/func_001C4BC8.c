typedef struct {
    char pad0[0xD0];
    char dD0[0x18];
    char dE8[0x1C];
    int w104;
    char pad108[0x2A0 - 0x108];
    int w2A0;
    char pad2A4[0x2E4 - 0x2A4];
    unsigned char b2E4;
} PL;
typedef struct { int a; char *b; } ARG;
extern PL D_000C6F04;
extern int D_0009D8DC[];
extern int D_000967D4, D_000967F4;
extern unsigned char D_000977BC;
extern char *func_0006E418();
extern void func_0006E39C();
extern void func_0006E644();
extern void func_00024754();

void func_001C4BC8(int n)
{
    unsigned char flag;
    ARG arg;

    D_000977BC = n;
    flag = D_000967D4;
    func_0006E39C(D_000C6F04.dE8, D_000967F4, func_0006E418(D_000967F4, n));
    func_00024754();
    if (D_0009D8DC[0] == 0 && D_0009D8DC[1] == 0 && D_0009D8DC[2] == 0 && D_0009D8DC[3] == 0
        && D_000C6F04.b2E4 == 0
        && !(flag != 0 && D_000967F4 == 0x16)
        && !((D_000967F4 == 0x12 || D_000967F4 == 0x16) && D_000C6F04.w104 == 0xFF0)) {
        arg.a = D_000C6F04.w2A0;
        arg.b = D_000C6F04.dD0;
        func_0006E644(&arg, D_000967F4, n);
    }
}
