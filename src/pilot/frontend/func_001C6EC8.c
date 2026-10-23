extern int D_000967F4; extern char D_001CE9E8[]; extern void DrawSync(); extern void VSync(); extern void SetDispMask(); extern void func_00024754(); extern void func_001C11E4(); extern void func_001C2114();
int func_001C6EC8(void){ DrawSync(0); VSync(0); func_001C11E4(); D_000967F4 = 0; func_001C2114(0, D_001CE9E8, 0); SetDispMask(0); func_00024754(); return 1; }
