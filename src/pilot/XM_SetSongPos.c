/* XM_SetSongPos (Driver 2) */
typedef struct {
    char pad[0x40];
    unsigned char keyon;
    char pad1[0x1f];
    unsigned char tmpvolume;
    char pad2[0x17];
} CH;
typedef struct {
    char pad[2];
    unsigned short songlength;
    char pad2[0x0e];
    unsigned short nch;
} HDR;
typedef struct {
    int a;
    int hdr;
    char padx[8];
    unsigned short vbtick;   /* 10 */
    unsigned short patbrk;   /* 12 */
    char pad[4];
    unsigned short posjmp;   /* 18 */
    char pad1[2];
    unsigned short songpos;  /* 1C */
    char pad2[2];
    unsigned short speed;    /* 20 */
    char pad3[0x58 - 0x22];
    CH ch[24];
} SONG;
extern short D_0008D218[];
extern SONG *D_000C7888[];
extern HDR *D_000C7970[];
extern SONG *D_00097810;
extern HDR *D_00097828;
extern CH *XMC;
extern int D_00097844;

void XM_SetSongPos(int id, unsigned short pos) {
    int t;
    if (D_0008D218[id] == -1) return;
    D_00097810 = D_000C7888[id];
    D_00097828 = D_000C7970[D_00097810->hdr];
    if (pos >= D_00097828->songlength) return;
    D_00097844 = 1;
    for (t = 0; t < D_00097828->nch; t++) {
        XMC = &D_00097810->ch[t];
        XMC->keyon = 0;
        XMC->tmpvolume = 64;
    }
    D_00097810->posjmp = 1;
    D_00097810->patbrk = 0;
    D_00097810->songpos = pos;
    D_00097810->vbtick = D_00097810->speed;
    D_00097844 = 0;
}
