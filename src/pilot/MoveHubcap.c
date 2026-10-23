/* MoveHubcap: MoveHubcap (Driver 2 name). 99.80: only diff is the %hi reloc addend annotation on a duplicated delay-slot lui (cc1 emits %hi(D_000B700C+80) for both); bytes believed identical. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;

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
    char pad0[0x4C];
    long angularVelocity[3];
} CAR_b062h;

extern CAR_b062h car_data[];
extern HUBCAP D_000B700C;
extern long long pauseflag;     /* pauseflag */
extern int D_000973DC;           /* gHubcapTime */
extern VECTOR camera_position;        /* camera_position */
extern VECTOR D_000B6E84;        /* ShadowPos */
extern MATRIX inv_camera_matrix;        /* inv_camera_matrix */
extern long long D_000970CC;     /* gHubcapModelPtr */
extern int func_0004FFE8();
extern void LoseHubcap();
extern void func_0004EA40();
extern int MapHeight();
extern void MulMatrix0();
extern void SetRotMatrix();
extern void func_0004EAC0();
extern void func_0002E230();
extern void ShowGroundLight();

void MoveHubcap(void)
{
    VECTOR Position;
    MATRIX Orientation;
    CVECTOR col = { 72, 72, 72 };
    char pad[0x54];
    CAR_b062h *car = &car_data[0];
    int VelocityMagnitude;
    int CurrentMapHeight;

    if (*(int *)&pauseflag == 0 && D_000973DC > 0)
        D_000973DC--;

    if (D_000B700C.Duration <= 0 && D_000973DC == 0) {
        VelocityMagnitude = car->angularVelocity[0] + car->angularVelocity[1] + car->angularVelocity[2];
        if (VelocityMagnitude < -1000000) {
            LoseHubcap((func_0004FFE8(2) & 1) | 2, VelocityMagnitude);
            D_000973DC = func_0004FFE8(3) & 0x417;
        } else if (VelocityMagnitude > 1000000) {
            LoseHubcap(func_0004FFE8(4) & 1, VelocityMagnitude);
            D_000973DC = func_0004FFE8(5) & 0x417;
        }
        return;
    }

    if (*(int *)&pauseflag == 0)
        D_000B700C.Duration--;

    if (D_000B700C.Duration > 0) {
        if (*(int *)&pauseflag == 0) {
            D_000B700C.Position.vx += D_000B700C.Direction.vx;
            D_000B700C.Position.vy += D_000B700C.Direction.vy;
            D_000B700C.Position.vz += D_000B700C.Direction.vz;
            func_0004EA40(&D_000B700C.LocalOrientation, -220);
            D_000B700C.Direction.vy += 5;
            CurrentMapHeight = MapHeight(&D_000B700C.Position);
            if (D_000B700C.Position.vy >= -CurrentMapHeight - 60) {
                D_000B700C.Position.vy = -CurrentMapHeight - 60;
                D_000B700C.Direction.vy = -(D_000B700C.Direction.vy / 2);
            }
        }
        MulMatrix0(&D_000B700C.Orientation, &D_000B700C.LocalOrientation, &Orientation);
        D_000B6E84.vx = Position.vx = D_000B700C.Position.vx - camera_position.vx;
        Position.vy = D_000B700C.Position.vy - camera_position.vy;
        D_000B6E84.vz = Position.vz = D_000B700C.Position.vz - camera_position.vz;
        SetRotMatrix(&inv_camera_matrix);
        D_000B6E84.vy = -MapHeight(&D_000B700C.Position);
        func_0004EAC0(&Position);
        func_0002E230(*(int *)&D_000970CC, &Orientation, &Position, 0);
        D_000B6E84.vy -= camera_position.vy;
        ShowGroundLight(&D_000B6E84, &col, 0x41);
    }
}
