typedef struct { short x, y, w, h; } RECT;
typedef struct { char s[12]; } NAME12;
extern volatile unsigned short D_00097538;
extern char D_001C02A0[], D_001C02AC[], D_001C02B8[], D_001C02C8[], D_001C02D0[];
extern char D_001C02D8[], D_001C02E8[], D_001C02F0[];
extern void memset();
extern void func_0004E170(), func_0004E348(), SetDispMask(), func_00051374(), DrawSync(), VSync();
extern int Loadfile();
extern void func_00074404(), func_000514E0(), func_000E6D60(), func_001C1B4C();

void func_001C37BC(void)
{
    RECT rect;
    NAME12 name;
    int buf;
    int i;

    name = *(NAME12 *)D_001C02A0;
    i = 0;
    memset(&buf, i, 4);
    func_0004E170();
    func_0004E348();
    rect.x = 0;
    rect.y = 0;
    rect.w = 320;
    rect.h = 480;
    SetDispMask(i);
    func_00051374();
    DrawSync(i);
    VSync(i);
    Loadfile(&name, 0xD0200);
    func_00074404(&rect, 0xD0214);
    DrawSync(i);
    func_000514E0();
    do {
        if (i == 2)
            SetDispMask(1);
        func_000514E0();
        i++;
    } while (!(D_00097538 & 0x40));
    SetDispMask(0);
    Loadfile(D_001C02AC, 0xE6600);
    func_000E6D60(D_001C02B8, D_001C02C8, 0, -2);
    func_000E6D60(D_001C02B8, D_001C02D0, 1, -2);
    func_000E6D60(D_001C02D8, D_001C02E8, 2, -2);
    func_000E6D60(D_001C02D8, D_001C02F0, 3, -2);
    func_001C1B4C();
    func_00051374();
    DrawSync(0);
    VSync(0);
}
