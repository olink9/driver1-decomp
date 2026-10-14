typedef struct {
    unsigned short start;
    char pad[2];
    unsigned char b4, b5, b6, b7;
    char pad2[2];
    unsigned char bA, bB;
} HDR;
typedef struct {
    char c0;
    char c1;
    short len;
    int pos;
    int addr;
    void *func;
} REQ;
typedef struct {
    short a;
    short b;
    int size;
    int f8;
    int fC;
    int f10;
} REQ2;
typedef struct { char pad[0x18]; int f18; char pad2[4]; } BE;
typedef struct { int w[4]; } Q16;

extern short D_0009728C[];
extern unsigned short *D_000972AC;
extern unsigned char *D_00097254;
extern long long D_00096834;
extern BE D_000BE0A4[];
extern int D_000AA7E8[];
extern int D_000AA7D4[];
extern int D_000AB040[];
extern REQ D_000A7BA4[];
extern REQ2 D_000A7EA4[];
extern int D_000972B0;
extern int D_0009724C;
extern int D_0009727C;
extern long long D_000970D4;
extern long long D_00097108;
extern long long D_00097350;
extern void func_0003542C();
extern void func_000354D8();
extern void func_0003490C();

static inline void AddReq(int sector, int len, int addr, void *func)
{
    REQ *r = &D_000A7BA4[D_000972B0];
    r->c0 = 0;
    r->c1 = 0;
    r->pos = sector;
    r->len = len;
    r->addr = addr;
    r->func = func;
    D_000972B0++;
}

int func_000342D4(int a, int b)
{
    HDR *hdr;
    int pos;
    int addr;
    int addr2;
    int other;

    D_0009728C[b] = a;
    hdr = (HDR *)(D_00097254 + D_000972AC[a]);
    addr = D_000AA7E8[b];
    addr2 = addr + 0x1000;
    pos = hdr->start;
    AddReq(D_000BE0A4[*(int *)&D_00096834].f18 / 2048 + pos, hdr->bA, addr, func_0003542C);
    pos += hdr->bA;
    AddReq(D_000BE0A4[*(int *)&D_00096834].f18 / 2048 + pos, hdr->bB, addr2, func_000354D8);
    pos += hdr->bB;
    other = D_0009724C;
    AddReq(D_000BE0A4[*(int *)&D_00096834].f18 / 2048 + pos, hdr->b6, other, 0);
    pos += hdr->b6;
    AddReq(D_000BE0A4[*(int *)&D_00096834].f18 / 2048 + pos, hdr->b5, (int)(*(int **)&D_000970D4 + D_000AA7D4[b]), 0);
    pos += hdr->b5;
    AddReq(D_000BE0A4[*(int *)&D_00096834].f18 / 2048 + pos, hdr->b7, (int)(*(Q16 **)&D_00097108 + (*(int *)&D_00097350 + D_000AB040[b])), 0);
    pos += hdr->b7;
    AddReq(D_000BE0A4[*(int *)&D_00096834].f18 / 2048 + pos, hdr->b4, D_000AA7E8[b], func_0003490C);
    pos += hdr->b4;

    D_000A7EA4[D_0009727C].a = a;
    D_000A7EA4[D_0009727C].b = b;
    D_000A7EA4[D_0009727C].f8 = other;
    D_000A7EA4[D_0009727C].fC = addr;
    D_000A7EA4[D_0009727C].f10 = addr2;
    D_000A7EA4[D_0009727C].size = pos - hdr->start;
    return 1;
}
