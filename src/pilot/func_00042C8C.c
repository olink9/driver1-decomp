typedef struct { int x, y, z, w; } V;
typedef struct { int x, y, z; } O;
typedef struct { O *obj; unsigned char n; char pad[3]; } LS;
extern int camera_position[];
extern int LightSortCorrect;
extern int D_00097464;
extern LS D_000B8190[];
extern int D_000B9170[];
extern int D_000B9230[];
extern int func_000776E4();
extern int MapHeight();
extern int find_lamp_streak();
extern void ShowLight();
extern void ShowLight1();
extern void DisplayLightReflections();

void func_00042C8C(O *o, int a1, int a2, int a3, int big)
{
    V v1, v2, v3;
    unsigned char c1[4] = { 140, 140, 140, 0 };
    unsigned char c2[4] = { 35, 35, 35, 0 };
    int size;
    int i;
    int h;
    unsigned char c;
    int id;
    LS *p;

    p = D_000B8190;
    id = (int)o + a1;
    v1.x = o->x - camera_position[0] + a1;
    v1.z = o->z - camera_position[2] + a3;
    size = 100;
    if (big == 0)
        size = 50;
    for (i = 0; i < 4; i++, p++) {
        if (o == p->obj) {
            if (p->n >= 3)
                return;
            c = (func_000776E4() & 0x3F) - (p->n * 32 - 90);
            c1[0] = c1[1] = c1[2] = c;
            c2[0] = c2[1] = c2[2] = c >> 1;
            break;
        }
    }
    v1.y = o->y - camera_position[1] + a2;
    v2.x = v1.x;
    v2.z = v1.z;
    h = MapHeight(&v1);
    c1[3] = 0;
    LightSortCorrect = -30;
    v2.y = -h - camera_position[1];
    v3 = v1;
    D_00097464 = find_lamp_streak(id);
    if (D_00097464 >= 0)
        c1[3] = 0x60;
    ShowLight(&v3, c1, size, D_000B9170);
    ShowLight1(&v3, c2, size * 3, D_000B9170);
    DisplayLightReflections(&v2, c2, (short)size * 2, D_000B9230);
    LightSortCorrect = -10;
}
