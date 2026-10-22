typedef struct { char pad0; unsigned char flash; char pad[394]; } CAR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;

extern CAR D_0009B1D4[];
extern unsigned char D_0008A8D8[];
extern char D_000B9210[];
extern long long D_0009688C;
extern long long cameraview;
extern long long FastForward;
extern long long gNight;
extern long long pauseflag;
extern long long CameraCnt;
extern int D_00097488;
extern void func_00043FDC();

void func_00046000(int player)
{
    SVECTOR v;
    CVECTOR col;
    unsigned char *flash;
    int side, j, k, sign, size;
    int c;

    sign = 1;
    flash = &D_0009B1D4[player].flash;
    if (*(int *)&D_0009688C == player && *(unsigned char *)&cameraview == 2)
        return;
    if (*(int *)&FastForward != 0)
        return;
    if (*(short *)(&D_0009B1D4[player].flash - 0x125) < 100)
        return;
    k = (D_00097488 + player) & 7;
    for (side = 1; side != -1; side--) {
        for (j = 2; j != -1; j--) {
            v.vx = D_0008A8D8[k] * sign;
            v.vy = -164;
            c = 90;
            if (*(int *)&gNight != 0)
                c = 50;
            if (*(int *)&pauseflag == 0 && (*(int *)&CameraCnt & 1))
                k++;
            v.vz = 15;
            k &= 7;
            size = (*flash < 128 ? *flash : 255 - *flash) >> 1;
            if (side) {
                col.r = 255;
                col.g = c;
                col.b = c;
            } else {
                col.r = c;
                col.g = c;
                col.b = 255;
            }
            if (*(int *)&pauseflag == 0)
                *flash += 16;
            func_00043FDC(&v, player, &col, size, D_000B9210, 255);
            if (*(int *)&pauseflag == 0 && (*(int *)&CameraCnt & 1))
                k++;
            k &= 7;
        }
        sign = -sign;
    }
}
