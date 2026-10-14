extern int D_00096D18;
extern int D_000BE744[];
extern int D_0008A8F0[];
extern char D_000BBE84[];
extern long long D_000970D8;
extern int D_000A0E20[];
extern int func_000398B0();
extern void func_00079D20();
extern void func_0004EAC0();
extern void func_0004E948();
extern void MulMatrix0();
extern void func_0002DCAC();

void func_00053644(void)
{
	int vec[3];
	short ang[3];
	char mat[32];
	if (D_00096D18 != 0) {
		if (func_000398B0(D_000BE744) == 1) {
			ang[0] = 0;
			ang[2] = 0;
			vec[0] = D_000BE744[0] - D_0008A8F0[0];
			vec[1] = D_000BE744[1] - D_0008A8F0[1];
			vec[2] = D_000BE744[2] - D_0008A8F0[2];
			ang[1] = (*(int *)&D_000970D8 * 4) & 0xFFF;
			func_00079D20(D_000BBE84);
			func_0004EAC0(vec);
			func_0004E948(mat, ang);
			MulMatrix0(D_000BBE84, mat, mat);
			__asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); lw $14, 8(%0); ctc2 $12, $5; ctc2 $13, $6; ctc2 $14, $7" : : "r"(vec) : "$12", "$13", "$14");
			func_00079D20(mat);
			func_0002DCAC(D_000A0E20[D_00096D18]);
		}
	}
}
