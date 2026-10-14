/* Driver 2: PrintDigit (PRES.C) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct { unsigned char xOffset, width; } FONT_DIGIT;
extern FONT_DIGIT D_0008B6EC[];
#define fontDigit D_0008B6EC

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
    unsigned char texture_number, texture_page;
} TEXTURE_DETAILS;
extern TEXTURE_DETAILS D_000B9140;
#define digit_texture D_000B9140
extern long long D_000B9148;

extern unsigned char D_00097698;
extern long long D_00097699;
extern long long D_0009769A;

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

short func_0005CF10(int x, int y, char *string)
{
    FONT_DIGIT *pDigit;
    char chr;
    int width;
    unsigned char index;
    SPRT *font;
    char fixedWidth;
    char vOff, h;
    POLY_FT3 *null;

    width = x;
    font = (SPRT *)current->primptr;

    while ((chr = *string) != 0) {
        char w;
        char let;
        string++;
        let = chr;
        if (let == 58)
            index = 11;
        else if (let == 47)
            index = 10;
        else
            index = let - 48;

        pDigit = &fontDigit[index];
        w = pDigit->width;

        if (let == 58)
            fixedWidth = 14;
        else
            fixedWidth = 26;

        if (index < 6) {
            vOff = 0;
            h = 28;
        } else {
            vOff = 28;
            h = 31;
        }

        setSprt(font);
        font->r0 = D_00097698;
        font->g0 = *(unsigned char *)&D_00097699;
        font->b0 = *(unsigned char *)&D_0009769A;
        font->x0 = width + ((fixedWidth - w) >> 1);
        font->y0 = y;
        font->u0 = digit_texture.u0 + pDigit->xOffset;
        font->v0 = digit_texture.v0 + vOff;
        font->clut = digit_texture.clutid;
        font->w = w;
        font->h = h;
        setSemiTrans(font, 1);

        addPrim(current->ot, font);

        width += fixedWidth;
        font++;
    }

    current->primptr = (char *)font;

    null = (POLY_FT3 *)current->primptr;
    setPolyFT3(null);
    setSemiTrans(null, 1);
    null->x0 = -1;
    null->y0 = -1;
    null->x1 = -1;
    null->y1 = -1;
    null->x2 = -1;
    null->y2 = -1;
    null->tpage = *(unsigned short *)&D_000B9148;
    addPrim(current->ot, null);
    current->primptr += sizeof(POLY_FT3);

    return width;
}
