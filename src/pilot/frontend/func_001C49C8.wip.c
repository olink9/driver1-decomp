/* WIP 97.8%: only register numbering of the 0xFFFFFF/0xFF000000 masks and current differs (ov1). */
typedef struct { int pad[2]; unsigned *ot; } B_001C49C8;
typedef struct { int pad; B_001C49C8 *b; } A_001C49C8;
extern A_001C49C8 *current;
extern unsigned *D_001D2DB0;

void func_001C49C8(void)
{
    unsigned *d = D_001D2DB0;
    A_001C49C8 *c = current;
    *d = (*d & 0xFF000000) | (*c->b->ot & 0xFFFFFF);
    *c->b->ot = (*c->b->ot & 0xFF000000) | ((unsigned)d & 0xFFFFFF);
}
