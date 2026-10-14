/* Driver 2: DoToneSlide (xmplay.c) - REDRIVER2 code with abs() for ABS() */
typedef struct { char pad0[0x50]; unsigned short period; char pad1[0x62 - 0x52]; unsigned short tmpperiod; unsigned short wantedperiod; char pad2[0x68 - 0x66]; unsigned short portspeed; } XMCHANNEL;
extern XMCHANNEL *D_0009781C;
#define ABS(x) ((x) < 0 ? -(x) : (x))
int abs(int);
void func_00071724(void){ int dist;
 dist = D_0009781C->period - D_0009781C->wantedperiod;
 if (dist == 0 || D_0009781C->portspeed > abs(dist)) D_0009781C->period = D_0009781C->wantedperiod;
 else if (dist > 0) D_0009781C->period -= D_0009781C->portspeed; else D_0009781C->period += D_0009781C->portspeed;
 D_0009781C->tmpperiod = D_0009781C->period; }
