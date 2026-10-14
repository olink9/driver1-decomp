typedef struct { int a[99]; } CAR;
extern unsigned char D_00096899;
extern unsigned char D_00096897;
extern unsigned char D_000968A4, D_000968A5, D_000968A6, D_000968A7;
extern unsigned char End[] __asm__("D_000968A7");
extern unsigned char Arr[] __asm__("D_000968A4");
extern unsigned short D_0009752C[];
extern long long D_00097534;
extern int D_0009706C;
extern int D_00096890;
extern int D_0009688C;
extern CAR D_0009B0A4[];
extern char D_0008EAA8[];
extern int D_0008A8F0[];

void func_00022168(void)
{
    unsigned char *p;
    CAR *car;
    int idx;
    char *t;
    int off;

    if (D_00096899 != 0 || D_00096897 == 6 || !(D_0009752C[0] & 0x100))
        return;
    {
        if (D_0009706C != *(int *)&D_00097534 - 1) {
            for (p = Arr; p < End; p++) {
                if (*p == D_00096897) {
                    D_00096897 = p[1];
                    break;
                }
            }
        }
        D_0009706C = *(int *)&D_00097534;
        if (D_00096897 >= 3) {
            D_000968A4 = 1;
            D_000968A5 = 0;
            D_000968A6 = 2;
            D_000968A7 = 1;
            D_00096897 = 2;
        }
        if (D_00096899 == 0) {
            D_00096890 = 0;
            D_0009688C = 0;
        }
        if (D_00096897 == 5 || D_00096897 == 1) {
            car = D_0009B0A4;
            car += D_00096890;
            t = D_0008EAA8;
            idx = (car->a[62] + 0x800) & 0xFFF;
            off = idx * 4;
            D_0008A8F0[0] = car->a[6] + *(short *)(t + off) * 460 / 4096;
            D_0008A8F0[2] = car->a[8] + *(short *)(t + off + 2) * 460 / 4096;
        }
    }
}
