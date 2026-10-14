/* Arpeggio (Driver 2) */
typedef struct { char p0[0x50]; short period; char p1[2]; unsigned char transpose; unsigned char note; short ownper; char p2[6]; unsigned short c2spd; } XMC2;
typedef struct { char p0[0x10]; unsigned short vbtick; char p1[0x1E]; int linear; } MS;
extern XMC2 *D_0009781C;
extern MS *D_00097814;
extern int func_0006F81C();
void func_000712E4(unsigned char dat)
{
    int note;
    short per; int n;
    unsigned short c;
    note = D_0009781C->note;
    if (dat != 0) {
        switch (D_00097814->vbtick % 3) {
        case 1: note += dat >> 4; break;
        case 2: note += dat & 0xf; break;
        }
        n = note + D_0009781C->transpose;
        c = D_0009781C->c2spd;
        if (D_00097814->linear == 1) {
            per = 0x1E00 - (short)c / 2;
            per -= (unsigned char)n * 64;
            per += 64;
        } else
            per = func_0006F81C((unsigned char)n, c);
        D_0009781C->period = per;
        D_0009781C->ownper = 1;
    }
}
