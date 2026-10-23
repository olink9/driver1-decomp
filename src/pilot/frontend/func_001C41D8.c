typedef struct { void (*a)(); void (*b)(); int c; unsigned char up, down, left, right; } MENUITEM;
typedef struct { int x, y; char *text; unsigned char r, g, b, pad; } MENUTEXT;
extern MENUITEM D_001CE048[];
extern MENUTEXT D_001CDFE8[];
extern char D_001CDFC0[];
extern char *D_001CE08C[], *D_001CE09C[];
extern int D_001CDEDC, D_001CE088, D_001CDE64, D_000967E0;
extern void func_001C2644(), func_001C4810(), func_001C2114(), func_000514E0();
extern void func_001C19C4(), func_001C49C8(), func_001C1BD4(), func_001C11E4();
extern int func_001C26E4();

int func_001C41D8(void)
{
    int i;
    int r;

    D_001CDEDC = 0;
    D_001CE088 = 0;
    func_001C2644(D_001CE048, 4, 0);
    D_001CDFE8[0].text = D_001CE08C[D_001CDE64];
    for (i = 0; i < 4; i++)
        D_001CDFE8[i + 1].text = D_001CE09C[i];
    for (i = D_000967E0; i < 4; i++) {
        D_001CDFE8[i + 1].r = 0x30;
        D_001CDFE8[i + 1].g = 0x30;
        D_001CDFE8[i + 1].b = 0x30;
    }
    if (D_000967E0 == 1) {
        D_001CE048[0].right = 0;
        D_001CE048[0].left = 0;
    } else if (D_000967E0 == 2) {
        D_001CE048[0].left = 1;
        D_001CE048[0].right = 1;
        D_001CE048[1].left = 0;
        D_001CE048[1].right = 0;
    } else if (D_000967E0 == 3) {
        D_001CE048[0].left = 2;
        D_001CE048[0].right = 1;
        D_001CE048[1].left = 0;
        D_001CE048[1].right = 2;
        D_001CE048[2].left = 1;
        D_001CE048[2].right = 0;
    } else {
        D_001CE048[0].left = 3;
        D_001CE048[0].right = 1;
        D_001CE048[1].left = 0;
        D_001CE048[1].right = 2;
        D_001CE048[2].left = 1;
        D_001CE048[2].right = 3;
        D_001CE048[3].left = 2;
        D_001CE048[3].right = 0;
    }
    func_001C4810();
    func_001C2114(D_001CDFC0, D_001CDFE8, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE048, &D_001CE088);
        if (r != 0)
            break;
        func_001C19C4();
        func_001C49C8();
        func_001C1BD4(D_001CDFE8);
        func_001C11E4();
    }
    if (r == -1)
        return 0;
    return r;
}
