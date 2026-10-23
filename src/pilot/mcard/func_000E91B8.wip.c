/* WIP 87.14: everything up to PrintString matches. Left: the two func_0005D164 arms of the D_000EF144 < 3U test are
   identical in C, so they come out identical and get cross-jumped into one; the target keeps two copies with
   different schedules (then-arm loads D_000EF140 first and stores the stack args last). 14 address-expression
   variants and a 0/1/2 switch (gives a signed tree) all fold the same - the original arms must differ somehow.
   The s1/s2 swap of the %hi(D_000EF13C)/%hi(D_000EF140) pseudos follows from the missing arm. */
typedef struct { int a; short b; char pad[0x12]; char name[0x28]; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; char c2; char c3; void *p4; E e[15]; } H;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { P_TAG tag; int w[9]; } POLY_FT4;
typedef struct { char *primptr; P_TAG **ot; int f8; int id; } DB;
extern DB *current;
extern H D_000EE930[];
extern POLY_FT4 D_000EF8D0[];
extern int D_000ED7CC;
extern int D_000ED7C8;
extern int D_000ED7C4;
extern int D_000EF13C;
extern int D_000EF140;
extern int D_000EF144;
extern char D_000ED784[];
extern char D_000E6864[];
extern char D_000E686C[];
extern char D_000E687C[];
extern void func_000EABBC(), func_0005C8F4(), func_000EB104(), func_0005D164();
extern int PrintString();

/* draw the memory card screen: background, animated icons, both cards, selected save's name */
void func_000E91B8(void)
{
    int i;
    int x;

    D_000EF8D0[current->id].tag.addr = current->ot[12]->addr;
    current->ot[12]->addr = (unsigned int)&D_000EF8D0[current->id];
    (D_000EF8D0 + 2)[current->id].tag.addr = current->ot[12]->addr;
    current->ot[12]->addr = (unsigned int)&(D_000EF8D0 + 2)[current->id];
    if (D_000ED7CC == 0) {
        if (--D_000ED7C8 == 0) {
            D_000ED7C8 = 12;
            D_000ED7C4++;
        }
    }
    for (i = 0; i < 2; i++) {
        if ((D_000EE930[i].s & 2) || (D_000EE930[i].s & 4))
            func_000EABBC(i);
    }
    func_0005C8F4(D_000ED784, 0xBA);
    func_000EB104(0);
    func_000EB104(1);
    if ((D_000EE930[D_000EF13C].s & 2) && (D_000EE930[D_000EF13C].e[D_000EF140].f43 & 1)) {
        x = PrintString(D_000E6864, 0x18, 0xCE);
        if ((unsigned int)D_000EF144 < 3)
            func_0005D164(D_000EE930[D_000EF13C].e[D_000EF140].name, x, 0xCE, 0x1000, 0xE00, 1);
        else
            func_0005D164(D_000EE930[D_000EF13C].e[D_000EF140].name, x, 0xCE, 0x1000, 0xE00, 1);
    } else if (D_000EE930[D_000EF13C].s != 4 && D_000EE930[D_000EF13C].s != 1) {
        if (D_000EF144 < 3)
            PrintString(D_000E686C, 0x1E, 0xCE);
        else
            PrintString(D_000E687C, 0x1E, 0xCE);
    }
}
