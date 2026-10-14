typedef struct {
    char pad0[0x34];
    unsigned char XMPlay;
    char pad1[0x50 - 0x35];
    int PlayMask;
} XMSONG;

extern short D_0008D218[];      /* Driver 2: XMSongIDs */
extern XMSONG *D_000C7888[];    /* Driver 2: XM_SngAddress */
extern XMSONG *D_00097810;      /* Driver 2: mu */

/* Driver 2: XM_PlayStart (xmplay.c) */
void func_0006FA5C(int Song_ID, int PlayMask)
{
    if (D_0008D218[Song_ID] == -1)
        return;
    if (PlayMask != 0) {
        D_00097810 = D_000C7888[Song_ID];
        D_00097810->PlayMask = PlayMask;
        D_00097810->XMPlay = 1;
    }
}
