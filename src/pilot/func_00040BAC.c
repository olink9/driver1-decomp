/* PlacePoolForCar (Driver 2 DEBRIS.C): car headlight / brake light pools */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
} TEXINF;

typedef struct {
    char pad[0xC8];
    int t[3];
    char pad2[396 - 0xD4];
} CAR_DATA;

typedef struct { unsigned char v; char pad[7]; } B8;

extern CAR_DATA D_0009B0A4[];
extern MATRIX D_000AF6A0[];
extern B8 D_00096897;
extern long long D_0009688C;
extern VECTOR D_0008A8F0;
extern int D_0008A8F4[];
extern int D_00097480;
extern short D_00097484;
extern unsigned char D_00097458;
extern unsigned char D_00097460;
extern long long D_0009717C;
extern MATRIX D_000BBE84;
extern VECTOR D_0008909C;
extern MATRIX D_000890BC;
extern TEXINF D_000B91E0;
extern char D_0008A73C[];

extern void func_00079D20();
extern void func_0004EAC0();
extern int func_0001F5B0();
extern void func_0002B9BC();

#define gte_SetRotMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $0;" "ctc2  $13, $1;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $2;" "ctc2  $13, $3;" "ctc2  $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile ("lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_rtv0tr() __asm__ volatile ("nop;" "nop;" "mvmva 1, 0, 0, 0, 0")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsv(r0) __asm__ volatile ( \
    "mfc2  $12, $9;" "mfc2  $13, $10;" "mfc2  $14, $11;" \
    "sh    $12, 0( %0 );" "sh    $13, 2( %0 );" "sh    $14, 4( %0 )" : : "r"(r0) : "$12", "$13", "$14", "memory")

#define ABS(x) ((x) < 0 ? -(x) : (x))

void func_00040BAC(int idx, CVECTOR *col, int front)
{
    SVECTOR s[27];
    SVECTOR sout[27];
    VECTOR s1[12];
    MATRIX final_matrix;
    VECTOR averagepos;
    VECTOR mid_position;
    VECTOR toss;
    int z;
    CAR_DATA *cp;
    int in_car;
    int sub_level;
    int car_road_height;
    int i;
    int temp_y;
    VECTOR *ptr1;
    SVECTOR *ptr;
    POLY_FT4 *poly;
    char *VertIdx;

    cp = &D_0009B0A4[idx];
    in_car = 0;
    sub_level = 0;
    if (D_00096897.v == 2)
        in_car = (idx == *(int *)&D_0009688C);

    if (front) {
        if (in_car) {
            s1[0].vx = 200;
            s1[0].vz = -1650;
            s1[1].vx = -600;
            s1[1].vz = -1650;
            s1[2].vx = -30;
            s1[2].vz = -550;
            s1[3].vx = -210;
            s1[3].vz = -550;
            s1[4].vx = 600;
            s1[4].vz = -1650;
            s1[5].vx = -200;
            s1[5].vz = -1650;
            s1[6].vx = 210;
            s1[6].vz = -550;
            s1[7].vx = 30;
            s1[7].vz = -550;
            s1[8].vx = -120;
            s1[8].vz = -2650;
            s1[10].vx = 120;
            s1[10].vz = -2650;
            s1[9].vx = -120;
            s1[9].vz = -540;
            s1[11].vx = 120;
            s1[11].vz = -540;
            D_00097480 = -800;
            sub_level = 3;
        } else {
            s1[0].vx = 200;
            s1[0].vz = -2150;
            s1[1].vx = -600;
            s1[1].vz = -2150;
            s1[2].vx = -30;
            s1[2].vz = -550;
            s1[3].vx = -210;
            s1[3].vz = -550;
            s1[4].vx = 600;
            s1[4].vz = -2150;
            s1[5].vx = -200;
            s1[5].vz = -2150;
            s1[6].vx = 210;
            s1[6].vz = -550;
            s1[7].vx = 30;
            s1[7].vz = -550;
            s1[8].vx = -120;
            s1[8].vz = -2250;
            s1[10].vx = 120;
            s1[10].vz = -2250;
            s1[9].vx = -120;
            s1[9].vz = -550;
            s1[11].vx = 120;
            s1[11].vz = -550;
            D_00097480 = -90;
            sub_level = 3;
        }
    } else {
        s1[0].vx = -300;
        s1[0].vz = 750;
        s1[1].vx = 300;
        s1[1].vz = 750;
        s1[2].vx = -300;
        s1[2].vz = 450;
        s1[3].vx = 300;
        s1[3].vz = 450;
    }

    func_00079D20(&D_000AF6A0[idx]);
    averagepos.vx = 0;
    averagepos.vz = 0;
    mid_position.vx = 0;
    mid_position.vy = 0;
    mid_position.vz = -500;
    func_0004EAC0(&mid_position);
    mid_position.vx += cp->t[0];
    mid_position.vz += cp->t[2];
    car_road_height = -func_0001F5B0(&mid_position);

    i = 0;
    ptr1 = s1;
    ptr = s;
    for (; (sub_level == 3) ? (i < 12) : (i < 4); i++, ptr1++, ptr++) {
        ptr1->vy = 0;
        func_0004EAC0(ptr1);
        toss.vx = ptr1->vx + cp->t[0];
        toss.vz = ptr1->vz + cp->t[2];
        ptr->vx = toss.vx - D_0008A8F0.vx;
        ptr->vz = toss.vz - D_0008A8F0.vz;
        averagepos.vx += ptr->vx;
        averagepos.vz += ptr->vz;
        temp_y = -func_0001F5B0(&toss);
        if (ABS(temp_y - car_road_height) > 500)
            temp_y = car_road_height;
        s[i].vy = temp_y - D_0008A8F4[0];
    }

    gte_SetRotMatrix(&D_000BBE84);
    gte_SetTransVector(&D_0008909C);

    switch (sub_level) {
    case 0:
        gte_ldv3c(s);
        gte_rtpt();
        poly = (POLY_FT4 *)current->primptr;
        poly->u0 = D_000B91E0.u0;
        poly->v0 = D_000B91E0.v0;
        poly->u1 = D_000B91E0.u1;
        poly->v1 = D_000B91E0.v1;
        poly->u2 = D_000B91E0.u2;
        poly->v2 = D_000B91E0.v2;
        poly->u3 = D_000B91E0.u3;
        poly->v3 = D_000B91E0.v3;
        ((P_TAG *)poly)->len = 9;
        poly->code = 0x2C;
        poly->code |= 2;
        poly->r0 = col->r >> 1;
        poly->g0 = col->b >> 1;
        poly->b0 = col->g >> 1;
        gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
        gte_stsz(&z);
        if (z > 40)
            z -= 40;
        if (z > 49) {
            gte_ldv0(&s[3]);
            gte_rtps();
            poly->tpage = D_000B91E0.tpageid | 0x20;
            poly->clut = D_000B91E0.clutid;
            ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[z >> 3])->addr;
            ((P_TAG *)&current->ot[z >> 3])->addr = (unsigned int)poly;
            current->primptr += 40;
            gte_stsxy(&poly->x3);
        }
        break;
    case 3:
    {
        char pad[64];
        for (i = 0; i < 12; i++) {
            gte_ldv0(&s[i]);
            gte_rtv0tr();
            gte_stsv(&sout[i]);
        }
        for (i = 0; i < 4; i++) {
            VertIdx = D_0008A73C + i * 4;
            *(char **)&D_0009717C = current->primptr;
            func_00079D20(&D_000890BC);
            gte_SetTransVector(&D_0008909C);
            if (i & 2)
                D_00097484 = D_00097458 * 80;
            else
                D_00097484 = D_00097460 * 80;
            if (D_00097484) {
                if (i & 1)
                    func_0002B9BC(sout + VertIdx[0], sout + VertIdx[2], sout + VertIdx[3], sout + VertIdx[1]);
                else
                    func_0002B9BC(sout + VertIdx[1], sout + VertIdx[3], sout + VertIdx[2], sout + VertIdx[0]);
                current->primptr = *(char **)&D_0009717C;
            }
        }
    }
        break;
    }

    D_00097480 = -10;
}
