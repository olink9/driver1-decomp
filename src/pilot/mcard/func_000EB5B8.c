typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; P_TAG *ot; } DB;
extern DB *current;
extern char D_000E6BC8[];
extern char D_000E6BD0[];
extern void func_0005D4C0();
extern int PrintString();
extern void SetTextColour();

void func_000EB5B8(int sel)
{
    POLY_F4 *p;
    int pad[2]; /* FAKEMATCH: unused, only sizes the frame (0x28) */

    func_0005D4C0(0x7E, 0x5C, 0x16A, sel, 0);
    if (sel == 0x80)
        SetTextColour(255, 0, 0);
    PrintString(D_000E6BC8, 0x78, 0xAC);
    SetTextColour(255, 255, 255);
    if (sel == 0x81)
        SetTextColour(255, 0, 0);
    PrintString(D_000E6BD0, 0x140, 0xAC);
    SetTextColour(255, 255, 255);
    p = (POLY_F4 *)current->primptr;
    ((P_TAG *)p)->len = 5;
    p->code = 0x2A;
    p->y0 = 0x5A;
    p->x0 = 100;
    p->y1 = 0x5A;
    p->x1 = 400;
    p->y2 = 0xBE;
    p->x3 = 400;
    p->y3 = 0xBE;
    p->x2 = 100;
    p->r0 = 16;
    p->g0 = 16;
    p->b0 = 16;
    ((P_TAG *)p)->addr = current->ot[8].addr;
    current->ot[8].addr = (unsigned int)p;
    current->primptr += sizeof(POLY_F4);
}


