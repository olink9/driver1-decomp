extern int DirectorMenuActive;
extern int D_00097618;
extern int CursorX;
extern int CursorY;
extern int D_00097648;
extern int D_00097650;
extern long long pauseflag;
extern long long CameraCnt;
extern char D_0008B698[], D_0008B69C[], D_0008B6A8[], D_0008B6B0[], D_0008B6B4[], D_0008B6B8[], D_0008B6C0[], D_00010D9C[];
extern void func_0004DF28();
extern void func_0004E08C();
extern void ShowIcons();
extern void SetTextColour();
extern void PrintString();

/* ShowReplayMenu */
void ShowReplayMenu(void)
{
    int a;
    int b;

    if (DirectorMenuActive > 0 || D_00097618 == 2 || D_00097618 == 3) {
        func_0004DF28();
        if (DirectorMenuActive == 1)
            ShowIcons(D_0008B69C, CursorX, 0);
        else
            ShowIcons(D_0008B69C, 99, 0);
    } else {
        func_0004E08C();
        ShowIcons(D_0008B698, 0, 0);
    }
    if (DirectorMenuActive >= 2) {
        if (D_00097650 == 0) {
            if (DirectorMenuActive == 2)
                ShowIcons(D_0008B6A8, CursorY - 1, 0);
            else
                ShowIcons(D_0008B6A8, 99, 0);
        } else {
            if (DirectorMenuActive == 2)
                ShowIcons(D_0008B6C0, CursorY - 1, D_00097650);
            else
                ShowIcons(D_0008B6C0, 99, D_00097650);
        }
    }
    if (DirectorMenuActive == 3)
        ShowIcons(D_0008B6B0, CursorX - 6, D_00097650);
    if (DirectorMenuActive == 4)
        ShowIcons(D_0008B6B4, CursorX - 6, D_00097650);
    if (DirectorMenuActive == 5)
        ShowIcons(D_0008B6B8, CursorX - 6, D_00097650);
    if (D_00097648 && *(int *)&pauseflag == 0) {
        a = *(int *)&CameraCnt & 31;
        if (a < 16)
            b = a;
        else
            b = 32 - a;
        SetTextColour(b << 3, 0, 0);
        PrintString(D_00010D9C, 100, 30);
    }
}
