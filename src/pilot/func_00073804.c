/* XM_GetFreeVAB */
extern short D_0008D208[];
int func_00073804(void) {
    int i;
    for (i = 0; i < 8; i++)
        if (D_0008D208[i] == -1)
            return i;
    return -1;
}
