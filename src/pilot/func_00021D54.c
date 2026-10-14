typedef struct {
    char pad0[3]; char flag; char pad1[6]; short s0;
    char pad2[6]; short s1; char pad3[6]; short s2;
    char pad4[6]; short s3; char pad5[8];
} ENT;
extern int D_000967AC;
extern ENT *D_00097040;

void func_00021D54(void) {
    int i;
    int n;
    ENT *p;
    int cnt = D_000967AC;
    p = D_00097040;
    i = 0;
    while (i < D_000967AC) {
        n = 0;
        if (p->s0 != -1) n++;
        if (p->s1 != -1) n++;
        if (p->s2 != -1) n++;
        if (p->s3 != -1) n++;
        if (n == 2) p->flag = 1;
        else p->flag = 0;
        i++;
        p++;
    }
}
