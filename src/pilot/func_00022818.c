/* func_00022818 - no Driver 2 counterpart (game/level init sequence) */

extern int D_00096834, D_000967FC, D_000967B8, D_00096844, D_00096800, D_00096808;
extern unsigned char D_00096899, D_000967BC, D_00096898, D_000967BE, D_000967BF, D_00096897, D_000970F8;
extern int D_00096850, D_00097098, D_00096804, D_000967F4, D_00096858, D_00096854, D_00096914;
extern int D_000970A4, D_000970DC, D_000970FC, D_000970D8, D_000970EC, D_00096810, D_00097080;
extern int D_00097074, D_000970D0;

extern int D_000C7004[];
extern long long D_00096A68, D_0009724C, D_000976AC, D_000977A0, D_00096A60, D_00096A64;
extern long long D_00096A6C, D_00097004, D_0009715C, D_000974A4, D_0009719C, D_00096E7C, D_000971B4;
extern long long D_00096AA8, D_00096AA9, D_00097424, D_00097658, D_00096FE4, D_00097668;

typedef struct {
    char pad0[0x84];
    char f84[0x100 - 0x84];
    int f100;
    char pad104[0x2DF - 0x104];
    unsigned char f2DF;
    unsigned char f2E0;
} S6F04;

typedef struct {
    int f0;
    short f4;
    short f6;
    char pad8[0x10];
    int f18;
} SE9E0;

extern char D_000BC9BC[], D_0008A8F0[], D_000C017C[], D_000101D8[], D_000101E4[];
extern char D_0008B1A4[], D_00096918[], D_0009B0A4[];
extern S6F04 D_000C6F04;
extern SE9E0 D_000BE9E0;
extern int D_00086F00[];
extern int D_0008A1DC[];


extern void func_00058814(), func_0004C9CC(), func_000595B0(), func_00026040(), func_000505E0();
extern void func_0004D544(), func_0004E1DC(), func_0004D69C(), func_0004C29C(), func_0004DF28();
extern void func_000212F0(), func_00068384(), func_00061220(), func_00068360(), func_000686BC();
extern void func_00059638(), func_0004D208(), func_00050E28(), func_1E06B8(), func_0002258C();
extern void func_0004ED34(), func_0005C99C(), func_00052198(), func_0003A068(), func_000541C8();
extern void func_0003181C(), func_000244B8(), func_1E320C(), func_000148C4(), func_0002DC24();
extern void func_000147C4(), func_00063C60(), func_0003E540(), func_00021BEC(), func_0005EF08();
extern void func_00016CE4(), func_0001FA1C(), func_000337D4(), func_000542CC(), func_00052A78();
extern void func_00052C04(), func_00033AF8(), func_0001AF2C(), func_1E1D30(), func_1E1008();
extern void func_00049538(), func_00058D70(), func_00059698(), func_00056B5C(), func_000661A8();
extern void func_000682C0(), func_0001AE7C(), func_0004D5F8(), func_1E2D70();
extern int VSync();

void func_00022818(void)
{
    int r;
    int i;
    int n;
    S6F04 *p;

    if (D_00096834 == 0 || D_00096834 == 6)
        D_000967FC = 0;
    else
        D_000967FC = 1;
    D_000967B8 = 1;

    func_00058814();
    func_0004C9CC(D_000BC9BC);

    if (D_00096844 != 0 && (D_00096800 == 1 || (unsigned)(D_00096808 - 0xB4) < 4)) {
        func_000595B0();
        D_00096844 = 1;
        D_00096899 = 1;
    }

    func_00026040();
    D_00096850 = 0;
    *(short *)&D_00096A68 = 0;

    if (D_000967BC != 0) {
        D_00097098 = 0x114600;
        *(int *)&D_0009724C = D_00097098;
        D_00097098 += 0x1000;
        func_000505E0();
    }

    if (D_00096804 == 0) {
        func_0004D544();
        func_0004E1DC(0);
        func_0004E1DC(1);
        func_0004E1DC(2);
        func_0004E1DC(3);
        func_0004E1DC(4);
        func_0004E1DC(5);
        func_0004E1DC(6);
        func_0004E1DC(7);
        func_0004E1DC(8);
        func_0004E1DC(9);
        func_0004D69C(D_0008A8F0);
        if (D_000967BC != 0)
            func_0004C29C(D_00096808);
        func_0004DF28();
    }

    if (D_000967BC != 0) {
        func_000505E0();
        func_000212F0();
        func_000505E0();
    }

    *(int *)&D_000976AC = 0;

    if (D_00096804 == 0) {
        func_00068384(&D_000C6F04);
        func_00061220(D_000C017C);
        if (D_000967F4 == 5 || D_000967F4 > 16 || D_000967F4 == 3) {
            if (D_000967BC != 0)
                func_00068360();
            func_000686BC(D_00096808);
            if ((unsigned)(D_00096808 - 0xAA) < 10) {
                if (D_00096808 != 0xAE) {
                    D_00096858 = 1;
                    D_00096898 = 8;
                }
                if (D_00096834 == 2)
                    D_00096854 = 1;
            }
        }
    }

    if (D_00096899 == 0) {
        if (D_00096898 == 3) {
            n = 15;
            r = VSync(-1) % n;
            i = 0;
            while (r == D_00096914) {
                r = (VSync(-1) + i) % n;
                i++;
            }
            func_00059638(D_000C7004[0], r);
            D_00096914 = r;
        } else if (D_00096898 == 8) {
            func_00059638(D_000C7004[0], 0);
        }
    }

    if (D_00096834 != 4)
        func_0004D208(*(int *)&D_000977A0);

    if (D_000967BC != 0) {
        if (D_00096834 == 2 || D_00096834 == 6) {
            func_00050E28(D_000101D8, 0x1E0000);
        } else if (D_00096800 == 1) {
            func_00050E28(D_000101E4, 0x1E0000);
            func_1E06B8();
        }
        func_00050E28(D_00086F00[D_00096834], 0x1F8BBC);
        func_0002258C();
        D_000967BE = 1;
        if (D_00096834 != 2 && D_00096804 == 0)
            func_0004ED34();
        func_0005C99C();
    } else {
        if (D_000967BE == 0)
            func_00052198();
        D_000967BE = 1;
        func_0003A068();
        if (D_000970A4 != 0 && D_00096899 == 0)
            func_000541C8();
        func_0003181C(D_00096834, 0);
    }

    if (D_00096898 == 3)
        func_000244B8(D_000C6F04.f100, D_000C6F04.f2DF);
    else if (D_00096898 == 8)
        func_000244B8(D_00096808, 1);

    if (D_00096834 == 6 && D_000967F4 == 0x16)
        func_1E320C();

    func_000148C4();
    func_0002DC24();
    func_000147C4();

    if (D_00096804 == 0 && D_00096834 != 2 && D_00096834 != 6)
        func_00063C60();

    func_0003E540();
    func_00021BEC();
    D_00096897 = 0;

    if (D_00096804 == 0) {
        if (D_000970D0 != 0)
            func_0005EF08();
        func_00016CE4();
    }

    func_0001FA1C();
    func_000337D4();
    func_0003A068();

    if (D_000970A4 != 0 && D_00096899 == 0) {
        func_000542CC();
        D_000967BE = 0;
    }

    if (D_000967BC != 0)
        func_00052A78();

    func_00052C04();
    func_00033AF8();
    func_000148C4();

    if (D_00096898 == 3 || D_00096898 == 8) {
        if (D_00096898 == 8)
            D_000967BF = 0;
        func_0001AF2C(0x17, *(int *)&D_00097668 + 4, *(short *)(*(int *)&D_00097668 + 0x10), D_000967BF, 4);
    }

    *(int *)&D_00096A60 = 0x640;
    *(int *)&D_00096A64 = 0x640;
    *(int *)&D_00096A6C = 0x800;
    D_0008A1DC[0] = -45;
    D_0008A1DC[1] = -300;
    D_0008A1DC[2] = -125;
    D_000970DC = 100;
    *(int *)&D_00097004 = 25;
    D_000970F8 = 0;
    D_000970FC = 0;

    if (D_00096858 != 0)
        func_1E1D30();
    if (D_00096854 != 0)
        func_1E1008();

    func_00049538();

    if (D_000967BC != 0) {
        *(int *)&D_0009715C = D_00097098;
        *(int *)&D_000974A4 = D_00097098;
        D_00097098 += 0x800;
        *(int *)&D_0009719C = D_00097098;
        D_00097098 += 0x100;
    }

    if (D_00096899 != 0)
        func_00058D70(D_000970D8);
    else
        func_00059698();

    func_00056B5C(&D_000BE9E0, *(int *)&D_00096E7C, D_0008B1A4, D_00096918);
    D_000BE9E0.f4 = 16;
    D_000BE9E0.f6 = 24;
    D_000BE9E0.f18 = 1;

    if (D_00096834 == 4)
        *(int *)&D_000971B4 = 0x400;
    else
        *(int *)&D_000971B4 = 0x190;

    if (D_000970EC != 0)
        func_000661A8();

    if (*(unsigned char *)&D_00096AA8 != 0 || *(unsigned char *)&D_00096AA9 != 0)
        *(int *)&D_00097424 = 7000;
    else
        *(int *)&D_00097424 = 0;

    p = &D_000C6F04;
    if (p->f2E0 != 0 && D_00096834 != 4) {
        func_000682C0(D_0009B0A4, p->f84);
        p->f2E0 = 0;
    }

    *(unsigned char *)&D_00097658 = 1;
    *(int *)&D_00096E7C = 0x55F0;

    if (D_00096899 == 0)
        func_0001AE7C();

    if (D_00096834 == 2)
        func_0004D5F8(1);
    else
        func_0004D5F8(0);

    if (D_00096834 == 6)
        func_1E2D70(D_00096810);

    D_00097080 = 0;
    *(int *)&D_00096FE4 = 0;
    D_00097074 = 1;
}
