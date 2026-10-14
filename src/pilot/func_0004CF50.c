/* no Driver 2 counterpart */
extern int D_00096C70;
extern long long D_00097708;
extern long long D_00097298;
extern long long D_00096C88;
extern long long D_00096888;
extern long long D_00096840;
extern long long D_00097080;
extern long long D_000BD97C;
extern long long D_00096770;
extern long long D_00096834;
extern int D_000974C0;
extern long long D_00096897;
#define gamemode (*(unsigned char *)&D_00096897)

typedef struct { unsigned short id; short pad; } SNDENT;
extern SNDENT D_0008AA04[];

typedef struct { int vx, vy, vz; } VEC3;
extern VEC3 D_000BD474;

typedef struct {
    char pad0[0xC8];
    int v1[4];
    VEC3 pos;
    char pad1[0x180 - 0xE4];
    short h180;
    short h182;
    char pad2[0x18C - 0x184];
} PLAYER;
extern PLAYER D_0009B0A4[];

extern int D_000BC9BC[];
typedef struct { int count; int list[64]; } QUEUE;

typedef struct { char pad[0x4E6]; unsigned short vol; } SNDSTATE;
extern SNDSTATE D_000BD498;

extern void func_00073020();
extern void func_0004DDA8();
extern void func_0004CA5C();
extern void func_0004D6B0();

void func_0004CF50(void)
{
    PLAYER *p;
    QUEUE *q;
    int v;

    if (D_00096C70 == 0 && *(int *)&D_00097708 >= 0x293) {
        D_00096C70 = 1;
        func_00073020(*(int *)&D_00096C88, D_0008AA04[*(int *)&D_00097298].id);
    }

    if (gamemode == 2 || gamemode == 0) {
        D_000BD474.vx = D_0009B0A4[0].pos.vx;
        D_000BD474.vy = D_0009B0A4[0].pos.vy;
        D_000BD474.vz = D_0009B0A4[0].pos.vz;
    } else {
        D_000BD474.vx = 0;
        D_000BD474.vy = 0;
        D_000BD474.vz = 0;
    }

    p = &D_0009B0A4[*(int *)&D_00096888];
    func_0004DDA8(1, p->v1, &p->pos, p->h182 - 12000, 0x1000);
    func_0004DDA8(0, p->v1, &p->pos, -p->h182 - 2000, (p->h180 >> 2) - (short)((p->h182 >> 7) - 1000));

    if (*(int *)&D_00096840 != 0) {
        if (*(int *)&D_00097080 != 0)
            func_0004DDA8(4, p->v1, &p->pos, 1000, 0x1000);
        else
            func_0004DDA8(4, p->v1, &p->pos, -10000, 0x1000);
    }

    func_0004CA5C();

    if (D_000BC9BC[0] > 0) {
        if ((*(int *)&D_000BD97C & 0xFFFF00FF) == 0) {
            q = (QUEUE *)D_000BC9BC;
            q->count--;
            v = q->list[q->count];
            if (v >= 100) {
                func_0004D6B0(9, 4, v - 100, 0, 0x1000);
            } else if (*(int *)&D_00096770 != 0 && *(int *)&D_00096834 != 2) {
                func_0004D6B0(9, 3, v, -4000, 0x1000);
            }
            D_000BD498.vol = D_000BD498.vol * 5 / 6;
        }
        D_000974C0 = 0;
    } else {
        D_000974C0++;
    }
}
