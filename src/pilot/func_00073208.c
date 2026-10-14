/* CurrentKeyStat */
struct MH { char pad[0x14]; unsigned short n; };
struct Ch { char pad[0x18]; unsigned char nothing; unsigned char dead; char pad2[0x76 - 0x1A]; unsigned char spu; char pad3[0x78 - 0x77]; };
struct MS { char pad[0x58]; struct Ch ch[1]; };
extern struct MS *D_00097814;
extern struct MH *D_00097818;
extern struct Ch *D_0009781C;
extern void func_0007EF18();

void func_00073208(void) {
    int t;
    char key[24];
    func_0007EF18(key);
    for (t = 0; t < D_00097818->n; t++) {
        D_0009781C = &D_00097814->ch[t];
        if (key[D_0009781C->spu] != 1 && D_0009781C->dead == 0) {
            D_0009781C->dead = 1;
            D_0009781C->nothing = 1;
        }
    }
}
