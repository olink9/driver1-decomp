typedef struct {
    unsigned char id;   /* 0x00 */
    unsigned char f1;   /* 0x01 */
    char pad2[3];
    unsigned char f5;   /* 0x05 */
    char pad6[2];
    unsigned char f8;   /* 0x08 */
    char pad9[0x50 - 9];
} ENT;

typedef struct {
    char pad[0x1A];
    short s1A, s1C, s1E;
    char pad20[0x38 - 0x20];
} B048_S;

extern ENT D_00098950[];
extern char D_000C4E38[];
extern char D_00099158[];
extern B048_S D_00099120;
extern long long D_00096834;
extern short D_00096FE8;
extern int D_00096FF4;
extern int D_00096FD0;
extern int D_00096FF0;
extern char D_00096FC4;
extern char D_00096FBC;
extern unsigned char D_00099AF0[];
extern void ClearMem();
extern void func_0001250C();
extern void func_00021D54();
extern void func_00021DE8();
extern void func_00021F5C();

static inline void ClearMemInline(unsigned char *p, int n)
{
    unsigned char *end = p + n;

    while (((int)p & 3) && p < end)
        *p++ = 0;
    while (p <= (unsigned char *)((int *)end - 1)) {
        *(int *)p = 0;
        p += 4;
    }
    while (p < end)
        *p++ = 0;
}

void func_00016CE4(void)
{
    int i;
    B048_S *b;
    ENT *e;
    unsigned char *q;

    ClearMemInline((unsigned char *)D_00098950, 2000);
    ClearMemInline((unsigned char *)D_000C4E38, 0x1EA0);
    ClearMem(D_00099158, 0x28);
    if (*(int *)&D_00096834 != 2) {
        b = &D_00099120;
        ClearMemInline((unsigned char *)b, 0x38);
        b->s1E = 450;
        b->s1A = -1;
        b->s1C = -1;
        D_00096FE8 = -1;
        D_00096FF4 = 0;
        func_00021DE8();
        func_00021F5C();
        func_00021D54();
    }
    func_0001250C();
    D_00096FD0 = 0x10101010;
    D_00096FF0 = 99;
    D_00096FC4 = 0;
    i = 0;
    q = D_00099AF0;
    e = D_00098950;
    do {
        *q = i;
        e->f8 = 0;
        e->id = i;
        i++;
        q++;
        e->f1 = 0;
        e->f5 = 0;
        e++;
    } while (i < 22);
    D_00096FBC = 0;
}
