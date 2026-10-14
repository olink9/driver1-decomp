typedef struct { char pad[2]; unsigned char f2; char pad2[77]; } S80;
extern S80 D_00098950[];
extern int D_00096C5C;
void func_0004C8F8(int a0) {
    if (D_00098950[a0 - 1].f2 != 1)
        D_00096C5C = a0;
}
