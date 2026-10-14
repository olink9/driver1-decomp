typedef struct { int x, y, z; } POS;

typedef struct {
    POS *pos;               /* 0x0 */
    short timer;            /* 0x4 */
    short s6;               /* 0x6 */
    short s8;               /* 0x8 */
    unsigned char flags;    /* 0xA */
    unsigned char b1;       /* 0xB */
    unsigned char b2;       /* 0xC */
    char padD[3];
} SNDENT;

typedef struct { int timer, base, range; } TIMER;

static inline int RandTime(TIMER *t, int r)
{
    return t->base + r % t->range;
}

typedef struct {
    char pad0[0x14];
    SNDENT ent[3];          /* 0x14 */
    TIMER t;                /* 0x44 */
    int *list;              /* 0x50 */
    int flags;              /* 0x54 */
} SNDOBJ;

extern long long D_00097708;
extern long long D_0009770C;
extern char D_000BC9BC[];
extern int func_0004FFE8(void);
extern void func_0004C96C();
extern void func_0004D6B0();
extern void func_0004D7E4();

void func_0006A6C8(SNDOBJ *p)
{
    SNDENT *e;
    unsigned char play;
    int r;
    POS *pos;

    if (p->flags & 1) {
        if (p->t.timer > 0) {
            p->t.timer--;
        } else {
            play = 0;
            if (!(p->flags & 2))
                play = 1;
            else if ((p->flags & 2) >> 1 && *(int *)&D_0009770C > 0 && *(int *)&D_00097708 > 0x292)
                play = 1;
            if (play) {
                r = func_0004FFE8();
                func_0004C96C(D_000BC9BC, p->list[1 + r % p->list[0]]);
                p->t.timer = RandTime(&p->t, r);
            }
        }
    }

    for (e = p->ent; e < &p->ent[3]; e++) {
        if (e->flags & 1) {
            if (e->timer > 0) {
                e->timer--;
            } else {
                switch (e->flags & 6) {
                case 0:
                    func_0004C96C(D_000BC9BC, e->b1);
                    break;
                case 4:
                    pos = e->pos;
                    func_0004D7E4(-1, e->b2, e->b1, pos->x, pos->y, pos->z, e->s6, e->s8);
                    break;
                case 2:
                    func_0004D6B0(-1, e->b2, e->b1, e->s6, e->s8);
                    break;
                }
                e->flags &= ~1;
            }
        }
    }
}
