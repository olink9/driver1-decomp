/* Driver 2: DoToneSlide (xmplay.c) - REDRIVER2 code with abs() for ABS() */
typedef struct { char pad0[0x50]; unsigned short period; char pad1[0x62 - 0x52]; unsigned short tmpperiod; unsigned short wantedperiod; char pad2[0x68 - 0x66]; unsigned short portspeed; } XMCHANNEL;
extern XMCHANNEL *XMC;
#define ABS(x) ((x) < 0 ? -(x) : (x))
int abs(int);
void DoToneSlide(void){ int dist;
 dist = XMC->period - XMC->wantedperiod;
 if (dist == 0 || XMC->portspeed > abs(dist)) XMC->period = XMC->wantedperiod;
 else if (dist > 0) XMC->period -= XMC->portspeed; else XMC->period += XMC->portspeed;
 XMC->tmpperiod = XMC->period; }
