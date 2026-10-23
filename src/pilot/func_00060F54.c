typedef struct {
    int x, y, z;
    char p0[0xE];
    short s1a;
    char p1[4];
    short s20, s22, s24;
    char p2[0x16];
    int i3c;
    char p3[2];
    short s42;
    char p4[0xC];
    char c50;
    char p5;
    char c52;
    char c53;
    char p6[2];
    char c56;
    char p7;
} Obj;
typedef struct { int x, z; unsigned short rot; char c0a; } Src;
extern int D_000976C0;
extern int D_000C0340[];
extern Obj D_000BFA7C[];
extern short rcossin_tbl[];
extern int MapHeight();
extern void func_0005D974();

void func_00060F54(Src *src, int arg)
{
    int idx;
    Obj *o;

    if (D_000976C0 < 20) {
        idx = D_000C0340[D_000976C0];
        D_000976C0++;
        o = &D_000BFA7C[idx];
        o->i3c = 16;
        if (idx != -1) {
            src->c0a = idx + 1;
            o->c56 = 6;
            o->c50 = 0;
            o->s20 = 0;
            o->s22 = 0;
            o->s24 = 0;
            o->s1a = src->rot;
            o->x = src->x - (rcossin_tbl[(src->rot & 0xfff) * 2] * 45 >> 11);
            o->z = src->z - (rcossin_tbl[(src->rot & 0xfff) * 2 + 1] * 45 >> 11);
            o->y = -MapHeight(o);
            o->s42 = idx;
            o->c53 = arg;
            func_0005D974(o);
        }
    }
}
