/* func_00042290 - no Driver 2 counterpart: initialise effect tables */
typedef struct { int x, y, z; } V3;
typedef struct { V3 v[3]; } TRI;

typedef struct { char pad0[0x16]; unsigned char f16; char pad17[0x18 - 0x17]; } S24;
typedef struct { char pad0[0x1C]; short f1C, f1E; char pad20[0x28 - 0x20]; } S40;
typedef struct { char pad0[0x48]; int f48; short f4C, f4E; } S80;
typedef struct { char pad0[0x1C]; short f1C, f1E; char pad20[0x2C - 0x20]; } S44;
typedef struct { int f0; int f4; } S8;
typedef struct { int f0; char pad[0x14]; } S24b;

extern long long D_00096CD0;
extern long long D_00096804;
extern int D_00097448;
extern int D_00097414;
extern int D_0009742C;
extern int D_00097430;
extern int D_00097444;
extern int D_00097478;
extern int D_00097480;
extern int D_00097488;
extern int D_0009745C;
extern short D_0009744C;
extern long long D_0009744E;
extern long long D_00097450;
extern short D_0009746C;
extern long long D_0009746E;
extern long long D_00097470;
extern S24 D_000B738C[];
extern short D_000B9048[];
extern S40 D_000BABC0[];
extern char D_000B9008[];
extern char D_000BBA80[];
extern TRI *D_0008A864[];
extern unsigned char *D_0008A75C[];
extern short D_000BB530[];
extern S80 D_000B92A0[];
extern short D_000B7EC8[];
extern S44 D_000B7584[];
extern short D_000B7E34[];
extern S8 D_000B8190[];
extern S24b D_000B7F3C[];
extern int func_000776E4();
extern void func_0004EA98();
extern void func_0004EAC0();
extern void func_00079D20();

void func_00042290(void)
{
    char pad[8];
    TRI t;
    unsigned char *p;
    int i;
    int j;

    *(int *)&D_00096CD0 = 0;
    for (i = 0; i < 20; i++)
        D_000B738C[i].f16 = 0;
    D_00097448 = 0;
    if (*(int *)&D_00096804 == 1)
        return;
    D_00097488 = func_000776E4() % 7;
    for (D_00097414 = 0; D_00097414 < 60; D_00097414++) {
        D_000B9048[D_00097414] = D_00097414;
        D_000BABC0[D_00097414].f1E = 0;
        D_000BABC0[D_00097414].f1C = D_00097414;
    }
    *(short *)&D_00097450 = 0;
    *(short *)&D_0009744E = 0;
    D_0009744C = 0;
    *(short *)&D_00097470 = 0;
    *(short *)&D_0009746E = 0;
    D_0009746C = 0;
    for (D_00097414 = 0; D_00097414 < 32; D_00097414++) {
        func_0004EA98(D_000B9008, &D_0009744C);
        func_0004EA98(D_000BBA80, &D_0009746C);
        for (i = 0; i < 5; i++) {
            if (i == 1)
                func_00079D20(D_000BBA80);
            else
                func_00079D20(D_000B9008);
            t = *D_0008A864[i];
            func_0004EAC0(&t.v[0]);
            func_0004EAC0(&t.v[1]);
            func_0004EAC0(&t.v[2]);
            p = D_0008A75C[i] + D_00097414 * 12;
            p[0] = t.v[0].x;
            p[1] = t.v[0].y;
            p[2] = t.v[0].z;
            p += 4;
            p[0] = t.v[1].x;
            p[1] = t.v[1].y;
            p[2] = t.v[1].z;
            p += 4;
            p[0] = t.v[2].x;
            p[1] = t.v[2].y;
            p[2] = t.v[2].z;
        }
        D_0009744C += 0x80;
        *(short *)&D_0009744E += 0x100;
        *(short *)&D_00097450 += 0x80;
        *(short *)&D_0009746E += 0x80;
    }
    D_00097414 = 0;
    for (D_0009742C = 0; D_0009742C < 80; D_0009742C++) {
        D_000BB530[D_0009742C] = D_0009742C;
        D_000B92A0[D_0009742C].f4E = 0;
        D_000B92A0[D_0009742C].f4C = D_0009742C;
        D_000B92A0[D_0009742C].f48 = 0;
    }
    D_0009742C = 0;
    D_00097444 = 0;
    for (D_00097430 = 0; D_00097430 < 50; D_00097430++) {
        D_000B7EC8[D_00097430] = D_00097430;
        D_000B7584[D_00097430].f1E = 0;
        D_000B7584[D_00097430].f1C = D_00097430;
    }
    D_00097430 = 0;
    D_000B7E34[0] = 0;
    D_00097478 = 0;
    for (j = 0; j < 4; j++)
        D_000B8190[j].f0 = 0;
    for (j = 0; j < 8; j++)
        D_000B7F3C[j].f0 = 0;
    D_00097480 = -10;
    D_0009745C = 0;
}
