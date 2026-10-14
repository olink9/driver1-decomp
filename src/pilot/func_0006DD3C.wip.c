typedef struct { int a, b, c, d; } Q;
extern int D_00096F84, D_000977D8, D_00096F80;
extern long long D_00097658, D_00096897;
extern Q D_000C71EC[], D_000C74CC[];
extern long long D_0009B19C, D_00096A60, D_00096A58;
extern char D_00011640[];
extern int sprintf();
extern void func_00050E28();
extern void func_0006DEC0();

void func_0006DD3C(int x)
{
	char buf[32];
	int *p;
	int q;
	int i;
	q = x / 10;
	D_00096F84 = 0;
	D_000977D8 = 0;
	*(char *)&D_00097658 = 0;
	p = (int *)0xD0204;
	*(char *)&D_00096897 = 5;
	sprintf(buf, D_00011640, q, x);
	func_00050E28(buf, 0xD0200);
	D_00096F84 = *(unsigned char *)0xD0200;
	for (i = 0; i < D_00096F84; i++) {
		D_000C71EC[i].a = *p++;
		D_000C71EC[i].b = *p++;
		D_000C71EC[i].c = *p++;
		p++;
	}
	for (i = 0; i < D_00096F84; i++) {
		D_000C74CC[i].a = *p++;
		D_000C74CC[i].b = *p++;
		D_000C74CC[i].c = *p++;
		p++;
	}
	*(int *)&D_00096A60 = 0x640;
	*(int *)&D_00096A58 = (*(int *)&D_0009B19C + 0x800) & 0xfff;
	func_0006DEC0();
	D_00096F80 = 2;
	if (x == 13)
		D_00096F84 -= 2;
}
