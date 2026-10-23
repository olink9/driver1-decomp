/* Credits: scroll D_001F3820 by 2 and draw every visible credits line from D_001E27E8 on;
   lines that report "gone" (0) advance the first visible line */
typedef struct {
    unsigned short type;
    unsigned char a, b;
    unsigned char *p;
} CREDLINE;

extern int D_001F3820, D_001E27E8, D_001E27E0, D_001E1AB8;
extern int D_001F3810[];
extern CREDLINE D_001E1ADC[];
extern int D_001E1F90[];
extern int func_001E14D8(int n, int y);
extern int func_001E1294(unsigned char *s, int y, int col);
extern int func_001E0F1C(int face, int y);
extern int func_001E0974(int pic, int slot);

void func_001E1660(void)
{
    CREDLINE *l;
    int i;
    int ok;
    int y;

    D_001F3820 += 2;
    i = D_001E27E8;
    while (D_001E1ADC[i].type != 10) {
        l = &D_001E1ADC[i];
        y = D_001E1F90[i++] - D_001F3820;
        if (y > 0x110)
            return;
        ok = 1;
        switch (l->type) {
        case 4:
            ok = func_001E14D8(l->a, y);
            break;
        case 0:
        case 1:
        case 2:
            ok = func_001E1294(l->p, y, l->a);
            break;
        case 3:
            if (D_001F3810[0] != l->a && D_001F3810[1] != l->a && !D_001E1AB8 && y >= 0xFB) {
                func_001E0974(l->a, D_001E27E0);
                l->b = D_001E27E0;
                D_001F3810[D_001E27E0] = l->a;
                D_001E27E0 = (D_001E27E0 + 1) & 1;
            } else {
                ok = func_001E0F1C(l->b, y);
            }
            break;
        }
        if (!ok)
            D_001E27E8++;
    }
}
