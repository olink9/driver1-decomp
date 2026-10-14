/* screen fade / letterbox bars */
typedef struct { char *primptr; unsigned int *ot; int pad; int id; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
    unsigned char r3, g3, b3, pad3;
    short x3, y3;
} POLY_G4;

typedef struct { unsigned int w[13]; } POLY_GT4;

extern POLY_GT4 D_000C775C[];
extern POLY_G4 D_000C77E4[];
extern POLY_F4 D_000C749C[];
extern POLY_F4 D_000C746C[];
extern int D_00096F80;
extern int D_00096F88;
extern unsigned char D_00096F8C;
extern long long D_00096897;

void func_0006E040(void)
{
    POLY_G4 *fade;
    POLY_F4 *top;
    POLY_F4 *bot;

    fade = &D_000C77E4[current->id];
    top = &D_000C749C[current->id];
    bot = &D_000C746C[current->id];

    if (D_00096F80 >= 1 && D_00096F80 <= 3) {
        fade->r0 = D_00096F88;
        fade->g0 = D_00096F88;
        fade->b0 = D_00096F88;
        fade->r1 = D_00096F88;
        fade->g1 = D_00096F88;
        fade->b1 = D_00096F88;
        fade->r2 = D_00096F88;
        fade->g2 = D_00096F88;
        fade->b2 = D_00096F88;
        fade->r3 = D_00096F88;
        fade->g3 = D_00096F88;
        fade->b3 = D_00096F88;
        fade->x0 = 0;
        fade->y0 = 0;
        fade->x1 = 0x200;
        fade->y1 = 0;
        fade->x2 = 0;
        fade->y2 = 0x100;
        fade->x3 = 0x200;
        fade->y3 = 0x100;
        fade->code |= 2;
        addPrim(current->ot[1], fade);
        addPrim(current->ot[1], &D_000C775C[current->id]);
    } else if (D_00096F80 == 4) {
        D_00096F8C += 4;
        if (D_00096F8C >= 42) {
            D_00096F8C = 42;
            *(unsigned char *)&D_00096897 = 0;
            D_00096F80 = 5;
        }
        top->x0 = 0;
        top->y0 = 0;
        top->x1 = 0x200;
        top->y1 = 0;
        top->x2 = 0;
        top->y2 = 42 - D_00096F8C;
        top->x3 = 0x200;
        top->y3 = 42 - D_00096F8C;
        bot->x0 = 0;
        bot->y0 = D_00096F8C + 214;
        bot->x1 = 0x200;
        bot->y1 = D_00096F8C + 214;
        bot->x2 = 0;
        bot->y2 = 0x100;
        bot->x3 = 0x200;
        bot->y3 = 0x100;
    }
    addPrim(current->ot[1], top);
    addPrim(current->ot[1], bot);

    if (D_00096F80 == 3)
        D_00096F88 -= 4;
    if (D_00096F88 <= 0)
        D_00096F88 = 0;
}
