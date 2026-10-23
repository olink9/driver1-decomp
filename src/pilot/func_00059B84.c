extern int D_0009763C;
extern long long CameraCnt;
void ShowReplayMenu(void);
void CameraBar(int);
void func_00059B84(void)
{
    if (D_0009763C == 0) {
        ShowReplayMenu();
        CameraBar(*(int *)&CameraCnt);
    }
}
