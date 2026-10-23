extern int D_00096C94; extern void SetMasterVolume(); extern void func_001CD7C0();
void func_001CDB98(void){ if (D_00096C94 < 0) { D_00096C94 += 100; SetMasterVolume(D_00096C94); func_001CD7C0(); } }
