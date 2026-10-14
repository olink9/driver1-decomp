typedef struct { int x, y, z; } V3;
typedef struct { int a, b; } P8;
typedef struct { int a, b, c, d; } Q16;
typedef struct { char pad[16]; unsigned char idx; char rest[63]; } E80;
typedef struct { char c[100]; } E100;
typedef struct { char c[32]; } E32;
extern char D_0009B0A4[];
extern E80 D_00098950[];
extern E32 D_000AF6A0[];
extern E100 D_0009AF04[];
extern E100 D_0009AF68[];
extern Q16 D_000A211C[];
extern P8 D_0009B094[];
extern void func_00079D20();
extern void func_0004EAC0();
extern int func_0001F5B0();

void func_000299B8(int n)
{
	V3 v;
	char pad[32];
	char *s5, *s6;
	P8 *p8;
	int i;
	Q16 *p, *q;

	s6 = D_0009B0A4;
	if (n != 0) {
		s6 += n * 396;
		s5 = (char *)&D_0009AF04[D_00098950[n - 1].idx];
		func_00079D20(&D_000AF6A0[n]);
		n = 2;
	} else {
		func_00079D20(D_000AF6A0);
		s5 = (char *)D_0009AF68;
	}
	i = 0;
	q = D_000A211C;
	p8 = D_0009B094;
	for (; i < 4; p8++, i += 2) {
		if (i & 2)
			v.x = *(short *)(s5 + i * 8 + 0x40) + 0x19;
		else
			v.x = *(short *)(s5 + i * 8 + 0x40);
		v.y = 0;
		v.z = -*(short *)(s5 + (((i + 1) & 3) << 3) + 0x44);
		func_0004EAC0(&v);
		v.x += *(int *)(s6 + 0xC8);
		v.z += *(int *)(s6 + 0xD0);
		p = &q[(i >> 1) + n];
		p->a = v.x;
		p->c = v.z;
		if (n != 0)
			p->b = func_0001F5B0(&v);
		else
			q[i >> 1].b = p8->a;
	}
}
