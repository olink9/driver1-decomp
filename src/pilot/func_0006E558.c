typedef struct { int pad[2]; int f8; } S12;
int func_0006E558(int a0, S12 *a1) {
    int i;
    for (i = 0; i < 5; i++, a1++)
        if (a1->f8 < a0)
            break;
    if (i == 5)
        return -1;
    return i;
}
