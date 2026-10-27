/* pc_display.c - show the PS1 display area straight from VRAM (Driver 1 PC port).
 *
 * A few screens put a picture into the frame buffer with LoadImage and just turn the display on (SetDispMask(1)),
 * drawing nothing: the PS1 GPU scans VRAM out as it is. PsyCross only presents what was drawn into its scene, so
 * on PC those screens stayed on the previous frame (options.c func_001C37BC, "View Controls": GFX\PAD.TIM).
 * PC_ShowDisplayVRAM(first) draws the current display area of VRAM into the scene as 15-bit textured quads (tiles
 * that stay inside one texture page); the next VSync presents it. Presenting stores the scene back into the display
 * area (GR_StoreFrameBuffer), which isn't lossless, so the picture is saved on the first call (first != 0) and
 * reloaded before every later one. */
#include <string.h>

typedef struct { short x, y, w, h; } PC_RECT16;
typedef struct { PC_RECT16 disp, screen; unsigned char isinter, isrgb24, pad0, pad1; } PC_DISPENV;
typedef struct { PC_RECT16 clip; short ofs[2]; } PC_DRAWENV_HEAD;   /* the start of PsyCross's DRAWENV */

extern PC_DISPENV activeDispEnv;          /* PsyCross PsyX_GPU.cpp */
extern PC_DRAWENV_HEAD activeDrawEnv;
extern void DrawOTag(unsigned int *p);
extern int StoreImage(PC_RECT16 *r, unsigned int *p);
extern int LoadImage(PC_RECT16 *r, unsigned int *p);
extern void DrawSync(int mode);

static unsigned int s_saved[1024 * 512 / 2];   /* the saved display area (15-bit pixels, up to the whole VRAM) */
static PC_RECT16 s_saved_rect;

#define MAX_TILES 64

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad1;
    short x3, y3; unsigned char u3, v3; unsigned short pad2;
} PC_POLY_FT4;

static PC_POLY_FT4 s_tiles[MAX_TILES];
/* black TILE under the picture: a 15-bit texel 0x0000 is transparent when drawn, but black when scanned out */
static struct { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0, w, h; } s_black;

void PC_ShowDisplayVRAM(int first)
{
    const PC_RECT16 d = activeDispEnv.disp;
    int n = 0, x, y, tw, th;

    if (first) {
        s_saved_rect = d;
        DrawSync(0);
        StoreImage(&s_saved_rect, s_saved);
        DrawSync(0);
    } else if (s_saved_rect.w == d.w && s_saved_rect.h == d.h) {
        LoadImage(&s_saved_rect, s_saved);
        DrawSync(0);
    }

    for (y = d.y; y < d.y + d.h && n < MAX_TILES; y += th) {
        const int pby = y & ~255;
        th = d.y + d.h - y;
        if (th > 128) th = 128;
        if (y - pby + th > 255) th = 255 - (y - pby);    /* v must fit a byte: row 255 of a page gets a 1-row tile */
        if (th <= 0) th = 1;
        for (x = d.x; x < d.x + d.w && n < MAX_TILES; x += tw, n++) {
            PC_POLY_FT4 *p = &s_tiles[n];
            const int pbx = x & ~63;
            const int u = x - pbx, v = y - pby;
            const int sx = x - d.x - activeDrawEnv.ofs[0] + activeDrawEnv.clip.x;
            const int sy = y - d.y - activeDrawEnv.ofs[1] + activeDrawEnv.clip.y;
            tw = d.x + d.w - x;
            if (tw > 128) tw = 128;
            memset(p, 0, sizeof(*p));
            p->tag = 9u << 24;
            p->code = 0x2D;                                /* POLY_FT4, raw texture, opaque */
            p->r0 = p->g0 = p->b0 = 0x80;
            p->tpage = (2 << 7) | ((pby & 0x100) >> 4) | ((pbx & 0x3FF) >> 6);   /* 15-bit direct */
            p->x0 = sx;      p->y0 = sy;      p->u0 = u;      p->v0 = v;
            p->x1 = sx + tw; p->y1 = sy;      p->u1 = u + tw; p->v1 = v;
            p->x2 = sx;      p->y2 = sy + th; p->u2 = u;      p->v2 = v + th > 255 ? 255 : v + th;
            p->x3 = sx + tw; p->y3 = sy + th; p->u3 = u + tw; p->v3 = v + th > 255 ? 255 : v + th;
        }
    }
    if (n == 0)
        return;
    for (x = 0; x < n; x++)
        s_tiles[x].tag = (9u << 24) | (x + 1 < n ? ((unsigned int)(unsigned long)&s_tiles[x + 1] & 0xFFFFFF) : 0xFFFFFF);
    s_black.tag = (3u << 24) | ((unsigned int)(unsigned long)&s_tiles[0] & 0xFFFFFF);
    s_black.code = 0x60;
    s_black.r0 = s_black.g0 = s_black.b0 = 0;
    s_black.x0 = activeDrawEnv.clip.x - activeDrawEnv.ofs[0];
    s_black.y0 = activeDrawEnv.clip.y - activeDrawEnv.ofs[1];
    s_black.w = d.w;
    s_black.h = d.h;
    DrawOTag((unsigned int *)&s_black);
}
