extern short D_000978B0[];
extern short rcossin_tbl[];
void func_0001250C(void)
{
	short *out = D_000978B0;
	int i = 0;
	short *tab = rcossin_tbl;
	do {
int s = tab[((i * 4) & 0xFFF) * 2]; int c = tab[((i * 4 + 0x400) & 0xFFF) * 2];
		int a = c*180, na = -a, b = s*450, a2 = s*180, na2 = -a2, c2 = c*450, nb = -b, nc2 = -c2;
		i++;
		out[0] = (na - b) >> 12;
		out[1] = (na2 + c2) >> 12;
		out += 2;
		out[0] = (a - b) >> 12;
		out[1] = (a2 + c2) >> 12;
		out += 2;
		out[0] = (na - nb) >> 12;
		out[1] = (na2 + nc2) >> 12;
		out += 2;
		out[0] = (a - nb) >> 12;
		out[1] = (a2 + nc2) >> 12;
		out += 2;
	} while (i < 256);
}
