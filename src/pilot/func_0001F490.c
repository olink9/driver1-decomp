void func_0001F490(short *p, int *q)
{
	int a, b, c, i, k;
	int *pi = (int *)p;
	char pad[16];
	a = p[4] * q[20];
	b = p[7] * q[21];
	c = p[10] * q[22];
	k = a + b + c; k = 440 * ((k + 0x800) >> 12);
	for (i = 2; i >= 0; i--) {
		q[20 + i] = q[20 + i] * 110 + ((p[4 + 3 * i] * k - (q[16 + i] << 7) + 0x800) >> 12);
		if (q[24] == 1)
			q[20 + i] -= q[16 + i] / 8;
		pi[26 + i] = q[20 + i];
		pi[22 + i] = q[12 + i];
	}
}
