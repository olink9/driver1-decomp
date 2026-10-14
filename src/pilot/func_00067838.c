typedef struct { int v[3][4]; int pad[4]; int e[3]; int o[3][4]; } Box;
extern void func_00067A44();

int func_00067838(Box *a, Box *b, int *out)
{
    int i;
    *out = 0x40000000;
    for (i = 0; i < 3; i++) {
        a->o[i][0] = (a->v[i][0] * a->e[i] + 0x800) >> 12;
        a->o[i][1] = (a->v[i][1] * a->e[i] + 0x800) >> 12;
        a->o[i][2] = (a->v[i][2] * a->e[i] + 0x800) >> 12;
        b->o[i][0] = (b->v[i][0] * b->e[i] + 0x800) >> 12;
        b->o[i][1] = (b->v[i][1] * b->e[i] + 0x800) >> 12;
        b->o[i][2] = (b->v[i][2] * b->e[i] + 0x800) >> 12;
    }
    func_00067A44(a, b, 2, out, 1);
    if (*out < 0) return 0;
    func_00067A44(b, a, 2, out, -1);
    if (*out < 0) return 0;
    *out = 0x40000000;
    for (i = 0; i < 2; i++) {
        func_00067A44(a, b, i, out, 1);
        if (*out < 0) return 0;
        func_00067A44(b, a, i, out, -1);
        if (*out < 0) return 0;
    }
    return 1;
}
