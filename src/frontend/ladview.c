/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * GFX\GLOAD.TIM/MCARD.BIN loader, DrawMapPSX views func_001CB380/func_001CB4A0, text func_001CB790/func_001CB95C,
 * REW/FFWD/PLAY/ACCEPT buttons, model draw func_001CBAA4-func_001CC894 (table D_001CF328). START: text pad at
 * 0x1CB18C. .rodata one block 0x1C0A40. END: weak (4 call edges cross 0x1CCD58; ladmenu has its own .data block
 * from 0x1CF368).
 */
/* ladview.c: 16 functions func_001CB190..func_001CC894 (0x1CB190-0x1CCD58)
 * Step 3: types from include/frontend.h + structs.h (DB *current, MODEL, CELL_OBJECT, MATRIX/VECTOR/SVECTOR,
 * POLY_FT3/FT4/G4/GT4, MENU_ITEM, MENU_TEXT). D_001CF318 is one VTX vertex scratch for every function (the
 * ushort uv read is `*(unsigned short *)&v.u`, the gte macros take its address); D_001CF308 is an int *;
 * func_001CC894 copies into the byte tables D_001CF30C/D_001CF314 through (int *) casts and reads the low byte of
 * D_001CF300 through a cast. E/T/SRC/CNT/XY/VTX/PLOTFN are file-local. No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { int a, b, c, d; } E;
typedef struct { unsigned char pad0, a; unsigned short b; unsigned char idx[4]; unsigned char p[8]; } T;
typedef struct { unsigned short x, y, z, w; } SRC;
typedef struct { short a, b; } CNT;
typedef struct { short x, y; } XY;
typedef struct { short x, y, z; unsigned char u, v; } VTX;
typedef void (*PLOTFN)(unsigned char *);

/* ---- externs ---- */
extern E D_001CF290[];
extern int D_0008531C[];
extern int D_00096834;
extern int D_00096804;
extern int gNight;
extern int D_001CF098;
extern RECT16 D_000976A4;
extern void func_001C9848();
extern void func_001C98F4();
extern void LoadFont();
extern void SetTextColour();
extern void func_0005C68C();
extern void func_0006F57C();
extern void func_001CAE64();
extern DB *current;
extern DB *D_00097524;
extern volatile int D_00097534;
extern char cameraview;
extern void SetDispMask();
extern void func_0007457C();
extern void func_00073F88();
extern void func_00079800();
extern void ShowLoadingScreen();
extern int Loadfile();
extern void func_00022818();
extern void func_000513B4();
extern void func_00051540();
extern void func_00051374();
extern void func_00077D7C();
extern MATRIX inv_camera_matrix;
extern int D_001CF2D4;
extern char D_001CF2D8[];
extern void func_00077D6C();
extern void func_001C9C0C();
extern void MulMatrix0();
extern void func_0002DBEC();
extern void func_0002C53C();
extern void InitFrustrumMatrix();
extern void func_0002DBB4();
extern void DrawMapPSX();
extern void func_000244B0();
extern int D_001CF2F8;
extern int D_001CF2FC;
extern int D_001CF300;
extern int D_00096C90;
extern void DrawSync();
extern void VSync();
extern void func_000513D4();
extern unsigned short D_00097538;
extern short D_0009752C;
extern int D_001CDECC;
extern void PrintString();
extern void func_0005C8F4();
extern void PrintStringRightAligned();
extern char D_001C0A78[];
extern char D_001D3240[];
extern char D_001D3260[];
extern char D_001D3230[];
extern char D_001D3250[];
extern char D_001D3270[];
extern void GetTextureDetails();
extern VTX *D_001CF318;
extern int *D_001CF31C;
extern unsigned int *D_001CF324;
extern int D_001CF304;
extern int D_001D3280;
extern int D_001D3284;
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern SRC *D_001D328C;
extern int *D_001CF308;
extern int D_001D3288;
extern int D_001D3290;
extern CNT D_001D3298[5];
extern XY *D_001CF320;
extern unsigned char *D_001CF30C;
extern unsigned char *D_001CF310;
extern signed char *D_001CF314;
extern int D_001CF328[];
extern int D_001CF338[];
extern int D_001CF33C[];
extern int D_001CF348[];
extern int D_00089F1C[];
extern int D_1F800010[];
extern MATRIX CompoundMatrix[];
extern VECTOR camera_position;
extern MODEL *modelpointers[];
extern void func_0002C564(VECTOR *v);
extern unsigned short GetTPage();

/* ---- functions defined in this file ---- */
void func_001CB190();
void func_001CB290();
void func_001CB380();
void func_001CB4A0();
int func_001CB790();
void func_001CB95C();
void func_001CBA18();
void func_001CBA94();
void func_001CBA9C();
void func_001CBAA4();
void func_001CBD08();
int func_001CBE30();
int func_001CBEFC();
void func_001CC1D0();
void func_001CC76C();
void func_001CC894();

/* ---------------- func_001CB190 (src/pilot/frontend/func_001CB190.c) ---------------- */
/* func_001CB190: LoadFont(&r) takes one arg (sibling func_001C2F78); stores in func_001C2F78 order. FAKEMATCH pad. */
/* main LoadFont is void(void); the a1=16 is reload's reg for the 16 stores */
void func_001CB190(void)
{
    RECT16 r;
    char pad[16]; /* FAKEMATCH: frame size only (0x38) */
    D_0008531C[16] = D_001CF290[D_00096834].a;
    D_00096804 = 1;
    D_0008531C[17] = D_001CF290[D_00096834].b;
    gNight = 0;
    D_0008531C[18] = D_001CF290[D_00096834].c;
    func_001CB290();
    func_001C9848(D_00096834);
    func_001C98F4();
    r.x = 0x3C0;
    r.y = 0x100;
    r.w = 16;
    r.h = 1;
    D_000976A4.x = 0x3D0;
    D_000976A4.y = 0x103;
    D_000976A4.w = 16;
    D_000976A4.h = 1;
    LoadFont(&r);
    SetTextColour(0, 0, 0);
    func_0005C68C(0, 0);
    func_0006F57C();
    D_001CF098 = 0;
    func_001CAE64();
}

/* ---------------- func_001CB290 (src/pilot/frontend/func_001CB290.c) ---------------- */
/* FAKEMATCH: local `nm` (= "GFX\\GLOAD.TIM") only steers register allocation */
void func_001CB290(void)
{
    int s;
    char *nm = "GFX\\GLOAD.TIM";
    SetDispMask(0);
    func_0007457C(current->ot, 0x1080);
    func_0007457C(D_00097524->ot, 0x1080);
    func_00073F88(1);
    func_00079800(0);
    ShowLoadingScreen(nm, 1, 0x17);
    Loadfile("MCARD.BIN", 0xE6600);
    s = D_00096834;
    D_00096834 = 4;
    func_00022818();
    D_00096834 = s;
    func_001CBA18();
    func_000513B4();
    func_00051540();
    func_00051374();
    func_00077D7C(0x1234);
    cameraview = 0;
    D_00097534 = 0;
    SetDispMask(1);
}

/* ---------------- func_001CB380 (src/pilot/frontend/func_001CB380.c) ---------------- */
/* FAKEMATCH: unused MATRIX local (`MATRIX tmp;`) only sizes the 0x58 frame (32 B below m); the rest is clean C. */
/* func_001CB380 - frontend 3D backdrop frame: if inv_camera_matrix row 0 is not degenerate, premultiply it by
   D_001CF2D8 (MulMatrix0 takes 3 args - the third, &inv_camera_matrix, is what left a2 holding its address and fixed
   the a2/a3 + t0/t1 register diffs of the 99.38 wip), then draw the map. */
void func_001CB380(void)
{
    MATRIX tmp;
    int s0, s1, s2;
    MATRIX m;
    func_00077D6C(D_001CF2D4);
    func_001C9C0C();
    s0 = inv_camera_matrix.m[0][0] * inv_camera_matrix.m[0][0];
    s1 = inv_camera_matrix.m[0][1] * inv_camera_matrix.m[0][1];
    s2 = inv_camera_matrix.m[0][2] * inv_camera_matrix.m[0][2];
    if (s0 + s1 + s2 <= 0x10D2A3F) {
        m = inv_camera_matrix;
        MulMatrix0(D_001CF2D8, &m, &inv_camera_matrix);
    }
    func_0002DBEC();
    func_0002C53C();
    InitFrustrumMatrix();
    func_0002DBB4();
    DrawMapPSX();
    func_000244B0(0xB4, 0xD7);
    D_00097534++;
}

/* ---------------- func_001CB4A0 (src/pilot/frontend/func_001CB4A0.c) ---------------- */
/* FAKEMATCH: pad[8] only sizes the frame (0x60, matrix at sp+0x18); the identical-arm restore at the end only adds refs to 'saved' so it outranks 'dir' in global-alloc (s4/s5). */
void func_001CB4A0(int dir, int steps)
{
    int saved, step, i;
    int s0, s1, s2;
    char pad[8]; /* FAKEMATCH: frame size */
    MATRIX m;

    D_001CF2F8 = 1;
    D_001CF2FC = 255 / steps;
    if (dir == 0)
        D_001CF300 = 0;
    else
        D_001CF300 = 255;
    if (dir == 0) {
        saved = D_00096C90;
        step = (saved - 10000) / steps;
    } else {
        saved = D_00096C90;
        D_00096C90 = -10000;
        step = (saved - 10000) / steps;
    }
    i = 0;
    if (dir != 0)
        SetDispMask(0);
    while (D_001CF2F8 != 0) {
        if (dir == 0)
            D_00096C90 += step;
        else
            D_00096C90 -= step;
        if (i == 3)
            SetDispMask(1);
        func_0002DBEC();
        func_0002C53C();
        func_001C9C0C();
        s0 = inv_camera_matrix.m[0][0] * inv_camera_matrix.m[0][0];
        s1 = inv_camera_matrix.m[0][1] * inv_camera_matrix.m[0][1];
        s2 = inv_camera_matrix.m[0][2] * inv_camera_matrix.m[0][2];
        if (s0 + s1 + s2 <= 0x10D2A3F) {
            m = inv_camera_matrix;
            MulMatrix0(D_001CF2D8, &m, &inv_camera_matrix);
        }
        InitFrustrumMatrix();
        func_0002DBB4();
        DrawMapPSX();
        D_00097534++;
        DrawSync(0);
        VSync(0);
        func_000513D4();
        if (dir == 0) {
            D_001CF300 += D_001CF2FC;
            if (D_001CF300 >= 255) {
                D_001CF300 = 255;
                D_001CF2F8 = 0;
            }
        } else {
            D_001CF300 -= D_001CF2FC;
            if (D_001CF300 <= 0) {
                D_001CF2F8 = 0;
                D_001CF300 = 0;
            }
        }
        i++;
    }
    if (dir == 0)
        D_00096C90 = saved;
    else
        D_00096C90 = saved;
}

/* ---------------- func_001CB790 (src/pilot/frontend/func_001CB790.c) ---------------- */
int func_001CB790(MENU_ITEM *m, int cur)
{
    int sel = cur;
    unsigned short pad = D_00097538;

    if (pad & 0x10)
        return -1;
    if (D_001CDECC) {
        if (D_0009752C & 0x8000)
            m[m[sel].left].enter();
        else if (D_0009752C & 0x2000)
            m[m[sel].right].enter();
        else if (pad & 0x1000)
            sel = m[sel].up;
        else if (pad & 0x4000)
            sel = m[sel].down;
        else if (pad & 0x40)
            sel = m[sel].action();
    } else {
        if (pad & 0x8000)
            sel = m[sel].left;
        else if (pad & 0x2000)
            sel = m[sel].right;
        else if (pad & 0x1000)
            sel = m[sel].up;
        else if (pad & 0x4000)
            sel = m[sel].down;
        else if (pad & 0x40)
            m[sel].action();
    }
    if (cur != sel) {
        m[cur].leave();
        m[sel].enter();
    }
    return sel;
}

/* ---------------- func_001CB95C (src/pilot/frontend/func_001CB95C.c) ---------------- */
/* func_001CB95C: goto-built loop (loop.c ignores it: no &p->text giv), param tested before its copy to p, `case 0: default:` first. */
void func_001CB95C(MENU_TEXT *q)
{
    MENU_TEXT *p;
    if (q->text == 0) return;
    p = q;
loop:
    SetTextColour(p->col.r, p->col.g, p->col.b);
    switch (p->justify) {
    case 0:
    default:
        PrintString(p->text, p->x, p->y);
        break;
    case 1:
        func_0005C8F4(p->text, p->y);
        break;
    case 2:
        PrintStringRightAligned(p->text, p->x, p->y);
        break;
    }
    p++;
    if (p->text != 0) goto loop;
}

/* ---------------- func_001CBA18 (src/pilot/frontend/func_001CBA18.c) ---------------- */
void func_001CBA18(void)
{
    GetTextureDetails("REW", D_001D3240);
    GetTextureDetails("FFWD", D_001D3260);
    GetTextureDetails("PLAY", D_001D3230);
    GetTextureDetails("ACCEPT", D_001D3250);
    GetTextureDetails(D_001C0A78, D_001D3270);
}

/* ---------------- func_001CBA94 (src/pilot/frontend/func_001CBA94.c) ---------------- */
void func_001CBA94(void){}

/* ---------------- func_001CBA9C (src/pilot/frontend/func_001CBA9C.c) ---------------- */
void func_001CBA9C(void){}

/* ---------------- func_001CBAA4 (src/pilot/frontend/func_001CBAA4.c) ---------------- */
void func_001CBAA4(unsigned char *p)
{
    POLY_FT3 *t;
    int z;

    if (func_001CBE30(p) != 0)
        return;
    if (func_001CBEFC(3) != 0)
        return;
    t = (POLY_FT3 *)current->primptr;
    ((P_TAG *)t)->len = 7;
    t->code = 0x24;
    t->r0 = 0x80;
    t->g0 = 0x80;
    t->b0 = 0x80;
    *(long *)&t->x0 = D_001CF31C[0];
    *(long *)&t->x1 = D_001CF31C[1];
    *(long *)&t->x2 = D_001CF31C[2];
    *(long *)&t->u0 = *(unsigned short *)&D_001CF318[p[0]].u | D_001D3284;
    *(long *)&t->u1 = *(unsigned short *)&D_001CF318[p[1]].u | D_001D3280;
    *(long *)&t->u2 = *(unsigned short *)&D_001CF318[p[2]].u;
    switch (D_001CF304) {
    default:
        z = (D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2;
        break;
    case 1:
        z = 0x107D;
        break;
    case 3:
        z = ((D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2) + 0x28;
        break;
    case 2:
        z = ((D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2) + 0x5A;
        break;
    }
    if (z < 8)
        z = 8;
    else if (z >= 0x1000)
        z = 0xFFF;
    ((P_TAG *)t)->addr = ((P_TAG *)&current->ot[z])->addr;
    ((P_TAG *)&current->ot[z])->addr = (unsigned)t;
    current->primptr += sizeof(POLY_FT3);
}

/* ---------------- func_001CBD08 (src/pilot/frontend/func_001CBD08.c) ---------------- */
/* func_001CBD08: pp[i * 2] giv (its reduced reg is born after the hoisted luis); t->a re-read into a local after the clut store
   (lo_sum globals count as varying addresses, so the int stores conflict with t->a / texture_pages loads). */
void func_001CBD08(T *t)
{
    unsigned char *ip = t->idx;
    int i = 0;
    unsigned char *pp = t->p;
    int a;
    for (; i < 3; i++) {
        D_001CF318[i].x = D_001D328C[*ip].x;
        D_001CF318[i].y = D_001D328C[*ip].y;
        D_001CF318[i].z = D_001D328C[*ip].z;
        D_001CF318[i].u = pp[i * 2];
        D_001CF318[i].v = pp[i * 2 + 1];
        ip++;
    }
    D_001D3284 = texture_cluts[t->a][t->b] << 16;
    a = t->a;
    D_001D3288 = 3;
    D_001D3290 = 0;
    D_001D3280 = texture_pages[a] << 16;
    func_001CBAA4(D_001CF308);
}

/* ---------------- func_001CBE30 (src/pilot/frontend/func_001CBE30.c) ---------------- */
#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_nclip() __asm__ volatile ("nop;" "nop;" "nclip")
#define gte_stopz(r0) __asm__ volatile ("swc2  $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3c(r0) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 4( %0 );" "swc2  $14, 8( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")
int func_001CBE30(unsigned char *p)
{
    int flag;
    gte_ldv3(&D_001CF318[p[0]], &D_001CF318[p[1]], &D_001CF318[p[2]]);
    gte_rtpt();
    gte_stsxy3c(D_001CF31C);
    if (D_001D3290 == 0) {
        gte_nclip();
        gte_stopz(&flag);
        if (flag < 0) return 1;
    }
    gte_stsz3c(D_001CF324);
    return 0;
}
#undef gte_ldv3
#undef gte_nclip
#undef gte_rtpt
#undef gte_stopz
#undef gte_stsxy3c
#undef gte_stsz3c

/* ---------------- func_001CBEFC (src/pilot/frontend/func_001CBEFC.c) ---------------- */
int func_001CBEFC(int n)
{
    int i;

    D_001D3298[0].a = D_001D3298[1].a = D_001D3298[2].a = D_001D3298[3].a = D_001D3298[4].a = 0;
    D_001D3298[0].b = D_001D3298[1].b = D_001D3298[2].b = D_001D3298[3].b = D_001D3298[4].b = 0;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].x < 0)
            D_001D3298[0].a++;
        if (D_001CF320[i].x < -254)
            D_001D3298[0].b++;
    }
    if (D_001D3298[0].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].y < 0)
            D_001D3298[1].a++;
        if (D_001CF320[i].y < -254)
            D_001D3298[1].b++;
    }
    if (D_001D3298[1].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].x > 0x200)
            D_001D3298[2].a++;
        if (D_001CF320[i].x > 0x2FE)
            D_001D3298[2].b++;
    }
    if (D_001D3298[2].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].y > 0x100)
            D_001D3298[3].a++;
        if (D_001CF320[i].y > 0x1FE)
            D_001D3298[3].b++;
    }
    if (D_001D3298[3].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF324[i] < 10)
            D_001D3298[4].a++;
        if (D_001CF324[i] < 200)
            D_001D3298[4].b++;
    }
    return D_001D3298[4].a == n;
}

/* ---------------- func_001CC1D0 (src/pilot/frontend/func_001CC1D0.c) ---------------- */
#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_nclip() __asm__ volatile ("nop;" "nop;" "nclip")
#define gte_stopz(r0) __asm__ volatile ("swc2  $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3c(r0) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 4( %0 );" "swc2  $14, 8( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")
/* func_001CBE30, inlined (renamed: the TU also defines func_001CBE30) */
static inline int func_001CBE30_inl(unsigned char *p)
{
    int flag;
    gte_ldv3(&D_001CF318[p[0]], &D_001CF318[p[1]], &D_001CF318[p[2]]);
    gte_rtpt();
    gte_stsxy3c(D_001CF31C);
    if (D_001D3290 == 0) {
        gte_nclip();
        gte_stopz(&flag);
        if (flag < 0) return 1;
    }
    gte_stsz3c(D_001CF324);
    return 0;
}
/* textured quad with recursive subdivision near the screen edges (func_001CBAA4 is the triangle version) */
void func_001CC1D0(unsigned char *p)
{
    POLY_FT4 *t;
    int z;
    int i, j, k;
    int b;

    if (func_001CBE30_inl(p) != 0)
        return;
    gte_ldv0(&D_001CF318[p[3]]);
    gte_rtps();
    gte_stsxy(D_001CF31C + 3);
    gte_stsz(D_001CF324 + 3);
    if (func_001CBEFC(4) != 0)
        return;
    if ((D_001D3298[0].b || D_001D3298[1].b || D_001D3298[2].b || D_001D3298[3].b) && D_001D3290 <= 0) {
        for (i = 0; i < 5; i++) {
            k = p[D_001CF30C[i]];
            b = p[D_001CF30C[i + 1]];
            D_001CF318[D_001D3288].x = (D_001CF318[k].x + D_001CF318[b].x) >> 1;
            D_001CF318[D_001D3288].y = (D_001CF318[k].y + D_001CF318[b].y) >> 1;
            D_001CF318[D_001D3288].z = (D_001CF318[k].z + D_001CF318[b].z) >> 1;
            D_001CF318[D_001D3288].u = (D_001CF318[k].u + D_001CF318[b].u) >> 1;
            D_001CF318[D_001D3288].v = (D_001CF318[k].v + D_001CF318[b].v) >> 1;
            D_001D3288++;
        }
        D_001D3290++;
        for (k = i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (D_001CF314[k] >= 0)
                    D_001CF310[j] = p[D_001CF314[k]];
                else
                    D_001CF310[j] = D_001CF314[k] + D_001D3288;
                k++;
            }
            func_001CC1D0(D_001CF310);
        }
        D_001D3288 -= 5;
        D_001D3290 -= 1;
        return;
    }
    t = (POLY_FT4 *)current->primptr;
    ((P_TAG *)t)->len = 9;
    t->code = 0x2C;
    t->r0 = 0x80;
    t->g0 = 0x80;
    t->b0 = 0x80;
    *(long *)&t->x0 = D_001CF31C[0];
    *(long *)&t->x1 = D_001CF31C[1];
    *(long *)&t->x2 = D_001CF31C[3];
    *(long *)&t->x3 = D_001CF31C[2];
    *(long *)&t->u0 = *(unsigned short *)&D_001CF318[p[0]].u | D_001D3284;
    *(long *)&t->u1 = *(unsigned short *)&D_001CF318[p[1]].u | D_001D3280;
    *(long *)&t->u2 = *(unsigned short *)&D_001CF318[p[3]].u;
    *(long *)&t->u3 = *(unsigned short *)&D_001CF318[p[2]].u;
    switch (D_001CF304) {
    default:
        z = (D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2;
        break;
    case 1:
        z = 0x107D;
        break;
    case 3:
        z = ((D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2) + 0x28;
        break;
    case 2:
        z = ((D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2) + 0x5A;
        break;
    }
    ((P_TAG *)t)->addr = ((P_TAG *)&current->ot[z])->addr;
    ((P_TAG *)&current->ot[z])->addr = (unsigned)t;
    current->primptr += sizeof(POLY_FT4);
}
#undef gte_ldv0
#undef gte_ldv3
#undef gte_nclip
#undef gte_rtps
#undef gte_rtpt
#undef gte_stopz
#undef gte_stsxy
#undef gte_stsxy3c
#undef gte_stsz
#undef gte_stsz3c

/* ---------------- func_001CC76C (src/pilot/frontend/func_001CC76C.c) ---------------- */
/* func_001CC76C: pp[i * 2] giv (its reduced reg is born after the hoisted luis); t->a re-read into a local after the clut store
   (lo_sum globals count as varying addresses, so the int stores conflict with t->a / texture_pages loads). */
void func_001CC76C(T *t)
{
    unsigned char *ip = t->idx;
    int i = 0;
    unsigned char *pp = t->p;
    int a;
    for (; i < 4; i++) {
        D_001CF318[i].x = D_001D328C[*ip].x;
        D_001CF318[i].y = D_001D328C[*ip].y;
        D_001CF318[i].z = D_001D328C[*ip].z;
        D_001CF318[i].u = pp[i * 2];
        D_001CF318[i].v = pp[i * 2 + 1];
        ip++;
    }
    D_001D3284 = texture_cluts[t->a][t->b] << 16;
    a = t->a;
    D_001D3288 = 4;
    D_001D3290 = 0;
    D_001D3280 = texture_pages[a] << 16;
    func_001CC1D0(D_001CF308);
}

/* ---------------- func_001CC894 (src/pilot/frontend/func_001CC894.c) ---------------- */
/* func_001CC894: model draw list + fade overlay. MATCH except a reloc-only diff: the scratchpad plot table base is ONE
 * local `plot = (int *)0x1F800000` shared by the copy (plot + 4, addiu 0x10) and the call table; it becomes a global
 * constant pseudo that reload rematerialises into t0 (lui 0x1f80 / addiu 0x10 = splat's %hi/%lo(D_1F800010), same bytes).
 * FAKEMATCH: char pad[16] only sizes the 0x58 frame (probably a second 16-byte local in the original). */
/* scratchpad plot table end (0x1F800000 + 0x10), splat's label */
/* scratchpad 0x1F800000 + 0x10: splat's label for the plot table end */
#define gte_SetRotMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $0;" "ctc2  $13, $1;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $2;" "ctc2  $13, $3;" "ctc2  $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw $12, 0(%0);" "lw $13, 4(%0);" "lw $14, 8(%0);" \
    "ctc2 $12, $5;" "ctc2 $13, $6;" "ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))
#define copy_words(D, S, N) { int n = (N); int *d = (D) + (N); do { d--; n--; *d = (S)[n]; } while (n); }
void func_001CC894(CELL_OBJECT **objs, int count)
{
    VECTOR v;
    char pad[16];
    int i, j;
    int last;
    CELL_OBJECT *obj;
    MODEL *model;
    unsigned char *poly;
    unsigned char t;
    int k;
    POLY_G4 *g4;
    POLY_GT4 *gt4;
    int *plot = (int *)0x1F800000;

    copy_words(D_001CF308, D_001CF338, 1);
    copy_words(plot, D_001CF328, 4);
    copy_words((int *)D_001CF30C, D_001CF33C, 3);
    copy_words((int *)D_001CF314, D_001CF348, 7);
    last = -1;
    for (i = 0; i < count; i++) {
        obj = *objs++;
        if (last != obj->yang) {
            last = obj->yang;
            gte_SetRotMatrix(&CompoundMatrix[last]);
        }
        v.vx = obj->pos.vx - camera_position.vx;
        v.vy = obj->pos.vy - camera_position.vy;
        v.vz = obj->pos.vz - camera_position.vz;
        func_0002C564(&v);
        gte_SetTransVector(&v);
        model = modelpointers[obj->type];
        D_001CF304 = 0;
        poly = (unsigned char *)model->poly_block;
        D_001D328C = (SRC *)model->vertices;
        if (model->flags2 & 0x400)
            D_001CF304 = 1;
        if (model->flags2 & 0x100)
            D_001CF304 |= 2;
        for (j = 0; j < model->num_polys; j++) {
            t = *poly;
            k = t & 0x1F;
            ((PLOTFN *)plot)[t & 3](poly);
            poly += D_00089F1C[k];
        }
    }
    if (D_001CF2F8) {
        g4 = (POLY_G4 *)current->primptr;
        ((unsigned char *)g4)[3] = 8;
        g4->code = 0x38;
        g4->x0 = 0;
        g4->y0 = 0;
        g4->x1 = 512;
        g4->y1 = 0;
        g4->x2 = 0;
        g4->y2 = 256;
        g4->x3 = 512;
        g4->y3 = 256;
        gt4 = (POLY_GT4 *)(current->primptr + 36);
        current->primptr += 88;
        ((unsigned char *)gt4)[3] = 12;
        gt4->code = 0x3E;
        gt4->x0 = -1;
        gt4->y0 = -1;
        gt4->x1 = 0;
        gt4->y1 = -1;
        gt4->x2 = -1;
        gt4->y2 = 0;
        gt4->x3 = 0;
        gt4->y3 = 0;
        gt4->tpage = GetTPage(0, 2, 0, 0);
        g4->r0 = *(unsigned char *)&D_001CF300;
        g4->g0 = *(unsigned char *)&D_001CF300;
        g4->b0 = *(unsigned char *)&D_001CF300;
        g4->r1 = *(unsigned char *)&D_001CF300;
        g4->g1 = *(unsigned char *)&D_001CF300;
        g4->b1 = *(unsigned char *)&D_001CF300;
        g4->r2 = *(unsigned char *)&D_001CF300;
        g4->g2 = *(unsigned char *)&D_001CF300;
        g4->b2 = *(unsigned char *)&D_001CF300;
        g4->r3 = *(unsigned char *)&D_001CF300;
        g4->g3 = *(unsigned char *)&D_001CF300;
        g4->b3 = *(unsigned char *)&D_001CF300;
        g4->code |= 2;
        addPrim(current->ot[1], g4);
        addPrim(current->ot[2], gt4);
    }
}
#undef addPrim
#undef copy_words
#undef gte_SetRotMatrix
#undef gte_SetTransVector

