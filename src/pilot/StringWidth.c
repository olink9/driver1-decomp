extern unsigned char D_000BF6EC[];
extern struct { char pad[4]; unsigned char width; char pad2[3]; } D_000BF2EC[];

int StringWidth(unsigned char *s) {
    int w = 0;
    unsigned char let;
    unsigned char c;
    while ((let = *s++) != 0) {
        if (let == 32) {
            w += 6;
        } else {
            c = D_000BF6EC[let];
            if (c != 0xFF) {
                w += D_000BF2EC[c].width;
            }
        }
    }
    return w;
}
