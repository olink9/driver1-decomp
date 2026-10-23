/* func_0006C964: pause menu loop (Driver 2: ShowPauseMenu-like) */
typedef struct { void (*f0)(); void (*f4)(); int a; unsigned char b[4]; } ENT;
typedef struct {
    short x, y;
    unsigned char flag, p5, p6, p7;
    char *text;
    unsigned char r, g, b, pf;
} MENTRY;
typedef struct {
    char p0[0x104];
    int w104;
    char p108[0x2A0 - 0x108];
    int w2A0;
    char p2A4[0x2E4 - 0x2A4];
    unsigned char b2E4;
} PL;
typedef struct { int a; char *b; } ARG;

extern PL D_000C6F04;
extern int D_0009D8DC[];
extern long long D_00097534, D_000967D4, D_000967F4, D_00096808, D_00096C90, D_00096C94;
extern long long D_00096874, D_00096834, pauseflag, D_000970A4, D_0008C388, D_00096A08;
extern long long D_000967E8, D_0009709C;
extern char *D_0008C388_[] __asm__("D_0008C388");
extern unsigned char D_000977BC, D_00096F1C, D_00096F1D, D_00096EFC, D_00096EFD;
extern int D_00096F00, D_00096F04, D_00096F08, D_00096F10, D_00096F18, D_00096F20, D_00096F24;
extern int D_000977C8;
extern MENTRY *D_000977C0, *D_000977C4;
extern ENT *D_000977CC, *D_000977D0;
extern signed char D_0008D0A0[], D_0008D094[];
extern char D_000977B4[4], D_000977B8[4], D_00096F6C[], D_00096F70[], D_000115F4[], D_0001160C[];
extern ENT D_0008D064[], D_0008C5B4[], D_0008C474[], D_0008CEA4[], D_0008CF44[], D_0008CFE4[];
extern ENT D_0008CE04[], D_0008CD44[], D_0008CBB4[], D_0008CAF4[], D_0008CC74[], D_0008CA14[];
extern ENT D_0008C964[], D_0008C8C4[], D_0008C7F4[];
extern MENTRY D_0008C524[], D_0008C3B4[], D_0008CE54[], D_0008CEE4[], D_0008CF94[], D_0008CDA4[];
extern MENTRY D_0008CCD4[], D_0008CB54[], D_0008CA84[], D_0008CC04[], D_0008C994[], D_0008C924[];
extern MENTRY D_0008C854[], D_0008C784[];

extern void func_0004DF28(), func_000359BC(), func_000514E0(), func_00025C04();
extern void SetTextColour(), func_0005C8F4(), PrintString();
extern int func_0006E854(), func_0006C4BC();
extern int sprintf();

static inline void InitMenu(ENT *t, int n, int idx)
{
    int i;
    for (i = 0; i < n; i++) {
        D_00096F08 = i;
        t[i].f4();
    }
    D_00096F08 = idx;
    t[idx].f0();
}

static inline void SetMenu(ENT *t, MENTRY *m, int n)
{
    D_000977C8 = n;
    D_000977CC = t;
    D_000977D0 = t;
    D_000977C0 = m;
    D_000977C4 = m;
    InitMenu(D_000977CC, D_000977C8, 1);
}

int func_0006C964(int mode)
{
    unsigned char first;
    MENTRY *p;
    ARG arg;
    int r;
    int n;
    ENT *t;
    int i;

    first = 1;
    *(volatile int *)&D_00097534;
    func_0004DF28();
    if (D_000C6F04.b2E4 == 0
        && !(*(int *)&D_000967D4 != 0 && *(int *)&D_000967F4 == 0x16)
        && !((*(int *)&D_000967F4 == 0x12 || *(int *)&D_000967F4 == 0x16) && D_000C6F04.w104 == 0xFF0)
        && D_0009D8DC[0] == 0 && D_0009D8DC[1] == 0 && D_0009D8DC[2] == 0 && D_0009D8DC[3] == 0) {
        arg.a = D_000C6F04.w2A0;
        arg.b = (char *)&D_000C6F04 + 0xD0;
        r = func_0006E854(&arg, *(int *)&D_000967F4, D_000977BC);
        if (r == 1) {
            D_00096F00 = r;
            goto next;
        }
    }
    D_00096F00 = 0;
next:
    D_00096F1D = 0;
    D_00096F1C = 0;
    if (*(int *)&D_000967F4 == 5) {
        for (i = 0; D_0008D0A0[i] != -1 && !D_00096F1D; i++) {
            if (*(int *)&D_00096808 == D_0008D0A0[i])
                D_00096F1D = 1;
            else
                D_00096F1D = 0;
        }
        for (i = 0; D_0008D094[i] != -1 && !D_00096F1C; i++) {
            if (*(int *)&D_00096808 == D_0008D094[i])
                D_00096F1C = 1;
            else
                D_00096F1C = 0;
        }
    } else {
        D_00096F1D = 0;
        D_00096F1C = 0;
    }

    func_000359BC();
    func_000514E0();
    D_00096F18 = mode;
    D_00096F10 = -1;
    D_00096F04 = 0;
    InitMenu(D_0008D064, 3, 1);

    if (mode == 0) {
        sprintf(D_000977B4, D_00096F6C, *(int *)&D_00096C94 / 100 + 100);
        sprintf(D_000977B8, D_00096F6C, *(int *)&D_00096C90 / 100 + 100);
        if (*(int *)&D_000967F4 == 0x16) {
            D_000977C8 = 8;
            D_000977CC = D_0008C5B4;
            D_000977D0 = D_0008C5B4;
            D_000977C0 = D_0008C524;
            D_000977C4 = D_0008C524;
            D_0008C524[2].text = (char *)D_00096F20;
        } else if (*(int *)&D_000967F4 == 3) {
            D_000977C8 = 8;
            D_000977CC = D_0008C5B4;
            D_000977D0 = D_0008C5B4;
            D_000977C0 = D_0008C524;
            D_000977C4 = D_0008C524;
            if (*(int *)&D_00096874 == 1)
                D_0008C524[2].text = (char *)D_00096F24;
            else
                D_0008C524[2].text = (char *)D_00096F20;
        } else if (*(int *)&D_000970A4 != 0) {
            D_000977C8 = 11;
            D_0008C474[8].b[3] = 9;
            D_0008C474[10].b[2] = 9;
            D_0008C3B4[10].y = 0xCB;
            D_000977CC = D_0008C474;
            D_000977D0 = D_0008C474;
            D_000977C0 = D_0008C3B4;
            D_000977C4 = D_0008C3B4;
            D_0008C3B4[9].text = D_0008C388_[0];
        } else {
            D_000977C8 = 11;
            D_0008C474[8].b[3] = 10;
            D_0008C474[10].b[2] = 8;
            D_0008C3B4[10].y = 0xBC;
            D_0008C3B4[9].text = D_00096F70;
            D_000977CC = D_0008C474;
            D_000977D0 = D_0008C474;
            D_000977C0 = D_0008C3B4;
            D_000977C4 = D_0008C3B4;
        }
        D_000977C0[D_00096EFC].text = D_000977B4;
        D_000977C0[D_00096EFD].text = D_000977B8;
        InitMenu(D_000977CC, D_000977C8, 0);
    } else {
        if (*(int *)&D_000967F4 == 3) {
            if (*(int *)&D_00096874 == 1)
                SetMenu(D_0008CEA4, D_0008CE54, 4);
            else if (*(int *)&D_00096834 == 6)
                SetMenu(D_0008CF44, D_0008CEE4, 5);
            else
                SetMenu(D_0008CFE4, D_0008CF94, 4);
        } else if (*(int *)&D_000967F4 == 5) {
            if (mode == 2) {
                if (D_00096F1C)
                    SetMenu(D_0008CE04, D_0008CDA4, 5);
                else
                    SetMenu(D_0008CD44, D_0008CCD4, 6);
            } else if (D_00096F1D) {
                if (D_00096F1C)
                    SetMenu(D_0008CBB4, D_0008CB54, 5);
                else
                    SetMenu(D_0008CAF4, D_0008CA84, 6);
            } else {
                if (D_00096F1C)
                    SetMenu(D_0008CC74, D_0008CC04, 6);
                else
                    SetMenu(D_0008CA14, D_0008C994, 7);
            }
        } else if (*(int *)&D_000967D4 != 0) {
            SetMenu(D_0008C964, D_0008C924, 3);
        } else if (D_00096F00 != 0) {
            SetMenu(D_0008C8C4, D_0008C854, 6);
        } else {
            SetMenu(D_0008C7F4, D_0008C784, 6);
        }
        first = 0;
        *(int *)&pauseflag = 1;
    }

    D_00096F08 = 1;
    while (D_00096F04 == 0) {
        if (first == 0) {
            if (*(int *)&D_00096A08 == 1)
                D_00096F08 = func_0006C4BC(D_000977CC, D_00096F08);
            func_00025C04();
        } else {
            (*(int *)&D_00097534)++;
        }
        if (*(int *)&D_000967E8 == 0) {
            if (*(int *)&D_00096A08 == 0) {
                SetTextColour(0xFF, 0xFF, 0xFF);
                func_0005C8F4(D_000115F4, 0x78);
            } else if (*(int *)&D_00096A08 == -1) {
                SetTextColour(0xFF, 0xFF, 0xFF);
                func_0005C8F4(D_0001160C, 0x78);
            } else {
                p = D_000977C0;
                i = 0;
                while (p[i].text != 0) {
                    SetTextColour(p[i].r, p[i].g, p[i].b);
                    switch (p[i].flag) {
                    case 0:
                    default:
                        PrintString(p[i].text, p[i].x, p[i].y);
                        break;
                    case 1:
                        func_0005C8F4(p[i].text, p[i].y);
                        break;
                    }
                    i++;
                }
                SetTextColour(0xFF, 0xFF, 0xFF);
            }
            if (D_00096F08 != -1) {
                D_000977C0[D_00096F08].r = 0xFF;
                D_000977C0[D_00096F08].g = 0;
                D_000977C0[D_00096F08].b = 0;
            }
        }
        first = 0;
    }
    if (*(int *)&D_0009709C != 0 || D_00096F10 > 0)
        *(int *)&pauseflag = 1;
    return D_00096F10;
}
