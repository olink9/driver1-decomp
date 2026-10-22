struct Ch { char pad[0x78]; };
struct MS { char pad[0x4C]; unsigned char *f4C; char pad2[0x58 - 0x50]; struct Ch ch[1]; };
extern struct MS *D_00097814;
extern struct Ch *D_0009781C;
extern int D_0009783C;
extern int JCalcPat();
int func_0007220C(int n) {
    int i;
    int off = 0;
    unsigned char c;
    for (i = 0; i < n; i++) {
        do {
            c = D_00097814->f4C[off++];
            if (c != 0xFF) {
                D_0009783C = c;
                D_0009781C = &D_00097814->ch[c];
                off += JCalcPat(D_00097814->f4C + off);
            }
        } while (c != 0xFF);
    }
    return off;
}
