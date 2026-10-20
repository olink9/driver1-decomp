/* Driver 2 name: CameraCollisionCheck */
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short m[3][3]; short pad; int t[3]; } MAT;
typedef struct {
    short type;
    short xpos, ypos, zpos;
    short pad8;
    short yang;
    short padC;
    short xsize, ysize, zsize;
} COLLISION_PACKET;
typedef struct {
    short pad0;
    unsigned short flags2;
    char pad4[4];
    short bounding_sphere;
    char padA[0x20 - 0xA];
    int *collision_block;
} MODEL;
typedef struct {
    int vx, vy, vz;
    char padC;
    unsigned char yang;
    unsigned short type;
} CELL_OBJECT;
typedef struct { unsigned short obj; unsigned short next; } CELL_DATA;

extern VEC D_0008A8F0;
extern MODEL *D_000A0E20[];
extern MAT D_000A6E8C[];
extern long long D_000970E4;
extern long long D_0009733C;
extern long long D_00097398;
extern long long D_00097368;
extern long long D_000970E0;
extern long long D_000970D4;
extern long long D_00097108;
extern int D_00096A60;

#define camera_position D_0008A8F0
#define scr_z (*(int *)&D_000970E4)
#define cells_across (*(int *)&D_0009733C)
#define units_across_halved (*(int *)&D_00097398)
#define units_down_halved (*(int *)&D_00097368)
#define cell_ptrs (*(unsigned short **)&D_000970E0)
#define cell_data (*(CELL_DATA **)&D_000970D4)
#define cell_objects (*(CELL_OBJECT **)&D_00097108)
#define modelpointers D_000A0E20
#define matrixtable D_000A6E8C

#define FIXED(x) ((x) / 4096)

int func_0003C998(void)
{
    char pad0[0x30];
    VEC temp_cam;
    VEC offset;
    char pad2[0x20];
    int count;
    int sphere;
    int cellx, cellz;
    int cx, cz;
    unsigned short cell;
    CELL_DATA *cd;
    CELL_OBJECT *cop;
    MODEL *model;
    int *boxptr;
    COLLISION_PACKET *collide;
    MAT *mat;
    int num_cb, box_loop;
    int xd, zd;
    int xmin, xmax, ymin, ymax, zmin, zmax;
    int tx, tz;
    int region, xm;
    int ox, oz;
    int sphere_sq;

    count = 0;
    { int sz2 = scr_z * 2 - 382; sphere = scr_z + sz2; }
    do {
        ox = (count % 3) * 1024 - 1024;
        oz = (count / 3) * 1024 - 1024;
        temp_cam.vx = camera_position.vx + ox;
        temp_cam.vz = camera_position.vz + oz;
        cellx = (temp_cam.vx + units_across_halved) / 3000;
        cellz = (temp_cam.vz + units_down_halved) / 3000;
        cx = cellx / cells_across;
        cz = cellz / cells_across;
        cellx -= cx * cells_across;
        cellz -= cz * cells_across;
        cell = cell_ptrs[((cx & 1) + (cz & 1) * 2) * cells_across * cells_across + cellx + cellz * cells_across];
        if (D_00096A60 > 0) {
            while (cell != 0xFFFF) {
                cd = &cell_data[cell];
                cop = &cell_objects[cd->obj];
                model = modelpointers[cop->type];
                boxptr = model->collision_block;
                if (boxptr != 0 && !(model->flags2 & 0x800)) {
                    sphere_sq = model->bounding_sphere;
                    sphere_sq *= sphere_sq;
                    xd = cop->vx - camera_position.vx;
                    if (xd < 0)
                        xd = camera_position.vx - cop->vx;
                    zd = cop->vz - camera_position.vz;
                    if (zd < 0)
                        zd = camera_position.vz - cop->vz;
                    if (sphere_sq + sphere * sphere > xd * xd + zd * zd) {
                        num_cb = *boxptr;
                        collide = (COLLISION_PACKET *)(boxptr + 1);
                        for (box_loop = 0; box_loop < num_cb; box_loop++, collide++) {
                            switch (collide->type) {
                            case 0:
                                mat = &matrixtable[cop->yang];
                                tx = camera_position.vx - cop->vx;
                                tz = camera_position.vz - cop->vz;
                                offset.vx = FIXED(tx * mat->m[0][0] + tz * mat->m[2][0]);
                                offset.vy = camera_position.vy - cop->vy;
                                offset.vz = FIXED(tx * mat->m[0][2] + tz * mat->m[2][2]);
                                offset.vx -= collide->xpos;
                                offset.vy -= collide->ypos;
                                offset.vz -= collide->zpos;
                                if (collide->yang != 0) {
                                    mat = &matrixtable[collide->yang];
                                    tx = offset.vx;
                                    tz = offset.vz;
                                    offset.vx = tx * mat->m[0][0] + tz * mat->m[2][0];
                                    offset.vz = tx * mat->m[0][2] + tz * mat->m[2][2];
                                }
                                if (collide->xsize < 500 && collide->zsize < 500)
                                    continue;
                                xmax = (collide->xsize >> 1) + sphere;
                                xmin = -xmax;
                                ymax = (collide->ysize >> 1) + sphere;
                                ymin = -ymax;
                                zmax = (collide->zsize >> 1) + sphere;
                                zmin = -zmax;
                                if (xmin < offset.vx && offset.vx < xmax &&
                                    zmin < offset.vz && offset.vz < zmax &&
                                    ymin < offset.vy && offset.vy < ymax)
                                    return 1;
                                break;
                            case 1:
                                mat = &matrixtable[-cop->yang & 0x3F];
                                offset.vx = FIXED(collide->xpos * mat->m[0][0] + collide->zpos * mat->m[2][0]);
                                offset.vz = FIXED(collide->xpos * mat->m[0][2] + collide->zpos * mat->m[2][2]);
                                offset.vx += cop->vx;
                                offset.vy = collide->ypos + cop->vy;
                                offset.vz += cop->vz;
                                offset.vx -= camera_position.vx;
                                offset.vy -= camera_position.vy;
                                offset.vz -= camera_position.vz;
                                if (offset.vx * offset.vx + offset.vy * offset.vy + offset.vz * offset.vz <
                                    (collide->xsize * collide->xsize) / 2)
                                    return 1;
                                break;
                            }
                        }
                    }
                }
                cell = cd->next;
            }
        }
        count++;
    } while (count < 9);
    return 0;
}
