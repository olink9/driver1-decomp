typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    P_TAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;
typedef struct { char *primptr; P_TAG *ot; } DB;
extern DB *current;
extern POLY_F4 D_000EF9A0;
extern int D_000ED7CC;
extern int D_00096E34;
extern unsigned short D_00097538;
extern void func_000514E0(), func_000EC260(), memset(), func_0007EC98(), func_0005C8F4();
extern void func_000E91B8(), func_000E90D8();
extern int func_0007EC88();

/* message box: split text at '\n' into up to 3 lines and show it (wait for a button if flag) */
void func_000EAE3C(char *text, int wait)
{
    char lines[3][0x60];
    POLY_F4 *p;
    int n;
    int i, nl, len, k;
    char *s, *start;

    p = &D_000EF9A0;
    ((P_TAG *)p)->len = 5;
    p->code = 0x28;
    p->r0 = 16;
    p->g0 = 16;
    p->b0 = 16;
    p->x0 = 0x56;
    p->y0 = 0x5C;
    p->x1 = 0x1B2;
    p->y1 = 0x5C;
    p->x2 = 0x56;
    p->y2 = 0x94;
    p->x3 = 0x1B2;
    p->y3 = 0x94;
    p->code = 0x29;
    if (wait)
        D_000ED7CC = 1;
    for (n = 0;; n++) {
        if (wait) {
            func_000514E0();
            if (D_00097538 & 0x40) {
                D_000ED7CC = 0;
                func_000EC260();
                return;
            }
        } else if (n >= 2)
            return;
        i = 0;
        nl = 0;
        len = 0;
        s = text;
        D_000EF9A0.tag.addr = current->ot[3].addr;
        current->ot[3].addr = (unsigned int)&D_000EF9A0;
        D_00096E34 = 0;
        memset(lines[0], 0, 0x28);
        memset(lines[1], 0, 0x28);
        memset(lines[2], 0, 0x28);
        start = text;
        while (i < func_0007EC88(text)) {
            if (*s == '\n') {
                func_0007EC98(lines[nl], start, len);
                start = text + (i + 1);
                nl++;
                len = 0;
                s++;
            } else {
                len++;
                i++;
                s++;
            }
        }
        if (nl > 0) {
            func_0007EC98(lines[nl], start + 1, len);
            nl++;
        }
        if (nl != 0) {
            for (k = 0; k < nl; k++)
                func_0005C8F4(lines[k], 100 + k * 14);
        } else
            func_0005C8F4(text, 100);
        D_00096E34 = 4;
        func_000E91B8();
        func_000E90D8();
    }
}
