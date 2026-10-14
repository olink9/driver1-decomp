/* SilenceXM (Driver 2) */
typedef struct {
    char pad[0x76];
    unsigned char spu;
    char pad2[1];
} CH;
typedef struct {
    char pad[0x14];
    unsigned short n;
} HDR;
typedef struct {
    int a;
    int hdr;
    char pad[0x50 - 8];
    int mask;
    char pad2[4];
    CH ch[24];
} SONG;
extern SONG *D_000C7888[];
extern HDR *D_000C7970[];
extern SONG *D_00097810;
extern HDR *D_00097828;
extern CH *D_0009781C;
extern void SpuSetVoiceVolume();
extern void SpuSetKey();

void func_00073324(int id) {
    int t, i, c, pmsk;
    D_00097810 = D_000C7888[id];
    D_00097828 = D_000C7970[D_00097810->hdr];
    i = 0;
    for (t = 0; t < D_00097828->n; t++) {
        pmsk = D_00097810->mask;
        pmsk &= (1 << t);
        if (pmsk != 0) {
            D_0009781C = &D_00097810->ch[t];
            c = D_0009781C->spu;
            i |= 1 << c;
            SpuSetVoiceVolume(c, 0, 0);
        }
    }
    SpuSetKey(0, i);
}
