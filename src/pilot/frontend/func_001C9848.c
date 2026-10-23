extern int D_00097098;
extern int D_001D3150;
extern int *D_001D3160[];
extern int *D_001D3190[];
extern char D_001C0A18[];
extern int sprintf();
extern int Loadfile();

void func_001C9848(int a)
{
    char buf[32];
    int *p;
    int i;
    sprintf(buf, D_001C0A18, a);
    D_001D3150 = D_00097098;
    D_00097098 += Loadfile(buf, D_00097098);
    p = (int *)D_001D3150;
    for (i = 0; i < 10; i++) {
        int n = *p++;
        D_001D3160[i] = p;
        D_001D3190[i] = p + 11;
        p += n >> 2;
    }
}
