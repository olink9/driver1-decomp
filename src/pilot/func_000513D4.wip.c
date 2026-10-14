typedef struct BUF { unsigned a; int ot; unsigned c; } BUF;
extern int D_00097534;
extern BUF *D_00097530;
extern BUF *D_00097524;
extern int D_00096CB0;
extern char D_000BDFA4[];
extern char D_000BE024[];
extern void PutDispEnv();
extern void PutDrawEnv();
extern int printf();
extern void func_00074684();
extern void func_0007457C();
extern char D_00010954[];

void func_000513D4(void)
{
    unsigned a, c, lim;
    if (D_00097534 & 1) {
        D_00097530 = (BUF *)D_000BDFA4;
        D_00097524 = (BUF *)(D_000BDFA4 + 0x80);
    } else {
        D_00097530 = (BUF *)D_000BE024;
        D_00097524 = (BUF *)(D_000BE024 - 0x80);
    }
    if (D_00096CB0 == 0) {
        PutDispEnv((char *)D_00097530 + 0x6C);
        PutDrawEnv((char *)D_00097530 + 0x10);
    }
    D_00096CB0 = 0;
    lim = 0x1E000;
    c = D_00097530->c;
    a = D_00097530->a;
    if (c + lim < a) {
        printf(D_00010954, c - (a - lim));
        if (D_00097534 != 0x78654321)
            do {
                __asm__ volatile("break 1024");
            } while (D_00097534 != 0x78654321);
    }
    D_00097530->a = D_00097530->c;
    func_00074684(D_00097524->ot + 0x41FC);
    func_0007457C(D_00097530->ot, 0x1080);
}
