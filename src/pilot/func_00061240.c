typedef struct { int vx, vy, vz; } V3;
extern char D_000976C8; extern V3 D_0008A8F0; extern long long D_00096897; extern long long D_000970D8; extern int D_0008B9F8[]; void func_000614DC(int);
int func_00061240(unsigned char *p){ int r; unsigned char c; *(short *)(p + 0x6C) = *(short *)(p + 0x34); r = p[0xD0];
 if (r == 0) { D_000976C8 = 1; c = p[0x90]; D_0008A8F0.vx = *(int *)(p + 0x84); D_0008A8F0.vy = *(int *)(p + 0x88); D_0008A8F0.vz = *(int *)(p + 0x8C); *(unsigned char *)&D_00096897 = c; p[0xD1] = 0; }
 else func_000614DC(D_0008B9F8[(unsigned char)(*(int *)&D_000970D8 % 4)]); return r; }
