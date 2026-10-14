typedef struct {
    int w[25];          /* 100 bytes; copied by a struct assignment loop */
} BLOCK100;

extern long long D_00096834;    /* absolute (8-byte) globals - see pilot/AGENT.md */
extern long long D_0009683C;
extern BLOCK100 D_000853D8[];
extern BLOCK100 D_000855CC[];
extern BLOCK100 D_00085630[];
extern BLOCK100 D_00085694[];
extern BLOCK100 D_0009AF04;
extern BLOCK100 D_0009AF68;
extern BLOCK100 D_0009AFCC;
extern BLOCK100 D_0009B030;

void func_0001BA14(void);

void func_00052620(void)
{
    int n, m;

    switch (*(int *)&D_00096834) {
    default:
    case 0:
        n = 0;
        break;
    case 1:
        n = 8;
        break;
    case 2:
        n = 16;
        break;
    case 3:
        n = 24;
        break;
    case 5:
        n = 32;
        break;
    case 7:
        n = 40;
        break;
    }
    m = n;
    D_0009AF04 = D_000853D8[m + *(int *)&D_0009683C];
    D_0009AF68 = D_000855CC[m];
    D_0009AFCC = D_00085630[m];
    D_0009B030 = D_00085694[m];
    func_0001BA14();
}
