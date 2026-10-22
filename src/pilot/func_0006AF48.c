/* func_0006AF48 - no Driver 2 counterpart: two gradient-coloured numbers (second one right-aligned to k digits). */
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
extern long long D_000975E0;
typedef struct { unsigned char v; char pad[7]; } B8;
extern B8 D_00097698;
extern B8 D_00097699;
extern B8 D_0009769A;
extern int func_0006B450();
extern int sprintf();
extern void PrintString();
extern int PrintDigit();
extern void SetTextColour();

static inline void SetColour(int val)
{
    COLSTEP *p;
    COLSTEP *q;
    int f;
    int g;

    p = D_0008C2E8;
    while (p->val < val)
        p++;
    switch (p->type) {
    case 0:
        break;
    case 2:
        { int lo = p[-1].val; val = lo + (*(unsigned char *)&D_000975E0 * (p->val - lo)) / 8; }
        break;
    }
    q = p - 1;
    f = ((val - q->val) << 12) / (p->val - q->val);
    g = 0x1000 - f;
    D_00097698.v = (g * q->r + f * p->r) >> 12;
    D_00097699.v = (g * q->g + f * p->g) >> 12;
    D_0009769A.v = (g * q->b + f * p->b) >> 12;
}

static inline short Off(short x, int k, unsigned char len)
{
    return x + (k - len) * 26;
}

static inline void ShowB(int *v, short x, short y)
{
    char buf[8];
    short w;
    unsigned char len;

    SetColour(v[0]);
    len = sprintf(buf, D_00096EF0, v[0]);
    w = PrintDigit(Off(x + 25, 2, len), y, buf);
    SetTextColour(0x80, 0x80, 0x80);
    if (v[1] != 0x7FFFFFFF)
        sprintf(buf, D_00096EF4, v[1]);
    else
        sprintf(buf, D_00096EF8);
    PrintString(buf, w, y + 13);
}

void func_0006AF48(int *v, short x, short y)
{
    char buf1[8];
    short w;

    w = func_0006B450(D_0008C248, 4);
    SetColour(v[9]);
    sprintf(buf1, D_00096EF0, v[9]);
    PrintString(buf1, w, D_0008C27A[0]);
    ShowB(v, x, y);
}
