extern char D_00010678[];
extern int D_0008A988[];
extern unsigned char D_0008A96C[];
extern void LoadfileSeg();
extern void LoadSoundBank();

static inline void ld(int i, int mode)
{
    char *h = D_00010678;
    int *t = D_0008A988;
    int len, off;
    if (t[1] == 0)
        LoadfileSeg(h, t, 0, 0x2C);
    len = D_0008A988[i + 1] - D_0008A988[i];
    off = D_0008A988[i];
    LoadfileSeg(h, 0x80100000, off, len);
    LoadSoundBank(0x80100000, len, mode);
}

void func_0004C29C(int arg)
{
    unsigned char *p;
    unsigned int k;

    ld(0, 0);
    ld(1, 3);

    p = D_0008A96C;
    k = p[1];
    if (k != 0xFF) {
        while (k != 0xFF) {
            if (!(p[0] < arg))
                break;
            p += 2;
            k = p[1];
        }
    }
    if ((k & 0xFF) != 0xFF)
        ld((k & 0xFF) + 2, 4);
}
