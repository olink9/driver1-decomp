typedef struct { int x; char p0[4]; int z; char p1[0xC]; short r0, r1, r2; char p2[2]; short s20; char p3[2]; short s24; char p4[0x14]; int w3C; char p5[2]; short s42; char p6[0xC]; unsigned char sp; char p7[2]; unsigned char i53; char p8[2]; unsigned char t56; } A;
typedef struct { char p0[0x18]; int x; char p1[4]; int z; char p2[0x12]; unsigned char f36; char p3[0x39]; A *link; char p4[0x5C]; unsigned char cnt; } B;
typedef struct { char p[0xA]; unsigned char flag; char q[1]; } E;
typedef struct { short s, c; } SC;
extern int D_000976C0;
extern E *D_000976CC;
extern int D_000C0340[];
extern short rcossin_tbl[];
extern int ratan2();
extern void func_00061408();

void func_00060C9C(A *a, B *b)
{
    int dx, dz;
    int sp;
    int ang;

    dx = b->x - a->x;
    dz = b->z - a->z;
    sp = a->sp;
    if (b->link == a && (b->f36 & 3) == 1)
        func_00061408(b, a);
    if ((unsigned)(dx + 0x13) < 0x27 && dz < 0x14 && dz >= -0x13) {
        a->w3C = 0;
        a->r2 = 0;
        a->r1 = 0;
        a->r0 = 0;
        D_000C0340[--D_000976C0] = a->s42;
        if (a->t56 == 6) {
            E *e = &D_000976CC[a->i53];
            if (e->flag)
                e->flag = 0;
        }
        b->cnt--;
    }
    ang = -ratan2(dz, dx) - 0x400 & 0xFFF;
    a->r1 = ang;
    a->s20 = sp * rcossin_tbl[(a->r1 + 0x800 & 0xFFF) * 2] >> 12;
    a->s24 = sp * rcossin_tbl[(a->r1 + 0x800 & 0xFFF) * 2 + 1] >> 12;
}
