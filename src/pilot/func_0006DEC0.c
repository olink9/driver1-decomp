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
        setPolyGT4(&D_000C775C[i]);
        setSemiTrans(&D_000C775C[i], 1);
        setXYWH(&D_000C775C[i], -1, -1, 1, 1);
        D_000C775C[i].tpage = GetTPage(0, 2, 0, 0);
        setPolyG4(&D_000C77E4[i]);
        setPolyF4(&D_000C749C[i]);
        setPolyF4(&D_000C746C[i]);
        setXYWH(&D_000C749C[i], 0, 0, 512, 42);
        setXYWH(&D_000C746C[i], 0, 214, 512, 42);
        setRGB0(&D_000C749C[i], 0, 0, 0);
        setRGB0(&D_000C746C[i], 0, 0, 0);
    }
    D_00096F88 = 0xFF;
    D_00096F8C = 0;
}
