typedef struct { short x, y, w, h; } RECT;
typedef struct { int used; unsigned char slot; char pad; short tpage; char rest[0x16C]; } TEX;
extern TEX D_000EFC08[];
extern char D_000E6C0C[];
extern int sprintf();
extern void Loadfile();
extern void func_00074404();
extern void DrawSync();

void func_000EC550(int n, int slot)
{
    char name[0x118];
    RECT r;
    TEX *p;
    int i;
    short tp;

    sprintf(name, D_000E6C0C, n);
    Loadfile(name, (char *)0xD0200);
    r.w = 0x40;
    r.h = 0x100;
    r.x = (slot % 11) * 64 + 0x140;
    r.y = (slot / 11) << 8;
    tp = ((r.y & 0x100) >> 4) | ((r.x & 0x3ff) >> 6) | ((r.y & 0x200) << 2); /* getTPage(0, 0, r.x, r.y) */
    D_000EFC08[n].tpage = tp;
    D_000EFC08[n].slot = slot;
    D_000EFC08[n].used = 1;
    func_00074404(&r, (char *)0xD0200);
    DrawSync(0);
    p = D_000EFC08;
    for (i = 0; i < 32; i++) {
        if (p->slot == slot)
            p->used = 0;
        p++;
    }
    D_000EFC08[n].used = 1;
}
