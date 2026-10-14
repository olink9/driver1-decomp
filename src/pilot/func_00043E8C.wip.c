typedef struct { char p0[0xC8]; int x; char p1[4]; int z; char p2[0x14]; short e8; char p3[0xE]; int rot; char p4[0x90]; } S;
extern S D_0009B0A4[];
typedef struct { int vx, vy, vz, pad; } V;
extern V D_0008A8F0;
extern long long D_00096C48;
extern int D_00097404, D_00097408, D_00097434, D_00097438, D_00097440, D_00097468;
extern int ratan2();

void func_00043E8C(int i)
{
    V *cam = &D_0008A8F0;
    int ang, a, b, r, c, cx, cz;
    int tilt;


    r = D_0009B0A4[i].rot;
    cx = D_0009B0A4[i].x;
    tilt = D_0009B0A4[i].e8;
    a = r & 0xfff;
    b = (r + 0x800) & 0xfff;
    ang = ratan2(cx - D_0008A8F0.vx, D_0009B0A4[i].z - cam->vz);
    D_00097434 = (((*(short *)&D_00096C48 - tilt + 0x800) & 0xfff) - 0x800);
    if (D_00097434 < 0) D_00097404 = -D_00097434; else D_00097404 = D_00097434;
    D_00097438 = (((ang & 0xfff) - a + 0x800) & 0xfff) - 0x800;
    D_00097408 = abs(D_00097438);
    c = (((ang & 0xfff) - b + 0x800) & 0xfff) - 0x800;
    D_00097438 = c;
    D_00097440 = 0x384 - (D_00097408 + D_00097404);
    if (c < 0) D_00097408 = -c; else D_00097408 = c;
    D_00097468 = 0x384 - (D_00097408 + D_00097404);
}
