/* FAKEMATCH: unused char pad[8] only sizes the frame (0x28); probably an unused local aggregate (RECT?) in the original. */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, x1, y1, x2, y2, x3, y3;
} POLY_F4;
typedef struct { char pad[0x20]; unsigned *ot; } OTB;
typedef struct { char *primptr; OTB *b; } DB;
extern DB *current;
extern char D_001C0920[], D_001C0928[];
extern void func_0005D4C0();
extern void SetTextColour(unsigned char, unsigned char, unsigned char);
extern int PrintString();
extern int PrintStringRightAligned();
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))

void func_001C84C0(int sel)
{
    POLY_F4 *poly;
    char pad[8]; /* FAKEMATCH: frame size */

    func_0005D4C0(0x22, 0xC2, 0x10C, sel, 1);
    if (sel == 0x80)
        SetTextColour(255, 0, 0);
    PrintString(D_001C0920, 0x22, 0x11E);
    SetTextColour(255, 255, 255);
    if (sel == 0x81)
        SetTextColour(255, 0, 0);
    PrintStringRightAligned(D_001C0928, 0x112, 0x11E);
    SetTextColour(255, 255, 255);

    poly = (POLY_F4 *)current->primptr;
    ((P_TAG *)poly)->len = 5;
    poly->code = 0x2A;
    poly->x0 = 0x20;
    poly->y0 = 0xBE;
    poly->x1 = 0x124;
    poly->y1 = 0xBE;
    poly->x2 = 0x20;
    poly->y2 = 0x136;
    poly->x3 = 0x124;
    poly->y3 = 0x136;
    poly->r0 = 16;
    poly->g0 = 16;
    poly->b0 = 16;
    addPrim(current->b->ot, poly);
    current->primptr += sizeof(POLY_F4);
}
