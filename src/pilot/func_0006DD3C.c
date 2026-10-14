typedef struct { int a, b, c, d; } Q;
extern int D_00096F84, D_000977D8, D_00096F80;
extern char D_00097658[], D_00096897[];
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
	int i;
	D_00096F84 = 0;
	D_000977D8 = 0;
	D_00097658[0] = 0;
	D_00096897[0] = 5;
	sprintf(buf, D_00011640, x / 10, x);
	func_00050E28(buf, 0xD0200);
	D_00096F84 = *(unsigned char *)0xD0200;
	p = (int *)0xD0204;
	for (i = 0; i < D_00096F84; i++) {
		D_000C71EC[i].a = p[0];
		D_000C71EC[i].b = p[1];
		D_000C71EC[i].c = p[2];
		p += 4;
	}
	for (i = 0; i < D_00096F84; i++) {
		D_000C74CC[i].a = p[0];
		D_000C74CC[i].b = p[1];
		D_000C74CC[i].c = p[2];
		p += 4;
	}
	*(int *)&D_00096A60 = 0x640;
	*(int *)&D_00096A58 = (*(int *)&D_0009B19C + 0x800) & 0xfff;
	func_0006DEC0();
	D_00096F80 = 2;
	if (x == 13)
		D_00096F84 -= 2;
}
