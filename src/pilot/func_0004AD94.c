/* run3-r4 (r4-19): BYTE-IDENTICAL. tools/fnscore.py prints 99.98 only because reloc_target() cannot parse a
 * NEGATIVE addend: row 757/759 is %hi/%lo(D_000BC004-0x20) (= &D_000BC004[D_000974B0 - 1]) vs the splat label
 * D_000BBFE4 at the same address 0xBBFE4. With the regex accepting [+-] addends the scorer gives MATCH 100.0;
 * rows 240/247 (D_000BC798+0x10 vs D_000BC7A8) already pass. Needs linkcheck, then promote to .c.
 * Fix of the last 4 register rows (bounding-sphere test): the D2 shape with fresh single-set locals -
 * distx = dx*dx; distz = dz*dz; extra = speed + 580; sphere = bs + extra; sphere *= sphere; if (distx + distz < sphere).
 * `sphere *= sphere` ties the product into the sum's quantity, which drops bs/sum below the speed quantity in
 * local-alloc priority (speed -> v0, bs/sum -> v1) and restores the lh-before-lw order and dx*dx in a0. */
/* Driver 2: CheckScenaryCollisions (Driver 1 version; car 0x19 = camera) */
#include "gte.h"

typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

typedef struct {
    unsigned char controlType;  /* 0x00 */
    char pad0[0x17];
    int where[3];               /* 0x18 */
    char pad1[0xF8 - 0x24];
    int direction;              /* 0xF8 */
    char pad4[0x108 - 0xFC];
    int speed;                  /* 0x108 */
    char pad5[0x12F - 0x10C];
    unsigned char hit;          /* 0x12F */
    char pad6[396 - 0x130];
} CAR_DATA;

typedef struct {
    VECTOR pos;
    int xsize, zsize;
    int theta;
    int height;
} BUILDING_BOX;

typedef struct {
    int vx, vy, vz;
    unsigned char pad;
    unsigned char yang;
    unsigned short type;
} CELL_OBJECT;

typedef struct {
    unsigned short shape_flags;
    unsigned short flags2;
    short instance_number;
    unsigned char tri_verts, zBias;
    short bounding_sphere;
    unsigned short num_point_normals;
    unsigned short num_vertices;
    unsigned short num_polys;
    int vertices, poly_block, normals, point_normals;
    int collision_block;
} MODEL;

typedef struct {
    short type, xpos, ypos, zpos, flags, yang, empty, xsize, ysize, zsize;
} COLLISION_PACKET;

typedef struct { unsigned short num, next; } PACKED_CELL_OBJECT_LINK;

extern CAR_DATA car_data[];
extern BUILDING_BOX D_000BC004[];
extern CELL_OBJECT *D_000BBF14[];
extern int D_000BC784[];
extern int D_000BC798[];
extern int RoadMapRegions[];
extern MODEL *modelpointers[];
extern MATRIX matrixtable[];
extern CELL_OBJECT D_000A7ABC[];
extern short rcossin_tbl[];
extern short D_0008EAAA[];

extern long long D_00096880, D_00096798, D_00097398, D_00097368, D_0009733C, D_00096A24;
extern long long D_000970E0, D_000970D4, D_00097108, D_000971D0, D_000967F4, D_00096834;
extern long long D_0008A8F8, D_00096A60, D_00096A64, pauseflag, D_00096899;
extern int D_0009B1AC[];

extern CELL_OBJECT **D_000974A4;
extern short D_000974A8;
extern int D_000974AC;
extern int D_000974B0;
extern int D_00096C50;

extern void func_0004AC88(int a, int b, int c);
extern int MapHeight(VECTOR *pos);
extern int CarBuildingCollision(CAR_DATA *cp, BUILDING_BOX *bbox, CELL_OBJECT *cop);
extern int func_0004BD48(BUILDING_BOX *boxes, int i, int j, int car);
extern void func_0004EAC0(VECTOR *v);

#define cam_dist (*(int *)&D_00096A60)

void func_0004AD94(int car)
{
    VECTOR player_pos;
    BUILDING_BOX bbox;
    VECTOR offset;
    CAR_DATA *cp;
    COLLISION_PACKET *collide;
    int num_cb;
    int x0, z0;
    int num, i, k, count, step;
    int cj, bi, bj, pi;
    int cellx, cellz, cx, cz, r, cbrx, cbrz, cbr;
    unsigned int cell;
    PACKED_CELL_OBJECT_LINK *ptr;
    CELL_OBJECT *cop;
    MODEL *model;
    BUILDING_BOX *bb;
    BUILDING_BOX *b0;
    int dx, dz, sphere;
    int *lst;
    MATRIX *mat;
    int extra, distx, distz;

    x0 = car_data[0].where[0];
    z0 = car_data[0].where[2];
    if (*(unsigned char *)&D_00096880 || (car == 0 && *(int *)&D_00096798))
        return;

    D_000974B0 = 0;
    D_000974AC = -1;
    cp = &car_data[car];
    for (b0 = D_000BC004; b0 < &D_000BC004[60]; b0++)
        b0->xsize = 0;

    num = 0;
    player_pos.vx = cp->where[0];
    player_pos.vy = cp->where[1];
    player_pos.vz = cp->where[2];
    lst = D_000BC798;
    cellx = (player_pos.vx + *(int *)&D_00097398 - 1500) / 3000;
    for (i = 0; i < 2; i++, cellx++) {
        cellz = (player_pos.vz + *(int *)&D_00097368 - 1500) / 3000;
        for (cj = 0; cj < 2; lst++, cj++, cellz++) {
            r = *(int *)&D_0009733C;
            cbrx = cellx / r;
            cbrz = cellz / r;
            cx = cellx - cbrx * r;
            cz = cellz - cbrz * r;
            cbr = (cbrx & 1) + (cbrz & 1) * 2;
            cell = ((unsigned short *)*(int *)&D_000970E0)[cbr * r * r + cx + cz * r];
            *lst = num;
            if (RoadMapRegions[cbr] == cbrx + cbrz * (*(int *)&D_00096A24 / *(int *)&D_0009733C)) {
                while (cell != 0xFFFF && num < 296) {
                    ptr = &((PACKED_CELL_OBJECT_LINK *)*(int *)&D_000970D4)[cell];
                    cop = &((CELL_OBJECT *)*(int *)&D_00097108)[ptr->num];
                    D_000974A4[num++] = cop;
                    cell = ptr->next;
                }
            }
        }
    }

    D_000BC798[4] = num;
    D_000BC784[4] = 0;
    D_000BC784[0] = D_000BC798[1] - D_000BC798[0];
    D_000BC784[1] = D_000BC798[2] - D_000BC798[1];
    D_000BC784[2] = D_000BC798[3] - D_000BC798[2];
    D_000BC784[3] = num - D_000BC798[3];
    func_0004AC88(0, 1, 4);
    func_0004AC88(2, 4, 0);
    func_0004AC88(0, 3, 4);
    num = D_000BC784[4];

    for (i = 0; i < num + *(int *)&D_000971D0; i++) {
        if (i >= num)
            cop = &D_000A7ABC[i - num];
        else
            cop = D_000974A4[D_000BC798[4] + i];
        model = modelpointers[cop->type];
        if (car == 0x17 && *(int *)&D_000967F4 == 0x16 && (model->flags2 & 0x800))
            continue;
        if ((unsigned)model->num_vertices - 3 < 300 && model->num_point_normals < 300 &&
            model->num_polys < 300 && model->collision_block != 0) {
            if (car == 0x19 && (model->flags2 & 0x100)) {
                int m = *(int *)&D_00096834;
                if (m == 3 || (m == 5 && *(int *)&D_0008A8F8 > -445000) ||
                    (m == 1 && cp->where[2] > 238000)) {
                    player_pos.vy = MapHeight(&player_pos) + 500;
                    cp->where[1] = player_pos.vy;
                }
            }
            dx = cop->vx - player_pos.vx;
            dz = cop->vz - player_pos.vz;
            distx = dx * dx;
            distz = dz * dz;
            extra = cp->speed + 580;
            sphere = model->bounding_sphere + extra;
            sphere *= sphere;
            if (distx + distz < sphere) {
                num_cb = *(int *)model->collision_block;
                collide = (COLLISION_PACKET *)(model->collision_block + 4);
                for (k = 0; k < num_cb; k++, collide++) {
                    switch (collide->type) {
                    case 0:
                        mat = &matrixtable[-cop->yang & 0x3f];
                        offset.vx = (collide->xpos * mat->m[0][0] + collide->zpos * mat->m[2][0]) / 4096;
                        offset.vz = (collide->xpos * mat->m[0][2] + collide->zpos * mat->m[2][2]) / 4096;
                        bbox.pos.vx = cop->vx + offset.vx;
                        bbox.pos.vy = cop->vy + collide->ypos;
                        bbox.pos.vz = cop->vz + offset.vz;
                        bbox.xsize = collide->zsize / 2;
                        bbox.zsize = collide->xsize / 2;
                        bbox.theta = (cop->yang + collide->yang) * 64 & 0xfff;
                        bbox.height = collide->ysize;
                        D_000974A8 = cop->type;
                        break;
                    case 1:
                        offset.vx = collide->xpos;
                        offset.vy = collide->ypos;
                        offset.vz = collide->zpos;
                        gte_SetRotMatrix(&matrixtable[-cop->yang & 0x3f]);
                        func_0004EAC0(&offset);
                        bbox.pos.vx = cop->vx + offset.vx;
                        bbox.pos.vy = cop->vy + offset.vy;
                        bbox.pos.vz = cop->vz + offset.vz;
                        bbox.xsize = collide->xsize / 2;
                        bbox.zsize = collide->xsize / 2;
                        bbox.theta = cop->yang * 64 & 0xfff;
                        bbox.height = collide->ysize;
                        D_000974A8 = cop->type;
                        break;
                    }
                    if (car == 0x19) {
                        if (modelpointers[cop->type]->flags2 & 0xA00)
                            goto next;
                        if (bbox.xsize > 100 || bbox.zsize > 100) {
                            bbox.xsize += 100;
                            bbox.zsize += 100;
                            if (D_000974B0 < 60) {
                                D_000BC004[D_000974B0] = bbox;
                                D_000BBF14[D_000974B0] = cop;
                                D_000974B0++;
                            } else {
                                if (cam_dist >= 0x104) {
                                count = 5;
                                while (CarBuildingCollision(cp, &bbox, D_000BBF14[D_000974B0]) && count > 0) {
                                    cam_dist -= D_000974AC;
                                    if (cam_dist < 0x104)
                                        cam_dist = 0x104;
                                    cp->where[0] = x0 + ((cam_dist * rcossin_tbl[(cp->direction & 0xfff) * 2] / 2 + 0x800) >> 12);
                                    cp->where[2] = z0 + ((cam_dist * rcossin_tbl[(cp->direction & 0xfff) * 2 + 1] / 2 + 0x800) >> 12);
                                    count--;
                                    if (cam_dist < 0x104)
                                        goto next;
                                }
                                }
                            }
                        }
                    } else {
                        if (D_000974B0 < 60 && !(modelpointers[cop->type]->flags2 & 0x800)) {
                            D_000BC004[D_000974B0] = bbox;
                            D_000BBF14[D_000974B0] = cop;
                                D_000974B0++;
                        } else if (CarBuildingCollision(cp, &bbox, cop)) {
                            cp->hit = 1;
                        }
                    }
                next:
                    if (D_00096C50 < D_000974B0)
                        D_00096C50 = D_000974B0;
                }
            }
        }
    }

    for (bi = 0; bi < D_000974B0; bi++) {
        if (D_000BC004[bi].xsize) {
            for (bj = bi + 1; bj < D_000974B0; bj++) {
                if (D_000BC004[bj].xsize && func_0004BD48(D_000BC004, bj, bi, car))
                    break;
            }
        }
    }

    if (car == 0x19) {
        bb = &D_000BC004[D_000974B0 - 1];
        for (pi = D_000974B0 - 1; pi >= 0; pi--, bb--) {
            if (bb->xsize) {
                count = 5;
                if (cam_dist >= 0x104)
                while (CarBuildingCollision(cp, bb, D_000BBF14[pi]) && count > 0) {
                    cam_dist -= D_000974AC;
                    if (cam_dist < 0x104)
                        cam_dist = 0x104;
                    cp->where[0] = x0 + ((cam_dist * rcossin_tbl[(cp->direction & 0xfff) * 2] / 2 + 0x800) >> 12);
                    cp->where[2] = z0 + ((cam_dist * D_0008EAAA[(cp->direction & 0xfff) * 2] / 2 + 0x800) >> 12);
                    count--;
                    if (cam_dist < 0x104)
                        break;
                }
            }
        }
        if (D_000974AC == -1 && cam_dist < *(int *)&D_00096A64 &&
            (!*(int *)&pauseflag || !*(unsigned char *)&D_00096899)) {
            int ox = cp->where[0];
            int oz = cp->where[2];
            step = 10;
            if (abs(D_0009B1AC[0]) >= 10 && cam_dist + 30 <= *(int *)&D_00096A64)
                step = 20;
            cam_dist += step;
            if (cam_dist > *(int *)&D_00096A64)
                cam_dist = *(int *)&D_00096A64;
            cp->where[0] = car_data[0].where[0] + ((cam_dist * rcossin_tbl[(cp->direction & 0xfff) * 2] / 2 + 0x800) >> 12);
            cp->where[2] = car_data[0].where[2] + ((cam_dist * rcossin_tbl[(cp->direction & 0xfff) * 2 + 1] / 2 + 0x800) >> 12);
            bb = D_000BC004;
            for (pi = 0; pi < D_000974B0; pi++, bb++) {
                if (bb->xsize && CarBuildingCollision(cp, bb, D_000BBF14[pi])) {
                    cam_dist -= step;
                    if (cam_dist < 0x104)
                        cam_dist = 0x104;
                    cp->where[0] = ox;
                    cp->where[2] = oz;
                    return;
                }
            }
        }
    } else {
        for (bi = 0; bi < D_000974B0; bi++) {
            if (D_000BC004[bi].xsize && CarBuildingCollision(cp, &D_000BC004[bi], D_000BBF14[bi]))
                cp->hit = 1;
        }
    }
}
