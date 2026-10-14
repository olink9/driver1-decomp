/* XM_Restart */
typedef struct {
    char pad0[0x19];
    unsigned char ChDead;
    char pad1a[0x38];
    unsigned short SPUPitch;
    char pad54[0x22];
    unsigned char SPUChannel;
    char pad77[1];
} XMCHN;
typedef struct {
    int pad0;
    int HeaderNum;
    char pad8[0x2C];
    unsigned char XMPlay;
    char pad35[0x23];
    XMCHN XM_Chnl[1];
} XMSONG;
typedef struct {
    char pad[0x14];
    unsigned short XMPSXChannels;
} XMHEADER;
extern short D_0008D218[];
extern XMSONG *D_000C7888[];
extern XMHEADER *D_000C7970[];
extern XMSONG *D_00097810;
extern XMHEADER *D_00097828;
extern XMCHN *D_0009781C;
extern void SpuSetVoicePitch();

void func_000734E8(int Song_ID)
{
    int t;

    if (D_0008D218[Song_ID] == -1)
        return;
    D_00097810 = D_000C7888[Song_ID];
    D_00097828 = D_000C7970[D_00097810->HeaderNum];
    if (D_00097810->XMPlay == 2) {
        D_00097810->XMPlay = 1;
        for (t = 0; t < D_00097828->XMPSXChannels; t++) {
            D_0009781C = &D_00097810->XM_Chnl[t];
            if (!D_0009781C->ChDead)
                SpuSetVoicePitch(D_0009781C->SPUChannel, D_0009781C->SPUPitch);
        }
    }
}
