/* func_00019314 */
typedef struct {
	char p0[0x1A]; short h1A; char p1[2]; short h1E; char p2[4];
	unsigned char b24; char p3; unsigned char b26; unsigned char b27; unsigned char b28;
} OBJ;
extern int car_data[];
extern long long D_00097044;
extern long long D_0009704C;
extern unsigned char D_0009E744[];
extern int func_00021558();
int func_00019314(OBJ *p, int dir)
{
	int r, idx;
	unsigned char *t;
	unsigned char v = dir;
	if (p->h1E != 0 || p->h1A != -1) {
	fail:
		return 0;
	}
	r = func_00021558(car_data[50], car_data[52]);
	if (r & 0xC000)
		goto fail;
	idx = r & 0xFFFF;
	if (*(short *)(*(int *)&D_00097044 + idx * 16 + 8) < 20)
		goto fail;
	if ((dir & 1) != *(unsigned char *)(*(int *)&D_0009704C + idx * 20 + 0x10))
		goto fail;
	t = D_0009E744 + idx * 20;
	p->b24 = t[v] + t[(v + 2) & 3];
	if (p->b24 == 0)
		goto fail;
	p->b26 = 0;
	p->h1A = r;
	p->b27 = dir;
	p->b28 = 1;
	return 1;
}
