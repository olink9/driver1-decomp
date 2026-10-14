extern int D_0009761C;
extern int D_00097618;
extern int D_00097624;
extern int D_00097628;
extern int D_00097648;
extern int D_00097650;
extern long long D_0009687C;
extern long long D_000970D8;
extern char D_0008B698[], D_0008B69C[], D_0008B6A8[], D_0008B6B0[], D_0008B6B4[], D_0008B6B8[], D_0008B6C0[], D_00010D9C[];
extern void func_0004DF28();
extern void func_0004E08C();
extern void func_0005972C();
extern void func_0005C7D0();
extern void func_0005CC68();

/* ShowReplayMenu */
void func_00059BBC(void)
{
    int a;
    int b;

    if (D_0009761C > 0 || D_00097618 == 2 || D_00097618 == 3) {
        func_0004DF28();
        if (D_0009761C == 1)
            func_0005972C(D_0008B69C, D_00097624, 0);
        else
            func_0005972C(D_0008B69C, 99, 0);
    } else {
        func_0004E08C();
        func_0005972C(D_0008B698, 0, 0);
    }
    if (D_0009761C >= 2) {
        if (D_00097650 == 0) {
            if (D_0009761C == 2)
                func_0005972C(D_0008B6A8, D_00097628 - 1, 0);
            else
                func_0005972C(D_0008B6A8, 99, 0);
        } else {
            if (D_0009761C == 2)
                func_0005972C(D_0008B6C0, D_00097628 - 1, D_00097650);
            else
                func_0005972C(D_0008B6C0, 99, D_00097650);
        }
    }
    if (D_0009761C == 3)
        func_0005972C(D_0008B6B0, D_00097624 - 6, D_00097650);
    if (D_0009761C == 4)
        func_0005972C(D_0008B6B4, D_00097624 - 6, D_00097650);
    if (D_0009761C == 5)
        func_0005972C(D_0008B6B8, D_00097624 - 6, D_00097650);
    if (D_00097648 && *(int *)&D_0009687C == 0) {
        a = *(int *)&D_000970D8 & 31;
        if (a < 16)
            b = a;
        else
            b = 32 - a;
        func_0005C7D0(b << 3, 0, 0);
        func_0005CC68(D_00010D9C, 100, 30);
    }
}
