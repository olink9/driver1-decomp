typedef struct { int pad[2]; int f8; } S12;
int func_0006E604(int a0, S12 *a1) {
    int i;
    for (i = 0; i < 5; i++, a1++)
        if (a0 < a1->f8)
            break;
    if (i == 5)
        return -1;
    return i;
}
