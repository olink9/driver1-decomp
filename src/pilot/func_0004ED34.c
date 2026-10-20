/* func_0004ED34 - load DATA\SKY.BIN sky textures (no Driver 2 match) */
typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned char r, g, b, cd; } CV;
typedef struct { short vx, vy; } DV;
typedef struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; } UV;
typedef struct { unsigned char v; char pad[7]; } B8;
typedef struct { char pad[0xD]; unsigned char d; } SkyCfg;

extern char D_00010898[];
extern CV D_0008AA58[];
extern long long D_00096834;
extern long long D_00096850;
extern B8 D_00096AA8;
extern B8 D_00096AA9;
extern CV D_00097090;
extern long long D_00097098;
extern RECT D_000974E0;
extern int D_000974EC;
extern short D_000974F8;
extern long long D_000977A4;
extern long long D_000977A8;
extern SkyCfg D_000BDCE8;
extern short D_000BDD20[];
extern UV D_000BDE40[];
extern unsigned char D_000BE278[];
extern DV D_000BE708[];

extern short GetTPage();
extern void func_00050EC4();
extern short func_0007A7F4();
extern void func_000516AC();
extern void func_00074404();
extern void DrawSync();

void func_0004ED34(void)
{
    unsigned short clut[16];
    RECT rect;
    DV dv;
    unsigned short *buf;
    RECT *crect;
    unsigned char *bmp;
    unsigned char *pal;
    unsigned short *pix;
    int *offs;
    int i, j, y, x;
    unsigned short p;

    buf = (unsigned short *)0xD1200;
    crect = &D_000974E0;
    rect.x = 0x200;
    rect.y = 0;
    D_000974F8 = GetTPage(0, 0, 0x200, 0);
    D_000974EC = 0;

    switch (*(int *)&D_00096834) {
    case 6:
        D_000974EC = 0;
        break;
    case 7:
        D_000974EC = 1;
        break;
    case 3:
        D_000974EC = 2;
        if (D_00096AA8.v || D_00096AA9.v)
            D_000974EC = 3;
        break;
    case 0:
        D_000974EC = 4;
        break;
    case 5:
        D_000974EC = 7;
        break;
    case 1:
        D_000974EC = 10;
        break;
    }

    if ((*(int *)&D_000977A8 | *(int *)&D_000977A4) == 0 &&
        (*(int *)&D_00096834 == 0 || *(int *)&D_00096834 == 5 || *(int *)&D_00096834 == 1)) {
        if (D_00096AA8.v || D_00096AA9.v)
            D_000974EC += 2;
        else if (*(int *)&D_00096850)
            D_000974EC += 1;
    }

    bmp = (unsigned char *)0xF8600;
    D_00097090 = D_0008AA58[D_000974EC];
    offs = *(int **)&D_00097098;
    func_00050EC4(D_00010898, offs, D_000974EC * 48, 0x34);
    func_00050EC4(D_00010898, 0xF8600, offs[0], offs[12] - offs[0]);

    dv.vx = D_000BE708[D_000BE278[D_000BDCE8.d]].vx;
    dv.vy = D_000BE708[D_000BE278[D_000BDCE8.d]].vy;

    for (i = 0; i < 12; i++) {
        pal = bmp + 0x36;
        rect.w = *(unsigned short *)(bmp + 0x12) >> 2;
        rect.h = *(unsigned short *)(bmp + 0x16);
        for (j = 0; j < 16; j++) {
            clut[j] = (pal[0] >> 3) << 10 | (pal[1] >> 3) << 5 | (pal[2] >> 3) | 0x8000;
            pal += 4;
        }
        func_00074404(crect, clut);
        D_000BDD20[i] = func_0007A7F4(crect->x, crect->y);
        func_000516AC(crect);

        pix = (unsigned short *)(pal + (rect.h - 1) * 32);
        for (y = 0; y < rect.h; y++) {
            for (x = 0; x < 16; x++) {
                p = *pix++;
                buf[(y << 4) + x] = (p & 0xF) << 4 | (p & 0xF0) >> 4 | (p & 0xF00) << 4 | (p & 0xF000) >> 4;
            }
            pix -= 32;
        }
        func_00074404(&rect, buf);

        D_000BDE40[i].u0 = rect.x << 2;
        D_000BDE40[i].u1 = (rect.x << 2) + 63;
        D_000BDE40[i].u2 = rect.x << 2;
        D_000BDE40[i].u3 = (rect.x << 2) + 63;
        D_000BDE40[i].v0 = rect.y;
        D_000BDE40[i].v1 = rect.y;
        { unsigned char ty = rect.y; D_000BDE40[i].v2 = rect.h + ty - 1; }
        { unsigned char ty = rect.y; D_000BDE40[i].v3 = rect.h + ty - 1; }

        bmp += offs[i + 1] - offs[i];
        rect.x += 16;
        if (rect.x == 0x240) {
            rect.x = 0x200;
            rect.y += rect.h;
        }
        DrawSync(0);
    }
}
