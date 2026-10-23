/* PrintDigit - Driver 2: PrintDigit (PRES.C).
   Locals as in D2's debug symbols: x and y used directly as parameters (y stays in s1), block-local let/convLet.
   The font colour bytes D_00097699/D_0009769A are fields of 8-byte structs: their loads are MEM_IN_STRUCT and so
   conflict with the in-struct font->y0 store, which keeps `sh y0` below the colour loads (as in the target).
   null->tpage via digit_texture.tpageid assembles as D_000B9140+8 (= D_000B9148, bytes identical). */
/* Driver 2: PrintDigit (PRES.C) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)

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

extern unsigned char D_00097698;
typedef struct { unsigned char v; char pad[7]; } B8;
extern B8 D_00097699;
extern B8 D_0009769A;

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

short PrintDigit(int x, int y, char *string)
{
    FONT_DIGIT *pDigit;
    SPRT *font;
    char width;
    char fixedWidth;
    char vOff;
    char h;
    POLY_FT3 *null;

    font = (SPRT *)current->primptr;

    while (*string != 0) {
        {
            char let;
            unsigned char convLet;
            let = *string++;
            if (let == 58)
                convLet = 11;
            else if (let == 47)
                convLet = 10;
            else
                convLet = let - 48;

            pDigit = &fontDigit[convLet];
            width = pDigit->width;

            if (let == 58)
                fixedWidth = 14;
            else
                fixedWidth = 26;

            if (convLet < 6) {
                vOff = 0;
                h = 28;
            } else {
                vOff = 28;
                h = 31;
            }
        }

        setSprt(font);
        font->r0 = D_00097698;
        font->g0 = D_00097699.v;
        font->b0 = D_0009769A.v;
        font->x0 = x + ((fixedWidth - width) >> 1);
        font->y0 = y;
        font->u0 = digit_texture.u0 + pDigit->xOffset;
        font->v0 = digit_texture.v0 + vOff;
        font->clut = digit_texture.clutid;
        font->w = width;
        font->h = h;
        setSemiTrans(font, 1);

        addPrim(current->ot, font);

        x += fixedWidth;
        font++;
    }

    current->primptr = (char *)font;

    null = (POLY_FT3 *)current->primptr;
    setPolyFT3(null);
    null->x0 = -1;
    null->y0 = -1;
    null->x1 = -1;
    null->y1 = -1;
    null->x2 = -1;
    null->y2 = -1;
    null->tpage = digit_texture.tpageid;
    setSemiTrans(null, 1);
    addPrim(current->ot, null);
    current->primptr += sizeof(POLY_FT3);

    return x;
}
