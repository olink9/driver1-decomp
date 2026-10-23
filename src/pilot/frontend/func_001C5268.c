typedef struct { void (*a)(); void (*b)(); int c; unsigned char up, down, left, right; } MENUITEM;
typedef struct { short x, y; int z; char *text; unsigned char r, g, b, pad; } MENUTEXT;
extern MENUITEM D_001CE33C[];
extern MENUTEXT D_001CE2AC[];
extern char *D_001CE204[][8];
extern char *D_001CE2F4; /* = D_001CE2AC[4].text */
extern int D_001CE200, D_001CE2A4, D_001CE2A8, D_001CE3BC, D_001CE3C0, D_000967E0;
extern void func_001C2644(), func_001C2114(), func_000514E0(), func_001C19C4(), func_001C1BD4(), func_001C11E4();
extern int func_001C26E4();

int func_001C5268(void)
{
    int i, r;

    D_001CE3BC = 0;
    for (i = 0; i < 8; i++) {
        D_001CE2AC[i].r = 0x80;
        D_001CE2AC[i].g = 0x80;
        D_001CE2AC[i].b = 0x80;
    }
    for (i = 0; i < 8; i++) {
        D_001CE33C[i].left = i - 1;
        D_001CE33C[i].right = i + 1;
    }
    D_001CE33C[0].left = 7;
    D_001CE33C[7].right = 0;
    if (D_001CE200 == 1) {
        D_001CE2A4 = 180;
        D_001CE2A8 = 36;
        for (i = 0; i < 4; i++)
            D_001CE2AC[i].y = D_001CE2A4 + i * D_001CE2A8;
        for (i = 0; i < 4; i++)
            D_001CE2AC[i].text = D_001CE204[1][i];
        D_001CE2AC[4].text = 0;
        D_001CE33C[0].left = 3;
        D_001CE33C[3].right = 0;
        D_001CE33C[D_000967E0 - 1].right = 0;
        D_001CE33C[0].left = D_000967E0 - 1;
        for (i = D_000967E0; i < 4; i++) {
            D_001CE2AC[i].r = 0x30;
            D_001CE2AC[i].g = 0x30;
            D_001CE2AC[i].b = 0x30;
        }
    } else if (D_001CE200 == 5) {
        D_001CE2A4 = 180;
        D_001CE2A8 = 34;
        for (i = 0; i < 4; i++)
            D_001CE2AC[i].y = D_001CE2A4 + i * D_001CE2A8;
        for (i = 0; i < 5; i++)
            D_001CE2AC[i].text = D_001CE204[4][i];
        D_001CE2F4 = 0;
        D_001CE33C[0].left = 3;
        D_001CE33C[3].right = 0;
    } else {
        D_001CE2A4 = 150;
        D_001CE2A8 = 30;
        for (i = 0; i < 8; i++)
            D_001CE2AC[i].y = D_001CE2A4 + i * D_001CE2A8;
        for (i = 0; i < 8; i++)
            D_001CE2AC[i].text = D_001CE204[D_001CE200][i];
        for (i = D_000967E0 * 2; i < 8; i++) {
            D_001CE2AC[i].r = 0x30;
            D_001CE2AC[i].g = 0x30;
            D_001CE2AC[i].b = 0x30;
        }
        D_001CE33C[D_000967E0 * 2 - 1].right = 0;
        D_001CE33C[0].left = D_000967E0 * 2 - 1;
    }
    func_001C2644(D_001CE33C, 4, D_001CE3BC);
    func_001C2114(0, D_001CE2AC, 1);
    for (;;) {
        func_000514E0();
        D_001CE3C0 = D_001CE3BC;
        r = func_001C26E4(D_001CE33C, &D_001CE3BC);
        if (r != 0)
            break;
        func_001C19C4();
        func_001C1BD4(D_001CE2AC);
        func_001C11E4();
    }
    if (r == -1)
        return 0;
    return r;
}
