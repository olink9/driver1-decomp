typedef struct { int a; short b; char pad[0x3A]; unsigned char idx; unsigned char n; char f42; char f43; } E;
typedef struct { unsigned short s; char c2; char c3; void *p4; E e[15]; } H;
typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; char rest[0x20]; } POLY_FT4;
typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; char rest[0x10]; } POLY_F4;
extern H D_000EE930[];
extern char D_000ED87A;
extern unsigned char D_000EF9F0;
extern unsigned char D_000EF9F1;
extern unsigned char D_000EF9F2;
extern int D_000EF13C;
extern int D_000EF140;
extern POLY_FT4 D_000EF150[2][15];
extern POLY_F4 D_000EF600[2][15];

/* pulse the highlight colour and tint the save-slot boxes */
void func_000E950C(void)
{
    int port;
    int i;
    int start;
    int cnt;

    if (D_000ED87A == 0) {
        D_000EF9F0 += 8;
        D_000EF9F1 += 8;
        D_000EF9F2 += 8;
        if (D_000EF9F0 >= 0xF8)
            D_000ED87A = 1;
    } else {
        D_000EF9F0 -= 8;
        D_000EF9F1 -= 8;
        D_000EF9F2 -= 8;
        if (D_000EF9F0 < 0x11)
            D_000ED87A = 0;
    }
    start = D_000EE930[D_000EF13C].e[D_000EF140].idx;
    cnt = D_000EE930[D_000EF13C].e[D_000EF140].n;
    for (port = 0; port < 2; port++) {
        for (i = 0; i < 15; i++) {
            if (D_000EE930[D_000EF13C].s == 4 || port != D_000EF13C) {
                D_000EF150[port][i].r0 = 0x60;
                D_000EF150[port][i].g0 = 0x60;
                D_000EF150[port][i].b0 = 0x60;
                D_000EF600[port][i].r0 = 0;
                D_000EF600[port][i].g0 = 0x80;
                D_000EF600[port][i].b0 = 0x80;
            } else if (i >= start && i < start + cnt) {
                D_000EF150[port][i].r0 = D_000EF9F0;
                D_000EF150[port][i].g0 = D_000EF9F1;
                D_000EF150[port][i].b0 = D_000EF9F2;
                D_000EF600[port][i].r0 = 0;
                D_000EF600[port][i].g0 = D_000EF9F1;
                D_000EF600[port][i].b0 = D_000EF9F2;
            } else {
                D_000EF150[port][i].r0 = 0x60;
                D_000EF150[port][i].g0 = 0x60;
                D_000EF150[port][i].b0 = 0x60;
                D_000EF600[port][i].r0 = 0;
                D_000EF600[port][i].g0 = 0x80;
                D_000EF600[port][i].b0 = 0x80;
            }
        }
    }
}
