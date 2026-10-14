/* per-frame bonus/score condition checks (no Driver 2 counterpart) */
typedef struct {
    char pad0[0xC8];
    int fC8;
    int padCC;
    int fD0;
    char padD4[0xF8 - 0xD4];
    int fF8;
    int padFC;
    int f100;
    int pad104;
    int f108;
    int pad10C;
    int f110;
    int f114;
    int f118;
} Car;

extern Car D_0009B0A4[];
extern short D_000988C8[];
extern short D_00096FE8;
extern long long D_00096850;
extern unsigned char D_000C6F02[];
extern char D_00010000[];
extern char D_00010018[];
extern int func_000635B0();
extern int func_00021558();
extern int func_00020AE4();
extern int func_0001AC18();
extern void func_000650E4();
extern int sprintf();

void func_0001A9BC(void)
{
    char buf[80];
    unsigned char dir;
    int r;
    unsigned short v;
    int t;

    dir = func_000635B0(D_0009B0A4[0].fF8);
    r = func_00021558(D_0009B0A4[0].fC8, D_0009B0A4[0].fD0);
    v = r;
    if (v == 0xFFFF) {
        D_00096FE8 = -1;
    } else if (v != 0x8000) {
        if (v < 0x4000) {
            sprintf(buf, D_00010000, v);
            if (v != D_00096FE8) {
                if (func_0001AC18(v))
                    func_000650E4(D_000988C8, 3, 0x1000);
                D_00096FE8 = r;
            }
        } else {
            sprintf(buf, D_00010018, r & 0x3FFF);
        }
    }

    t = func_00020AE4(D_0009B0A4[0].fC8, D_0009B0A4[0].fD0);
    if (t != -1 && (t & 0xC000) == 0 && t == ((dir + 2) & 3)) {
        D_000988C8[16]++;
        if (D_0009B0A4[0].f108 > 120)
            D_000C6F02[0] = 1;
    } else {
        D_000988C8[16] = 0;
    }
    func_000650E4(D_000988C8, 8, 0x1000);

    if (*(int *)&D_00096850 != 0 && D_0009B0A4[0].f114 > 1000 && D_0009B0A4[0].f118 > 1000)
        func_000650E4(D_000988C8, 9, 0x1000);

    if (D_0009B0A4[0].f100 != 0 || abs(D_0009B0A4[0].f110) > 16000)
        D_000988C8[20]++;
    else
        D_000988C8[20] = 0;
    func_000650E4(D_000988C8, 10, 0x1000);

    if (D_0009B0A4[0].f108 > 180)
        func_000650E4(D_000988C8, 2, 0x1000);
}
