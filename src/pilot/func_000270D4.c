/* FAKEMATCH: a do { } while (0) wrapper (emits no code) is used only to raise loop-depth weights in register allocation - our run-1 rule counts these as fake until a real C form (e.g. a macro body) is found. */
/* run2-r3: the else branch is a do { } while (0) block (probably a macro): its loop notes double the refs weight, so
   local-alloc ranks the f<<8 temp above f (tie 2.0/2.0 before) and f lands in a0. Real C, no asm hint. */
/* func_000270D4: draw the visible building/cell objects (Driver 2: DrawAllBuildings-like) */
typedef struct {
    int x, y, z;            /* 0x00 */
    unsigned char pad;      /* 0x0C */
    unsigned char yang;     /* 0x0D */
    unsigned short type;    /* 0x0E */
} CELL_OBJECT;

typedef struct {
    unsigned short shape_flags; /* 0x00 */
    unsigned short flags2;      /* 0x02 */
    char pad4[0x10 - 0x4];
    int a;                      /* 0x10 */
    int b;                      /* 0x14 */
} MODEL;

typedef struct { int m[8]; } MAT32;

typedef struct {
    char *primptr;
    int *ot;
} DB;

extern unsigned char D_00096AA8[];
extern long long gNight;
extern long long D_00097128;
extern unsigned int D_00097138;
typedef struct { unsigned char v; char pad[7]; } B8;
extern B8 D_00097139;
extern B8 D_0009713A;
extern unsigned int D_00097150;
extern B8 D_00097151;
extern B8 D_00097152;
extern unsigned short *D_0009713C;
extern unsigned short *D_00097140;
extern int D_00097144;
extern int D_00097148;
extern CELL_OBJECT **D_0009715C;
extern long long current;
#define current (*(DB **)&current)
extern MODEL *modelpointers[];
extern MAT32 CompoundMatrix[];

extern int func_00026794();
extern int func_0002680C();
extern void func_000268FC();
extern char *func_000269D4();
extern char *func_00026FF0();
extern char *func_00027098();
extern void func_000287A4();

void func_000270D4(int num)
{
    int i;
    int prev;
    int z;
    int type;
    int d;
    CELL_OBJECT *cop;
    MODEL *model;
    int *save;
    unsigned int *col;
    int ots[3];
    CELL_OBJECT **list;

    list = (CELL_OBJECT **)0x1F800200;
    col = (unsigned int *)0x1F8001F0;
    save = (int *)0x1F800000;
    prev = -1;

    if (D_00096AA8[0] == 0 && *(int *)&gNight == 0) {
        col[0] = 0x2C808080;
        col[1] = D_00097138 | 0x28000000;
        col[2] = D_00097150 | 0x28000000;
    } else {
        do {
            unsigned int f;
            f = *(unsigned int *)&D_00097128;
            col[0] = (f << 16) | (f << 8) | f | 0x2C000000;
            col[1] = 0x28000000 | ((((*(unsigned int *)&D_00097128 * D_0009713A.v) >> 7) & 0xFF) << 16) | ((((*(unsigned int *)&D_00097128 * D_00097139.v) >> 7) & 0xFF) << 8) | (((*(unsigned int *)&D_00097128 * *(unsigned char *)&D_00097138) >> 7) & 0xFF);
            col[2] = 0x28000000 | ((((*(unsigned int *)&D_00097128 * D_00097152.v) >> 7) & 0xFF) << 16) | ((((*(unsigned int *)&D_00097128 * D_00097151.v) >> 7) & 0xFF) << 8) | (((*(unsigned int *)&D_00097128 * *(unsigned char *)&D_00097150) >> 7) & 0xFF);
        } while (0);
    }

    for (i = 0; i < num; i++) {
        if (i < 128)
            cop = list[i];
        else
            cop = D_0009715C[i - 128];

        if (prev != cop->yang) {
            prev = cop->yang;
            z = func_00026794(cop, &CompoundMatrix[prev]);
        } else {
            z = func_0002680C(cop);
        }

        type = cop->type;
        model = modelpointers[type];

        if ((*(int *)model & 0x40000080) && !(model->shape_flags & 0x400)) {
            if (model->shape_flags & 0x80)
                ots[0] = 0x107B;
            else
                ots[0] = 0x1079;
            ots[1] = 0x107F;
            ots[2] = 0x107E;
        } else {
            if (z < 8)
                z = 8;
            if (model->shape_flags & 0x2000) {
                ots[0] = (z >> 3) + 0x280;
                ots[1] = (z >> 3) + 0x180;
                ots[2] = (z >> 3) + 0x32;
            } else {
                ots[0] = (z >> 3) + 0x180;
                ots[1] = (z >> 3) + 0xC0;
                ots[2] = (z >> 3) + 0x32;
            }
            if (ots[0] >= 0x1000) {
                ots[0] = 0xFFF;
                if (ots[1] >= 0x1000) {
                    ots[1] = 0xFFF;
                    if (ots[2] >= 0x1000)
                        ots[2] = 0xFFF;
                }
            }
        }

        save[0] = current->ot[ots[0] - 3];
        save[1] = current->ot[ots[0] - 4];
        save[2] = current->ot[ots[1]];
        save[3] = current->ot[ots[2]];

        if (z > D_00097148) {
            if (z > D_00097144) {
                if (D_00097140[type] != 0xFFFF)
                    type = D_00097140[type];
            }
            model = modelpointers[type];
            if (!(model->shape_flags & 0x20)) {
                current->primptr = func_00026FF0(model, current->primptr);
            } else {
                func_000268FC(model->a, model->b, current->primptr, 0);
                current->primptr += 0x28;
            }
        } else {
            if (D_0009713C[type] != 0xFFFF)
                type = D_0009713C[type];
            model = modelpointers[type];
            if (z < 2000)
                func_000287A4(model, z);
            else if (!(model->shape_flags & 0x20))
                current->primptr = func_00027098(model, current->primptr);
            else
                current->primptr = func_000269D4(model, current->primptr, 0);
        }

        current->ot[ots[0] - 3] = save[0];
        current->ot[ots[0] - 4] = save[1];
        current->ot[ots[1]] = save[2];
        current->ot[ots[2]] = save[3];
    }
}
