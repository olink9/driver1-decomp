/* FAKEMATCH: 'yy = y' local copy only gives the text loop its own pseudo for y (target copies y from s2 into a1 before the loop); an inlined print helper would be the real form but didn't match */
/* Credits: print one centred text line (centre x 0x181) at y in colour D_001E1F70[col]; PrintString-style */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned long *ot; } DB;

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

typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct { unsigned long tag; unsigned long code[2]; } DR_MODE;

extern DB *current;
extern unsigned char D_001E1F70[][4];
extern unsigned short D_00097684;
extern int D_00097694;
extern int StringWidth(unsigned char *s);
extern void SetDrawMode();

int func_001E1294(unsigned char *string, int y, int col)
{
    OUT_FONTINFO *pFontInfo;
    SPRT *font;
    unsigned char width;
    int index;
    int chr;
    unsigned char r, g, b;
    int x;
    int yy;

    if (y + 16 < 48)
        return 0;

    font = (SPRT *)current->primptr;
    r = D_001E1F70[col][0];
    g = D_001E1F70[col][1];
    b = D_001E1F70[col][2];
    x = -(StringWidth(string) / 2) + 0x181;

    yy = y;
    chr = *string++;
    while (chr) {
        if (chr == ' ') {
            x += 6;
        } else {
            index = AsciiTable[chr];
            if (index != 0xFF) {
                pFontInfo = &fontinfo[index];
                width = pFontInfo->width;

                ((P_TAG *)font)->len = 4;
                font->code = 0x64;
                font->r0 = r;
                font->g0 = g;
                font->b0 = b;
                font->x0 = x;
                font->y0 = pFontInfo->offy + yy;
                font->u0 = pFontInfo->x;
                font->v0 = pFontInfo->y + 203;
                font->clut = D_00097684;
                font->w = width;
                font->h = pFontInfo->height;
                font->code |= 2;
                ((P_TAG *)font)->addr = ((P_TAG *)&current->ot[4])->addr;
                ((P_TAG *)&current->ot[4])->addr = (unsigned long)font;
                font++;
                x += width;
            }
        }
        chr = *string++;
    }

    SetDrawMode((DR_MODE *)font, 1, 1, D_00097694, 0);
    ((P_TAG *)font)->addr = ((P_TAG *)&current->ot[4])->addr;
    ((P_TAG *)&current->ot[4])->addr = (unsigned long)font;
    current->primptr = (char *)((DR_MODE *)font + 1);
    return 1;
}
