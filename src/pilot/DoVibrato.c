/* Driver 2: DoVibrato */
typedef struct { char p0[0x10]; unsigned short vbtick; } MS;
typedef struct { char p0[0x50]; unsigned short period; char p1[0x10]; unsigned short tmpperiod; char p2[0xB]; unsigned char wavecontrol; unsigned char vibpos; unsigned char vibspd; unsigned char vibdepth; } XM;
extern XM *XMC;
extern MS *D_00097814;
extern unsigned char D_0008D948[];
void DoVibrato(void)
{
	unsigned char q;
	unsigned short temp = 0;
	q = (XMC->vibpos >> 2) & 0x1f;
	switch (XMC->wavecontrol & 3) {
	case 0: temp = D_0008D948[q]; break;
	case 1:
		q <<= 3;
		if (XMC->vibpos > 127) q = 255 - q;
		temp = q;
		break;
	case 2: temp = 255; break;
	}
	temp *= XMC->vibdepth;
	temp >>= 7;
	temp <<= 2;
	if (XMC->vibpos < 128)
		XMC->period = XMC->tmpperiod + temp;
	else
		XMC->period = XMC->tmpperiod - temp;
	if (D_00097814->vbtick) XMC->vibpos += XMC->vibspd;
}
