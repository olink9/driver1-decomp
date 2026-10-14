/* XM_Pause (Driver 2) */
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
    char pad[0x34 - 8];
    unsigned char play;
    char pad2[0x58 - 0x35];
    CH ch[24];
} SONG;
extern short D_0008D218[];
extern SONG *D_000C7888[];
extern HDR *D_000C7970[];
extern SONG *D_00097810;
extern HDR *D_00097828;
extern CH *D_0009781C;
extern void SpuSetVoicePitch();

void func_00073408(int id) {
    int t;
    if (D_0008D218[id] == -1) return;
    D_00097810 = D_000C7888[id];
    D_00097828 = D_000C7970[D_00097810->hdr];
    if (D_00097810->play == 1) {
        D_00097810->play = 2;
        for (t = 0; t < D_00097828->n; t++) {
            D_0009781C = &D_00097810->ch[t];
            SpuSetVoicePitch(D_0009781C->spu, 0);
        }
    }
}
