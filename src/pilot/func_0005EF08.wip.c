typedef struct {
    char pad0[0x42];
    short f42;
    char pad1[0x58 - 0x44];
} E;
typedef struct {
    short s0;
    char c2;
    char pad2[12 - 3];
} Ent;
typedef struct {
    char pad[8];
    Ent e[1];
} L;
extern E D_000BFA7C[];
extern int D_000C0340[];
extern int D_000976C0, D_000976B8;
extern char D_000976C8;
extern L *D_000976CC;
extern void func_000774C4();
extern void func_0005EFD0();

void func_0005EF08(void)
{
    int i;
    Ent *p;
    L *l;

    l = D_000976CC;
    func_000774C4(D_000BFA7C, 0, 0x6E0);
    func_0005EFD0();
    D_000976C0 = 0;
    for (i = 0; i < 20; i++) {
        D_000C0340[i] = i;
        D_000BFA7C[i].f42 = i;
    }
    D_000976B8 = 0;
    p = l->e;
    if (l != 0 && p->s0 != 0x270F) {
        p = l->e;
        do {
            p->c2 = 0;
            p++;
            D_000976B8++;
        } while (p->s0 != 0x270F);
    }
    D_000976B8 = 0;
    D_000976C8 = 1;
}
