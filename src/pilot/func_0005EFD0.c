typedef struct { char pad[0x3C]; unsigned int flags; char pad2[0x58 - 0x40]; } O;
extern O D_000BFA7C[];
extern int D_000C0340[];
extern void func_0005F06C();

void func_0005EFD0(void) {
    int i;
    for (i = 0; i < 20; i++) {
        if (D_000BFA7C[i].flags & 0x10)
            func_0005F06C(&D_000BFA7C[D_000C0340[i]]);
    }
}
