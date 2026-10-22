typedef struct { char pad[4]; short w, h; char pad2[0x14 - 8]; int f14; int f18; } ELEM;
extern long long D_00096804;
extern long long D_00096834;
extern long long D_00096E78;
extern ELEM D_000BE774;
extern ELEM D_000BE998;
extern ELEM D_000BE9C4;
extern char D_0008B1E0[];
extern char D_0008B180[];
extern char D_0008B1C8[];
extern char D_00096D38[];
extern char D_00096D40[];
extern char D_00096D48[];
extern int D_000975DC;
extern void func_00056B5C();
extern void InitOverheadMap();

void func_000542CC(void)
{
    if (*(int *)&D_00096804 != 1 && *(int *)&D_00096834 != 2) {
        func_00056B5C(&D_000BE774, 0, D_0008B1E0, D_00096D38);
        D_000BE774.w = 0x14C;
        D_000BE774.h = 0x18;
        D_000BE774.f14 = 1;
        D_000975DC = 0;
        InitOverheadMap();
        func_00056B5C(&D_000BE998, 0x1000, D_0008B180, D_00096D40);
        D_000BE998.w = 0x10;
        D_000BE998.h = 0x2E;
        D_000BE998.f18 = 1;
        func_00056B5C(&D_000BE9C4, *(int *)&D_00096E78, D_0008B1C8, D_00096D48);
        D_000BE9C4.w = 0x14C;
        D_000BE9C4.h = 0x18;
        D_000BE9C4.f14 = 1;
    }
}
