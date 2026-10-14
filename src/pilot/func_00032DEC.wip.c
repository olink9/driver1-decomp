/* test_changemode */
typedef struct { unsigned char type; unsigned char data; unsigned short nsectors; int sector; int addr; int pad; } SQ;
typedef struct { int a; int nsectors; int c; int d; int e; } RI;
extern SQ D_000A7BA4[];
extern RI D_000A7EA4[];
extern volatile int D_000969EC; extern int D_000972B0, D_000971F4, D_00097204, D_000971F0, D_000971F8, D_000971FC;
extern int D_000972BC, D_0009720C, D_00097210, D_00097220, D_000972B4, D_00097268, D_00097274, D_00097280;
extern void func_00034B18(), func_00034C74(), func_00035044(), func_000352D4();
extern void func_0007A12C(), func_0007A27C();

void func_00032DEC(void)
{
    SQ *current = &D_000A7BA4[D_000969EC];
    int ta;

    if (D_000969EC != D_000972B0) {
      if (D_000971F4 == current->sector) {
        ta = current->addr;
        D_000971F0 = ta;
        D_00097204 = 1;
        switch (current->type) {
        case 0:
            D_000971F8 = D_000A7EA4[D_000972BC + 1].nsectors;
            D_000971FC = current->nsectors;
            func_0007A12C(func_00034C74);
            break;
        case 1:
            D_0009720C = 0;
            D_00097210 = 0;
            D_000971F8 = 17;
            D_000971FC = 1;
            D_00097220 = D_000972B4;
            D_000971F0 = current->addr + 0x4000;
            func_0007A12C(func_00034B18);
            break;
        case 3:
            D_000971F8 = current->nsectors;
            func_0007A12C(func_000352D4);
            break;
        case 2:
            D_00097274 = 0;
            D_0009720C = 0;
            D_00097210 = 0;
            D_000971FC = 2;
            D_000971F8 = current->nsectors;
            D_000971F0 = ta + ((D_00097268 & 1) << 12);
            D_00097280 = current->data;
            func_0007A12C(func_00035044);
            break;
        }
      } else {
        D_00097204 = 0;
        func_0007A12C(0);
      }
    } else {
        D_00097204 = 0;
        func_0007A12C(0);
        func_0007A27C(9, 0);
    }
}
