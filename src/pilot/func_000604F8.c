typedef struct {
    int x0, x4, x8, xC, x10, x14;
    short h18, h1A, h1C, h1E, h20, h22, h24, h26;
    int x28, x2C, x30;
    char pad[0x50 - 0x34];
    unsigned char c50;
} OBJ;
extern short D_0008EAA8[];
extern int ratan2();

void func_000604F8(OBJ *o) {
    int dz;
    int r = o->c50;
    o->xC = o->x28 - o->x0;
    o->x10 = o->x2C - o->x4;
    o->x14 = dz = o->x30 - o->x8;
    o->h18 = 0;
    o->h1C = 0;
    o->h1A = -ratan2(dz, o->xC) - 0x400;
    o->h1A &= 0xfff;
    o->h20 = (r * D_0008EAA8[((o->h1A + 0x800) & 0xfff) * 2]) >> 12;
    o->h24 = (r * D_0008EAA8[((o->h1A + 0x800) & 0xfff) * 2 + 1]) >> 12;
}
