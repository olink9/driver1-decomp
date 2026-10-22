/* ShowIcons (Driver 2, director.c) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

typedef struct {
    unsigned char coords[8];
    unsigned short tpageid;
    unsigned short clutid;
} TEXTURE_DETAILS;

typedef struct {
    short x, y;
    TEXTURE_DETAILS *texture;
    char *TextPtr;
    short tx, ty;
} REPLAY_ICON;

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
    unsigned short pad1;
} POLY_FT3;

typedef struct { int RecordingEnd; } REPLAY_PARAMETER_BLOCK;

extern REPLAY_PARAMETER_BLOCK *ReplayParameterPtr;
extern long long CameraCnt;
#define CameraCnt (*(int *)&CameraCnt)
extern int D_0009760C;
extern long long vblcounter;
#define vblcounter (*(int *)&vblcounter)
extern int D_00096DA4, FastForward, D_00096DA8, D_00097604;
extern unsigned char D_000BEEBC[];
extern unsigned char D_000BEEC3[];
extern unsigned char D_000BEECE[];
extern long long D_00096F1C;
extern unsigned char tracking_car;
extern REPLAY_ICON D_0008B528[];

extern int NoMoreCamerasErrorMessage();
extern int OK_To_Zoom();
extern int FirstCamera();
extern void SetTextColour();
extern void PrintString();

void ShowIcons(unsigned char *menu, int selected, int x_offset)
{
    int m;
    int ramp;
    SPRT *icon;
    POLY_FT3 *null;
    TEXTURE_DETAILS *Icon_texture;
    REPLAY_ICON *ri;
    int count;
    unsigned char c;
    int x, y;

    count = 0;
    x_offset++;

    if (CameraCnt >= ReplayParameterPtr->RecordingEnd) {
        if (D_0009760C == 0)
            D_0009760C = vblcounter;
        D_00096DA4 = 0;
        FastForward = 0;
        D_000BEEBC[8] = 1;
        D_000BEEBC[5] = 1;
        D_000BEEBC[4] = 1;
        D_000BEEBC[3] = 1;
        D_000BEEBC[2] = 1;
        D_000BEEBC[1] = 1;
    } else {
        D_000BEEBC[8] = 0;
        D_000BEEBC[5] = 0;
        D_000BEEBC[3] = 0;
        D_000BEEBC[2] = 0;
        D_000BEEBC[1] = 0;
        if (NoMoreCamerasErrorMessage())
            D_000BEEBC[4] = 1;
        else
            D_000BEEBC[4] = 0;
    }

    if (*(unsigned char *)&D_00096F1C)
        D_000BEEC3[0] = 1;
    else
        D_000BEEC3[0] = 0;

    if (tracking_car && OK_To_Zoom())
        D_000BEECE[0] = 0;
    else
        D_000BEECE[0] = 1;

    if (FirstCamera()) {
        D_000BEEBC[0x15] = 1;
        D_000BEEBC[6] = 1;
    } else {
        D_000BEEBC[0x15] = 0;
        D_000BEEBC[6] = 0;
    }

    while (*menu != 255) {
        ri = &D_0008B528[*menu];
        Icon_texture = ri->texture;

        if (selected == count) {
            SetTextColour(255, 255, 64);
            PrintString(ri->TextPtr, ri->tx + x_offset, ri->ty);
        }

        icon = (SPRT *)current->primptr;
        ((P_TAG *)icon)->len = 4;
        icon->code = 100;

        if (D_000BEEBC[*menu]) {
            icon->code = 0x66;
            icon->r0 = 32;
            icon->g0 = 32;
            icon->b0 = 32;
        } else if (selected == count) {
            ramp = D_00097604 & 0xf;
            if (ramp < 8)
                m = 7 - ramp;
            else
                m = ramp - 7;
            ramp = m * 28;
            if (D_00096DA8) {
                c = ramp + 30;
                icon->r0 = c;
                icon->g0 = c;
                icon->b0 = c;
                D_00097604++;
            } else {
                icon->r0 = 255;
                icon->g0 = 255;
                icon->b0 = 255;
                D_00097604 = 0;
            }
        } else {
            icon->r0 = 100;
            icon->g0 = 100;
            icon->b0 = 100;
        }

        icon->clut = Icon_texture->clutid;
        icon->u0 = Icon_texture->coords[0];
        icon->v0 = Icon_texture->coords[1];
        x = ri->x + x_offset;
        y = ri->y + 16;
        icon->x0 = x;
        icon->y0 = y;
        icon->w = Icon_texture->coords[2] - Icon_texture->coords[0] + 1;
        icon->h = Icon_texture->coords[5] - Icon_texture->coords[1] + 1;

        ((P_TAG *)icon)->addr = ((P_TAG *)current->ot)->addr;
        ((P_TAG *)current->ot)->addr = (unsigned int)icon;
        current->primptr += 0x28;

        null = (POLY_FT3 *)current->primptr;
        ((P_TAG *)null)->len = 7;
        null->code = 0x24;
        null->x0 = -1;
        null->y0 = -1;
        null->x1 = -1;
        null->y1 = -1;
        null->x2 = -1;
        null->y2 = -1;
        null->tpage = Icon_texture->tpageid;
        ((P_TAG *)null)->addr = ((P_TAG *)current->ot)->addr;
        ((P_TAG *)current->ot)->addr = (unsigned int)null;
        current->primptr += 0x20;

        menu++;
        count++;
    }

    if (D_00096DA4) {
        if (--D_00096DA4 == 0)
            FastForward = 1;
    }
}
