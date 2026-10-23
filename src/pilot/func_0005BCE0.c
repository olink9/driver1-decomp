typedef struct { char p0; unsigned char f1; char pad[0x4e]; } S50;

extern S50 D_00098900[];
extern long long cameraview;
extern long long D_00096898;
extern long long D_0009688C;
extern int D_0008A1E0[];
extern char car_data[];
extern char D_0009D438[];
extern int camera_position[];
extern int Long2DDistance();
extern int func_0005BDF8();

int func_0005BCE0(int a)
{
    S50 *p = &D_00098900[a];
    char *base;

    if (*(unsigned char *)&cameraview == 0) return 0;
    if (*(unsigned char *)&cameraview == 2) {
        if (a == 0) return 0;
        if (p->f1 == 0) {
            *(int *)&D_0009688C = 0;
            D_0008A1E0[0] = -300;
            return 1;
        }
    }
    if (*(unsigned char *)&D_00096898 != 3)
        base = car_data;
    else
        base = D_0009D438;
    if (Long2DDistance(base + 0xC8, camera_position) >= 0x55F1) {
        *(unsigned char *)&cameraview = 0;
        D_0008A1E0[0] = -300;
        *(int *)&D_0009688C = 0;
        return 1;
    } else if (func_0005BDF8(camera_position[0], camera_position[2]) == 0) {
        *(unsigned char *)&cameraview = 2;
        *(int *)&D_0009688C = 0;
        return 1;
    }
    return 0;
}
