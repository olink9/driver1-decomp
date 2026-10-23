/* WIP 97.8%: only register numbering of the 0xFFFFFF/0xFF000000 masks and current differs (ov1). */
typedef struct { int pad[2]; unsigned *ot; } B_001C5EE0;
typedef struct { int pad; B_001C5EE0 *b; } A_001C5EE0;
extern A_001C5EE0 *current;
extern unsigned *D_001D2E20;

void func_001C5EE0(void)
{
    unsigned *d = D_001D2E20;
    A_001C5EE0 *c = current;
    *d = (*d & 0xFF000000) | (*c->b->ot & 0xFFFFFF);
    *c->b->ot = (*c->b->ot & 0xFF000000) | ((unsigned)d & 0xFFFFFF);
}
