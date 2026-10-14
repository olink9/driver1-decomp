/* DrawPercentageBar (Driver 2, overlay.c) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))

typedef struct { unsigned char r, g, b, cd; } CVECTOR;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
    unsigned char r3, g3, b3, pad3;
    short x3, y3;
} POLY_G4;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, x1, y1, x2, y2, x3, y3;
    unsigned int pad;
} LINE_F4;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, x1, y1;
} LINE_F2;

typedef struct {
    unsigned int tag;
    unsigned int code[1];
} DR_TPAGE;

typedef struct {
    char *tag;
    short xpos, ypos;
    short width, height;
    short position, max;
    void *pColourBand;
    int flags;
    int active;
} PERCENTAGE_BAR;

extern void func_0006ADFC();
extern void func_0007EAA4();
extern void func_0007EA84();
extern void func_0005C7D0();
extern void func_0005C864();
extern void func_0005CC68();

static inline void TransparencyOn(void *potz, unsigned short tpage)
{
    DR_TPAGE *null = (DR_TPAGE *)current->primptr;
    ((P_TAG *)null)->len = 1;
    null->code[0] = 0xE1000600 | (tpage & 0x9ff);
    addPrim(potz, null);
    current->primptr += sizeof(DR_TPAGE);
}

void func_00056BDC(PERCENTAGE_BAR *bar)
{
    POLY_G4 *poly;
    POLY_G4 *poly2;
    LINE_F4 *line4;
    LINE_F2 *line2;
    char *tag;
    int min_x;
    int max_x;
    int min_y;
    int max_y;
    CVECTOR temp;

    if (bar->active == 0)
        return;

    if (bar->position > bar->max)
        bar->position = bar->max;

    if (bar->max != 0) {
        if (bar->flags & 1) {
            max_x = bar->xpos + bar->width;
            min_x = max_x - (bar->width * bar->position) / bar->max;
        } else {
            min_x = bar->xpos;
            max_x = min_x + (bar->width * bar->position) / bar->max;
        }
    } else {
        max_x = bar->xpos;
        min_x = max_x;
    }

    min_y = bar->ypos;
    max_y = bar->ypos + bar->height;

    poly = (POLY_G4 *)current->primptr;
    ((P_TAG *)poly)->len = 8;
    poly->code = 0x38;

    func_0006ADFC(bar->pColourBand, (bar->position << 12) / bar->max, &temp);

    poly->r0 = temp.r;
    poly->g0 = temp.g;
    poly->b0 = temp.b;
    poly->r1 = temp.r;
    poly->g1 = temp.g;
    poly->b1 = temp.b;

    temp.r = temp.r / 4;
    temp.g = temp.g / 4;
    temp.b = temp.b / 4;

    poly->r2 = temp.r;
    poly->g2 = temp.g;
    poly->b2 = temp.b;
    poly->r3 = temp.r;
    poly->g3 = temp.g;
    poly->b3 = temp.b;
    poly->x0 = min_x;
    poly->y0 = min_y;
    poly->x1 = max_x;
    poly->y1 = min_y;
    poly->x2 = min_x;
    poly->y2 = max_y;
    poly->x3 = max_x;
    poly->y3 = max_y;

    addPrim(current->ot + 1, poly);
    current->primptr += sizeof(POLY_G4);

    min_x = bar->xpos;
    max_x = bar->xpos + bar->width;

    poly2 = (POLY_G4 *)current->primptr;
    ((P_TAG *)poly2)->len = 8;
    poly2->code = 0x3A;
    poly2->r0 = 0;
    poly2->g0 = 0;
    poly2->b0 = 0;
    poly2->r1 = 0;
    poly2->g1 = 0;
    poly2->b1 = 0;
    poly2->r2 = 100;
    poly2->g2 = 100;
    poly2->b2 = 100;
    poly2->r3 = 100;
    poly2->g3 = 100;
    poly2->b3 = 100;
    poly2->x0 = min_x;
    poly2->y0 = min_y;
    poly2->x1 = max_x;
    poly2->y1 = min_y;
    poly2->x2 = min_x;
    poly2->y2 = max_y;
    poly2->x3 = max_x;
    poly2->y3 = max_y;

    addPrim(current->ot + 1, poly2);
    current->primptr += sizeof(POLY_G4);

    line4 = (LINE_F4 *)current->primptr;
    line2 = (LINE_F2 *)(line4 + 1);
    func_0007EAA4(line4);
    line4->x0 = min_x - 1;
    line4->y0 = min_y - 1;
    line4->x1 = max_x;
    line4->y1 = min_y - 1;
    line4->x2 = max_x;
    line4->y2 = max_y;
    line4->x3 = min_x - 1;
    line4->y3 = max_y;
    line4->r0 = 80;
    line4->g0 = 80;
    line4->b0 = 80;
    line2->r0 = 80;
    line2->g0 = 80;
    line2->b0 = 80;
    addPrim(current->ot + 1, line4);

    func_0007EA84(line2);
    line2->x0 = min_x - 1;
    line2->y0 = min_y - 1;
    line2->x1 = min_x - 1;
    line2->y1 = max_y;
    addPrim(current->ot + 1, line2);
    current->primptr += sizeof(LINE_F4) + sizeof(LINE_F2);

    TransparencyOn(current->ot + 1, 0x20);

    tag = bar->tag;
    if (tag != 0) {
        short tagX;
        short tagY;

        tagX = bar->xpos;
        tagY = bar->ypos - 11;
        func_0005C7D0(255, 255, 64);
        if (bar->flags & 1)
            func_0005C864(tag, tagX + bar->width - 8, tagY);
        else
            func_0005CC68(tag, tagX + 8, tagY);
    }
}
