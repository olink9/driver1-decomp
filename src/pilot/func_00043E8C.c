typedef struct { char p0[0xC8]; int x; char p1[4]; int z; char p2[0x14]; short e8; char p3[0xE]; int rot; char p4[0x90]; } S;
extern S car_data[];
typedef struct { int vx, vy, vz, pad; } V;
extern V camera_position;
extern long long D_00096C48;
extern int D_00097404, D_00097408;
extern int D_00097434, D_00097438, D_00097440, D_00097468;
extern int ratan2();

void func_00043E8C(int i)
{
    V *cam;
    int ang, a, b, r, c, cx;
    int tilt, t;


    r = car_data[i].rot;
    cam = &camera_position;
    cx = car_data[i].x;
    tilt = car_data[i].e8;
    a = r & 0xfff;
    b = (r + 0x800) & 0xfff;
    ang = ratan2(cx - camera_position.vx, car_data[i].z - cam->vz);
    D_00097434 = (((*(short *)&D_00096C48 - tilt + 0x800) & 0xfff) - 0x800);
    if (D_00097434 < 0) D_00097404 = -D_00097434; else D_00097404 = D_00097434;
    t = (((ang & 0xfff) - a + 0x800) & 0xfff) - 0x800;
    D_00097438 = t;
    if (t < 0) D_00097408 = -t; else D_00097408 = t;
    c = (((ang & 0xfff) - b + 0x800) & 0xfff) - 0x800;
    D_00097438 = c;
    D_00097440 = 0x384 - (D_00097408 + D_00097404);
    if (c < 0) D_00097408 = -c; else D_00097408 = c;
    D_00097468 = 0x384 - (D_00097408 + D_00097404);
}
