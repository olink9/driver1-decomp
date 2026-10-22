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

extern ENTRY car_data[];
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
extern int current_region;
extern int D_00097320;
extern int D_0009731C;
extern XZ D_00099158[];
extern VEC camera_position;

extern void UnpackRegion();
extern void CheckUnpackNewRegions();
extern void CheckLoadAreaData();
extern void StartSpooling();

void func_00039D94(void)
{
    int cz, cx, rz, rx, t;
    XZ *p;

    cz = (car_data[*(int *)&D_00096884].xD0 + D_00097368) / 3000;
    rz = cz / D_0009733C;
    t = D_00096A24 / D_0009733C;
    cx = (car_data[*(int *)&D_00096884].xC8 + D_00097398) / 3000;
    rx = cx / D_0009733C;

    D_00097300 = cz;
    D_000972FC = cx;
    D_00097314 = rz;
    D_00097318 = D_00097314 & 1;
    D_0009730C = rx;
    D_00097310 = D_0009730C & 1;
    D_00097328 = D_00097310 + (D_00097318 << 1);
    D_00097324 = current_region;
    current_region = rx + rz * t;
    D_00097320 = cx - rx * D_0009733C;
    D_0009731C = cz - rz * D_0009733C;

    if (D_00097324 == -1)
    {
        UnpackRegion(current_region, D_00097328);
        p = &D_00099158[D_00097328];
        p->x = D_0009730C * D_0009733C * 3000 - D_00097398;
        p->z = D_00097314 * D_0009733C * 3000 - D_00097368;
    }

    CheckUnpackNewRegions();

    if (D_00097324 == current_region || D_00097324 == -1)
        CheckLoadAreaData(D_00097320, D_0009731C);

    D_000972FC = (camera_position.vx + D_00097398) / 3000;
    D_00097300 = (camera_position.vz + D_00097368) / 3000;
    StartSpooling();
}
