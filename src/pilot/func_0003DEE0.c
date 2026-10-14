/* OK_To_Zoom */
struct P { char pad[0xC8]; int x; int y; int z; };
struct V3 { int x, y, z; };
extern struct P D_0009B0A4;
extern long long D_000970E4;
extern int D_0008A8F0[];
extern int func_0005C458();
extern int func_0003C998();
#define SCRZ (*(int *)&D_000970E4)

int func_0003DEE0(void) {
    int old, r;
    struct V3 t;
    old = SCRZ;
    t.x = D_0009B0A4.x;
    t.y = -D_0009B0A4.y;
    t.z = D_0009B0A4.z;
    SCRZ = (func_0005C458(D_0008A8F0, &t) >> 4) + 0x100;
    if (SCRZ > 0x320) SCRZ = 0x320;
    if (SCRZ < 0x100) SCRZ = 0x100;
    r = func_0003C998() == 0;
    SCRZ = old;
    return r;
}
