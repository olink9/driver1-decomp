/* AddLeaf (Driver 2 name) */
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { unsigned char r, g, b, pad; } RGBX;
typedef struct {
    VEC position;
    SVEC direction;
    unsigned short life;
    unsigned short flags;
    unsigned short num;
    unsigned short pos;
    RGBX rgb;
    char step;
    char type;
    short sin_index1;
    short sin_index2;
    char sin_addition1;
    char sin_addition2;
} LEAF;

extern int D_00096AC0;
extern int D_00097430;
extern short D_000B7EC8[];
extern LEAF D_000B7584[];
extern long long gNight;
extern unsigned char D_00096AA8;
extern int func_000776E4();

static inline int AllocateLeaf(void)
{
    if (D_00097430 < 50)
        return D_000B7EC8[D_00097430++];
    return -1;
}

void func_00041AC4(VEC *Position, int num_leaves, int Type)
{
    int num;
    int temprand;
    int i;
    LEAF *myleaf;

    if (D_00096AC0 == 0)
        return;

    for (i = 0; i < num_leaves; i++) {
        temprand = func_000776E4();
        num = AllocateLeaf();
        if (num < 0)
            return;

        myleaf = &D_000B7584[num];

        myleaf->position.vx = Position->vx + (temprand & 0xfe);
        myleaf->position.vy = Position->vy;
        myleaf->position.vz = Position->vz + (temprand >> 8 & 0xfe);

        myleaf->life = 600;
        myleaf->flags = 2;

        myleaf->direction.vx = 0;
        myleaf->direction.vy = 0;
        myleaf->direction.vz = 0;

        myleaf->pos = (temprand >> 7) & 0xff;
        myleaf->step = (temprand & 7) + 1;

        if (Type == 1) {
            if ((temprand & 3) == 0) {
                myleaf->rgb.r = 110;
                myleaf->rgb.g = 112;
                myleaf->rgb.b = 15;
            } else if ((temprand & 3) == 1) {
                myleaf->rgb.r = 20;
                myleaf->rgb.g = 44;
                myleaf->rgb.b = 0;
            } else {
                myleaf->rgb.r = 60;
                myleaf->rgb.g = 50;
                myleaf->rgb.b = 30;
            }
        } else {
            myleaf->rgb.b = myleaf->rgb.g = myleaf->rgb.r = (temprand & 0x3f) + 80;
        }

        if (*(int *)&gNight != 0)
            *(int *)&myleaf->rgb = *(int *)&myleaf->rgb >> 2 & 0x3f3f3f3f;
        else if (D_00096AA8 != 0)
            *(int *)&myleaf->rgb = *(int *)&myleaf->rgb >> 1 & 0x7f7f7f7f;

        myleaf->sin_index1 = temprand & 0xfff;
        myleaf->sin_index2 = temprand >> 4 & 0xfff;
        myleaf->sin_addition1 = -(temprand & 3);
        myleaf->sin_addition2 = -(temprand >> 8 & 7);

        myleaf->type = Type;
    }
}
