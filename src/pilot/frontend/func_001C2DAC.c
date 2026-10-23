/* func_001C2DAC. FAKEMATCH: the empty volatile asm at the top is a sched2 barrier that keeps the four prologue saves
   above the lui/lw of D_001CDEC8 (no C form found: volatile, [], struct, switch, inline helper, no param all tried). */
typedef struct { short x, y, w, h; } RECT16;
typedef struct {
    RECT16 clip;
    short ofs[2];
    RECT16 tw;
    unsigned short tpage;
    unsigned char dtd, dfe, isbg, r0, g0, b0;
    unsigned long dr_env[16];
} DRAWENV;
typedef struct {
    RECT16 disp;
    RECT16 screen;
    unsigned char isinter, isrgb24, pad0, pad1;
} DISPENV;
typedef struct {
    char *primptr;
    unsigned long *ot;
    unsigned long *f8;
    unsigned long fC;
    DRAWENV draw;
    DISPENV disp;
} DB;
extern short D_001CDE98[], D_001CDEA8[];
extern int D_001CDEC8, D_001CDEC0, D_001CDEC4, D_001CDEB8, D_001CDEBC;
extern char D_000BDFB4[];
extern DB *current;
extern DB *D_00097524;
extern void func_00079800(), func_0007D66C(), func_0007D720(), PutDispEnv(), PutDrawEnv();

void func_001C2DAC(int unused)
{
    short *t;
    int mode;
    char *p;
    DB *db;

    __asm__ volatile(""); /* FAKEMATCH: sched barrier */
    if (D_001CDEC8 == 1) {
        t = D_001CDE98;
        mode = 1;
        D_001CDEC0 = 50;
        D_001CDEC4 = 302;
    } else {
        t = D_001CDEA8;
        mode = 0;
        D_001CDEC0 = 60;
        D_001CDEC4 = 262;
    }
    func_00079800(mode);
    p = D_000BDFB4;
    func_0007D66C(p, t[0], t[1], t[4], t[5]);
    func_0007D720(p + 0x5C, t[0], t[1], t[4], t[5]);
    func_0007D66C(p + 0x80, t[0], t[1], t[4], t[5]);
    func_0007D720(p + 0xDC, t[0], t[1], t[4], t[5]);
    D_001CDEB8 = t[4];
    D_001CDEBC = t[5];
    db = (DB *)(p - 0x10);
    db[0].draw.isbg = 0;
    db[0].disp.isinter = 1;
    db[0].draw.dfe = 0;
    db[0].disp.screen.x = t[6];
    db[0].disp.screen.y = t[7];
    db[0].disp.screen.h = t[5];
    current = &db[0];
    db[0].f8 = (unsigned long *)0xD8600;
    db[0].primptr = (char *)0xD8600;
    db[0].ot = (unsigned long *)0xD0200;
    db[1].draw.isbg = 0;
    db[1].disp.isinter = 1;
    db[1].draw.dfe = 0;
    db[1].disp.screen.x = t[6];
    D_00097524 = &db[1];
    db[1].disp.screen.y = t[7];
    db[1].disp.screen.h = t[5];
    db[1].f8 = (unsigned long *)0xF6600;
    db[1].primptr = (char *)0xF6600;
    db[1].ot = (unsigned long *)0xD4400;
    PutDispEnv(&db[0].disp);
    PutDrawEnv(&current->draw);
}
