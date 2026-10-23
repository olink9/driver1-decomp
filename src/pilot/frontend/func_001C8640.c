/* Name-entry keyboard: cursor D_001D30D8 over AsciiTable (0..0x7F chars, 0x80 = DEL, 0x81 = END), 15 columns */
typedef struct { unsigned char a, b; } PAIR;

extern int D_001D30D8, D_001D30E8, D_001CEE30;
extern unsigned short D_0009752C, D_00097538;
extern unsigned char D_000BF6EC[];
extern unsigned char D_001CEE34[];
extern unsigned char D_001D30E0[];
extern PAIR D_001CEE58[], D_001CEE38[];
extern int func_0005D86C();

static inline unsigned char inlist(unsigned char c)
{
    unsigned char found = 0;
    unsigned char *p = D_001CEE34;
    unsigned char *end = p + 3;

    for (; p < end; p++) {
        if (*p == c) {
            found = 1;
            break;
        }
    }
    return found;
}

static inline void skip_fwd(void)
{
    for (;;) {
        if (func_0005D86C(D_000BF6EC[D_001D30D8]) && !inlist(D_001D30D8))
            break;
        if (++D_001D30D8 > 0x80) {
            D_001D30D8 = 0x81;
            break;
        }
    }
}

static inline void skip_back(void)
{
    for (;;) {
        if (func_0005D86C(D_000BF6EC[D_001D30D8]) && !inlist(D_001D30D8))
            break;
        if (--D_001D30D8 < 0) {
            D_001D30D8 = 0x81;
            break;
        }
    }
}

int func_001C8640(void)
{
    int ret = 0;
    unsigned short pad = D_0009752C;
    int found;
    int found2;
    int i;
    int n;

    if (pad & 0x2000) {
        if (D_001D30E8 <= 0) {
            found = 0;
            for (i = 0; i < 5; i++) {
                if (D_001D30D8 == D_001CEE58[i].b) {
                    D_001D30D8 = D_001CEE58[i].a;
                    found = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found) {
                if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x80;
                else if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x81;
                else {
                    if (++D_001D30D8 > 0x81)
                        D_001D30D8 = 0;
                    if (D_001D30D8 < 0x80)
                        skip_fwd();
                }
                D_001D30E8 = 8;
            }
        }
    } else if (pad & 0x8000) {
        if (D_001D30E8 <= 0) {
            found = 0;
            for (i = 0; i < 5; i++) {
                if (D_001D30D8 == D_001CEE58[i].a) {
                    D_001D30D8 = D_001CEE58[i].b;
                    found = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found) {
                if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x80;
                else if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x81;
                else {
                    if (--D_001D30D8 < 0)
                        D_001D30D8 = 0x81;
                    if (D_001D30D8 < 0x80)
                        skip_back();
                }
                D_001D30E8 = 8;
            }
        }
    } else if (pad & 0x4000) {
        if (D_001D30E8 <= 0) {
            found2 = 0;
            for (i = 0; i < 15; i++) {
                if (D_001D30D8 == D_001CEE38[i].b) {
                    if (D_001D30D8 >= 0x6F && D_001D30D8 < 0x75)
                        D_001D30D8 = 0x80;
                    else if (D_001D30D8 >= 0x70)
                        D_001D30D8 = 0x81;
                    else
                        D_001D30D8 = D_001CEE38[i].a;
                    found2 = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found2) {
                n = 0;
                if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x21;
                else if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x36;
                else {
                    do {
                        D_001D30D8++;
                        n++;
                        skip_fwd();
                    } while (n < 15);
                }
                D_001D30E8 = 8;
            }
        }
    } else if (pad & 0x1000) {
        if (D_001D30E8 <= 0) {
            found = 0;
            for (i = 0; i < 15; i++) {
                if (D_001D30D8 == D_001CEE38[i].a) {
                    D_001D30D8 = D_001CEE38[i].b;
                    found = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found) {
                n = 15;
                if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x6F;
                else if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x7A;
                else {
                    do {
                        D_001D30D8--;
                        n--;
                        skip_back();
                    } while (n > 0);
                }
                D_001D30E8 = 8;
            }
        }
    } else if (D_00097538 & 0x10) {
        ret = -1;
    } else if (D_00097538 & 0x40) {
        if (D_001D30D8 == 0x81)
            ret = 1;
        else if (D_001D30D8 == 0x80) {
            if (D_001CEE30 > 0) {
                D_001D30E0[D_001CEE30] = ' ';
                D_001CEE30--;
            }
        } else if (D_001CEE30 < 6) {
            D_001D30E0[D_001CEE30] = D_001D30D8;
            D_001CEE30++;
        }
    }
    if (--D_001D30E8 < 0)
        D_001D30E8 = 0;
    return ret;
}
