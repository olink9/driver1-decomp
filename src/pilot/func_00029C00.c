/* unknown Driver 2 counterpart: per-player trail/skid ring buffer update */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { unsigned char v; char pad[7]; } B8;
typedef struct {
    unsigned char flag, pad[2], type;
    int p[4][3];
} TRAIL;

extern int D_000A5D7C[];
extern int D_000A1CCC[];
extern VECTOR D_000A20DC[];
extern VECTOR D_000A211C[];
extern TRAIL track_buffer[][64];
extern unsigned short *modelpointers[];
extern B8 D_00097028;
extern int D_000A294C[];
extern long long wetness;
extern short D_0008EAAA[];
extern int D_000A1C08[];
extern B8 D_0009701C;

extern int ROADS_GetRouteData();
extern void GetSmokeDrift();
extern void Setup_Smoke();
extern void func_00047FA4();
extern int ratan2();

void func_00029C00(int player)
{
    VECTOR v[4];
    VECTOR pos;
    VECTOR vel;
    char pad[16];
    short surf[4];
    VECTOR *a;
    VECTOR *b;
    TRAIL *t;
    int idx;
    int angle;
    int c, s;
    int dx, dz;

    idx = D_000A5D7C[player] + D_000A1CCC[player];
    a = &D_000A20DC[player];
    b = &D_000A211C[player];
    if (idx >= 64)
        idx -= 64;
    t = &track_buffer[player][idx];

    if (ROADS_GetRouteData(b->vx, b->vz, surf) != 0) {
        if (modelpointers[surf[0]][0] & 0x80)
            return;
        if (modelpointers[surf[0]][1] & 0x4000) {
            t->type = 2;
            D_00097028.v = 1;
        } else
            t->type = 1;
    } else
        t->type = 1;

    pos.vx = b->vx;
    pos.vy = -b->vy - 50;
    pos.vz = b->vz;

    if ((D_000A294C[player]++ & 3) == 1) {
        GetSmokeDrift(&vel);
        if (t->type == 2) {
            VECTOR v2 = {0, 50, 0};
            Setup_Smoke(&pos, 100, 500, 3, 0, &vel, 0);
            func_00047FA4(&pos, &v2, 5, 2);
        } else if (*(int *)&wetness == 0) {
            Setup_Smoke(&pos, 100, 500, 2, 0, &vel, 0);
        }
    }

    dx = a->vx - b->vx;
    dz = a->vz - b->vz;
    angle = -ratan2(dz, dx) & 0xfff;
    c = (D_0008EAAA[angle * 2] * 40) / 4096;
    s = (D_0008EAAA[angle * 2 - 1] * 40) / 4096;

    v[1].vx = v[0].vx = a->vx;
    v[1].vy = v[0].vy = a->vy - 10;
    v[1].vz = v[0].vz = a->vz;
    v[3].vx = v[2].vx = b->vx;
    v[3].vy = v[2].vy = b->vy - 10;
    v[3].vz = v[2].vz = b->vz;
    v[0].vx += s;
    v[0].vz += c;
    v[2].vx += s;
    v[2].vz += c;

    if (D_000A1CCC[player] != 64)
        D_000A1CCC[player]++;
    else {
        if (++D_000A5D7C[player] >= 64)
            D_000A5D7C[player] -= 64;
    }

    if (D_000A1C08[player] == 1 && D_0009701C.v == 1)
        t->flag = 1;
    else
        t->flag = 0;

    t->p[0][0] = v[0].vx;
    t->p[0][1] = v[0].vy;
    t->p[0][2] = v[0].vz;
    t->p[1][0] = v[1].vx;
    t->p[1][1] = v[1].vy;
    t->p[1][2] = v[1].vz;
    t->p[2][0] = v[2].vx;
    t->p[2][1] = v[2].vy;
    t->p[2][2] = v[2].vz;
    t->p[3][0] = v[3].vx;
    t->p[3][1] = v[3].vy;
    t->p[3][2] = v[3].vz;
    D_000A1C08[player] = 1;
}
