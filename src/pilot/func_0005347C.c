typedef struct { char pad0[0xE]; unsigned short w0E; } E;
typedef struct { E *p; int pad[3]; int v[3]; unsigned short a; unsigned char b; char pad2; } T;
extern int camera_position[];
extern char inv_camera_matrix[];
extern int modelpointers[];
extern T D_000BE754;
extern int func_000398B0();
extern void SetRotMatrix();
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
			vec[0] = D_000BE754.v[0] - camera_position[0];
			vec[1] = D_000BE754.v[1] - camera_position[1];
			vec[2] = D_000BE754.v[2] - camera_position[2];
			ang[1] = D_000BE754.b;
			trans[2] = -((short)D_000BE754.a >> 2);
			func_0004E948(mat, ang);
			SetRotMatrix(mat);
			func_0004EAC0(trans);
			vec[0] += trans[0];
			vec[2] += trans[2];
			ang[0] = D_000BE754.a;
			func_0004E948(mat, ang);
			SetRotMatrix(inv_camera_matrix);
			func_0004EAC0(vec);
			MulMatrix0(inv_camera_matrix, mat, mat);
			__asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); lw $14, 8(%0); ctc2 $12, $5; ctc2 $13, $6; ctc2 $14, $7" : : "r"(vec) : "$12", "$13", "$14");
			SetRotMatrix(mat);
			func_0002DCAC(modelpointers[D_000BE754.p->w0E]);
		}
	}
}
