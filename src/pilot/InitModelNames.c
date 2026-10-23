extern int D_00096804;
extern char D_000968DC[];
extern int D_000970CC;
extern char D_00010184[];
extern int D_000970C0;
extern char D_00010190[];
extern int D_000970AC;
extern char D_0001019C[];
extern int D_000970C4;
extern char D_000101A8[];
extern int D_000970B0;
extern char D_000101B4[];
extern int D_000970B4;
extern char D_000101C0[];
extern int D_000970B8;
extern char D_000101CC[];
extern int D_000970BC;
extern char D_000968E4[];
extern int D_000970C8;
extern char D_000968EC[];
extern int D_00097120;
extern char D_000968F4[], D_000968FC[], D_00096904[], D_0009690C[];
extern int D_000A1AA0[];
extern int func_000396A8();
extern void MangleWheelModels();

/* Driver 2: InitModelNames */
void InitModelNames(void)
{
    if (D_00096804 == 1) return;
    D_000970CC = func_000396A8(D_000968DC);
    D_000970C0 = func_000396A8(D_00010184);
    D_000970AC = func_000396A8(D_00010190);
    D_000970C4 = func_000396A8(D_0001019C);
    D_000970B0 = func_000396A8(D_000101A8);
    D_000970B4 = func_000396A8(D_000101B4);
    D_000970B8 = func_000396A8(D_000101C0);
    D_000970BC = func_000396A8(D_000101CC);
    D_000970C8 = func_000396A8(D_000968E4);
    D_00097120 = func_000396A8(D_000968EC);
    D_000A1AA0[0] = func_000396A8(D_000968F4);
    D_000A1AA0[1] = func_000396A8(D_000968FC);
    D_000A1AA0[2] = func_000396A8(D_00096904);
    D_000A1AA0[3] = func_000396A8(D_0009690C);
    MangleWheelModels();
}
