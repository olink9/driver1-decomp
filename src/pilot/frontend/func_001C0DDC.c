/* The callee returns int: its call_value sets v0, so v0 has 2 sets and sched1 gives `v0 = 3` no birthing boost. */
extern int D_001CDE5C; extern char D_001CDCD8[]; extern void func_001C2114(); extern int func_001C5A30();
int func_001C0DDC(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 3; func_001C5A30(); D_001CDE5C = 1; return 3; }
