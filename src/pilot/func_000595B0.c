extern long long D_00096800;
extern int D_00096DE4, D_00097670;
extern char D_00010E04[], D_00010E18[];
extern int sprintf();
extern void func_00050E28();
void func_000595B0(void) {
    char buf[0x80];
    if (*(int *)&D_00096800 == 1) {
        sprintf(buf, D_00010E04);
    } else {
        sprintf(buf, D_00010E18, D_00096DE4);
        D_00096DE4++;
        if (D_00096DE4 >= 3) D_00096DE4 = 1;
    }
    func_00050E28(buf, D_00097670);
}
