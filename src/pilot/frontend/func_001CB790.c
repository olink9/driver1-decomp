typedef struct {
    void (*enter)();
    void (*leave)();
    int (*select)();
    unsigned char up, down, left, right;
} MENUITEM;
extern unsigned short D_00097538;
extern short D_0009752C;
extern int D_001CDECC;

int func_001CB790(MENUITEM *m, int cur)
{
    int sel = cur;
    unsigned short pad = D_00097538;

    if (pad & 0x10)
        return -1;
    if (D_001CDECC) {
        if (D_0009752C & 0x8000)
            m[m[sel].up].enter();
        else if (D_0009752C & 0x2000)
            m[m[sel].down].enter();
        else if (pad & 0x1000)
            sel = m[sel].left;
        else if (pad & 0x4000)
            sel = m[sel].right;
        else if (pad & 0x40)
            sel = m[sel].select();
    } else {
        if (pad & 0x8000)
            sel = m[sel].up;
        else if (pad & 0x2000)
            sel = m[sel].down;
        else if (pad & 0x1000)
            sel = m[sel].left;
        else if (pad & 0x4000)
            sel = m[sel].right;
        else if (pad & 0x40)
            m[sel].select();
    }
    if (cur != sel) {
        m[cur].leave();
        m[sel].enter();
    }
    return sel;
}
