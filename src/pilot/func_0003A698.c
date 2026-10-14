/* Driver 1 car draw / per-car update (no Driver 2 counterpart found) */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { int m[8]; } MAT8;
typedef struct { unsigned char c[8]; } B8;

typedef struct {
    int pad0;
    int mat[5];
    char pad18[0x40 - 0x18];
    int f40;
    int pad44;
    int f48;
    char pad4C[0xC8 - 0x4C];
    int x, y, z;
    char padD4[0x108 - 0xD4];
    int f108;
    char pad10C[0x114 - 0x10C];
    int f114;
    int f118;
    char pad11C[0x17C - 0x11C];
    short f17C;
    char pad17E[0x18C - 0x17E];
} CAR;

typedef struct {
    unsigned char f0, f1;
    char pad2[0x17 - 2];
    unsigned char f17;
    char pad18[80 - 0x18];
} PED;

typedef struct { char pad[0x38]; void *f38; char pad3C[60 - 0x3C]; } VIEWS;
typedef struct { char c[50]; } S50;
typedef struct { char c[150]; } S150;
typedef struct { char c[1024]; } S1024;

extern long long D_00096834;
extern long long D_000971A0;
extern B8 D_00096897;
extern long long D_0009688C;
extern long long D_0009687C;
extern long long D_00097004;
extern long long D_000970DC;
extern long long D_000970E8;
extern long long D_00096874;
extern long long D_00096978;
extern long long D_000972F0;
extern long long D_00096E7C;
extern B8 D_00096898;
extern B8 D_00097458;
extern B8 D_00097460;
extern B8 D_000971C0;
extern int D_000973B4;
extern int D_000973B8;
extern int D_00096A48;
extern int D_00096A44;
extern int D_0009D8DC[];
extern int D_0009D8E0[];
typedef struct { int m[5]; char pad[0x18C - 20]; } CARMAT;
extern CARMAT D_0009B0A8[];
extern VECTOR D_0008A8F0;
extern CAR D_0009B0A4[];
extern unsigned short *D_000A0E20[];
extern int D_000A1AE0[];
extern S50 D_000B6DBC[];
extern S150 D_000AF97A[];
extern VIEWS D_000AF5B0[];
extern S1024 D_000B09BC[];
extern MAT8 D_000AF6A0[];
extern MAT8 D_000BBE84;
extern char D_000AB454[];
extern char D_000B08B8[];
extern PED D_00098950[];

extern int func_000217C8();
extern void func_0002ADEC();
extern void func_0002C564();
extern int func_0002C5F0();
extern void func_0002DBB4();
extern void func_0002E230();
extern void func_0003B168();
extern void func_0003B208();
extern void func_0003B994();
extern void func_0003BA7C();
extern void func_00040BAC();
extern void func_000412D4();
extern void func_00043E8C();
extern void func_00046234();
extern void func_000467DC();
extern void func_00046A1C();
extern void func_00046C58();
extern unsigned char func_0004FFE8();
extern void MulMatrix0();

#define I32(x) (*(int *)&(x))

static inline void CopyMatrix(int *s, int *d)
{
    d[0] = ~s[0];
    d[1] = s[1] ^ 0xFFFF;
    d[2] = s[2];
    d[3] = ~s[3];
    d[4] = s[4] ^ 0xFFFF;
}

void func_0003A698(int i, int view)
{
    int a18[2];
    short tmp[4];
    char pad28[0x60];
    VECTOR pos;
    VECTOR v98;
    char padA8[8];
    MAT8 mat;
    int view2;
    int zero;
    int dist;
    int skid;
    int y;
    CAR *cp;

    view &= 3;
    if (I32(D_00096834) == 3)
        I32(D_000971A0) = 0x282828;

    if (D_00096897.c[0] == 2 && i == I32(D_0009688C)) {
        CAR *cars = D_0009B0A4;
        int *src = D_0009B0A4[i].mat;
        int *dst = D_000AF6A0[i].m;

        if (view == 0 && func_000217C8(cars->x, cars->z, tmp)) {
            if ((*D_000A0E20[tmp[0]] & 0x80) && cars->y < 220) {
                if (I32(D_0009687C) == 0) {
                    if (I32(D_00097004) != 0)
                        I32(D_00097004) = I32(D_00097004) - 1;
                    D_0009B0A4[i].f40 -= D_0009B0A4[i].f40 >> 4;
                    D_0009B0A4[i].f48 -= D_0009B0A4[i].f48 >> 4;
                }
            } else {
                I32(D_00097004) = 25;
                I32(D_000970DC) = 100;
            }
        }
        CopyMatrix(src, dst);

        if (D_0009B0A4[i].f114 < 500)
            D_00097458.c[0] = 2;
        else if (D_0009B0A4[i].f114 < 1000)
            D_00097458.c[0] = 1;
        else
            D_00097458.c[0] = 0;

        if (D_0009B0A4[i].f118 < 500)
            D_00097460.c[0] = 2;
        else if (D_0009B0A4[i].f118 < 1000)
            D_00097460.c[0] = 1;
        else
            D_00097460.c[0] = 0;

        if (I32(D_000970E8) != 0)
            func_00040BAC(i, a18, 1);
        return;
    }

    D_000971C0.c[0] = 0;
    view2 = view;
    cp = &D_0009B0A4[i];
    zero = 0;
    if (i == 0)
        view2 = 0;

    v98.vx = pos.vx = cp->x;
    y = -cp->y;
    pos.vy = y;
    v98.vz = pos.vz = cp->z;
    if (D_0009D8DC[1] != 0) {
        pos.vy = y + 120;
        if (D_0009D8DC[2] != 0)
            pos.vy = y + 180;
    }

    func_0002DBB4();
    dist = func_0002C5F0(&pos, 800);
    pos.vx -= D_0008A8F0.vx;
    pos.vy -= D_0008A8F0.vy;
    pos.vz -= D_0008A8F0.vz;
    func_0002C564(&pos);

    if (dist != -1) {
        D_000973B4++;
        {
            short *p;
            int j;
            CopyMatrix(D_0009B0A8[i].m, D_000AF6A0[i].m);
            if (D_0009D8E0[0] != 0) {
                p = (short *)&D_000AF6A0[i];
                for (j = 0; j < 9; j++) {
                    *p = *p >> 2;
                    p++;
                }
            }
        }

        if (i != 0 && i != 23) {
            if ((I32(D_00096834) == 2 ? D_00096A48 * 5 : D_00096A48) < pos.vz || D_000973B8 != 0) {
                int m = D_000A1AE0[view2];
                I32(D_000972F0) = (int)&D_000B6DBC[view2];
                if (m != 0)
                    func_0002E230(m, &D_000AF6A0[i], &pos, 0);
                goto done;
            }
        }

        if (i == 0)
            func_0003B994(cp, i);

        {
            int s = cp->f108 << 13;
            if (cp->f17C >= I32(D_00096E7C)) {
                if ((unsigned)(s + 59999) <= 119998)
                    func_00046C58(i, s);
                if ((unsigned)(s + 399999) <= 1199998)
                    func_00046A1C(i, 1, s);
            } else if (cp->f114 > 3000 || cp->f118 > 3000) {
                if ((unsigned)(s + 399999) <= 1199998)
                    func_00046A1C(i, 1, s);
            } else if (cp->f114 > 2000 || cp->f118 > 2000) {
                if ((unsigned)(s + 399999) <= 1199998)
                    func_00046A1C(i, 0, s);
            }

            if ((unsigned)(s + 59999) <= 119998) {
                if ((i == 0 && I32(D_00096874) == 0) ||
                    (i != 0 && D_00098950[i - 1].f1 != 1 && D_00096A44 != 0)) {
                    skid = 0;
                    if (cp->f114 > 3000 || cp->f118 > 3000)
                        skid = 1;
                    func_000467DC(i, s, skid);
                }
            }
        }

        if (view == 0) {
            if (func_000217C8(cp->x, cp->z, tmp)) {
                if (I32(D_00096834) != 1 && (*D_000A0E20[tmp[0]] & 0x80) && cp->y < 220) {
                    if (I32(D_0009687C) == 0) {
                        if (I32(D_00097004) != 0)
                            I32(D_00097004) = I32(D_00097004) - 1;
                        func_000412D4(i, tmp[1] + 100, I32(D_00097004) == 0);
                        D_0009B0A4[i].f40 -= D_0009B0A4[i].f40 >> 4;
                        D_0009B0A4[i].f48 -= D_0009B0A4[i].f48 >> 4;
                    }
                } else {
                    I32(D_00097004) = 25;
                    I32(D_000970DC) = 100;
                    func_0002ADEC(i, &pos, zero);
                }
            }
            func_0003BA7C(i, D_000AB454);
            I32(D_000972F0) = (int)D_000B08B8;
        } else {
            I32(D_000972F0) = (int)&D_000AF97A[i];
            func_0002ADEC(i, &pos, zero);
        }

        {
            VIEWS *vp = &D_000AF5B0[view2 & 3];
            vp->f38 = &D_000B09BC[i];
            MulMatrix0(&D_000BBE84, &D_000AF6A0[i], &mat);
            if (D_00096898.c[0] == 8 && i == 23 && I32(D_00096874) == 0)
                I32(D_00096978) = 1;
            if (i != 0 || I32(D_00096874) != 1) {
                func_0003B168(vp, &mat, &pos, &v98);
                func_0003B208(i, &mat, &pos, 0, view2);
                I32(D_00096978) = 0;
            }
        }
done:
        func_00043E8C(i);
        if (i != 0) {
            if (D_00098950[i - 1].f1 != 1 && func_0004FFE8(i) == 0) {
                if (I32(D_000970E8) != 0)
                    D_00098950[i - 1].f17 |= 0x40;
                else
                    D_00098950[i - 1].f17 &= ~0x40;
            }
        }
        if ((i == 0 && I32(D_000970E8) != 0) || (i != 0 && (D_00098950[i - 1].f17 & 0x40)))
            func_00046234(i);

        if (D_0009D8E0[0] != 0)
            CopyMatrix(D_0009B0A8[i].m, D_000AF6A0[i].m);
    }
    D_000971C0.c[0] = 1;
}
