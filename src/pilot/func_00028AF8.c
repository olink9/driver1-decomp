/* Clips the screen-space edge a->b against x = -128 / 640 and appends the
   surviving vertices (with interpolated uv) to D_000A1BC8 (count D_00097158) */
typedef struct { short vx, vy; } DVECTOR;
typedef struct { unsigned char u, v; } UV;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { DVECTOR xy; UV uv; short pad; } CLIPPT;

extern DVECTOR *D_00096948;
extern UV *D_0009694C;
extern int D_00097158;
extern CLIPPT D_000A1BC8[];

extern int SquareRoot0(int a);

void func_00028AF8(int a, int b)
{
    char pad[16];
    SVECTOR d;
    SVECTOR duv;
    int len;
    int t;
    int f;

    if (D_00096948[a].vx < -128 && D_00096948[b].vx < -128)
        return;
    if (D_00096948[a].vx > 640 && D_00096948[b].vx > 640)
        return;

    d.vx = D_00096948[b].vx - D_00096948[a].vx;
    d.vy = D_00096948[b].vy - D_00096948[a].vy;
    len = SquareRoot0(d.vx * d.vx + d.vy * d.vy);
    if (len == 0)
        return;

    duv.vx = D_0009694C[b].u - D_0009694C[a].u;
    duv.vy = D_0009694C[b].v - D_0009694C[a].v;
    SquareRoot0(duv.vx * duv.vx + duv.vy * duv.vy);

    if (D_00096948[a].vx >= -128 && D_00096948[a].vx <= 640) {
        D_000A1BC8[D_00097158].xy = D_00096948[a];
        D_000A1BC8[D_00097158].uv = D_0009694C[a];
        D_00097158++;
    } else {
        if (D_00096948[a].vx < -128) {
            t = -D_00096948[a].vx - 128;
            D_000A1BC8[D_00097158].xy.vx = -128;
        } else {
            t = D_00096948[a].vx - 640;
            D_000A1BC8[D_00097158].xy.vx = 640;
        }
        f = (t << 12) / len;
        D_000A1BC8[D_00097158].xy.vy = (f * d.vy) / 4096 + D_00096948[a].vy;
        D_000A1BC8[D_00097158].uv.u = (f * duv.vx) / 4096 + D_0009694C[a].u;
        D_000A1BC8[D_00097158].uv.v = (f * duv.vy) / 4096 + D_0009694C[a].v;
        D_00097158++;
    }

    if (D_00096948[b].vx >= -128 && D_00096948[b].vx <= 640)
        return;

    if (D_00096948[b].vx < -128) {
        t = -D_00096948[b].vx - 128;
        D_000A1BC8[D_00097158].xy.vx = -128;
    } else {
        t = D_00096948[b].vx - 640;
        D_000A1BC8[D_00097158].xy.vx = 640;
    }
    f = -((t << 12) / len);
    D_000A1BC8[D_00097158].xy.vy = (f * d.vy) / 4096 + D_00096948[b].vy;
    D_000A1BC8[D_00097158].uv.u = (f * duv.vx) / 4096 + D_0009694C[b].u;
    D_000A1BC8[D_00097158].uv.v = (f * duv.vy) / 4096 + D_0009694C[b].v;
    D_00097158++;
}
