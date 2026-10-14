#include "common.h"
extern POLY_GT4 D_000C775C[];
extern POLY_G4 D_000C77E4[];
extern POLY_F4 D_000C749C[];
extern POLY_F4 D_000C746C[];
extern int D_00096F88;
extern char D_00096F8C;

void func_0006DEC0(void) {
    int i;
    for (i = 0; i < 2; i++) {
        setlen(&D_000C775C[i], 12);
        D_000C775C[i].code = 0x3E;
        D_000C775C[i].x0 = -1; D_000C775C[i].y0 = -1;
        D_000C775C[i].x1 = 0; D_000C775C[i].y1 = -1;
        D_000C775C[i].x2 = -1; D_000C775C[i].y2 = 0;
        D_000C775C[i].x3 = 0; D_000C775C[i].y3 = 0;
        D_000C775C[i].tpage = GetTPage(0, 2, 0, 0);
        setlen(&D_000C77E4[i], 8);
        D_000C77E4[i].code = 0x38;
        setlen(&D_000C749C[i], 5);
        D_000C749C[i].code = 0x28;
        setlen(&D_000C746C[i], 5);
        D_000C746C[i].code = 0x28;
        D_000C749C[i].y2 = 0x2A;
        D_000C749C[i].y3 = 0x2A;
        D_000C749C[i].x0 = 0; D_000C749C[i].y0 = 0;
        D_000C749C[i].x1 = 0x200; D_000C749C[i].y1 = 0;
        D_000C749C[i].x2 = 0; D_000C749C[i].x3 = 0x200;
        D_000C746C[i].y0 = 0xD6; D_000C746C[i].y1 = 0xD6;
        D_000C746C[i].x0 = 0; D_000C746C[i].x1 = 0x200;
        D_000C746C[i].x2 = 0; D_000C746C[i].y2 = 0x100;
        D_000C746C[i].x3 = 0x200; D_000C746C[i].y3 = 0x100;
        D_000C749C[i].r0 = 0; D_000C749C[i].g0 = 0; D_000C749C[i].b0 = 0;
        D_000C746C[i].r0 = 0; D_000C746C[i].g0 = 0; D_000C746C[i].b0 = 0;
    }
    D_00096F88 = 0xFF;
    D_00096F8C = 0;
}
