typedef struct { char p0[2]; short s2; char p4[2]; unsigned char b6, b7; char p8[3]; char b11; char p12[4]; } E16;
typedef struct { char p0[0xA]; short sA; char pC[0xE]; short s1A; char p1C[0x10]; } E44;
extern int D_000967A8;
extern E16 *D_00097044;
extern E44 *D_00097040;
extern unsigned char D_00086710[];
#define AT(o) ((E16 *)((o) + (int)D_00097044))

static inline unsigned char chk(short k)
{
    E44 *d = (E44 *)(D_00097044[k].s2 * 44 + (int)D_00097040);
    if (d->sA == k) return 0;
    if (d->s1A == k) return 0;
    return 1;
}

void func_00021DE8(void)
{
    int i = 0;
    int off = i;
    for (; i < D_000967A8; i++, off += 16) {
        E16 *e = AT(off);
        unsigned char t = e->b7 & D_00086710[e->b6];
        if (t == 0) {
            if (chk(i) == 0) AT(off)->b11 = 0x82;
            else AT(off)->b11 = 0x83;
        } else if (t == D_00086710[e->b6]) {
            if (chk(i) == 0) AT(off)->b11 = 0x80;
            else AT(off)->b11 = 0x81;
        } else {
            e->b11 = 0;
        }
    }
}
