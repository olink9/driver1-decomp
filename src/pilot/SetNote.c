/* SetNote (Driver 2) */
typedef struct {
    char p0[0x40];
    unsigned char keyon;      /* 40 */
    unsigned char kick;       /* 41 */
    unsigned char sample;     /* 42 */
    char p1[0x12];
    unsigned char note;       /* 55 */
    char p2[0xa];
    unsigned char tmpvolume;  /* 60 */
    char p3[0xe];
    unsigned char wavecontrol;/* 6F */
    unsigned char vibpos;     /* 70 */
    char p4[2];
    unsigned char trmpos;     /* 73 */
} XMCH;
typedef struct {
    char p[0x818];
    unsigned char **JAP_InstrumentOffset;
} XMH;
extern int D_0009783C;
extern XMCH *XMC;
extern int D_00097818;

void SetNote(unsigned char note) {
    unsigned char *j;
    if (D_0009783C > 23) return;
    if (note == 96) {
        XMC->keyon = 0;
        if (XMC->sample == 254) {
            XMC->tmpvolume = 64;
        } else {
            j = *(unsigned char **)(D_00097818 + XMC->sample * 4 + 0x818);
            if (!(j[233] & 1))
                XMC->tmpvolume = 64;
        }
    } else {
        XMC->note = note;
        XMC->kick = 1;
        if (!(XMC->wavecontrol & 0x80)) XMC->trmpos = 0;
        if (!(XMC->wavecontrol & 0x08)) XMC->vibpos = 0;
    }
}
