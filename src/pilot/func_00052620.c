/* Driver 2 name unknown: copies four 100-byte tables selected by D_00096834 (and D_0009683C), then calls func_0001BA14 */
typedef struct { int w[25]; } BLOCK100;
extern long long D_00096834;
extern long long D_0009683C;
extern BLOCK100 D_000853D8[];
extern BLOCK100 D_000855CC[];
extern BLOCK100 D_00085630[];
extern BLOCK100 D_00085694[];
extern BLOCK100 car_cosmetics;
extern BLOCK100 D_0009AF68;
extern BLOCK100 D_0009AFCC;
extern BLOCK100 D_0009B030;
void func_0001BA14(void);

void func_00052620(void)
{
    int n, m;

    switch (*(int *)&D_00096834) {
    default:
    case 0: n = 0; break;
    case 1: n = 8; break;
    case 2: n = 16; break;
    case 3: n = 24; break;
    case 5: n = 32; break;
    case 7: n = 40; break;
    }
    m = n;
    {
        int i; int *s, *d;
        d = (int *)&car_cosmetics;
        s = (int *)&D_000853D8[*(int *)&D_0009683C + m];
        for (i = 25; i > 0; i--) *d++ = *s++;
    }
    {
        int i; int *s, *d;
        s = (int *)&D_000855CC[m];
        d = (int *)&D_0009AF68;
        for (i = 25; i > 0; i--) *d++ = *s++;
    }
    {
        int i; int *s, *d;
        s = (int *)&D_00085630[m];
        d = (int *)&D_0009AFCC;
        for (i = 25; i > 0; i--) *d++ = *s++;
    }
    {
        int i; int *s, *d;
        s = (int *)&D_00085694[m];
        d = (int *)&D_0009B030;
        for (i = 25; i > 0; i--) *d++ = *s++;
    }
    func_0001BA14();
}
