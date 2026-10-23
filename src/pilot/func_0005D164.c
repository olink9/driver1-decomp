/* scaled/multi-line font print into POLY_FT4s (PRES.C family) */
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
extern int D_00096E34;

typedef struct { unsigned addr:24; unsigned len:8; unsigned char r0, g0, b0, code; } P_TAG;
#define setlen(p, _len) (((P_TAG *)(p))->len = (unsigned char)(_len))
#define setcode(p, _code) (((P_TAG *)(p))->code = (unsigned char)(_code))
#define getcode(p) (unsigned char)(((P_TAG *)(p))->code)
#define setPolyFT4(p) setlen(p, 9), setcode(p, 0x2C)
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

int func_0005D164(char *string, int x, int y, int sx, int sy, int semi)
{
    POLY_FT4 *font;
    signed char index;
    unsigned char chr;
    int x0;

    x0 = x;
    if (current == 0)
        return -1;
    font = (POLY_FT4 *)current->primptr;

    while ((chr = *string) != 0) {
        string++;
        if (chr == '\n') {
            y += (sy * 12) >> 12;
            x = x0;
        } else if (chr == ' ') {
            x += (sx * 6) >> 12;
        } else {
            index = AsciiTable[chr];
            if (index == -1)
                continue;

            setPolyFT4(font);
            font->r0 = D_00097698;
            font->g0 = *(unsigned char *)&D_00097699;
            font->b0 = *(unsigned char *)&D_0009769A;
            setSemiTrans(font, semi);
            font->clut = D_00097684;
            font->tpage = D_00097694;
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
            x += (fontinfo[index].width * sx) >> 12;
        }
    }

    current->primptr = (char *)font;
    return x;
}
