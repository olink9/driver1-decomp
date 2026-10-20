extern int D_0009E714[];
extern void func_00021558();

static inline int grid(int v) { return v / 1500 * 1500 + 750; }
static inline int snap(int v) { return v / 1500 * 1500; }
static inline int sub(int a, int b) { return a - b; }
void func_00060328(char *obj, int *pos)
{
	int x, z;
	x = grid(pos[0] + D_0009E714[2]) - D_0009E714[2];
	z = D_0009E714[3] - snap(sub(D_0009E714[3] + 750, pos[2]));
	func_00021558(x, z - 1500);
	func_00021558(x, z + 1500);
	func_00021558(x - 1500, z);
	func_00021558(x + 1500, z);
	switch (*(short *)(obj + 0x44)) {
	case 1:
		pos[0] = x - 550;
		pos[2] = z - 550;
	case 2:
		pos[0] = x + 550;
		pos[2] = z + 550;
		break;
	case 4:
		pos[0] = x - 350;
		pos[2] = z - 350;
		break;
	case 8:
		pos[0] = x + 350;
		pos[2] = z + 350;
		break;
	}
}
