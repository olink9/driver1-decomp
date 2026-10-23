typedef struct { char pad[4]; unsigned char w; char pad2[3]; } GLYPH;
extern unsigned char D_000BF6EC[];
extern GLYPH D_000BF2EC[];
extern void PrintString();

void PrintStringRightAligned(unsigned char *s, int w, int c) {
    int sum = 0;
    unsigned char ch = *s;
    unsigned char *q = s + 1;
    unsigned char g;
    for (; ch != 0; ch = *q++) {
        if (ch == 0x20) {
            sum += 6;
        } else {
            g = D_000BF6EC[ch];
            if (g != 0xFF)
                sum += D_000BF2EC[g].w;
        }
    }
    PrintString(s, w - sum, c);
}
