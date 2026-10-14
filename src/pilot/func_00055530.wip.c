typedef struct { unsigned int tag; unsigned int rgb; short x, y; } P3;
typedef struct { P3 *cur; unsigned int **ot; } Buf;
extern struct { Buf *p; int pad; } D_00097530;
extern void func_000539C8();
void func_00055530(int a, int b)
{
    short v[8];
    P3 *c;
    Buf *bf;
    unsigned int t;
    func_000539C8(a, v, 1, 0, 0);
    c = D_00097530.p->cur;
    t = (unsigned int)c & 0xFFFFFF;
    ((unsigned char *)c)[3] = 2;
    c->rgb = b;
    ((unsigned char *)c)[7] = 0x68;
    bf = D_00097530.p;
    c->x = v[0];
    c->y = v[4];
    c->tag = (c->tag & 0xFF000000) | (*bf->ot[0] & 0xFFFFFF);
    *bf->ot = (unsigned int *)((*bf->ot[0] & 0xFF000000) | t);
    bf->cur++;
}
