/* Driver 2: GetEngineRevs */
typedef struct { int lowidl_ws, low_ws, hi_ws, ratio_ac, ratio_id; } GEAR_DESC;
extern GEAR_DESC D_0008A9B4[];
typedef struct { char pad0[0xF0]; int acc; char pad1[0x10]; int ws; char pad2[0x7C]; unsigned char gear; } CAR;
unsigned short GetEngineRevs(CAR *cp)
{
	int gear, old, ws, acc;
	GEAR_DESC *gd;
	gear = cp->gear;
	ws = cp->ws;
	acc = cp->acc;
	if (ws > 0) {
		ws >>= 11;
		if (gear < 0) gear = 0;
		if (gear > 3) gear = 3;
		{
			/* D2: gd = &geard[type][gear] - the row base is computed first */
			GEAR_DESC *base = D_0008A9B4;
			gd = base + gear;
		}
		do {
			old = gear;
			if (acc > 0 ? ws < gd->low_ws : ws < gd->lowidl_ws) { gd--; gear--; }
			if (gd->hi_ws < ws) { gd++; gear++; }
		} while (old != gear);
	} else {
		ws = -ws / 2048;
		gear = 0;
	}
	cp->gear = gear;
	if (acc == 0) return D_0008A9B4[gear].ratio_id * ws;
	return D_0008A9B4[gear].ratio_ac * ws;
}
