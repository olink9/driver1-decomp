typedef struct { int a[99]; } CAR;
extern unsigned char D_00096899;
extern unsigned char cameraview;
extern unsigned char D_000968A4, D_000968A5, D_000968A6, D_000968A7;
extern long long D_0009752C;
extern long long D_00097534;
extern int D_0009706C;
extern int D_00096890;
extern int D_0009688C;
extern long long car_data;
extern short rcossin_tbl[];
extern int camera_position[];

void func_00022168(void)
{
    unsigned char *p;
    CAR *car;
    int idx;

    if (D_00096899 != 0 || cameraview == 6 || !(*(unsigned short *)&D_0009752C & 0x100))
        return;
    if (D_0009706C != *(int *)&D_00097534 - 1) {
        for (p = &D_000968A4; p < &D_000968A7; p++) {
            if (*p == cameraview) {
                cameraview = p[1];
                break;
            }
        }
    }
    D_0009706C = *(int *)&D_00097534;
    if (cameraview >= 3) {
        D_000968A4 = 1;
        D_000968A5 = 0;
        D_000968A6 = 2;
        D_000968A7 = 1;
        cameraview = 2;
    }
    if (D_00096899 == 0) {
        D_00096890 = 0;
        D_0009688C = 0;
    }
    if (cameraview == 5 || cameraview == 1) {
        car = (CAR *)&car_data; car += D_00096890;
        idx = (car->a[62] + 0x800) & 0xFFF;
        camera_position[0] = car->a[6] + rcossin_tbl[idx * 2] * 460 / 4096;
        camera_position[2] = car->a[8] + rcossin_tbl[idx * 2 + 1] * 460 / 4096;
    }
}
