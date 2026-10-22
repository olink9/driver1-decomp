void CdInit(void);
void CdControl(int, char *, int);
int func_0007A12C(void *);
void cbready(void);
extern int D_00096F94;
extern int D_00096F98;
extern int D_000977FC;
void func_0006F57C(void) {
    char buf[4];
    D_00096F94 = 0;
    D_00096F98 = 0;
    CdInit();
    buf[0] = 0xE8;
    CdControl(0xE, buf, 0);
    D_000977FC = func_0007A12C(cbready);
}
