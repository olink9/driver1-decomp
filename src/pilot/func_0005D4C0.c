/* print the whole font character set (debug/font test) into POLY_FT4s (PRES.C family) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct {
    unsigned char x, y;
    char pad;
    signed char offy;
    unsigned char width, height;
    unsigned char pad2[2];
} OUT_FONTINFO;
extern OUT_FONTINFO D_000BF2EC[];
extern unsigned char D_000BF6EC[];
#define fontinfo D_000BF2EC
#define AsciiTable D_000BF6EC

extern unsigned short D_00097684;
extern unsigned short D_00097694;
extern int D_00096E34;

typedef struct { unsigned addr:24; unsigned len:8; unsigned char r0, g0, b0, code; } P_TAG;
#define setlen(p, _len) (((P_TAG *)(p))->len = (unsigned char)(_len))
#define setcode(p, _code) (((P_TAG *)(p))->code = (unsigned char)(_code))
#define setPolyFT4(p) setlen(p, 9), setcode(p, 0x2C)
#define setPolyFT3(p) setlen(p, 7), setcode(p, 0x24)
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
} POLY_FT3;

void func_0005D4C0(int x, int y, int maxx, int hilite, int mode)
{
    POLY_FT4 *font;
    POLY_FT3 *null;
    signed char index;
    int i;
    int x0;
    int sx, sy;

    sy = sx = 4096;
    x0 = x;
    if (current == 0)
        return;
    font = (POLY_FT4 *)current->primptr;

    for (i = 0; i < 124; i++) {
        index = AsciiTable[i];
        if (mode != 0 && (mode != 1 || i == '"' || i == '\'' || i == '`'))
            continue;

        setPolyFT4(font);
        if (hilite == i) {
            font->r0 = 255;
            font->g0 = 0;
            font->b0 = 0;
        } else {
            font->r0 = 255;
            font->g0 = 255;
            font->b0 = 255;
        }
        font->clut = D_00097684;
        font->tpage = D_00097694 | 0x20;
        font->u0 = fontinfo[index].x;
        font->v0 = fontinfo[index].y + 203;
        font->u1 = fontinfo[index].x + fontinfo[index].width;
        font->v1 = fontinfo[index].y + 203;
        font->u2 = fontinfo[index].x;
        font->v2 = (fontinfo[index].y + 203) + fontinfo[index].height;
        font->u3 = fontinfo[index].x + fontinfo[index].width;
        font->v3 = (fontinfo[index].y + 203) + fontinfo[index].height;
        font->x0 = x;
        font->y0 = ((fontinfo[index].offy * sy) >> 12) + y;
        font->x1 = x + ((fontinfo[index].width * sx) >> 12);
        font->y1 = ((fontinfo[index].offy * sy) >> 12) + y;
        font->x2 = x;
        font->y2 = ((fontinfo[index].offy * sy) >> 12) + y + ((fontinfo[index].height * sy) >> 12);
        font->x3 = x + ((fontinfo[index].width * sx) >> 12);
        font->y3 = ((fontinfo[index].offy * sy) >> 12) + y + ((fontinfo[index].height * sy) >> 12);

        addPrim(current->ot + D_00096E34, font);
        font++;
        if (fontinfo[index].width != 0)
            x += 16;
        if (maxx < x) {
            x = x0;
            y += 16;
        }
    }

    current->primptr = (char *)font;
    null = (POLY_FT3 *)font;
    setPolyFT3(null);
    null->x0 = -1;
    null->y0 = -1;
    null->x1 = -1;
    null->y1 = -1;
    null->x2 = -1;
    null->y2 = -1;
    null->tpage = D_00097694;
    addPrim(current->ot + 6, null);
    current->primptr += sizeof(POLY_FT3);
}
