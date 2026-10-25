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
extern int D_80126770;

void func_80100EA8(DB *db)
{
    SetDefDrawEnv((DRAWENV *)&db[0].draw, 0, 0, 512, 240);
    SetDefDispEnv(&db[0].disp, 0, 240, 512, 240);
    SetDefDrawEnv((DRAWENV *)&db[1].draw, 0, 240, 512, 240);
    SetDefDispEnv(&db[1].disp, 0, 0, 512, 240);
    db[0].draw.b0 = 50;
    db[1].draw.b0 = 50;
    setRECT(&db[0].disp.screen, 0, 0, 0, 240);
    setRECT(&db[1].disp.screen, 0, 0, 0, 240);
    db[0].draw.r0 = 0;
    db[0].draw.g0 = 0;
    db[1].draw.r0 = 0;
    db[1].draw.g0 = 0;
    db[0].draw.isbg = 1;
    db[1].draw.isbg = 1;
    FntLoad(960, 256);
    SetDumpFnt(D_80126770 = FntOpen(18, 16, 310, 200, 0, 512));
}
