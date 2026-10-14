/* func_00050024 - Driver 2 DrawSkyDome-like: draw sky segments by camera heading, then fill the gap with a TILE */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;

extern long long D_000967FC;
extern int D_000A1AA0[];
extern long long D_00096C4A;            /* camera_angle.vy */
extern int D_000A0E20[], D_000A0E24[], D_000A0E28[], D_000A0E2C[];
extern int D_000974E8;
extern long long D_000977A8;
extern long long D_000977A4;
extern long long D_00096AA8;
extern int D_000974EC;
extern unsigned char D_0008AA58[][4];
extern int D_000974F0, D_000974F4, D_000974DC;
extern long long D_00097090;
#define SKYCOL ((unsigned char *)&D_00097090)

extern void func_0004F200();
extern void func_0004FBA0();
extern int func_0004F7C8();

#define ANG (*(unsigned short *)&D_00096C4A)

void func_00050024(void)
{
    int tops[8];
    int n;
    int i;
    int top, bottom;
    TILE *tile;
    unsigned char *col;

    n = 0;
    func_0004F200();
    if (*(int *)&D_000967FC == 1 && D_000A1AA0[0] != 0) {
        if (ANG < 1450 || ANG > 3700)
            func_0004FBA0(D_000A1AA0[1], 0);
        if (ANG > 650 && ANG < 2450)
            func_0004FBA0(D_000A1AA0[2], 1);
        if (ANG > 1700 && ANG < 3450)
            func_0004FBA0(D_000A1AA0[3], 2);
        if (ANG < 400 || ANG > 2700)
            func_0004FBA0(D_000A1AA0[0], 3);
    }
    if (ANG < 1450 || ANG > 3700)
        tops[n++] = func_0004F7C8(D_000A0E20[0], 0);
    if (ANG > 650 && ANG < 2450)
        tops[n++] = func_0004F7C8(D_000A0E24[0], 12);
    if (ANG > 1700 && ANG < 3450)
        tops[n++] = func_0004F7C8(D_000A0E28[0], 24);
    if (ANG < 400 || ANG > 2700)
        tops[n++] = func_0004F7C8(D_000A0E2C[0], 36);
    if (D_000974E8 == 0) {
        top = 0;
        bottom = 0;
        for (i = 0; i < n; i++) {
            if (bottom < tops[i])
                bottom = tops[i];
        }
    } else {
        top = 0x100;
        bottom = 0x100;
        for (i = 0; i < n; i++) {
            if (tops[i] < top)
                top = tops[i];
        }
    }
    if (top == bottom)
        return;
    tile = (TILE *)current->primptr;
    ((P_TAG *)tile)->len = 3;
    tile->code = 0x60;
    if (*(int *)&D_000977A8 || *(int *)&D_000977A4 || *(unsigned char *)&D_00096AA8) {
        col = D_0008AA58[D_000974EC];
        SKYCOL[0] = (D_000974F0 * col[0]) >> 7;
        SKYCOL[1] = (D_000974F4 * col[1]) >> 7;
        SKYCOL[2] = (D_000974DC * col[2]) >> 7;
    }
    tile->r0 = SKYCOL[0];
    tile->g0 = SKYCOL[1];
    tile->b0 = SKYCOL[2];
    tile->x0 = 0;
    tile->y0 = top;
    tile->w = 512;
    tile->h = bottom;
    ((P_TAG *)tile)->addr = ((P_TAG *)&current->ot[0x107F])->addr;
    ((P_TAG *)&current->ot[0x107F])->addr = (unsigned int)tile;
    current->primptr += sizeof(TILE);
}
