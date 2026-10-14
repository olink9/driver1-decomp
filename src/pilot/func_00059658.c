typedef struct { int pad[5]; int f14; int pad2[3]; } S24;
extern S24 *D_00097608;
S24 *func_00059658(void) {
    int i;
    for (i = 0; i < 60; i++)
        if (D_00097608[i].f14 == 100000)
            return &D_00097608[i];
    return 0;
}
