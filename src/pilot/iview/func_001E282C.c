typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct {
    VECTOR a;
    VECTOR b;
    char pad[0x14];
    unsigned char r, g, b2;
    char pad2;
} PATHSEG;

extern int CameraCnt;
extern int pauseflag;
extern int D_000AF980[];
extern int D_001E3750;
extern int D_001E35B0;
extern PATHSEG D_001E3758[];
extern int car_data[];
extern void SetRotMatrix();
extern void func_0004EAC0();

void func_001E282C(void)
{
    VECTOR v;

    if (CameraCnt & 3)
        return;
    if (pauseflag)
        return;
    SetRotMatrix(D_000AF980);
    D_001E3758[D_001E3750].r = 0;
    D_001E3758[D_001E3750].g = 0;
    D_001E3758[D_001E3750].b2 = 0;
    if (D_001E3750 == 0) {
        if (D_001E35B0) {
            D_001E3758[39].r = 80;
            D_001E3758[39].g = 80;
            D_001E3758[39].b2 = 80;
        }
    } else {
        D_001E3758[D_001E3750 - 1].r = 80;
        D_001E3758[D_001E3750 - 1].g = 80;
        D_001E3758[D_001E3750 - 1].b2 = 80;
    }
    v.vx = 200;
    v.vy = 140;
    v.vz = -200;
    func_0004EAC0(&v);
    v.vx += car_data[0x245C / 4];
    v.vy -= car_data[0x2460 / 4];
    v.vz += car_data[0x2464 / 4];
    D_001E3758[D_001E3750].a = v;
    v.vx = -200;
    v.vy = 140;
    v.vz = -200;
    func_0004EAC0(&v);
    v.vx += car_data[0x245C / 4];
    v.vy -= car_data[0x2460 / 4];
    v.vz += car_data[0x2464 / 4];
    D_001E3758[D_001E3750].b = v;
    if (++D_001E3750 >= 40) {
        D_001E3750 = 0;
        D_001E35B0 = 1;
    }
}
