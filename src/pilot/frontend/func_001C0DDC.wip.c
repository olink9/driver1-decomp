/* WIP: control flow and stores right; target loads the return constant before 'li v1,1; sw v1' (return value held in a separate register from the stored 1). Not found in 5+ attempts. */
extern int D_001CDE5C; extern char D_001CDCD8[]; extern void func_001C2114(); extern void func_001C5A30();
int func_001C0DDC(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 3; func_001C5A30(); D_001CDE5C = 1; return 3; }
