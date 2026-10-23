/* WIP: control flow and stores right; target loads the return constant before 'li v1,1; sw v1' (return value held in a separate register from the stored 1). Not found in 5+ attempts. */
extern int D_001CDE5C, D_001CDE64; extern char D_001CDCD8[]; extern void func_001C2114(); extern void func_001C4D04();
int func_001C1014(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 4; D_001CDE64 = 0; func_001C4D04(); D_001CDE5C = 1; return 5; }
