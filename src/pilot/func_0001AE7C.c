extern long long D_00096AA8;
extern long long D_00096AA9;
extern long long gNight;
extern long long ReplayParameterPtr;
void func_0001AE7C(void) {
int a = *(unsigned char *)&D_00096AA8 != 0;
    int b = *(unsigned char *)&D_00096AA9;
    unsigned short *p = *(unsigned short **)&ReplayParameterPtr;
    int f;
    if (b) a |= 2;
    f = a;
    if (*(int *)&gNight == 1) f |= 4;
    p[9] = f;
}
