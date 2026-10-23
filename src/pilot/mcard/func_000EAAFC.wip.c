/* WIP 92.58: only the scheduling of the y/x arithmetic around the 2nd RECT stores differs (target: sll/addiu/addu
   after li 0x390 and before the first sh; every source order tried puts them before the li or after the sh) */
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
    r.w = 0x10;
    r.h = 1;
    r.y = y * 16 + (x + 0x160);
    func_00074404(&r, b);
    DrawSync(0);
}
