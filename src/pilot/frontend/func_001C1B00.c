extern int D_001CDE88, D_001CDE8C; extern void SetDispMask(); extern void func_001C2DAC(); extern void func_001C2F78(); extern void func_001C1B4C(); extern void func_00051374();
int func_001C1B00(void){ SetDispMask(0); func_001C2DAC(0x280); func_001C2F78(); func_001C1B4C(); D_001CDE88 = 0; D_001CDE8C = 0; func_00051374(); return 1; }
