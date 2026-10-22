typedef struct { int mask; int mode; short l; short r; int delay; int feedback; } RA;
extern int SpuReserveReverbWorkArea(int);
extern void SpuSetReverbModeParam(RA *);
extern void SpuSetReverbDepth(RA *);
void AllocateReverb(int a, short b)
{
    RA attr;
    if (SpuReserveReverbWorkArea(1) == 1) {
        attr.mask = 7;
        attr.mode = a | 0x100;
        attr.l = b;
        attr.r = b;
        SpuSetReverbModeParam(&attr);
        SpuSetReverbDepth(&attr);
    }
}
