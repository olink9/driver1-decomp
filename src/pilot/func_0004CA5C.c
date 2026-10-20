/* car sound channel allocation (no Driver 2 counterpart listed) */
typedef struct {
    char pad0[3];
    unsigned char f3;
    char pad4[0xC8 - 4];
    int pos[4];
    int vel[4];
    char padE8[0x12E - 0xE8];
    unsigned char f12E;
    char pad12F[0x180 - 0x12F];
    short f180;
    short f182;
    char pad184[0x18C - 0x184];
} CAR;

typedef struct {
    char pad0;
    unsigned char f1;
    char pad2[0xD - 2];
    unsigned char fD;
    char padE[2];
    unsigned char f10;
    char pad11[0x50 - 0x11];
} CS;

typedef struct {
    char pad0[0x40];
    unsigned char f40;
    char pad41;
    unsigned short f42;
    char pad44[4];
    int f48;
    char pad4C[4];
    unsigned short f50;
    char pad52[0x5C - 0x52];
    int f5C;
    char pad60[0x70 - 0x60];
    int f70;
    char pad74[0x84 - 0x74];
} CHAN;

extern CS D_00098950[];
extern int D_0008A8F0[];
extern CAR D_0009B0A4[];
extern CHAN D_000BD498[];
typedef struct { int v; int pad; } W8;
extern long long D_0009779C;
extern int *D_000C7000[];
extern W8 D_00097798;
extern int D_0008909C[];
extern void func_0004DDA8();
#define ABS(x) ((x) < 0 ? -(x) : (x))
extern void func_0004DB84();

static inline int sub(int a, int b) { return a - b; }

void func_0004CA5C(void)
{
    char pad[16];
    unsigned short order[36];
    int dist[26];
    int n;
    int i;
    int j;
    unsigned short c;
    int t;
    int left3;
    int left5;
    int ch;
    CAR *cp;
    int *pos;

    n = 0;
    for (i = 0; i < 25; i++) {
        if (i != 0 && D_00098950[i - 1].f1 != 0 && D_00098950[i - 1].f1 != 1) {
            order[n++] = i;
            dist[i] = ABS(D_0009B0A4[i].pos[0] - D_0008A8F0[0]) + ABS(D_0009B0A4[i].pos[2] - D_0008A8F0[2]);
        }
    }

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int a = order[i];
            int b = order[j];
            if (dist[a] > dist[b]) {
                order[i] = b;
                order[j] = a;
            }
        }
    }

    left3 = 2;
    left5 = 2;
    for (i = 0; i < n; i++) {
        c = order[i];
        if (c < 25) {
            cp = &D_0009B0A4[c];
            pos = cp->pos;
            if (cp->f12E == 1) {
                if (left5 && D_00098950[c - 1].f10 == 1 && (t = D_00098950[c - 1].fD) < 80) {
                    left5--;
                    ch = left5 + 5;
                    D_000BD498[ch].f42 = 50;
                    func_0004DDA8(ch, pos, cp->vel, -2000 - t * 30, sub(c * 4, t * 48 - 4096));
                }
            }
            if (cp->f12E == 1 || cp->f3 == 1) {
                if (left3) {
                    left3--;
                    ch = left3 + 7;
                    D_000BD498[ch].f42 = 50;
                    func_0004DDA8(ch, cp->pos, cp->vel, -2000, sub(cp->f180 >> 2, (cp->f182 >> 7) - 1000));
                }
            }
        }
    }

    for (i = 0; i < n; i++) {
        c = order[i];
        if (c < 25) {
            cp = &D_0009B0A4[c];
            pos = cp->pos;
            if (cp->f12E != 1 && cp->f3 != 1 && left3) {
                left3--;
                D_000BD498[left3 + 7].f42 = 50;
                func_0004DDA8(left3 + 7, pos, cp->vel, -6000, 0x830);
            }
        }
    }

    if (*(int *)&D_0009779C && left5) {
        left5--;
        ch = left5 + 5;
        D_000BD498[ch].f42 = 50;
        func_0004DDA8(ch, D_000C7000[0], D_0008909C, -2000, D_00097798.v);
    }

    for (i = left5 - 1; i >= 0; i--)
        func_0004DB84(i + 5, -10000);
    for (i = left3 - 1; i >= 0; i--)
        func_0004DB84(i + 7, -10000);

    for (ch = 0; ch < 16; ch++) {
        CHAN *p = &D_000BD498[ch];
        int a;
        if (p->f40 == 0 && p->f42 != 0 && (a = p->f5C) != 0)
            func_0004DDA8(ch, a, p->f70, p->f48, p->f50);
    }
}
