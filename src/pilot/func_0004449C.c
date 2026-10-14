/* ShowLight (Driver 2) */
typedef struct { short vx, vy, vz, pad; } SV;
typedef struct { unsigned char r, g, b, cd; } CV;
typedef struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; unsigned short tpageid, clutid; } TD;
typedef struct { unsigned char *primptr; unsigned long **ot; } DB;
typedef struct { unsigned long tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut;
  short x1, y1; unsigned char u1, v1; unsigned short tpage; short x2, y2; unsigned char u2, v2; unsigned short pad2;
  short x3, y3; unsigned char u3, v3; unsigned short pad3; } PFT4;
typedef struct { unsigned long tag; unsigned char r0, g0, b0, code; short x0, y0;
  unsigned char r1, g1, b1, p1; short x1, y1; unsigned char r2, g2, b2, p2; short x2, y2;
  unsigned char r3, g3, b3, p3; short x3, y3; } PG4;
typedef struct { unsigned long tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut;
  short x1, y1; unsigned char u1, v1; unsigned short tpage; short x2, y2; unsigned char u2, v2; unsigned short pad2; } PFT3;
typedef struct { short x, y; } SXY;
typedef struct { SXY trails[4]; int pad; short clock; short pad2; } LAMP;
typedef struct { char pad[0x134]; SXY trails[4][4]; short old_clock[4]; char pad2[16]; } CAR;
typedef struct { unsigned char v; char pad[7]; } B8;

extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern int D_00097480;
extern long long D_00097534;
#define FrameCnt (*(volatile int *)&D_00097534)
extern long long D_000970E8;
extern B8 D_00097660;
extern int D_00097464;
extern int D_0009740C;
extern LAMP D_000B738C[];
extern CAR D_0009B0A4[];
extern short D_0008EAAA[];
extern int D_000B8FE8[];
extern void func_0002C564();
extern int ratan2(int, int);

#define SetTransVec(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define SetRotMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $0;" "ctc2  $13, $1;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $2;" "ctc2  $13, $3;" "ctc2  $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define ldv3c(r0) __asm__ volatile("lwc2 $0,0(%0);lwc2 $1,4(%0);lwc2 $2,8(%0);lwc2 $3,12(%0);lwc2 $4,16(%0);lwc2 $5,20(%0)" : : "r"(r0))
#define rtpt() __asm__ volatile("nop;nop;rtpt")
#define rtps() __asm__ volatile("nop;nop;rtps")
#define stsxy3(a,b,c) __asm__ volatile("swc2 $12,0(%0);swc2 $13,0(%1);swc2 $14,0(%2)" : : "r"(a),"r"(b),"r"(c) : "memory")
#define stsxy(a) __asm__ volatile("swc2 $14,0(%0)" : : "r"(a) : "memory")
#define stsz(a) __asm__ volatile("swc2 $19,0(%0)" : : "r"(a) : "memory")
#define ldv0(r0) __asm__ volatile("lwc2 $0,0(%0);lwc2 $1,4(%0)" : : "r"(r0))


typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define addPrimZ(p) \
    ((P_TAG *)(p))->addr = ((P_TAG *)&current->ot[z])->addr; ((P_TAG *)&current->ot[z])->addr = (unsigned)(p)

void func_0004449C(void *v1, CV *col, short size, TD *texture) {
    SV vert[4];
    int z;
    PFT4 *poly;
    int index;
    int tail_width;

    func_0002C564(v1);
    SetTransVec(v1);
    SetRotMatrix(D_000B8FE8);

    vert[2].vx = (-size * 3) >> 1;
    vert[0].vx = (-size * 3) >> 1;
    vert[3].vx = (size * 3) >> 1;
    vert[1].vx = (size * 3) >> 1;
    vert[3].vz = 0;
    vert[2].vz = 0;
    vert[1].vz = 0;
    vert[0].vz = 0;
    vert[1].vy = -size;
    vert[0].vy = -size;
    vert[3].vy = size;
    vert[2].vy = size;
    ldv3c(&vert[0]);
    rtpt();
    poly = (PFT4 *)current->primptr;
    poly->u0 = texture->u0;
    poly->v0 = texture->v0;
    poly->u1 = texture->u1;
    poly->v1 = texture->v1;
    poly->u2 = texture->u2;
    poly->v2 = texture->v2;
    poly->u3 = texture->u3;
    poly->v3 = texture->v3;
    ((unsigned char *)poly)[3] = 9;
    ((unsigned char *)poly)[7] = 0x2E;
    poly->r0 = col->r;
    poly->g0 = col->g;
    poly->b0 = col->b;
    stsxy3(&poly->x0, &poly->x1, &poly->x2);
    stsz(&z);
    if (z < 40)
        return;
    ldv0(&vert[3]);
    rtps();
    poly->tpage = texture->tpageid | 0x20;
    poly->clut = texture->clutid;
    z = (z >> 3) + D_00097480;
    if (z < 0) z = 0;
    stsxy(&poly->x3);
    if (z < 10000)
        tail_width = (10000 - z) >> 13;
    else
        tail_width = 0;
    addPrimZ(poly);
    current->primptr += 0x28;

    index = -1;
    if ((col->cd & 0x20) && *(int *)&D_000970E8) {
        if (D_00097660.v || D_000B738C[D_00097464].clock != (FrameCnt & 0xffff) - 1) {
            int i;
            for (i = 0; i < 4; i++) {
                D_000B738C[D_00097464].trails[i].x = (poly->x0 + poly->x3) >> 1;
                D_000B738C[D_00097464].trails[i].y = (poly->y0 + poly->y3) >> 1;
            }
            D_000B738C[D_00097464].clock = FrameCnt;
        } else {
            PG4 *trail;
            PFT3 *null;
            int angle, width, dx, dy, length;
            SV tail;
            SV head;
            tail.vx = D_000B738C[D_00097464].trails[(FrameCnt - 3) & 3].x;
            tail.vy = D_000B738C[D_00097464].trails[(FrameCnt - 3) & 3].y;
            head.vx = (poly->x0 + poly->x3) >> 1;
            head.vy = (poly->y0 + poly->y3) >> 1;
            D_000B738C[D_00097464].clock = FrameCnt;
            D_000B738C[D_00097464].trails[FrameCnt & 3].x = head.vx;
            D_000B738C[D_00097464].trails[FrameCnt & 3].y = head.vy;
            width = poly->x0 - poly->x3;
            width = abs(width);
            length = tail.vx - head.vx;
            angle = tail.vy - head.vy;
            length = abs(length) + abs(angle);
            if (size > 1 && length > 1) {
                trail = (PG4 *)current->primptr;
                trail->x0 = trail->x1 = (poly->x0 + poly->x3) >> 1;
                angle = -ratan2(tail.vx - head.vx, tail.vy - head.vy);
                if (col->cd & 0x40) {
                    dx = (D_0008EAAA[(angle & 0xfff) * 2] * width * 3) >> 16;
                    dy = (D_0008EAAA[(angle & 0xfff) * 2 - 1] * width * 3) >> 16;
                } else {
                    dx = (D_0008EAAA[(angle & 0xfff) * 2] * width * 3) >> 15;
                    dy = (D_0008EAAA[(angle & 0xfff) * 2 - 1] * width * 3) >> 15;
                }
                *(int *)&trail->x0 = ((head.vy + dy) << 16) + (head.vx + dx);
                *(int *)&trail->x1 = ((head.vy - dy) << 16) + (head.vx - dx);
                *(int *)&trail->x2 = ((tail.vy - tail_width) << 16) + tail.vx;
                *(int *)&trail->x3 = ((tail.vy + tail_width) << 16) + tail.vx;
                ((unsigned char *)trail)[3] = 8;
                ((unsigned char *)trail)[7] = 0x3A;
                trail->r0 = col->r >> 1;
                trail->g0 = col->g >> 1;
                trail->b0 = col->b >> 1;
                trail->r1 = col->r >> 1;
                trail->g1 = col->g >> 1;
                trail->b1 = col->b >> 1;
                trail->r2 = 0;
                trail->g2 = 0;
                trail->b2 = 0;
                trail->r3 = 0;
                trail->g3 = 0;
                trail->b3 = 0;
                addPrimZ(trail);
                null = (PFT3 *)(current->primptr += 0x24);
                ((unsigned char *)null)[3] = 7;
                ((unsigned char *)null)[7] = 0x24;
                null->x0 = -1;
                null->y0 = -1;
                null->x1 = -1;
                null->y1 = -1;
                null->x2 = -1;
                null->y2 = -1;
                null->tpage = 0x20;
                addPrimZ(null);
                current->primptr += 0x20;
            }
        }
        index = -1;
    } else if (col->cd & 2) index = 0;
    else if (col->cd & 4) index = 1;
    else if (col->cd & 8) index = 2;
    else if (col->cd & 0x10) index = 3;

    if (index < 0)
        return;

    if (D_00097660.v || D_0009B0A4[D_0009740C].old_clock[index] != (FrameCnt & 0xffff) - 1) {
        int i;
        for (i = 0; i < 4; i++) {
            D_0009B0A4[D_0009740C].trails[index][i].x = (poly->x0 + poly->x3) >> 1;
            D_0009B0A4[D_0009740C].trails[index][i].y = (poly->y0 + poly->y3) >> 1;
        }
        D_0009B0A4[D_0009740C].old_clock[index] = FrameCnt;
    } else {
        PG4 *trail;
        PFT3 *null;
        int angle, width, dx, dy, length, c, sn;
        SV tail;
        SV head;
        tail.vx = D_0009B0A4[D_0009740C].trails[index][(FrameCnt - 3) & 3].x;
        tail.vy = D_0009B0A4[D_0009740C].trails[index][(FrameCnt - 3) & 3].y;
        head.vx = (poly->x0 + poly->x3) >> 1;
        head.vy = (poly->y0 + poly->y3) >> 1;
        D_0009B0A4[D_0009740C].old_clock[index] = FrameCnt;
        D_0009B0A4[D_0009740C].trails[index][FrameCnt & 3].x = head.vx;
        D_0009B0A4[D_0009740C].trails[index][FrameCnt & 3].y = head.vy;
        width = poly->x0 - poly->x3;
            width = abs(width);
        dx = tail.vx - head.vx;
        dy = tail.vy - head.vy;
        length = abs(dx) + abs(dy);
        if (size > 1 && length > 1) {
            trail = (PG4 *)current->primptr;
            angle = -ratan2(dx, dy) & 0xfff;
            c = (D_0008EAAA[angle * 2] * width * 3) >> 15;
            sn = (D_0008EAAA[angle * 2 - 1] * width * 3) >> 15;
            *(int *)&trail->x0 = ((head.vy + sn) << 16) + (head.vx + c);
            *(int *)&trail->x1 = ((head.vy - sn) << 16) + (head.vx - c);
            *(int *)&trail->x2 = ((tail.vy - tail_width) << 16) + tail.vx;
            *(int *)&trail->x3 = ((tail.vy + tail_width) << 16) + tail.vx;
            ((unsigned char *)trail)[3] = 8;
            ((unsigned char *)trail)[7] = 0x3A;
            if (col->cd & 0x18) {
                trail->r0 = col->r >> 1;
                trail->g0 = col->g >> 1;
                trail->b0 = col->b >> 1;
                trail->r1 = col->r >> 1;
                trail->g1 = col->g >> 1;
                trail->b1 = col->b >> 1;
            } else {
                trail->r0 = col->r;
                trail->g0 = col->g;
                trail->b0 = col->b;
                trail->r1 = col->r;
                trail->g1 = col->g;
                trail->b1 = col->b;
            }
            trail->r2 = 0;
            trail->g2 = 0;
            trail->b2 = 0;
            trail->r3 = 0;
            trail->g3 = 0;
            trail->b3 = 0;
            addPrimZ(trail);
            null = (PFT3 *)(current->primptr += 0x24);
            ((unsigned char *)null)[3] = 7;
            ((unsigned char *)null)[7] = 0x24;
            null->x0 = -1;
            null->y0 = -1;
            null->x1 = -1;
            null->y1 = -1;
            null->x2 = -1;
            null->y2 = -1;
            null->tpage = 0x20;
            addPrimZ(null);
            current->primptr += 0x20;
        }
    }
}
