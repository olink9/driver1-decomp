extern unsigned char D_000BF6EC[];
struct G { char p[4]; unsigned char w; char q[3]; };
extern struct G D_000BF2EC[];
extern void PrintString();

void func_0005C8F4(unsigned char *s, short y)
{
    int w = 0;
    unsigned char *p = s + 1;
    unsigned char ch = *s;
    while (ch != 0) {
        int c = ch;
        if (c == 0x20) {
            w += 6;
        } else {
            int k = D_000BF6EC[c];
            if (k != 0xFF)
                w += D_000BF2EC[k].w;
        }
        ch = *p++;
    }
    PrintString(s, (short)((512 - w) >> 1), y);
}
