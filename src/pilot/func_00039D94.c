typedef struct {
    char pad0[0xC8];
    int xC8;
    int padCC;
    int xD0;
    char padD4[0x18C - 0xD4];
} ENTRY;

typedef struct {
    int vx, vy, vz, pad;
} VEC;

typedef struct {
    int x, z;
} XZ;

extern ENTRY D_0009B0A4[];
extern long long D_00096884;
extern int D_00097368;
extern int D_00097398;
extern int D_0009733C;
extern int D_00096A24;
extern int D_00097300;
extern int D_000972FC;
extern int D_00097314;
extern unsigned char D_00097318;
extern int D_0009730C;
extern unsigned char D_00097310;
extern unsigned char D_00097328;
extern int D_00097324;
extern int D_0009732C;
extern int D_00097320;
extern int D_0009731C;
extern XZ D_00099158[];
extern VEC D_0008A8F0;

extern void func_00034638();
extern void func_00039978();
extern void func_00033F9C();
extern void func_00035394();

void func_00039D94(void)
{
    int cz, cx, rz, rx, t;
    XZ *p;

    cz = (D_0009B0A4[*(int *)&D_00096884].xD0 + D_00097368) / 3000;
    rz = cz / D_0009733C;
    t = D_00096A24 / D_0009733C;
    cx = (D_0009B0A4[*(int *)&D_00096884].xC8 + D_00097398) / 3000;
    rx = cx / D_0009733C;

    D_00097300 = cz;
    D_000972FC = cx;
    D_00097314 = rz;
    D_00097318 = D_00097314 & 1;
    D_0009730C = rx;
    D_00097310 = D_0009730C & 1;
    D_00097328 = D_00097310 + (D_00097318 << 1);
    D_00097324 = D_0009732C;
    D_0009732C = rx + rz * t;
    D_00097320 = cx - rx * D_0009733C;
    D_0009731C = cz - rz * D_0009733C;

    if (D_00097324 == -1)
    {
        func_00034638(D_0009732C, D_00097328);
        p = &D_00099158[D_00097328];
        p->x = D_0009730C * D_0009733C * 3000 - D_00097398;
        p->z = D_00097314 * D_0009733C * 3000 - D_00097368;
    }

    func_00039978();

    if (D_00097324 == D_0009732C || D_00097324 == -1)
        func_00033F9C(D_00097320, D_0009731C);

    D_000972FC = (D_0008A8F0.vx + D_00097398) / 3000;
    D_00097300 = (D_0008A8F0.vz + D_00097368) / 3000;
    func_00035394();
}
