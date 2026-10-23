/* Driver 2: PrintString (PRES.C) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)

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

extern unsigned char D_00097698;
extern long long D_00097699;
extern long long D_0009769A;
extern unsigned short D_00097684;
extern unsigned short D_00097694;
extern long long D_000967E8;
extern void DrawPrim();

typedef struct { unsigned addr:24; unsigned len:8; unsigned char r0, g0, b0, code; } P_TAG;
#define setlen(p, _len) (((P_TAG *)(p))->len = (unsigned char)(_len))
#define setcode(p, _code) (((P_TAG *)(p))->code = (unsigned char)(_code))
#define getcode(p) (unsigned char)(((P_TAG *)(p))->code)
#define setSprt(p) setlen(p, 4), setcode(p, 0x64)
#define setPolyFT3(p) setlen(p, 7), setcode(p, 0x24)
#define setSemiTrans(p, abe) ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;

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
    unsigned short pad;
} POLY_FT3;

int PrintString(char *string, int x, int y)
{
    OUT_FONTINFO *pFontInfo;
    SPRT *font;
    unsigned char width;
    int index;
    unsigned char chr;
    POLY_FT3 *null;

    font = (SPRT *)current->primptr;
    if (current == 0)
        return -1;

    chr = *string++;
    while (chr) {
        if (chr == ' ') {
            x += 6;
        } else {
            index = AsciiTable[chr];
            if (index == 0xFF)
                index = AsciiTable['?'];
            pFontInfo = &fontinfo[index];
            width = pFontInfo->width;

            setSprt(font);
            font->r0 = D_00097698;
            font->g0 = *(unsigned char *)&D_00097699;
            font->b0 = *(unsigned char *)&D_0009769A;
            font->x0 = x;
            font->y0 = pFontInfo->offy + y;
            font->u0 = pFontInfo->x;
            font->v0 = pFontInfo->y + 203;
            font->clut = D_00097684;
            font->w = width;
            font->h = pFontInfo->height;
            setSemiTrans(font, 1);

            if (*(int *)&D_000967E8)
                DrawPrim(font);
            else
                addPrim(current->ot, font);

            font++;
            x += width;
        }
        chr = *string++;
    }

    current->primptr = (char *)font;

    null = (POLY_FT3 *)font;
    setPolyFT3(null);
    null->tpage = D_00097694;
    setSemiTrans(null, 1);
    null->x0 = -1;
    null->y0 = -1;
    null->x1 = -1;
    null->y1 = -1;
    null->x2 = -1;
    null->y2 = -1;
    addPrim(current->ot, null);
    current->primptr += sizeof(POLY_FT3);

    return x;
}
