/* Driver 2: DoVibrato */
typedef struct { char p0[0x10]; unsigned short vbtick; } MS;
typedef struct { char p0[0x50]; unsigned short period; char p1[0x10]; unsigned short tmpperiod; char p2[0xB]; unsigned char wavecontrol; unsigned char vibpos; unsigned char vibspd; unsigned char vibdepth; } XM;
extern XM *D_0009781C;
extern MS *D_00097814;
extern unsigned char D_0008D948[];
void func_0007178C(void)
{
	unsigned char q;
	unsigned short temp = 0;
	q = (D_0009781C->vibpos >> 2) & 0x1f;
	switch (D_0009781C->wavecontrol & 3) {
	case 0: temp = D_0008D948[q]; break;
	case 1:
		q <<= 3;
		if (D_0009781C->vibpos > 127) q = 255 - q;
		temp = q;
		break;
	case 2: temp = 255; break;
	}
	temp *= D_0009781C->vibdepth;
	temp >>= 7;
	temp <<= 2;
	if (D_0009781C->vibpos < 128)
		D_0009781C->period = D_0009781C->tmpperiod + temp;
	else
		D_0009781C->period = D_0009781C->tmpperiod - temp;
	if (D_00097814->vbtick) D_0009781C->vibpos += D_0009781C->vibspd;
}
