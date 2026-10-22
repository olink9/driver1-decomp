/* camera placement in car (Driver 2 name unknown) */
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short m[3][3]; int t[3]; } MAT;
typedef struct {
    char a[0x18];
    int px, py, pz;
    char b[0xE8 - 0x24];
    unsigned short ax, ay, az;
    char c[0x18C - 0xEE];
} CAR;

extern CAR car_data[];
extern long long D_00096899;
extern int D_00096A84;
extern int D_00096A74;
extern int D_00096A78;
extern short D_00096A7A;
extern int D_00096A7C;
extern long long D_0009752C;
extern short rcossin_tbl[];
extern VEC camera_position;
typedef struct { short v; short pad[3]; } S8;
extern S8 D_00096C48, D_00096C4A, D_00096C4C;
extern long long D_000970E4, D_00096A48;
extern MAT D_000AF6A0[];
extern MAT D_0008AA38, inv_camera_matrix, D_000BBEA4, D_0008A1EC, D_000AF9C0;
extern struct { int a[4]; int cur; int prev; } D_0009D8DC;
extern void func_00077D6C();
extern void InvertMatrix();
extern void func_0004EA6C(void *m, short a);
extern void MulMatrix0();
extern void RotMatrixY();
extern void SetRotMatrix();
extern void func_0004EAC0();

#define US(x) (*(unsigned short *)&x)
#define S(x) (*(short *)&x)
#define I(x) (*(int *)&x)

void func_0003D810(int player, int inside)
{
    VEC v;
    CAR *cp;

    cp = &car_data[player];
    if (*(unsigned char *)&D_00096899 == 0) {
        player = 0;
        cp = &car_data[0];
    }
    v.vx = 0;
    v.vz = D_00096A84;
    if (inside)
        v.vy = -60;
    else
        v.vy = 10;
    if ((US(D_0009752C) & 3) == 3)
        v.vz = 0;

    camera_position.vx = cp->px + (rcossin_tbl[(cp->ay & 0xFFF) * 2] * v.vz + 0x800 >> 12);
    camera_position.vy = -cp->py + v.vy;
    camera_position.vz = cp->pz + (rcossin_tbl[(cp->ay & 0xFFF) * 2 + 1] * v.vz + 0x800 >> 12);

    if ((US(D_0009752C) & 3) != 3) {
        if (US(D_0009752C) & 1) {
            if (D_00096A7C < 2)
                D_00096A7C++;
            else
                D_00096A74 = 0x4000000;
        } else if (US(D_0009752C) & 2) {
            if (D_00096A7C < 2)
                D_00096A7C++;
            else
                D_00096A74 = 0xFC000000;
        } else {
            D_00096A7C = 0;
            D_00096A74 = 0;
        }
    }

    D_00096A78 += (D_00096A74 - D_00096A78) >> 1;

    if ((US(D_0009752C) & 3) == 3)
        D_00096C4A.v = (-cp->ay + 0x800) & 0xFFF;
    else
        D_00096C4A.v = ((D_00096A78 >> 16) - cp->ay) & 0xFFF;
    D_00096C48.v = cp->ax & 0xFFF;
    D_00096C4C.v = cp->az & 0xFFF;

    func_00077D6C(0x100);
    I(D_000970E4) = 0x100;
    InvertMatrix(&D_000AF6A0[player], &inv_camera_matrix);

    if ((US(D_0009752C) & 3) != 3) {
        func_0004EA6C(&inv_camera_matrix, 0x800);
        func_0004EA6C(&inv_camera_matrix, D_00096A7A);
        v.vz = -250;
    } else {
        v.vz = 250;
    }

    MulMatrix0(&D_0008AA38, &inv_camera_matrix, &inv_camera_matrix);
    if (D_0009D8DC.cur) {
        inv_camera_matrix.m[1][0] = -inv_camera_matrix.m[1][0];
        inv_camera_matrix.m[1][1] = -inv_camera_matrix.m[1][1];
        inv_camera_matrix.m[1][2] = -inv_camera_matrix.m[1][2];
    }
    if (D_0009D8DC.cur != D_0009D8DC.prev) {
        inv_camera_matrix.m[0][0] = -inv_camera_matrix.m[0][0];
        inv_camera_matrix.m[0][1] = -inv_camera_matrix.m[0][1];
        inv_camera_matrix.m[0][2] = -inv_camera_matrix.m[0][2];
    }
    InvertMatrix(&inv_camera_matrix, &D_000BBEA4);

    D_0008A1EC.m[0][0] = 0x1000;
    D_0008A1EC.m[0][2] = 0;
    D_0008A1EC.m[2][0] = 0;
    D_0008A1EC.m[2][2] = 0x1000;
    RotMatrixY(-D_00096C4A.v & 0xFFF, &D_0008A1EC);
    RotMatrixY(0x800, &D_0008A1EC);
    MulMatrix0(&inv_camera_matrix, &D_0008A1EC, &D_000AF9C0);

    v.vx = 0;
    v.vy = -60;
    SetRotMatrix(&D_000BBEA4);
    func_0004EAC0(&v);
    I(D_00096A48) = 10000;
    camera_position.vx += v.vx;
    camera_position.vy += v.vy;
    camera_position.vz += v.vz;
}
