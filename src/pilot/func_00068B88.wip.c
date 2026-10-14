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
    E *p, *q;
    int v;
    for (p = D_0008BD58, q = D_0008BD58; p < D_0008BD58 + 0x49; p++, q++) {
        if (p->st == 1) {
            int *a = p->w2;
            if (q->st == 1) {
                q->nx = a;
            } else if (q->nx) {
                v = a[-1];
                *q->nx = v;
                if (q == D_0008BD58 + 0x1f) D_000BE9C4.h = v;
                else if (q == D_0008BD58 + 0xa) D_000BE9E0.h = v;
            }
        } else {
            if (q->st == 1) {
                q->nx = &p[1].st;
            } else if (q->nx) {
                v = p->w3;
                *q->nx = v;
                if (q == D_0008BD58 + 0x1f) D_000BE9C4.h = v;
                else if (q == D_0008BD58 + 0xa) D_000BE9E0.h = v;
            }
        }
    }
}
