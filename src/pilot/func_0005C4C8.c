extern int D_0009762C;
extern char tracking_car;
extern long long cameraview;
extern long long CameraCnt;
extern int *ReplayParameterPtr;
extern int D_00097620;
extern short *D_0009765C;
extern int camera_position[3];
extern int D_0008A8F4[];
extern int car_data[];

void func_0005C4C8(void)
{
    int v[3];
    if (D_0009762C == 90) {
        tracking_car = 1;
        *(char *)&cameraview = 1;
        if (*(int *)&CameraCnt + D_00097620 < *ReplayParameterPtr) {
            camera_position[0] = D_0009765C[0] << 10;
            v[0] = camera_position[0];
            camera_position[2] = D_0009765C[1] << 10;
            v[2] = camera_position[2];
        } else {
            camera_position[0] = car_data[0x917];
            v[0] = camera_position[0];
            camera_position[2] = car_data[0x919];
            v[2] = camera_position[2];
        }
        D_0008A8F4[0] = -1000;
    }
    if (D_0009762C == 0) {
        D_0009762C = D_00097620;
        D_0009765C += 2;
    } else {
        D_0009762C = D_0009762C - 1;
    }
}
