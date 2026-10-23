/* addPrim(current->...->ot, D_001D2DB0): the P_TAG bitfield form gives the target's mask registers (ov1-opus). */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { int pad[2]; unsigned *ot; } B_001C49C8;
typedef struct { int pad; B_001C49C8 *b; } A_001C49C8;
extern A_001C49C8 *current;
extern unsigned *D_001D2DB0;
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))

void func_001C49C8(void)
{
    addPrim(current->b->ot, D_001D2DB0);
}
