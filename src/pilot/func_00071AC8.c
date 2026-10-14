/* UpdateWithTimer */
typedef struct {
    int pad0;
    int HeaderNum;
    char pad8[0x1A];
    unsigned short SongBPM;
    char pad24[0x10];
    unsigned char XMPlay;
    char pad35[7];
    int JBPM;
    int PCounter;
    char pad[0xB98 - 0x44];
    int JUp;
} XMSONG;
extern XMSONG *D_00097814;
extern int D_00097818;
extern int D_00097838;
extern int D_00097840;
extern XMSONG *D_000C7888[];
extern int D_000C7970[];
extern void func_00071BB4();
extern void func_0007232C();
extern void func_00072684();

void func_00071AC8(int SC)
{
    D_00097814 = D_000C7888[SC];
    if (D_00097814->XMPlay != 1)
        return;
    if (D_00097838 == 1)
        D_00097814->PCounter++;
    D_00097818 = D_000C7970[D_00097814->HeaderNum];
    D_00097814->JBPM += D_00097814->SongBPM;
    if (D_00097814->JBPM < D_00097840) {
        if (D_00097814->JUp == 0 && D_00097838 == 0) {
            D_00097814->JUp = 1;
            func_00072684();
            func_0007232C();
        }
    } else {
        D_00097814->JBPM -= D_00097840;
        func_00071BB4(SC);
    }
}
