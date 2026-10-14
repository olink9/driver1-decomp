/* sector_ready */
extern int D_0009751C;
extern int D_00097514;
extern int D_00097510;
extern char D_0009750D;
extern void func_0007A4FC();
extern void func_0007A12C();
extern int func_00079F70();
extern void func_0007A580();
extern void func_0007A27C();

void func_0005122C(a0, result)
int a0;
unsigned char *result;
{
    char p[8];
    int intr = a0 & 0xff;

    if (intr == 1) {
        func_0007A4FC(D_0009751C, 0x200);
        D_0009751C += 0x800;
        D_00097510++;
        D_00097514--;
        if (D_00097514 == 0) {
            D_0009750D = intr;
            func_0007A12C(0);
            func_0007A27C(9, 0);
        }
    } else {
        if (*result & 0x10) {
            func_0007A12C(0);
            do {
            } while (func_00079F70(1) != 2);
            func_0007A12C(func_0005122C);
        }
        func_0007A580(D_00097510, p);
        func_0007A27C(0x1B, p);
    }
}
