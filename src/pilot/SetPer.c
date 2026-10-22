/* SetPer (Driver 2) */
typedef struct {
    char p0[6]; short OldLVol; short OldRVol; short OldPeriod; unsigned char OldSample; char p1[7];
    int SOffset; char p2[2];
    unsigned char panenvflg, panenvpts, panenvsus, panenvbeg, panenvend; char p3;
    short panenvp, panenva, panenvb; unsigned short keyoffspd;
    unsigned char envflg, envpts, envsus, envbeg, envend; char p4;
    short envp, enva, envb; char p5[14];
    unsigned char sample; char p6[17];
    unsigned char transpose, note; char p7[8];
    unsigned short c2spd; short p8;
    unsigned short tmpperiod, wantedperiod;
} XM;
typedef struct { char p[0x30]; int flags; } MH0;
typedef struct { char p[0x818]; unsigned char *inst[1]; } MH;
extern int D_0009783C;
extern XM *XMC;
extern MH *D_00097818;
extern MH0 *D_00097814;
extern int func_0006F81C();
static inline unsigned short GetPeriod(unsigned short fine, unsigned char note) {
    int t;
    if (D_00097814->flags == 1) {
        t = 7680 - ((short)fine / 2);
        return t - note * 64 + 64;
    }
    return func_0006F81C(note, fine);
}
#define XMC XMC
#define mh D_00097818

void SetPer(void) {
    unsigned char *j;
    unsigned short period;
    unsigned char a;

    if (D_0009783C > 23) return;
    a = XMC->note;
    a += XMC->transpose;
    period = GetPeriod(XMC->c2spd, a);
    XMC->wantedperiod = period;
    XMC->tmpperiod = period;
    XMC->OldPeriod = 0;
    XMC->SOffset = 0;
    XMC->OldLVol = -1;
    XMC->OldRVol = -1;
    if (XMC->OldSample != XMC->sample) {
        XMC->OldSample = XMC->sample;
        j = mh->inst[XMC->sample];
        XMC->keyoffspd = j[239];
        XMC->keyoffspd *= 4;
        XMC->envflg = j[233];
        if (XMC->envflg & 1) {
            XMC->envpts = j[225];
            XMC->envsus = j[227];
            XMC->envbeg = j[228];
            XMC->envend = j[229];
        }
        XMC->panenvflg = j[234];
        if (XMC->panenvflg & 1) {
            XMC->panenvpts = j[226];
            XMC->panenvsus = j[230];
            XMC->panenvbeg = j[231];
            XMC->panenvend = j[232];
        }
    }
    if (XMC->panenvflg & 1) {
        XMC->panenvp = 0;
        XMC->panenva = 0;
        XMC->panenvb = 1;
    }
    if (XMC->envflg & 1) {
        XMC->envp = 0;
        XMC->enva = 0;
        XMC->envb = 1;
    }
}
