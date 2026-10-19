typedef struct E {
    int st;
    int *nx;
    int *w2;
    int w3;
} E;
typedef struct { char pad[0xE]; short h; } H;
extern E D_0008BD58[];
extern H D_000BE9C4;
extern H D_000BE9E0;

void func_00068B88(void) {
    E *p; int i;
    int v;
    int *n;
    for (p = D_0008BD58, i = 0; p < D_0008BD58 + 0x49; i++, p++) {
        if (p->st == 1) {
            int *a = p->w2;
            if (D_0008BD58[i].st == 1) {
                D_0008BD58[i].nx = a;
            } else {
                n = D_0008BD58[i].nx;
                v = a[-1];
                if (n) {
                *n = v;
                if (&D_0008BD58[i] == D_0008BD58 + 0x1f) D_000BE9C4.h = v;
                else if (&D_0008BD58[i] == D_0008BD58 + 0xa) D_000BE9E0.h = v;
                }
            }
        } else {
            if (D_0008BD58[i].st == 1) {
                D_0008BD58[i].nx = (int *)(p + 1);
            } else {
                n = D_0008BD58[i].nx;
                v = p->w3;
                if (n) {
                *n = v;
                if (&D_0008BD58[i] == D_0008BD58 + 0x1f) D_000BE9C4.h = v;
                else if (&D_0008BD58[i] == D_0008BD58 + 0xa) D_000BE9E0.h = v;
                }
            }
        }
    }
}
