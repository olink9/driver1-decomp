typedef struct { char pad0[0xE]; unsigned short w0E; } E;
typedef struct { E *p; int pad[3]; int v[3]; unsigned short a; unsigned char b; char pad2; } T;
extern int D_0008A8F0[];
extern char D_000BBE84[];
extern int D_000A0E20[];
extern T D_000BE754;
extern int func_000398B0();
extern void func_00079D20();
extern void func_0004EAC0();
extern void func_0004E948();
extern void MulMatrix0();
extern void func_0002DCAC();

void func_0005347C(void)
{
	int vec[3];
	short ang[3];
	char mat[32];
	int trans[3];
	if (D_000BE754.p != 0) {
		if (func_000398B0(D_000BE754.v) == 1) {
			ang[0] = 0;
			ang[2] = 0;
			trans[0] = 0;
			trans[1] = 0;
			vec[0] = D_000BE754.v[0] - D_0008A8F0[0];
			vec[1] = D_000BE754.v[1] - D_0008A8F0[1];
			vec[2] = D_000BE754.v[2] - D_0008A8F0[2];
			ang[1] = D_000BE754.b;
			trans[2] = -((short)D_000BE754.a >> 2);
			func_0004E948(mat, ang);
			func_00079D20(mat);
			func_0004EAC0(trans);
			vec[0] += trans[0];
			vec[2] += trans[2];
			ang[0] = D_000BE754.a;
			func_0004E948(mat, ang);
			func_00079D20(D_000BBE84);
			func_0004EAC0(vec);
			MulMatrix0(D_000BBE84, mat, mat);
			__asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); lw $14, 8(%0); ctc2 $12, $5; ctc2 $13, $6; ctc2 $14, $7" : : "r"(vec) : "$12", "$13", "$14");
			func_00079D20(mat);
			func_0002DCAC(D_000A0E20[D_000BE754.p->w0E]);
		}
	}
}
