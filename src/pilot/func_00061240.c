typedef struct { int vx, vy, vz; } V3;
extern char D_000976C8; extern V3 camera_position; extern long long cameraview; extern long long CameraCnt; extern int D_0008B9F8[]; void func_000614DC(int);
int func_00061240(unsigned char *p){ int r; unsigned char c; *(short *)(p + 0x6C) = *(short *)(p + 0x34); r = p[0xD0];
 if (r == 0) { D_000976C8 = 1; c = p[0x90]; camera_position.vx = *(int *)(p + 0x84); camera_position.vy = *(int *)(p + 0x88); camera_position.vz = *(int *)(p + 0x8C); *(unsigned char *)&cameraview = c; p[0xD1] = 0; }
 else func_000614DC(D_0008B9F8[(unsigned char)(*(int *)&CameraCnt % 4)]); return r; }
