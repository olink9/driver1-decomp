/* WIP 99.49: only one swapped pair after the first sprintf - target issues `li v0,1` (hires/clear = 1) before
 * `addiu s1,s2,%lo(D_80112868)` (the movie struct base); this C emits them the other way round. Tried: all 120
 * orders of the five movie-field stores, a MOVIE *m local (top / before use), array decl, volatile struct, switch
 * for 70/71, a temp for D_80126758, chained `hires = clear = 1`. Branch layout (call written in both arms) is right. */
#include "common.h"
/* PsyQ 4.x DRAWENV (0x5C bytes): PsyCross's DRAWENV has an extra `drt` byte */
typedef struct {
    RECT16 clip;
    short ofs[2];
    RECT16 tw;
    u_short tpage;
    u_char dtd, dfe, isbg, r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV1;
typedef struct {
    DRAWENV1 draw;
    DISPENV disp;
} DB;
typedef struct {
    char *name;         /* 0x00 */
    short hires;        /* 0x04 */
    short clear;        /* 0x06 */
    u_short dispw;      /* 0x08 */
    u_short x, y;       /* 0x0A */
    u_short w, h;       /* 0x0E */
    int endFrame;       /* 0x14 */
    int vlcsize;        /* 0x18 */
    short vol;          /* 0x1C */
} MOVIE;
extern DB D_8012DB48[2];
extern short D_80126750;
extern int D_80126758;
extern int D_80126778[];
extern MOVIE D_80112868;
extern void func_80100E4C(), func_801011C4(), func_80100EA8(DB *), func_80100C80(), func_801010B4();
extern short func_8010127C(MOVIE *m, int (*cb)());
extern int func_80100DF8();
extern int sprintf(char *, const char *, ...);

int main(int argc, int movie)
{
    char buf[40];

    func_80100E4C();
    func_801011C4();
    func_80100EA8(D_8012DB48);
    func_80100C80();
    PutDispEnv(&D_8012DB48[D_80126750].disp);
    SetDispMask(1);
    D_80126758 = movie;
    D_80126750 ^= 1;
    sprintf(buf, "\\DRIVER\\NFMV\\%d\\RENDER%d.STR;1", movie / 10, movie);
    D_80112868.name = buf;
    D_80112868.hires = 1;
    D_80112868.clear = 1;
    D_80112868.vol = 127;
    D_80112868.endFrame = D_80126778[D_80126758] - 3;
    if (D_80126758 != 70 && D_80126758 != 71)
        func_8010127C(&D_80112868, func_80100DF8);
    else {
        sprintf(buf, "\\DRIVER\\NFMV\\7\\RENDER73.STR;1");
        D_80112868.name = buf;
        D_80112868.endFrame = D_80126778[73] - 3;
        func_8010127C(&D_80112868, func_80100DF8);
    }
    DrawSync(0);
    func_801010B4();
    return 1;
}
