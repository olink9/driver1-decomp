/* CameraBar (Driver 2 name) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, x1, y1;
} LINE_F2;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
    unsigned char r3, g3, b3, pad3;
    short x3, y3;
} POLY_G4;

struct PC {
    char pad[0x12];
    short anglepad;
    int FrameCnt;
    char pad2[8];
    unsigned char cameraview;
    unsigned char next;
    char pad3[2];
};
extern struct PC *D_00097608;
extern int *D_00097668;

#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))

void func_00058FCC(int CameraCnt)
{
    int x;
    int bar_x;
    POLY_G4 *camera;
    LINE_F2 *bar;
    int max_x;
    int min_x;
    int min_y;
    int max_y;
    int frame;
    char red;
    char green;
    char blue;
    int first;
    int last;
    unsigned int idx;

    red = 0;
    green = 0;
    blue = 0;
    first = 1;
    last = 1;
    min_y = 15;
    max_y = 20;
    idx = 0;

    bar = (LINE_F2 *)current->primptr;
    ((P_TAG *)bar)->len = 3;
    bar->code = 0x40;
    bar->r0 = 0;
    bar->g0 = 0;
    bar->b0 = 0;
    bar->x0 = 256;
    bar->y0 = 12;
    bar->x1 = 256;
    bar->y1 = 22;
    addPrim(current->ot, bar);
    current->primptr += sizeof(LINE_F2);

    while (D_00097608[idx].FrameCnt != 100000 && idx < 60) {
        min_x = D_00097608[idx].FrameCnt;
        if (D_00097608[idx].next < 60)
            max_x = D_00097608[D_00097608[idx].next].FrameCnt;
        else
            max_x = 200000;

        frame = *D_00097668 + 1;
        if (frame < max_x) {
            last = 2;
            max_x = frame;
        }

        min_x += 256;
        max_x += 256;
        bar_x = min_x - (min_x & 0x1f) - CameraCnt;
        min_x -= CameraCnt;
        max_x -= CameraCnt;

        if (first && min_x > 49 && min_x < 463) {
            bar = (LINE_F2 *)current->primptr;
            ((P_TAG *)bar)->len = 3;
            bar->code = 0x40;
            bar->r0 = 0;
            bar->g0 = 0;
            bar->b0 = 0;
            bar->x0 = min_x;
            bar->y0 = min_y;
            bar->x1 = min_x;
            bar->y1 = 19;
            addPrim(current->ot, bar);
            current->primptr += sizeof(LINE_F2);
        }

        x = 0;
        while (x < last) {
            if (max_x - 50U < 413) {
                bar = (LINE_F2 *)current->primptr;
                ((P_TAG *)bar)->len = 3;
                bar->code = 0x40;
                bar->r0 = 0;
                bar->g0 = 0;
                bar->b0 = 0;
                bar->x0 = max_x + x;
                bar->y0 = min_y;
                bar->x1 = max_x + x;
                bar->y1 = 19;
                addPrim(current->ot, bar);
                current->primptr += sizeof(LINE_F2);
            }
            x++;
        }

        x = bar_x;
        while (x < max_x) {
            if (x - 50U < 413) {
                bar = (LINE_F2 *)current->primptr;
                ((P_TAG *)bar)->len = 3;
                bar->code = 0x40;
                bar->r0 = 0;
                bar->g0 = 0;
                bar->b0 = 0;
                bar->x0 = x;
                bar->y0 = 17;
                bar->x1 = x;
                bar->y1 = 19;
                addPrim(current->ot, bar);
                current->primptr += sizeof(LINE_F2);
            }
            x += 32;
        }

        switch (D_00097608[idx].cameraview & 7) {
        case 0:
            red = 255;
            green = 0;
            blue = 0;
            break;
        case 1:
            red = 0;
            green = 255;
            blue = 0;
            break;
        case 2:
            if (D_00097608[idx].anglepad != 0) {
                red = 70;
                green = 255;
                blue = 255;
            } else {
                red = 70;
                green = 70;
                blue = 255;
            }
            break;
        case 5:
            red = 255;
            green = 255;
            blue = 0;
            break;
        }

        camera = (POLY_G4 *)current->primptr;
        ((P_TAG *)camera)->len = 8;
        camera->code = 0x38;
        camera->r1 = red >> 1;
        camera->g1 = green >> 1;
        camera->b1 = blue >> 1;
        camera->r0 = red;
        camera->g0 = green;
        camera->b0 = blue;
        camera->r2 = red >> 2;
        camera->g2 = green >> 2;
        camera->b2 = blue >> 2;
        camera->r3 = red >> 2;
        camera->g3 = green >> 2;
        camera->b3 = blue >> 2;

        idx = D_00097608[idx].next;

        if (min_x > 463)
            return;
        if (max_x > 463)
            max_x = 463;
        if (max_x > 48) {
            if (min_x < 49)
                min_x = 49;
            camera->x0 = min_x;
            camera->y0 = min_y;
            camera->x1 = max_x;
            camera->y1 = min_y;
            camera->x2 = min_x;
            camera->y2 = max_y;
            camera->x3 = max_x;
            camera->y3 = max_y;
            addPrim(current->ot, camera);
            current->primptr += sizeof(POLY_G4);
            first = 0;
        }
    }
}
