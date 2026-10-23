typedef struct { short x, y, w, h; } RECT;
extern int func_00074404();
extern int DrawSync();

void func_000EAAFC(void *a, void *b, int y, int x)
{
    RECT r;

    r.x = 0x380 + x * 4;
    r.y = 0x100 + y * 48;
    r.w = 4;
    r.h = 0x30;
    func_00074404(&r, a);
    DrawSync(0);
    r.x = 0x390;
    /* (short) keeps fold from reassociating the +0x160 onto the product; r.y is 16-bit anyway */
    r.y = y * 16 + (short)(x + 0x160);
    r.w = 0x10;
    r.h = 1;
    func_00074404(&r, b);
    DrawSync(0);
}
