typedef struct { unsigned char type, data; unsigned short nsectors; int sector; int addr; int pad; } SPOOLQ;
typedef struct { int a; int nsectors; int c, d, e; } RI;
extern SPOOLQ D_000A7BA4[];
extern RI D_000A7EA4[];
extern int D_000969EC, D_000971F0, D_000971F4, D_000971F8, D_000971FC, D_00097204;
extern int D_0009720C, D_00097210, D_00097220, D_00097224, D_00097260, D_00097268;
extern int D_00097274, D_00097280, D_000972B4, D_000972BC;
extern void data_cb_textures(), ready_cb_textures(), ready_cb_regions(), data_cb_regions();
extern void func_00034EE4(), ready_cb_soundbank(), data_cb_misc(), func_000352D4();
extern void func_0007A12C(), func_0007A27C(), func_0007A53C(), func_0007A580();

/* UpdateSpool */
void UpdateSpool(void)
{
	char pos[4];
	SPOOLQ *current;
	int sec;
	int n;

	current = &D_000A7BA4[D_000969EC];
	D_000971F0 = current->addr;

	switch (current->type) {
	case 0:
		D_00097260 = 5;
		n = D_000A7EA4[D_000972BC].nsectors;
		D_000971FC = current->nsectors;
		D_000971F8 = n;
		func_0007A53C(data_cb_regions);
		func_0007A12C(ready_cb_regions);
		break;
	case 1:
		D_00097260 = 5;
		D_0009720C = 0;
		D_00097210 = 0;
		D_000971F8 = 17;
		D_000971FC = 1;
		D_00097220 = D_000972B4;
		func_0007A53C(data_cb_textures);
		func_0007A12C(ready_cb_textures);
		D_000971F0 += 0x4000;
		break;
	case 3:
		D_00097260 = 5;
		D_000971F8 = current->nsectors;
		func_0007A53C(data_cb_misc);
		func_0007A12C(func_000352D4);
		break;
	case 2:
		D_00097260 = 5;
		D_00097274 = 0;
		D_0009720C = 0;
		D_00097210 = 0;
		D_000971FC = 2;
		D_000971F8 = current->nsectors;
		D_00097280 = current->data;
		func_0007A53C(func_00034EE4);
		func_0007A12C(ready_cb_soundbank);
		D_000971F0 += (D_00097268 & 1) << 12;
		break;
	}
	sec = current->sector;
	D_00097224 = 0;
	D_00097204 = 0;
	D_000971F4 = sec;
	func_0007A580(sec, pos);
	func_0007A27C(0x1B, pos);
}
