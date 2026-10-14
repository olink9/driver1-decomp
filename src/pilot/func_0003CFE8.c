/* follow-car camera (Driver 2 name unknown) */
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct {
    unsigned char id;
    char a[0x17];
    int px, py, pz;
    char b[0xC8 - 0x24];
    int c8;
    int cc;
    char c[0xF8 - 0xD0];
    int ang;
    char d[0x18C - 0xFC];
} CAR;

extern CAR D_0009B0A4[];
extern CAR D_0009D750[];
extern CAR *D_000973C0;
extern long long D_0009687C;
extern long long D_0009752C;
extern long long D_00096DA8;
extern int D_00096A74;
extern int D_00096A78;
extern short D_00096A7A;
extern int D_00096A7C;
extern int D_00096A58;
extern int D_00096A60;
extern short D_00096A68;
extern int D_00096A6C;
extern int D_000973C4, D_000973CC, D_000973BC;
extern short D_0008EAA8[];
extern VEC D_0008A8F0;
extern VEC D_0008A1DC;
typedef struct { short v; short pad[3]; } S8;
extern S8 D_00096C48, D_00096C4A, D_00096C4C;
extern long long D_000970E4, D_00096A48;
extern int func_0001F5B0();
extern void func_0004AD94();
extern int ratan2();
extern void func_00077D6C();
extern void func_0004EB7C();

#define US(x) (*(unsigned short *)&x)
#define I(x) (*(int *)&x)

void func_0003CFE8(int player)
{
    CAR *cp;
    int angle;
    CAR *p;
    char pad[32];

    cp = &D_0009B0A4[player];
    if (I(D_0009687C) == 0) {
        if ((US(D_0009752C) & 3) != 3) {
            if (US(D_0009752C) & 1) {
                if (D_00096A7C < 2)
                    D_00096A7C++;
                else
                    D_00096A74 = 0x4000000;
            } else if (US(D_0009752C) & 2) {
                if (D_00096A7C < 2)
                    D_00096A7C++;
                else
                    D_00096A74 = 0xFC000000;
            } else {
                D_00096A7C = 0;
                D_00096A74 = 0;
            }
        }
        D_00096A78 += (D_00096A74 - D_00096A78) >> 1;
    }

    D_00096C48.v = 25;
    angle = D_00096A58 - D_00096A7A;

    if (I(D_0009687C) == 0 && (US(D_0009752C) & 3) == 3) {
        angle = cp->ang & 0xFFF;
    } else {
        if (I(D_0009687C) == 0 || I(D_00096DA8) == 2)
            D_00096A58 = (D_00096A58 + ((((cp->ang + D_00096A6C - D_00096A58 + 0x800) & 0xFFF) - 0x800) / 8)) & 0xFFF;
        D_0008A8F0.vx = cp->px + D_0008EAA8[((angle + D_00096A68) & 0xFFF) * 2] * D_00096A60 / 4096;
        D_0008A8F0.vz = cp->pz + D_0008EAA8[((angle + D_00096A68) & 0xFFF) * 2 + 1] * D_00096A60 / 4096;
    }

    D_0008A8F0.vy = -cp->cc + D_0008A1DC.vy;
    D_000973C4 = D_0008A1DC.vy - func_0001F5B0(&D_0008A8F0) - 100;
    D_000973CC = func_0001F5B0(&cp->c8);
    D_000973BC = func_0001F5B0(&D_0008A8F0);
    if (D_0008A8F0.vy > D_000973C4) {
        if ((D_000973CC - D_000973BC >= 0 ? D_000973CC - D_000973BC : D_000973BC - D_000973CC) < 900) {
            D_00096C48.v = ((D_0008A8F0.vy - D_000973C4) >> 1) + 25;
            D_0008A8F0.vy = D_000973C4;
        }
    }

    D_000973C0 = D_0009D750;
    D_0009D750[0] = D_0009D750[-25];
    D_000973C0->id = 25;
    p = D_000973C0;
    p->ang = angle & 0xFFF;
    p->px = cp->px + ((D_00096A60 * D_0008EAA8[p->ang * 2] + 0x800 >> 12) / 2);
    p->py = -D_0008A8F0.vy;
    p->pz = cp->pz + ((D_00096A60 * D_0008EAA8[(p->ang & 0xFFF) * 2 + 1] + 0x800 >> 12) / 2);
    func_0004AD94(25);
    D_0008A8F0.vy = D_000973C0->py = -D_000973C0->py;
    D_000973C0->ang &= 0xFFF;
    D_0008A8F0.vx = cp->px + (D_00096A60 * D_0008EAA8[D_000973C0->ang * 2] + 0x800 >> 12);
    D_0008A8F0.vz = cp->pz + (D_00096A60 * D_0008EAA8[(D_000973C0->ang & 0xFFF) * 2 + 1] + 0x800 >> 12);
    D_00096C4A.v = -ratan2(cp->px - D_0008A8F0.vx, cp->pz - D_0008A8F0.vz) & 0xFFF;
    D_00096C4C.v = 0;
    func_00077D6C(0x100);
    I(D_000970E4) = 0x100;
    I(D_00096A48) = 10000;
    func_0004EB7C();
}
