extern int D_00096E30;
extern long long D_00097098;
extern int D_000976A0;
extern int D_00097688;
extern volatile long long D_00097534;
extern char *D_000BF7EC[];
extern void func_00050E28();
extern int printf();

void func_0005C68C(void)
{
    unsigned char *p;
    unsigned char *q;
    int i;
    unsigned char c;

    int t = *(int *)&D_00097098;
    D_000976A0 = t;
    func_00050E28(D_00096E30, t);
    p = (unsigned char *)D_000976A0;
    D_00097688 = 0xA4;
    *(int *)&D_00097098 = *(int *)&D_00097098 + 0x9C4;
    while (*p++ != 10)
        ;
    if (D_00097688 >= 0xA5) {
        printf("too many lines!\n");
        while (*(volatile int *)&D_00097534 != 0x78654321) {
            __asm__ volatile("break 1024");
        }
    }
    for (i = 0; i < D_00097688; i++) {
        D_000BF7EC[i] = (char *)p;
        c = *p;
        q = p;
        while (c != 10) {
            if (c >= 0x15)
                q = p;
            p++;
            c = *p;
        }
        p++;
        q[1] = 0;
    }
}
