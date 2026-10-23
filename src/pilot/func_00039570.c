extern char D_000967AC[];
extern long long D_00097040;
extern void Getlong(char *a, int b);
void func_00039570(int a0) {
    Getlong(D_000967AC, a0);
    a0 += 4;
    *(int *)&D_00097040 = a0;
}
