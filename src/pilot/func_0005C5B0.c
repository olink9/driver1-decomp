/* NoMoreCamerasErrorMessage */
typedef struct { int pad[5]; int f14; int pad2[3]; } S24;
extern S24 *D_00097608;
extern long long D_00097534;
#define FrameCnt (*(volatile int *)&D_00097534)
extern void func_0005C7D0();

static inline S24 *FindFreeCamera(void) {
    int i;
    for (i = 0; i < 60; i++)
        if (D_00097608[i].f14 == 100000)
            return &D_00097608[i];
    return 0;
}

int func_0005C5B0(void) {
    int strobe;

    if (FindFreeCamera() == 0) {
    if ((FrameCnt & 0x1f) < 16)
        strobe = FrameCnt & 0x1f;
    else
        strobe = 32 - (FrameCnt & 0x1f);

    func_0005C7D0(strobe << 3, 0, 0);
    return 1;
    }
    return 0;
}
