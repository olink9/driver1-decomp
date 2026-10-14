extern char D_000976C8; extern int D_0008A8F0[]; extern unsigned char D_00096897[8]; extern int D_000970D8[]; extern int D_0008B9F8[]; void func_000614DC(int);
int func_00061240(unsigned char *p){ int r; *(short *)(p + 0x6C) = *(short *)(p + 0x34); r = p[0xD0];
 if (r == 0) { D_000976C8 = 1; D_00096897[0] = p[0x90]; D_0008A8F0[0] = *(int *)(p + 0x84); D_0008A8F0[1] = *(int *)(p + 0x88); D_0008A8F0[2] = *(int *)(p + 0x8C); p[0xD1] = 0; }
 else func_000614DC(D_0008B9F8[(unsigned char)(D_000970D8[0] % 4)]); return r; }
