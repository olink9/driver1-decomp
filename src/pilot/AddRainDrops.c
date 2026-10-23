/* AddRainDrops (Driver 2 name) */
#include "gte.h"
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { VEC position; short oldx; short index; } RAIN_TYPE;
typedef struct { short type; short a, b, c; } ROUTE_DATA;
typedef struct { short flags; unsigned short flags2; } MODEL;
extern long long pauseflag;
extern unsigned char D_00096AA8, D_00096AA9;
extern int D_00097474, wetness, D_00097428, D_000973FC;
extern short D_000B8028[];
extern RAIN_TYPE D_000B81C8[];
extern int D_000BBEA4[];
extern int camera_position[];
extern MODEL *modelpointers[];
extern void ROADS_GetRouteData();

static inline int AllocateRainDrop(void)
{
    if (D_00097428 < 180)
        return D_000B8028[D_00097428++];
    return -1;
}

static inline void ReleaseRainDrop(int RainIndex)
{
    D_000B8028[--D_00097428] = RainIndex;
}

void AddRainDrops(void)
{
    SVEC v;
    ROUTE_DATA routeData;
    RAIN_TYPE *rt;
    int RainIndex;
    int count;
    int first;

    first = 1;
    if (*(int *)&pauseflag != 0)
        return;
    D_00097474 = 0;
    if (D_00096AA8 != 0)
        count = D_00096AA8;
    else
        count = D_00096AA9;

    wetness += 5;
    if (wetness > 7000)
        wetness = 7000;

    gte_SetRotMatrix(D_000BBEA4);
    gte_SetTransVec0(camera_position);

    while (count-- != 0) {
        RainIndex = AllocateRainDrop();

        if (RainIndex < 0)
            return;

        rt = &D_000B81C8[RainIndex];
        D_000973FC = D_000973FC * 0x19660D + 0x3C6EF35F;
        v.vz = ((D_000973FC >> 20) & 511) + 200;
        D_000973FC = D_000973FC * 0x19660D + 0x3C6EF35F;
        v.vy = -((D_000973FC >> 20) & 511);
        D_000973FC = D_000973FC * 0x19660D + 0x3C6EF35F;
        v.vx = ((D_000973FC >> 20) & 511) - 256;

        if (v.vz > 255) {
            int depth;
            depth = (unsigned char)(v.vz >> 8) + 1;
            v.vy *= depth;
            v.vx *= depth;
        }

        gte_ldv0(&v);
        gte_rtv0tr();

        rt->position.pad = 0;
        rt->index = RainIndex;
        gte_stlvnl(&rt->position);

        if (first) {
            ROADS_GetRouteData(rt->position.vx, rt->position.vz, &routeData);
            if (modelpointers[routeData.type]->flags2 & 0x100) {
                D_00097474 = 1;
                ReleaseRainDrop(RainIndex);
                return;
            }
        }
        first = 0;
    }
}
