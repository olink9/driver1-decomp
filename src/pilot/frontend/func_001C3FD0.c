/* FAKEMATCH: unused pad[8] only places the locals (target: r@0x10, counts@0x20, name@0x30). k++ is written in each arm to give k its s-reg priority over sector. */
typedef struct { short x, y, w, h; } RECT;
typedef struct { int used; unsigned char slot; char pad; short tpage; char rest[0x16C]; } TEX;
typedef struct { int n[3]; } COUNTS;
extern TEX D_001CFDF0[];
extern COUNTS D_001C0300;
extern char D_001C030C[];
extern int sprintf();
extern int LoadfileSeg();
extern void func_00074404();

void func_001C3FD0(void)
{
    RECT r;
    char pad[8]; /* FAKEMATCH: frame layout */
    COUNTS counts;
    char name[16];
    int file;
    int j;
    int k;
    int sector;
    short tp;
    unsigned char *buf;

    counts = D_001C0300;
    k = 0;
    r.w = 0x40;
    r.h = 0x100;
    for (file = 0; file < 2; file++) {
        sprintf(name, D_001C030C, file + 1);
        LoadfileSeg(name, (char *)0xD8600, 4, 0x8000);
        buf = (unsigned char *)0xD8600;
        sector = *buf;
        for (j = 0; j < counts.n[file] - 1; j++) {
            if (file == 1 && (j < 3 || j > 6)) {
                sector += 0x8000;
                k++;
            } else {
                LoadfileSeg(name, (char *)0xD8600, sector, 0x8000);
                r.x = (k % 11) * 64 + 0x140;
                r.y = (k / 11) << 8;
                tp = ((r.y & 0x100) >> 4) | ((r.x & 0x3ff) >> 6) | ((r.y & 0x200) << 2);
                D_001CFDF0[k].tpage = tp;
                D_001CFDF0[k].slot = k;
                D_001CFDF0[k].used = 1;
                func_00074404(&r, buf);
                sector += 0x8000;
                k++;
            }
        }
    }
}
