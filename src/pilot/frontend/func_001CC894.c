/* func_001CC894: model draw list + fade overlay. MATCH except a reloc-only diff: the scratchpad plot table base is ONE
 * local `plot = (int *)0x1F800000` shared by the copy (plot + 4, addiu 0x10) and the call table; it becomes a global
 * constant pseudo that reload rematerialises into t0 (lui 0x1f80 / addiu 0x10 = splat's %hi/%lo(D_1F800010), same bytes).
 * FAKEMATCH: char pad[16] only sizes the 0x58 frame (probably a second 16-byte local in the original). */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { unsigned int *p; unsigned int **ot; } DB;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char r3, g3, b3, p3;
    short x3, y3;
} PG4;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char u2, v2; unsigned short pad2;
    unsigned char r3, g3, b3, p3;
    short x3, y3;
    unsigned char u3, v3; unsigned short pad3;
} PGT4;
typedef struct {
    int x, y, z;
    char pad[1];
    unsigned char mat;      /* 0x0D */
    unsigned short model;   /* 0x0E */
} OBJ;
typedef struct {
    short pad0;
    unsigned short flags;   /* 0x02 */
    char pad4[0xA];
    unsigned short num_polys; /* 0x0E */
    void *verts;            /* 0x10 */
    unsigned char *polys;   /* 0x14 */
} MODEL;
typedef void (*PLOTFN)(unsigned char *);

extern int *D_001CF308, *D_001CF30C, *D_001CF314;
extern int D_001CF328[], D_001CF338[], D_001CF33C[], D_001CF348[];
extern int D_001CF2F8, D_001CF304;
extern unsigned char D_001CF300;
extern void *D_001D328C;
extern int D_00089F1C[];
extern int D_1F800010[];   /* scratchpad plot table end (0x1F800000 + 0x10), splat's label */   /* scratchpad 0x1F800000 + 0x10: splat's label for the plot table end */
extern MATRIX CompoundMatrix[];
extern VECTOR camera_position;
extern MODEL *modelpointers[];
extern DB *current;
extern void func_0002C564(VECTOR *v);
extern unsigned short GetTPage();

#define gte_SetRotMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $0;" "ctc2  $13, $1;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $2;" "ctc2  $13, $3;" "ctc2  $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw $12, 0(%0);" "lw $13, 4(%0);" "lw $14, 8(%0);" \
    "ctc2 $12, $5;" "ctc2 $13, $6;" "ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))

#define copy_words(D, S, N) { int n = (N); int *d = (D) + (N); do { d--; n--; *d = (S)[n]; } while (n); }

void func_001CC894(OBJ **objs, int count)
{
    VECTOR v;
    char pad[16];
    int i, j;
    int last;
    OBJ *obj;
    MODEL *model;
    unsigned char *poly;
    unsigned char t;
    int k;
    PG4 *g4;
    PGT4 *gt4;
    int *plot = (int *)0x1F800000;

    copy_words(D_001CF308, D_001CF338, 1);
    copy_words(plot, D_001CF328, 4);
    copy_words(D_001CF30C, D_001CF33C, 3);
    copy_words(D_001CF314, D_001CF348, 7);
    last = -1;
    for (i = 0; i < count; i++) {
        obj = *objs++;
        if (last != obj->mat) {
            last = obj->mat;
            gte_SetRotMatrix(&CompoundMatrix[last]);
        }
        v.vx = obj->x - camera_position.vx;
        v.vy = obj->y - camera_position.vy;
        v.vz = obj->z - camera_position.vz;
        func_0002C564(&v);
        gte_SetTransVector(&v);
        model = modelpointers[obj->model];
        D_001CF304 = 0;
        poly = model->polys;
        D_001D328C = model->verts;
        if (model->flags & 0x400)
            D_001CF304 = 1;
        if (model->flags & 0x100)
            D_001CF304 |= 2;
        for (j = 0; j < model->num_polys; j++) {
            t = *poly;
            k = t & 0x1F;
            ((PLOTFN *)plot)[t & 3](poly);
            poly += D_00089F1C[k];
        }
    }
    if (D_001CF2F8) {
        g4 = (PG4 *)current->p;
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
        gt4 = (PGT4 *)(current->p + 9);
        current->p += 22;
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
        g4->r0 = D_001CF300;
        g4->g0 = D_001CF300;
        g4->b0 = D_001CF300;
        g4->r1 = D_001CF300;
        g4->g1 = D_001CF300;
        g4->b1 = D_001CF300;
        g4->r2 = D_001CF300;
        g4->g2 = D_001CF300;
        g4->b2 = D_001CF300;
        g4->r3 = D_001CF300;
        g4->g3 = D_001CF300;
        g4->b3 = D_001CF300;
        g4->code |= 2;
        addPrim(current->ot[1], g4);
        addPrim(current->ot[2], gt4);
    }
}
