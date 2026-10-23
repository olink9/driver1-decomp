typedef struct { int x, y, z, w; } V;
typedef struct { int x, y, z; char pad[1]; unsigned char t; } O;
extern int camera_position[];
extern int LightSortCorrect;
extern int D_000B9170[];
extern int D_000B9230[];
extern int MapHeight();
extern void ShowLight();
extern void DisplayLightReflections();

void func_00042EBC(O *o, int a1, int a2, int a3)
{
    V v1, v2, v3;
    char c1[4];
    char c2[4];
    int h;

    switch (o->t >> 4) {
    case 0:
        v1.x = o->x - camera_position[0] + a1;
        v1.z = o->z - camera_position[2] + a3;
        break;
    case 1:
        v1.x = o->x - camera_position[0] + a3;
        v1.z = o->z - camera_position[2] - a1;
        break;
    case 2:
        v1.x = o->x - camera_position[0] - a1;
        v1.z = o->z - camera_position[2] - a3;
        break;
    case 3:
        v1.x = o->x - camera_position[0] - a3;
        v1.z = o->z - camera_position[2] + a1;
        break;
    }
    c1[0] = 25; c1[1] = 25; c1[2] = 25;
    v1.y = o->y - camera_position[1] + a2;
    v2.x = v1.x;
    v2.z = v1.z;
    h = MapHeight(&v1);
    c1[3] = 0;
    LightSortCorrect = -10;
    v2.y = -h - camera_position[1];
    v3 = v1;
    ShowLight(&v3, c1, 400, D_000B9170);
    c2[0] = 140; c2[1] = 140; c2[2] = 140;
    DisplayLightReflections(&v2, c2, 80, D_000B9230);
}
