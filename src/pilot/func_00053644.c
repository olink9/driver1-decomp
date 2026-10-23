extern int D_00096D18;
extern int D_000BE744[];
extern int camera_position[];
extern char inv_camera_matrix[];
extern long long CameraCnt;
extern int modelpointers[];
extern int func_000398B0();
extern void SetRotMatrix();
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
			vec[0] = D_000BE744[0] - camera_position[0];
			vec[1] = D_000BE744[1] - camera_position[1];
			vec[2] = D_000BE744[2] - camera_position[2];
			ang[1] = (*(int *)&CameraCnt * 4) & 0xFFF;
			SetRotMatrix(inv_camera_matrix);
			func_0004EAC0(vec);
			func_0004E948(mat, ang);
			MulMatrix0(inv_camera_matrix, mat, mat);
			__asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); lw $14, 8(%0); ctc2 $12, $5; ctc2 $13, $6; ctc2 $14, $7" : : "r"(vec) : "$12", "$13", "$14");
			SetRotMatrix(mat);
			func_0002DCAC(modelpointers[D_00096D18]);
		}
	}
}
