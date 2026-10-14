/* func_0006AF48 - no Driver 2 counterpart: two gradient-coloured numbers (WIP 95.33: buf2 address CSEd into s0, case-2 load order) */
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
extern void func_0005CC68();
extern int func_0005CF10();
extern void func_0005C7D0();

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
        val = p[-1].val + (*(unsigned char *)&D_000975E0 * (p->val - p[-1].val)) / 8;
        break;
    }
    q = p - 1;
    f = ((val - q->val) << 12) / (p->val - q->val);
    g = 0x1000 - f;
    D_00097698.v = (g * q->r + f * p->r) >> 12;
    D_00097699.v = (g * q->g + f * p->g) >> 12;
    D_0009769A.v = (g * q->b + f * p->b) >> 12;
}

void func_0006AF48(int *v, short x, short y)
{
    char buf1[8];
    char buf2[8];
    COLSTEP *p;
    COLSTEP *q;
    short w;
    int val;
    int f;
    int g;
    unsigned char len;

    w = func_0006B450(D_0008C248, 4);
    SetColour(v[9]);
    sprintf(buf1, D_00096EF0, v[9]);
    func_0005CC68(buf1, w, D_0008C27A[0]);

    SetColour(v[0]);
    len = sprintf(buf2, D_00096EF0, v[0]);
    x += 25;
    w = func_0005CF10((short)(x + (2 - len) * 26), y, buf2);
    func_0005C7D0(0x80, 0x80, 0x80);
    if (v[1] != 0x7FFFFFFF)
        sprintf(buf2, D_00096EF4, v[1]);
    else
        sprintf(buf2, D_00096EF8);
    func_0005CC68(buf2, w, y + 13);
}
