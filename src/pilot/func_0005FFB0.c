typedef struct { char pad[10]; unsigned char f; char pad2[1]; } E12;
typedef struct {
    int f0;            /* 0x00 */
    int f4;
    int f8;            /* 0x08 */
    char pad0C[0x0C];
    short f18;         /* 0x18 */
    short f1A;
    short f1C;
    char pad1E[0x0A];
    char f28[0x14];    /* 0x28 */
    int f3C;           /* 0x3C */
    short pad40;
    short f42;         /* 0x42 */
    short f44;         /* 0x44 */
    char pad46[0x0D];
    unsigned char f53; /* 0x53 */
    char pad54[2];
    unsigned char f56; /* 0x56 */
} OBJ;
extern int D_000976C0;
extern E12 *D_000976CC;
extern int D_000C0340[];
extern int func_0005FBE0();
extern void func_0006009C();
extern void func_00060328();
extern void func_000604F8();
extern void func_0005D974();

void func_0005FFB0(OBJ *p)
{
    int r;
    char *q = p->f28;

    r = func_0005FBE0(0, p->f0, p->f8, q);
    if (r == 0) {
        p->f3C = 0;
        p->f1C = 0;
        p->f1A = 0;
        p->f18 = 0;
        D_000C0340[--D_000976C0] = p->f42;
        if (p->f56 == 6) {
            E12 *e = &D_000976CC[p->f53];
            if (e->f != 0)
                e->f = 0;
        }
    } else {
        p->f44 = r;
        func_0006009C(p, p);
        func_00060328(p, q);
        func_000604F8(p);
        func_0005D974(p);
    }
}
