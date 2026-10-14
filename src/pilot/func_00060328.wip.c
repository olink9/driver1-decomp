extern int D_0009E714[];
extern void func_00021558();

void func_00060328(char *obj, int *pos)
{
	int x, z, t;
	x = (pos[0] + D_0009E714[2]) / 1500 * 1500 + 750;
	x -= D_0009E714[2];
	t = D_0009E714[3] + 750;
	t -= pos[2];
	z = D_0009E714[3] - t / 1500 * 1500;
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
