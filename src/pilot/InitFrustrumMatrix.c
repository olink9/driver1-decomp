/* InitFrustrumMatrix (Driver 2 name) */
typedef struct { short m[3][3]; int t[3]; } MAT;
extern short rcossin_tbl[];      /* rcossin_tbl */
typedef struct { short vx, vy, vz, pad; } SVEC;
extern SVEC D_00096C48;     /* camera_angle */
extern int D_000971B4;          /* FrAng */
extern MAT D_000A5E1C;          /* frustrum_matrix */
extern int rcos(int);
#define VY (D_00096C48.vy)
static inline int sub(int a, int b) { return a - b; }

void InitFrustrumMatrix(void)
{
    int a;

    a = -VY;
    D_000A5E1C.m[0][0] = rcossin_tbl[(a & 0xfff) * 2];
    D_000A5E1C.m[0][1] = 0;
    D_000A5E1C.m[0][2] = rcos(a);

    a = sub(D_000971B4 - 1024, VY);
    D_000A5E1C.m[1][0] = rcossin_tbl[(a & 0xfff) * 2];
    D_000A5E1C.m[1][1] = 0;
    D_000A5E1C.m[1][2] = rcos(a);

    a = sub(-VY, D_000971B4 - 1024);
    D_000A5E1C.m[2][0] = rcossin_tbl[(a & 0xfff) * 2];
    D_000A5E1C.m[2][1] = 0;
    D_000A5E1C.m[2][2] = rcos(a);

    D_000A5E1C.t[0] = -100;
}
