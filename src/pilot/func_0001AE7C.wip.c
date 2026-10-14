extern long long D_00096AA8;
extern long long D_00096AA9;
extern long long D_00096850;
extern long long D_00097668;
void func_0001AE7C(void) {
    int a = *(unsigned char *)&D_00096AA8 != 0;
    int b = *(unsigned char *)&D_00096AA9 != 0;
    unsigned short *p = *(unsigned short **)&D_00097668;
    int f = a;
    if (b) f = a | 2;
    if (*(int *)&D_00096850 == 1) f |= 4;
    p[9] = f;
}
