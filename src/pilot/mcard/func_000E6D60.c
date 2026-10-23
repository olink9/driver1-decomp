typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
} TEX;
typedef struct {
    char b0, b1;
    unsigned char u0, v0;
    unsigned short tpage, clut;
    unsigned char b8, b9;
    unsigned char u1, v1;
    unsigned short tpage1, clut1;
    short n;
    short val;
    unsigned char col[256][4];
} SPR;
typedef struct { short n; short pad; unsigned char col[256][4]; } PAL;
extern SPR D_000ED8B0[];
extern char D_000E6600[];
extern void Loadfile();
extern void func_001C334C();
extern int sprintf();

void func_000E6D60(char *file, char *name, unsigned char idx, short val)
{
    TEX tex;
    char buf[8];
    PAL *pal;
    int i;

    Loadfile(file, (char *)0xD0200);
    func_001C334C(name, &tex);
    D_000ED8B0[idx].u0 = tex.u0;
    D_000ED8B0[idx].v0 = tex.v0;
    D_000ED8B0[idx].tpage = tex.tpage;
    D_000ED8B0[idx].clut = tex.clut;
    D_000ED8B0[idx].val = val;
    pal = (PAL *)0xD0200;
    if (idx < 2) {
        D_000ED8B0[idx].b1 = 0x7F;
        D_000ED8B0[idx].b8 = 0x80;
        D_000ED8B0[idx].b0 = 0;
        D_000ED8B0[idx].b9 = 0xFF;
        sprintf(buf, D_000E6600, idx + 1);
        func_001C334C(buf, &tex);
        D_000ED8B0[idx].u1 = tex.u0;
        D_000ED8B0[idx].v1 = tex.v0;
        D_000ED8B0[idx].tpage1 = tex.tpage;
        D_000ED8B0[idx].clut1 = tex.clut;
    } else {
        D_000ED8B0[idx].b0 = 0;
        D_000ED8B0[idx].b1 = 0xFF;
    }
    D_000ED8B0[idx].n = pal->n;
    for (i = 0; i < 256; i++) {
        D_000ED8B0[idx].col[i][0] = pal->col[i][0];
        D_000ED8B0[idx].col[i][1] = pal->col[i][1];
        D_000ED8B0[idx].col[i][2] = pal->col[i][2];
        D_000ED8B0[idx].col[i][3] = pal->col[i][3];
    }
}
