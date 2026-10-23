typedef struct { int x, y, z, a, b, d; } A;
typedef struct { char pad[0xC8]; int cx; int pad2; int cy; char pad3[396 - 0xD4]; } B;
extern A D_0008B080[];
extern B car_data[];
extern long long D_00096834;
extern long long D_00096888;
extern int D_00096D24;
extern int D_00096D28;

void SetMapPos(void)
{
    D_00096D24 = D_0008B080[*(int *)&D_00096834].x + car_data[*(int *)&D_00096888].cy / D_0008B080[*(int *)&D_00096834].d;
    D_00096D28 = D_0008B080[*(int *)&D_00096834].y + car_data[*(int *)&D_00096888].cx / D_0008B080[*(int *)&D_00096834].d;
}
