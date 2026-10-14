/* func_0006B4AC - no Driver 2 counterpart: mission HUD (timers/messages). WIP 97.99: case 0x16 stack layout (t should be at sp+0x20, inline buf at 0x50 - temp-slot order), andi/li order after sprintf, sll delay slot before bltz */
typedef struct { int type; char *s1; char *s2; int pad; } MSG;
typedef struct { int f[12]; } TIMER;
typedef struct {
    char p0[0xC8];
    TIMER t;
    char p1[0x104 - 0xF8];
    int f104;
    int f108;
    int f10C;
    int f110;
    int f114;
    char p2[0x124 - 0x118];
    char *f124;
    char *f128;
    char p3[0x2E1 - 0x12C];
    unsigned char b2E1;
    unsigned char b2E2;
} GS;

extern long long D_000967F4;
extern long long D_00096F80;
extern long long D_0009687C;
extern int D_000C7040[];
extern int D_000C71A0[];
extern GS D_000C6F04;
extern MSG D_0008BD58[];
extern char D_0008C228[];
extern char D_0008C288[];
extern char D_00096E80[];
extern char D_00096ED4[];
extern char D_00096EE8[];
extern char D_00096EF0[];
extern char D_00096EF4[];
extern char D_00096EF8[];
extern int sprintf();
extern void func_0005C7D0();
extern void func_0005C8F4();
extern int func_0005CC68(char *string, short x, short y);
extern int func_0005CF10(short x, short y, char *string);
extern unsigned char func_0006ACC8();
extern void func_0006AF48();
extern int func_0006B450();

static inline void ShowCount(int v)
{
    char buf[8];
    int w;

    w = func_0005CC68(D_00096E80, 0x171, 0xD);
    func_0006ACC8(buf, v, 1);
    func_0005CC68(buf, w, 0xD);
}

static inline void ShowNum(int n)
{
    char buf[8];

    sprintf(buf, D_00096ED4, n);
    func_0005C8F4(buf, 0x32);
}

static inline void ShowLap(int v)
{
    char buf[8];
    int w;
    unsigned char r;

    func_0005C7D0(0x80, 0x80, 0x80);
    r = func_0006ACC8(buf, v, 0);
    w = func_0005CF10(0xBB, 0x18, buf);
    sprintf(buf, D_00096EE8, r);
    func_0005CC68(buf, w, 0x25);
    func_0005C7D0(0xFF, 0xFF, 0x40);
}

static inline void ShowTime(char *tbl, int n, int *v)
{
    char buf[8];
    int w;
    unsigned char len;

    func_0006B450(tbl, n);
    func_0005C7D0(0x80, 0x80, 0x80);
    len = sprintf(buf, D_00096EF0, v[0]);
    w = func_0005CF10((short)(0x197 + (1 - len) * 26), 0x18, buf);
    func_0005C7D0(0x80, 0x80, 0x80);
    if (v[1] != 0x7FFFFFFF)
        sprintf(buf, D_00096EF4, v[1]);
    else
        sprintf(buf, D_00096EF8);
    func_0005CC68(buf, w, 0x25);
}

void func_0006B4AC(GS *gs)
{
    char pad[16];
    int snd;
    int w;
    unsigned char len;
    char *s;

    snd = -1;
    func_0005C7D0(0xFF, 0xFF, 0x40);
    if (*(int *)&D_000967F4 == 0x11) {
        func_0006AF48(&gs->t.f[2], 0x171, 0x18);
    } else if (*(int *)&D_000967F4 == 0x12) {
        ShowTime(D_0008C228, 2, &gs->t.f[2]);
    } else if (*(int *)&D_000967F4 == 0x16) {
        int a = gs->t.f[7], b = gs->t.f[6], c = gs->t.f[8];
        TIMER t;
        t.f[8] = c;
        t.f[2] = a;
        t.f[3] = b;
        ShowTime(D_0008C288, 6, &t.f[2]);
    } else if (*(int *)&D_000967F4 == 0x13) {
        ShowCount(gs->t.f[8]);
    }
    if (gs->f128)
        func_0005CC68(gs->f128, 0x10, 0xC8);
    if (gs->b2E2)
        ShowNum(gs->b2E1);
    if ((D_000C71A0[0] & 4) && *(int *)&D_00096F80 == 5)
        ShowLap(D_000C71A0[1]);
    if (gs->f104 == 0xFF0 || gs->f104 == 0x7FFF) {
        gs->f114 = 1;
        switch (gs->f10C) {
        case 0: break;
        case 1: snd = 5; break;
        case 2: snd = 0x15; break;
        case 3: snd = 0x16; break;
        case 5: snd = 0x40; break;
        case 6: snd = 0x41; break;
        }
    } else if (D_000C7040[0] & 1) {
        if (D_000C7040[0] & 2)
            snd = 7;
        else if (D_000C7040[0] & 4)
            snd = 0x14;
        else if (D_000C7040[0] & 8)
            snd = 0x24;
    }
    if (D_000C6F04.f114) {
        if (snd >= 0) {
            if ((s = D_0008BD58[snd].s1) != 0 || (s = D_0008BD58[snd].s2) != 0) {
                if (*(int *)&D_0009687C == 0)
                    func_0005C8F4(s, 0x54);
            }
        } else if ((s = D_000C6F04.f124) != 0 && *(int *)&D_0009687C != 1) {
            func_0005C8F4(s, 0x54);
        }
    } else {
        if (*(int *)&D_00096F80 == 2)
            *(int *)&D_00096F80 = 3;
        D_000C6F04.f124 = 0;
    }
}
