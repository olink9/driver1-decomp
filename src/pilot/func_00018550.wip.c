typedef struct {
    char pad0[0xFC];
    int f_FC;
    char pad1[0x17C - 0x100];
    short f_17C;
    char pad2[396 - 0x17E];
} S;
typedef struct {
    char c0;
    char c1;
    char pad[0x13];
    char c15;
    char c16;
    char pad2[0x38 - 0x17];
    int f38;
    int f3C;
} A;
typedef struct {
    char pad[0x794];
    int f794;
} B;
typedef struct {
    char pad[0x26];
    unsigned char c26;
} C;
extern S D_0009B230[];
extern C D_00099120;
extern B *func_00063B74();
extern void func_0001A008();

void func_00018550(A *a)
{
    C *c;
    S *s;
    B *b;

    s = &D_0009B230[*(unsigned char *)a];
    if (s->f_17C == 0) {
        a->f3C = 0;
        a->f38 = 0;
        a->c15 = 1;
        a->c16 = 1;
    } else {
        b = func_00063B74(a, s);
        if (b != 0) {
            a->c1 = 2;
            c = &D_00099120;
            s->f_FC = 0;
            b->f794 = 0;
            c->c26--;
            if (c->c26 == 0)
                func_0001A008();
        }
    }
}
