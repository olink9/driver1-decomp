typedef struct { int pad[2]; unsigned *ot; } B_001C49C8;
typedef struct { int pad; B_001C49C8 *b; } A_001C49C8;
extern A_001C49C8 *current;
extern unsigned *D_001D2DB0;

void func_001C49C8(void)
{
    unsigned lo = 0xFFFFFF, hi = 0xFF000000;
    unsigned *d = D_001D2DB0;
    A_001C49C8 *c = current;
    *d = (*d & hi) | (*c->b->ot & lo);
    *c->b->ot = (*c->b->ot & hi) | ((unsigned)d & lo);
}
