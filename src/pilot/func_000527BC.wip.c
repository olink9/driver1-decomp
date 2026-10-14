typedef struct { int a, b, c; } E;
extern long long D_0009683C;
extern int D_000A1AB0[], D_000A1AD0[], D_000A1AE0[];
extern int func_00052944();
extern void func_0003BBF8();
extern void func_00052620();

void func_000527BC(char *base)
{
	int k;
	int i, j;
	E *e;
	int t;
	for (k = 0; k < 4; k++) {
		D_000A1AB0[k] = 0;
		D_000A1AD0[k] = 0;
		D_000A1AE0[k] = 0;
	}
	base += 4;
	j = 1;
	for (i = 0; i < 6; i++) {
		if (i == 5) {
			j = 0;
			i = *(int *)&D_0009683C + 5;
		}
		e = (E *)(i * 12 + (int)base);
		if (e->a != -1)
			D_000A1AB0[j] = func_00052944(base + (e->a + 0x9C), 0, 1);
		if (e->b != -1)
			D_000A1AD0[j] = func_00052944(base + (e->b + 0x9C), 0, 1);
		t = e->c;
		if (t != -1)
			D_000A1AE0[j] = func_00052944(base + (t + 0x9C), 1, 1);
		j++;
	}
	func_0003BBF8();
	func_00052620();
}
