/* WIP 39%: target stores D_001CDECC=0 first (lui v0/sw) then builds p (lui/addiu/addiu 0xA0); gcc hoists the p address calc above the store, using v1 for DECC. Tried ordering, blocks, unsized array, char* forms. */
typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF468;
extern T_001CF468 D_001CF468[];
extern int D_001CDECC;

void func_001CD514(void)
{
    T_001CF468 *p = D_001CF468;
    D_001CDECC = 0;
    p += 10;
    p->r = 255;
    p->g = 255;
    p->b = 255;
}
