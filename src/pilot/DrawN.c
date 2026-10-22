#include "common.h"
typedef struct { int x, y; } XP;
typedef struct { unsigned long *ot; } DBX;
typedef struct { char *primptr; unsigned long *ot; } DBY;
extern XP D_0008B218[];
extern long long current;
extern long long gNight;
#define current (*(DBY **)&current)
extern void DrawPrim();

/* DrawN */
void DrawN(VECTOR *pScreenPosition, int direct)
{
	unsigned char i;
	LINE_F2 *linef2;
	XP *pPoint;
	XP lastPoint;

	pPoint = D_0008B218;
	i = 0;
	lastPoint.x = pPoint[-1].x + pScreenPosition->vx;
	lastPoint.y = pPoint[-1].y + pScreenPosition->vz;
	do {
		linef2 = (LINE_F2 *)*(long *)current;
		setLineF2(linef2);
		linef2->x0 = lastPoint.x;
		linef2->y0 = lastPoint.y;
		lastPoint.x = pPoint->x + pScreenPosition->vx;
		lastPoint.y = pPoint->y + pScreenPosition->vz;
		linef2->x1 = lastPoint.x;
		linef2->y1 = lastPoint.y;
		if (*(int *)&gNight) {
			linef2->r0 = 75; linef2->g0 = 75; linef2->b0 = 75;
		} else {
			linef2->r0 = 96; linef2->g0 = 96; linef2->b0 = 96;
		}
		setSemiTrans(linef2, 1);
		linef2->code = linef2->code | 2;
		if (direct != 0) {
			DrawPrim(linef2);
		} else {
			addPrim(current->ot, linef2);
		}
		current->primptr += sizeof(LINE_F2);
		i++;
		pPoint++;
	} while (i < 3);
}
