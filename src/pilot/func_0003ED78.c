/* LoseHubcap (Driver 2 name) - player car only in Driver 1 */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;

typedef struct {
    int Present[4];
    VECTOR Offset[4];
    MATRIX Orientation;
    MATRIX LocalOrientation;
    VECTOR Position;
    VECTOR Direction;
    int Rotation;
    int Duration;
} HUBCAP;

typedef struct {
    char pad0[0x40];
    long linearVelocity[3];
    long angularVelocity[3];
    char pad1[0xC8 - 0x58];
    long t[3];
    char pad2[0x104 - 0xD4];
    int wheel_speed;
} CAR;

extern CAR D_0009B0A4;
extern MATRIX D_000AF6A0;
extern HUBCAP D_000B700C;
extern void func_00079D20();
extern void func_0004EAC0();
extern void func_0004E948();

void func_0003ED78(int Hubcap)
{
    char pad[0x30];
    SVECTOR InitialLocalAngle = { 0, 0, 10 };
    SVECTOR unused = { 0, 3072, 0 };
    VECTOR R;
    VECTOR VW;
    long AY;

    if (D_0009B0A4.wheel_speed < 0 || !D_000B700C.Present[Hubcap])
        return;

    D_000B700C.Present[Hubcap] = 0;
    D_000B700C.Position.vx = D_000B700C.Offset[Hubcap].vx;
    D_000B700C.Position.vy = D_000B700C.Offset[Hubcap].vy;
    D_000B700C.Position.vz = D_000B700C.Offset[Hubcap].vz;

    func_00079D20(&D_000AF6A0);
    func_0004EAC0(&D_000B700C.Position);

    D_000B700C.Position.vx += D_0009B0A4.t[0];
    D_000B700C.Position.vy -= D_0009B0A4.t[1];
    D_000B700C.Position.vz += D_0009B0A4.t[2];

    D_000B700C.Orientation = D_000AF6A0;
    func_0004E948(&D_000B700C.LocalOrientation, &InitialLocalAngle);

    if (Hubcap == 2 || Hubcap == 3) {
        D_000B700C.Orientation.m[0][0] = -D_000B700C.Orientation.m[0][0];
        D_000B700C.Orientation.m[1][0] = -D_000B700C.Orientation.m[1][0];
        D_000B700C.Orientation.m[2][0] = -D_000B700C.Orientation.m[2][0];
        D_000B700C.Orientation.m[0][2] = -D_000B700C.Orientation.m[0][2];
        D_000B700C.Orientation.m[1][2] = -D_000B700C.Orientation.m[1][2];
        D_000B700C.Orientation.m[2][2] = -D_000B700C.Orientation.m[2][2];
    }

    R = D_000B700C.Offset[Hubcap];
    AY = D_0009B0A4.angularVelocity[1] >> 12;
    VW.vx = (AY * R.vz) / 4096;
    VW.vy = 0;
    VW.vz = (-AY * R.vx) / 4096;
    VW.vx += D_0009B0A4.linearVelocity[0] >> 12;
    VW.vy = D_0009B0A4.linearVelocity[1] >> 12;
    VW.vz += D_0009B0A4.linearVelocity[2] >> 12;
    D_000B700C.Direction = VW;
    D_000B700C.Duration = 100;
}
