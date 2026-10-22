/* XM_DoFullUpdate */
typedef struct { int pad0; int hdr; char pad1[0x2C]; unsigned char play; char pad2[0xB98 - 0x35]; int jup; } Song;
typedef struct { char pad[0x10]; unsigned short vb; } Dummy;
extern Song *D_000C7888[];
extern int D_000C7970[];
extern Song *D_00097814;
extern int D_00097818;
extern void UpdateEffs(), ApplyEffs(), UpdateHardware(), CurrentKeyStat(), UpdatePatternData();
void XM_DoFullUpdate(int sc)
{
    unsigned char s0;
    D_00097814 = D_000C7888[sc];
    s0 = D_00097814->play;
    if (s0 == 1) {
        D_00097818 = D_000C7970[D_00097814->hdr];
        if (D_00097814->jup == 0) {
            UpdateEffs();
            ApplyEffs();
        }
        D_00097814->jup = 0;
        UpdateHardware();
        if (*(unsigned short *)((char *)D_00097814 + 0x10) == s0)
            CurrentKeyStat();
        UpdatePatternData(sc);
    }
}
