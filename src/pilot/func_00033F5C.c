/* SetupModels */
extern long long D_00097324;
extern int D_00097248;
extern void func_00033E54(void);
void func_00033F5C(void) {
    if (*(int *)&D_00097324 == -1)
        func_00033E54();
    else
        D_00097248 = 1;
}
