extern char D_00099031[];
extern char D_000967BC, D_00096899;
extern int AttractMode, D_00096878, pauseflag, D_00096890, D_00097084, D_00097088, D_0009709C, D_000970A4, D_000970D0, CameraCnt, D_00097118, D_0009711C;
extern long long D_00096AA8, D_00096AA9, D_00096AB0, FastForward, D_0009743C, D_00097534, D_000977A4, D_000977A8, D_00096A58;
extern void func_00042290(), func_00029980(), func_00077D7C(), func_0004FF4C();
void func_00026040(void)
{
    func_00042290();
    func_00029980();
    D_00099031[0] = 0;
    D_00096890 = 0;
    *(int *)&FastForward = 0;
    if (D_000967BC) {
        D_000970A4 = 0;
        D_000970D0 = 0;
    }
    *(int *)&D_000977A8 = 0;
    *(int *)&D_000977A4 = 0;
    *(char *)&D_00096AA9 = 0;
    *(char *)&D_00096AA8 = 0;
    if (D_00096899 && AttractMode == 0)
        pauseflag = 1;
    else
        pauseflag = 0;
    D_00097088 = 0;
    *(int *)&FastForward = 0;
    D_0009709C = 0;
    D_00097084 = 0;
    D_0009711C = 0;
    D_00096878 = 0;
    D_00097118 = 0;
    *(int *)&D_00096AB0 = 0;
    *(int *)&D_0009743C = 0;
    func_00077D7C(0x1234);
    func_0004FF4C(0xD431, 0x350B1);
    *(int *)&D_00097534 = 0;
    CameraCnt = 0;
    *(int *)&D_00096A58 = 0x800;
}
