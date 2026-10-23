/* WIP 80.5: target addresses entries via two bases (t1 = D_000EE938 hoisted, t3 = t1-8) and an index a3 = n*0x404 + i*0x44
   with D_000E685C loaded via a pointer (lui hoisted, addiu in loop, lw/lh); my char* form folds to one base + negative offsets */
typedef struct { short s; char c2; char c3; void *p4; char pad[0x3FC]; } H;
extern H D_000EE930[];
extern struct { int a; short b; } D_000E685C;
extern char D_000E684C[];

void func_000E8DD4(int n)
{
    int i;
    char *q;

    D_000EE930[n].s = 1;
    D_000EE930[n].c2 = 0;
    D_000EE930[n].c3 = 0;
    if (n == 0)
        D_000EE930[0].p4 = D_000E684C;
    else
        D_000EE930[n].p4 = D_000E684C;
    q = (char *)&D_000EE930[n];
    for (i = 0; i < 15; i++) {
        *(int *)(q + 8) = D_000E685C.a;
        *(short *)(q + 12) = D_000E685C.b;
        q[0x48] = i;
        *(int *)(q + 0x44) = 0;
        q[0x49] = 1;
        q[0x4B] = 0;
        q += 0x44;
    }
}
