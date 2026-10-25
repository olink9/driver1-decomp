/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * "Options:" menu func_001C3530 + handler table D_001CDF48, controller screen (GFX\PAD.TIM), centre screen
 * ("Center Screen", SC1/SC2), FRNT.B%d page loader. START: text pad + .rodata pad 0x1C025C. END: .rodata pad
 * 0x1C031C. Font/MCARD.BIN/FRNT.B%d literals duplicate fecore's -> separate object. .rodata one block 0x1C02A0.
 */
/* options.c: 16 functions func_001C3530..func_001C3FD0 (0x1C3530-0x1C41D8)
 * Step 3: types from include/frontend.h: this file sees `current`/D_00097524 as DB_ENV * (it moves the display
 * envs), D_001CFDF0 as TP_SHEET[], MENU tables. D_00097538 is plain, func_001C37BC's wait loop reads it through a
 * volatile cast. NAME12/SCR/LIM/COUNTS are file-local. No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { char s[12]; } NAME12;
typedef struct { char pad[0xC]; short x, y; } SCR;
typedef struct { short xmin, ymin, xmax, ymax; } LIM;
typedef struct { int n[3]; } COUNTS;

/* ---- externs ---- */
extern int D_001CDF88;
extern MENU_ITEM D_001CDF48[];
extern MENU_TEXT D_001CDEE8[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern char D_001CDEFC;
extern int func_001C6748();
extern char D_001CDF0C;
extern int func_001C6438();
extern char D_001CDF1C;
extern char D_001CDF2C;
extern unsigned short D_00097538;
extern void memset();
extern void func_0004E170();
extern void func_0004E348();
extern void SetDispMask();
extern void func_00051374();
extern void DrawSync();
extern void VSync();
extern int Loadfile();
extern void func_00074404();
extern void func_000E6D60();
extern void func_001C1B4C();
extern DB_ENV *current;
extern DB_ENV *D_00097524;
extern short D_0009752C;
extern int D_001CDEC8;
extern LIM D_001CDFAC[];
extern SCR D_0008AB8C;
extern SCR D_0008AB9C;
extern char D_001C0260[];
extern void func_00051540();
extern void func_000EC550();
extern void func_0007457C();
extern void func_001C334C();
extern void func_0005C8F4();
extern void func_001C2DAC();
extern int func_0007A7F4();
extern TP_SHEET D_001CFDF0[];
extern COUNTS D_001C0300;
extern char D_001C030C[];
extern int sprintf();
extern int LoadfileSeg();

/* ---- functions defined in this file ---- */
int func_001C3530();
void func_001C35F4();
void func_001C3604();
int func_001C3610();
void func_001C3668();
void func_001C3678();
int func_001C3684();
void func_001C36DC();
void func_001C36EC();
int func_001C36F8();
void func_001C374C();
void func_001C375C();
int func_001C3768();
void func_001C37BC();
int func_001C3988();
void func_001C3FD0();

/* ---------------- func_001C3530 (src/pilot/frontend/func_001C3530.c) ---------------- */
int func_001C3530(void)
{
    int r;
    D_001CDF88 = 0;
    func_001C2644(D_001CDF48, 2, 0);
    func_001C2114(0, D_001CDEE8, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CDF48, &D_001CDF88);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CDEE8);
        func_001C11E4();
    }
    func_001C2114(0, D_001CDEE8, 0);
    return r;
}

/* ---------------- func_001C35F4 (src/pilot/frontend/func_001C35F4.c) ---------------- */
void func_001C35F4(void) { D_001CDEFC = 1; }

/* ---------------- func_001C3604 (src/pilot/frontend/func_001C3604.c) ---------------- */
void func_001C3604(void) { D_001CDEFC = 0; }

/* ---------------- func_001C3610 (src/pilot/frontend/func_001C3610.c) ---------------- */
int func_001C3610(void){ int r; func_001C2114(0, D_001CDEE8, 0); r = func_001C6748(); func_001C2114(0, D_001CDEE8, 1); return r; }

/* ---------------- func_001C3668 (src/pilot/frontend/func_001C3668.c) ---------------- */
void func_001C3668(void) { D_001CDF0C = 1; }

/* ---------------- func_001C3678 (src/pilot/frontend/func_001C3678.c) ---------------- */
void func_001C3678(void) { D_001CDF0C = 0; }

/* ---------------- func_001C3684 (src/pilot/frontend/func_001C3684.c) ---------------- */
int func_001C3684(void){ int r; func_001C2114(0, D_001CDEE8, 0); r = func_001C6438(); func_001C2114(0, D_001CDEE8, 1); return r; }

/* ---------------- func_001C36DC (src/pilot/frontend/func_001C36DC.c) ---------------- */
void func_001C36DC(void) { D_001CDF1C = 1; }

/* ---------------- func_001C36EC (src/pilot/frontend/func_001C36EC.c) ---------------- */
void func_001C36EC(void) { D_001CDF1C = 0; }

/* ---------------- func_001C36F8 (src/pilot/frontend/func_001C36F8.c) ---------------- */
int func_001C36F8(void){ func_001C2114(0, D_001CDEE8, 0); func_001C37BC(); func_001C2114(0, D_001CDEE8, 1); return 0; }

/* ---------------- func_001C374C (src/pilot/frontend/func_001C374C.c) ---------------- */
void func_001C374C(void) { D_001CDF2C = 1; }

/* ---------------- func_001C375C (src/pilot/frontend/func_001C375C.c) ---------------- */
void func_001C375C(void) { D_001CDF2C = 0; }

/* ---------------- func_001C3768 (src/pilot/frontend/func_001C3768.c) ---------------- */
int func_001C3768(void){ func_001C2114(0, D_001CDEE8, 0); func_001C3988(); func_001C2114(0, D_001CDEE8, 1); return 0; }

/* ---------------- func_001C37BC (src/pilot/frontend/func_001C37BC.c) ---------------- */
void func_001C37BC(void)
{
    RECT rect;
    NAME12 name;
    int buf;
    int i;

    name = *(NAME12 *)"GFX\\PAD.TIM";
    i = 0;
    memset(&buf, i, 4);
    func_0004E170();
    func_0004E348();
    rect.x = 0;
    rect.y = 0;
    rect.w = 320;
    rect.h = 480;
    SetDispMask(i);
    func_00051374();
    DrawSync(i);
    VSync(i);
    Loadfile(&name, 0xD0200);
    func_00074404(&rect, 0xD0214);
    DrawSync(i);
    func_000514E0();
    do {
        if (i == 2)
            SetDispMask(1);
        func_000514E0();
        i++;
    } while (!(*(volatile unsigned short *)&D_00097538 & 0x40));
    SetDispMask(0);
    Loadfile("MCARD.BIN", 0xE6600);
    func_000E6D60("gfx\\font1.bnk", "DRFONT1", 0, -2);
    func_000E6D60("gfx\\font1.bnk", "DRFONT2", 1, -2);
    func_000E6D60("gfx\\font3.bnk", "DRFONT3", 2, -2);
    func_000E6D60("gfx\\font3.bnk", "DRFONT4", 3, -2);
    func_001C1B4C();
    func_00051374();
    DrawSync(0);
    VSync(0);
}

/* ---------------- func_001C3988 (src/pilot/frontend/func_001C3988.c) ---------------- */
/* screen position adjust: d-pad moves both display envs within D_001CDFAC[mode] limits, X keeps, triangle restores */
int func_001C3988(void)
{
    POLY_FT4 a[2][2];
    TEX tex;
    POLY_FT4 *p;
    POLY_FT4 *q;
    int i;
    int done = 0;

    func_00051540();
    func_000EC550(0x15, 0xE);
    func_000EC550(0x16, 0xF);
    func_0007457C(current->ot, 0x1080);
    func_0007457C(D_00097524->ot, 0x1080);
    for (i = 0; i < 2; i++) {
        func_001C334C("SC1", &tex);
        ((P_TAG *)&a[0][i])->len = 9;
        a[0][i].code = 0x2C;
        a[0][i].r0 = 128; a[0][i].g0 = 128; a[0][i].b0 = 128;
        a[0][i].code &= ~2;
        a[0][i].code |= 1;
        a[0][i].x0 = 256; a[0][i].y0 = 0;
        a[0][i].x1 = 512; a[0][i].y1 = 0;
        a[0][i].x2 = 256; a[0][i].y2 = 240;
        a[0][i].x3 = 512; a[0][i].y3 = 240;
        a[0][i].tpage = 24;
        a[0][i].clut = func_0007A7F4(960, tex.clut + 256);
        a[0][i].u0 = tex.u0; a[0][i].v0 = tex.v0; a[0][i].u1 = tex.u1; a[0][i].v1 = tex.v1;
        a[0][i].u2 = tex.u2; a[0][i].v2 = tex.v2; a[0][i].u3 = tex.u3; a[0][i].v3 = tex.v3;
    }
    for (i = 0; i < 2; i++) {
        func_001C334C("SC2", &tex);
        q = (POLY_FT4 *)(i * sizeof(POLY_FT4) + (int)a);
        p = q + 2;
        p->code = 0x2C;
        ((P_TAG *)p)->len = 9;
        p->r0 = 128; p->g0 = 128; p->b0 = 128;
        p->code &= ~2;
        p->code |= 1;
        p->x0 = 0; p->y0 = 0;
        p->x1 = 256; p->y1 = 0;
        p->x2 = 0; p->y2 = 240;
        p->x3 = 256; p->y3 = 240;
        a[1][i].tpage = 25;
        p->clut = func_0007A7F4(960, tex.clut + 256);
        p->u0 = tex.u0; p->v0 = tex.v0; p->u1 = tex.u1; p->v1 = tex.v1;
        p->u2 = tex.u2; p->v2 = tex.v2; p->u3 = tex.u3; p->v3 = tex.v3;
    }
    SetDispMask(1);
    while (!done) {
        func_000514E0();
        if (D_0009752C & 0x8000) {
            if (current->disp.screen.x > D_001CDFAC[D_001CDEC8].xmin) {
                current->disp.screen.x--;
                D_00097524->disp.screen.x--;
            }
        } else if ((unsigned short)D_0009752C & 0x2000) {
            if (current->disp.screen.x < D_001CDFAC[D_001CDEC8].xmax) {
                current->disp.screen.x++;
                D_00097524->disp.screen.x++;
            }
        } else if ((unsigned short)D_0009752C & 0x1000) {
            if (current->disp.screen.y > D_001CDFAC[D_001CDEC8].ymin) {
                current->disp.screen.y--;
                D_00097524->disp.screen.y--;
            }
        } else if ((unsigned short)D_0009752C & 0x4000) {
            if (current->disp.screen.y < D_001CDFAC[D_001CDEC8].ymax) {
                current->disp.screen.y++;
                D_00097524->disp.screen.y++;
            }
        } else if (D_00097538 & 0x40) {
            D_0008AB8C.x = current->disp.screen.x;
            D_0008AB8C.y = current->disp.screen.y;
            D_0008AB9C.x = current->disp.screen.x;
            D_0008AB9C.y = current->disp.screen.y;
            break;
        } else if (D_00097538 & 0x10) {
            if (D_001CDEC8) {
                current->disp.screen.x = D_0008AB8C.x;
                current->disp.screen.y = D_0008AB8C.y;
                D_00097524->disp.screen.x = D_0008AB8C.x;
                D_00097524->disp.screen.y = D_0008AB8C.y;
            } else {
                current->disp.screen.x = D_0008AB9C.x;
                current->disp.screen.y = D_0008AB9C.y;
                D_00097524->disp.screen.x = D_0008AB9C.x;
                D_00097524->disp.screen.y = D_0008AB9C.y;
            }
            break;
        }
        ((P_TAG *)&a[0][current->id])->addr = ((P_TAG *)&current->ot[1])->addr;
        ((P_TAG *)&current->ot[1])->addr = (unsigned)&a[0][current->id];
        ((P_TAG *)&a[1][current->id])->addr = ((P_TAG *)&current->ot[1])->addr;
        ((P_TAG *)&current->ot[1])->addr = (unsigned)&a[1][current->id];
        func_0005C8F4(D_001C0260, 120);
        func_001C11E4();
    }
    func_0007457C(current->ot, 0x1080);
    func_0007457C(D_00097524->ot, 0x1080);
    DrawSync(0);
    SetDispMask(0);
    func_001C2DAC(0x280);
    func_001C3FD0(0, 0);
    func_001C1B4C();
    return 0;
}

/* ---------------- func_001C3FD0 (src/pilot/frontend/func_001C3FD0.c) ---------------- */
/* FAKEMATCH: unused pad[8] only places the locals (target: r@0x10, counts@0x20, name@0x30). k++ is written in each arm to give k its s-reg priority over sector. */
void func_001C3FD0(void)
{
    RECT r;
    char pad[8]; /* FAKEMATCH: frame layout */
    COUNTS counts;
    char name[16];
    int file;
    int j;
    int k;
    int sector;
    short tp;
    unsigned char *buf;

    counts = D_001C0300;
    k = 0;
    r.w = 0x40;
    r.h = 0x100;
    for (file = 0; file < 2; file++) {
        sprintf(name, D_001C030C, file + 1);
        LoadfileSeg(name, (char *)0xD8600, 4, 0x8000);
        buf = (unsigned char *)0xD8600;
        sector = *buf;
        for (j = 0; j < counts.n[file] - 1; j++) {
            if (file == 1 && (j < 3 || j > 6)) {
                sector += 0x8000;
                k++;
            } else {
                LoadfileSeg(name, (char *)0xD8600, sector, 0x8000);
                r.x = (k % 11) * 64 + 0x140;
                r.y = (k / 11) << 8;
                tp = ((r.y & 0x100) >> 4) | ((r.x & 0x3ff) >> 6) | ((r.y & 0x200) << 2);
                D_001CFDF0[k].tpage = tp;
                D_001CFDF0[k].slot = k;
                D_001CFDF0[k].used = 1;
                func_00074404(&r, buf);
                sector += 0x8000;
                k++;
            }
        }
    }
}

