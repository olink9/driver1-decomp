/* func_0006AF48 - no Driver 2 counterpart: two gradient-coloured numbers */
typedef struct {
    unsigned char r, g, b, pad;
    int val;
    int type;
} COLSTEP;

extern char D_0008C248[];
extern short D_0008C27A[];
extern COLSTEP D_0008C2E8[];
extern char D_00096EF0[];
extern char D_00096EF4[];
extern char D_00096EF8[];
extern unsigned char D_000975E0[];
extern unsigned char D_00097698[];
extern unsigned char D_00097699[];
extern unsigned char D_0009769A[];
extern int func_0006B450();
extern int sprintf();
extern void func_0005CC68(char *, short, short);
extern int func_0005CF10(short, short, char *);
extern void func_0005C7D0();

void func_0006AF48(int *v, int x, int y)
{
    char buf1[8];
    char buf2[8];
    COLSTEP *p;
    COLSTEP *q;
    int w;
    int val;
    int f;
    int g;
    unsigned char len;

    w = func_0006B450(D_0008C248, 4);
    val = v[9];
    p = D_0008C2E8;
    while (p->val < val)
        p++;
    if (p->type != 0 && p->type == 2)
        val = p[-1].val + (D_000975E0[0] * (p->val - p[-1].val)) / 8;
    q = p - 1;
    f = ((val - q->val) << 12) / (p->val - q->val);
    g = 0x1000 - f;
    D_00097698[0] = (g * q->r + f * p->r) >> 12;
    D_00097699[0] = (g * q->g + f * p->g) >> 12;
    D_0009769A[0] = (g * q->b + f * p->b) >> 12;
    sprintf(buf1, D_00096EF0, v[9]);
    func_0005CC68(buf1, w, D_0008C27A[0]);

    val = v[0];
    p = D_0008C2E8;
loop:
    if (p->val < val) {
        p++;
        goto loop;
    }
    if (p->type != 0 && p->type == 2)
        val = p[-1].val + (D_000975E0[0] * (p->val - p[-1].val)) / 8;
    q = p - 1;
    f = ((val - q->val) << 12) / (p->val - q->val);
    g = 0x1000 - f;
    D_00097698[0] = (g * q->r + f * p->r) >> 12;
    D_00097699[0] = (g * q->g + f * p->g) >> 12;
    D_0009769A[0] = (g * q->b + f * p->b) >> 12;
    len = sprintf(buf2, D_00096EF0, v[0]);
    w = func_0005CF10(x + 25 + (2 - len) * 26, y, buf2);
    func_0005C7D0(0x80, 0x80, 0x80);
    if (v[1] != 0x7FFFFFFF)
        sprintf(buf2, D_00096EF4, v[1]);
    else
        sprintf(buf2, D_00096EF8);
    func_0005CC68(buf2, w, y + 13);
}
