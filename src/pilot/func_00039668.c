extern char D_000967AC[];
extern long long D_0009703C;
extern void Getlong(char *a, int b);
void func_00039668(int a0) {
    Getlong(D_000967AC, a0);
    a0 += 4;
    *(int *)&D_0009703C = a0;
}
