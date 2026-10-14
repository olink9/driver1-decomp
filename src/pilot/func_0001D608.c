/* StepCarPhysics */
extern int D_00097030;
extern char *D_0009AC48[];
void func_0001D608(char *a0, int a1) {
    if (a1)
        a0[3] = 1;
    else
        a0[3] = 0;
    D_0009AC48[D_00097030++] = a0;
}
