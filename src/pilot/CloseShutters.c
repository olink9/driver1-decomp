/* CloseShutters */
#include "common.h"
extern int current, D_00097524;
extern void DrawPrim(), func_0007457C(), SetDispMask();

void CloseShutters(int speed, int width, int height)
{
    POLY_F4 poly[2];
    int h2 = height;
    int h = 0;
    int done = 0;

    setPolyF4(&poly[0]);
    setPolyF4(&poly[1]);
    setRGB0(&poly[0], 0, 0, 0);
    setRGB0(&poly[1], 0, 0, 0);
    do {
        setXYWH(&poly[0], 0, 0, width, h);
        setXYWH(&poly[1], 0, h2 - h, width, h);
        DrawPrim(&poly[0]);
        DrawPrim(&poly[1]);
        h += speed;
        if (h >= 256) done = 1;
    } while (!done);
    func_0007457C(*(int *)(current + 4), 0x1080);
    func_0007457C(*(int *)(D_00097524 + 4), 0x1080);
    SetDispMask(0);
}
