extern long long D_00096834; extern int D_0008AE5C[], D_0008AE3C[], D_000975AC, D_000A0E20[];
extern void func_000538E8();
typedef struct { int a; int b; char c; char p[3]; int d; } E4;
void func_00053828(void)
{
    int i;
    E4 *p;
    D_000975AC = D_0008AE5C[*(int *)&D_00096834];
    if (D_000975AC != 0) {
        p = (E4 *)(D_0008AE3C[*(int *)&D_00096834] + 4);
        for (i = 0; i < D_000975AC; i++, p++) {
            if (p->c != 0) func_000538E8(D_000A0E20[p->a]);
        }
    }
}
