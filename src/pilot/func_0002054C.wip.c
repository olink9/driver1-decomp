typedef struct {
    char pad0[0x78];
    struct { unsigned char on; char b1; unsigned char b2; char b3; } wheel[4];
    char pad88[0x100 - 0x88];
    int f100;
    int pad104[2];
    int f10C;
    int f110;
    char pad114[396 - 0x114];
} CAR;

extern CAR D_0009B0A4[];
extern int D_0009B16C[];
extern long long D_00096AA8;
extern long long D_00096AA9;
extern unsigned char D_0009702C;
extern unsigned char D_00097028;
extern unsigned char D_0009701C;
extern int D_00096790;

extern void func_00020844();
extern void func_000299B8();
extern void func_00029B88();
extern void func_00029C00();
extern void func_0004DB84();
extern void func_0004DDA8();

void func_0002054C(int player)
{
    CAR *cp;
    int onground;
    unsigned char change;
    int sp;
    int i;
    int vol;

    onground = 0;
    change = 0;
    cp = &D_0009B0A4[player];
    sp = cp->f10C;
    func_00020844(cp);
    if (player != 0)
        return;
    for (i = 0; i < 4; i++) {
        if (cp->wheel[i].on)
            onground = 1;
    }
    vol = 0;
    if (onground) {
        if (cp->f100 != 0 || abs(cp->f110) > 16000) {
            change = D_0009702C = 1;
            vol = 13000;
            if (cp->f100 == 0)
                vol = (abs(cp->f110) - 16000) / 2 + 1;
        }
    }
    if (abs(sp) > 16000) {
        D_0009702C = 0;
        change = 1;
    }
    if (player == 0) {
        if (vol != 0 && (cp->wheel[1].b2 == 0 || cp->wheel[3].b2 == 0) && *(unsigned char *)&D_00096AA8 == 0 && *(unsigned char *)&D_00096AA9 == 0) {
            if (vol > 13000)
                vol = 13000;
            func_0004DDA8(3, D_0009B16C, D_0009B16C + 4, vol - 10000, 4096);
        } else {
            func_0004DB84(3, -10000);
        }
    }
    D_00097028 = 0;
    func_000299B8(0);
    if (change) {
        if (D_00096790 == D_0009702C)
            D_0009701C = 1;
        else
            D_0009701C = 0;
        func_00029C00(0);
        func_00029C00(1);
        D_00096790 = D_0009702C;
    } else {
        D_00096790 = -1;
    }
    func_00029B88(0);
}
