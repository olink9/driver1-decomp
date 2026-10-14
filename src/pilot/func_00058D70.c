// FindNextChange
struct PC { char pad[0x14]; int FrameCnt; char pad2[9]; char next; char pad3[2]; };
extern struct PC *D_00097608;
extern struct PC *D_00096DAC;
void func_00058D70(int CameraCnt) {
    int nextframe = 100001;
    int found = 0;
    int count = 0;
    struct PC *rc = D_00096DAC;
    D_00096DAC = D_00097608;
    do {
        if (D_00097608[count].FrameCnt >= CameraCnt && D_00097608[count].FrameCnt < nextframe) {
            nextframe = D_00097608[count].FrameCnt;
            D_00096DAC = &D_00097608[count];
            found = 1;
        }
        count++;
    } while (count < 60);
    if (!found) {
        D_00096DAC = rc;
        rc->next = 0xFE;
    }
}
