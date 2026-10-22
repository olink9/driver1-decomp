typedef struct { char c[4]; } COL;
extern long long CameraCnt;
extern void memset();
extern int func_000776E4();
extern void GetSmokeDrift();
extern void Setup_Smoke();

void func_0004A718(int *p)
{
	int v[3];
	int w[3];
	COL col2;
	COL col;
	memset(&col, 0, 4);
	col.c[0] = (func_000776E4() & 0xF) + 0x10;
	col.c[1] = func_000776E4() & 0xF;
	col2 = col;
	if ((*(int *)&CameraCnt & 7) == 0) {
		v[0] = p[0] - 0x20;
		v[2] = p[2] - 0x20;
		v[1] = p[1] - 0x30;
		v[0] += func_000776E4() & 0x1F;
		v[2] += func_000776E4() & 0x1F;
		GetSmokeDrift(w);
		w[1] = 3;
		w[0] >>= 1;
		w[2] >>= 1;
		Setup_Smoke(v, 50, 400, 6, 1, w, 0);
	}
}
