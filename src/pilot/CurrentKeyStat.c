/* CurrentKeyStat */
struct MH { char pad[0x14]; unsigned short n; };
struct Ch { char pad[0x18]; unsigned char nothing; unsigned char dead; char pad2[0x76 - 0x1A]; unsigned char spu; char pad3[0x78 - 0x77]; };
struct MS { char pad[0x58]; struct Ch ch[1]; };
extern struct MS *D_00097814;
extern struct MH *D_00097818;
extern struct Ch *XMC;
extern void SpuGetAllKeysStatus();

void CurrentKeyStat(void) {
    int t;
    char key[24];
    SpuGetAllKeysStatus(key);
    for (t = 0; t < D_00097818->n; t++) {
        XMC = &D_00097814->ch[t];
        if (key[XMC->spu] != 1 && XMC->dead == 0) {
            XMC->dead = 1;
            XMC->nothing = 1;
        }
    }
}
