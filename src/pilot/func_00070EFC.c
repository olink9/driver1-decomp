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
extern XMCH *D_0009781C;
extern int D_00097818;

void func_00070EFC(unsigned char note) {
    unsigned char *j;
    if (D_0009783C > 23) return;
    if (note == 96) {
        D_0009781C->keyon = 0;
        if (D_0009781C->sample == 254) {
            D_0009781C->tmpvolume = 64;
        } else {
            j = *(unsigned char **)(D_00097818 + D_0009781C->sample * 4 + 0x818);
            if (!(j[233] & 1))
                D_0009781C->tmpvolume = 64;
        }
    } else {
        D_0009781C->note = note;
        D_0009781C->kick = 1;
        if (!(D_0009781C->wavecontrol & 0x80)) D_0009781C->trmpos = 0;
        if (!(D_0009781C->wavecontrol & 0x08)) D_0009781C->vibpos = 0;
    }
}
