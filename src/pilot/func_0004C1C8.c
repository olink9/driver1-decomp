extern char D_000BC8AC[];
extern char D_000BC7AC[];
extern short D_0008A910[];
extern int D_000974B8;
extern void memset();

void func_0004C1C8(void)
{
    int i;
    char *r;
    char pad[8];
    short *p;
    short v;
    short s;
    char *q;

    char k = 0x30;
    i = 255;
    r = D_000BC8AC;
    r += i;
    for (; i >= 0; i--)
        *r-- = k;
    p = D_0008A910;
    q = D_000BC7AC;
    D_000974B8 = 0;
    memset(q, 14, 0x100);
    p++;
    s = *p;
    v = D_0008A910[0];
    p++;
    while (s != -1) {
        if (s == -2)
            v = *p++;
        else
            q[s] = v;
        s = *p++;
    }
}
