#include "common.h"
#include "gte.h"

extern long long D_00096898;
extern VECTOR camera_position;
extern long long D_00097368, D_00097398, D_0009733C, D_00096A24;
extern long long D_00097128, D_00096AA8, gNight;
extern MATRIX D_000AF9C0;
extern MATRIX inv_camera_matrix;
extern MATRIX CompoundMatrix[];
extern MATRIX matrixtable[];
extern unsigned char D_000A80C4[];
extern int D_000C6F04[];
extern int func_00030198();
extern void func_00039D94();
extern void func_0002DBB4();
extern void func_0003A270();

void func_0002D7C4(int *ox, int *oz, int *col)
{
    int *scr = (int *)0x1F8001F0;
    unsigned char mode = *(unsigned char *)&D_00096898;
    int i;
    int cx, cz, q1, q2, rx, v;

    if (mode == 3 && (func_00030198() & 2)) {
        D_000C6F04[0x104 / 4] = 0xFF0;
        D_000C6F04[0x10C / 4] = mode;
    }
    i = 0;
    func_00039D94();
    cz = (camera_position.vz + *(int *)&D_00097368) / 3000;
    q1 = cz / *(int *)&D_0009733C;
    q2 = *(int *)&D_00096A24 / *(int *)&D_0009733C;
    cx = (camera_position.vx + *(int *)&D_00097398) / 3000;
    rx = cx / *(int *)&D_0009733C;
    func_0003A270((rx & 1) | ((q1 & 1) << 1), rx + q1 * q2,
                  (cz % *(int *)&D_0009733C) * *(int *)&D_0009733C + cx % *(int *)&D_0009733C,
                  D_000A80C4);
    *ox = cx - 9;
    *oz = cz - 9;
    for (; i < 0x40; i += 16)
        MulMatrix0(&inv_camera_matrix, &matrixtable[i], &CompoundMatrix[i]);
    v = *(int *)&D_00097128;
    *col = (v << 16) | (v << 8) | v;
    if (*(unsigned char *)&D_00096AA8 == 0 && *(int *)&gNight == 0)
        *scr = 0x2C808080;
    else
        *scr = *col | 0x2C000000;
    func_0002DBB4();
    *(MATRIX *)0x1F800000 = D_000AF9C0;
    gte_SetRotMatrix((MATRIX *)0x1F800000);
}
