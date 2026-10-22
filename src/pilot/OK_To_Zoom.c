/* OK_To_Zoom */
struct P { char pad[0xC8]; int x; int y; int z; };
struct V3 { int x, y, z; };
extern struct P car_data;
extern long long D_000970E4;
extern int camera_position[];
extern int dist();
extern int CameraCollisionCheck();
#define SCRZ (*(int *)&D_000970E4)

int OK_To_Zoom(void) {
    int old, r;
    struct V3 t;
    old = SCRZ;
    t.x = car_data.x;
    t.y = -car_data.y;
    t.z = car_data.z;
    SCRZ = (dist(camera_position, &t) >> 4) + 0x100;
    if (SCRZ > 0x320) SCRZ = 0x320;
    if (SCRZ < 0x100) SCRZ = 0x100;
    r = CameraCollisionCheck() == 0;
    SCRZ = old;
    return r;
}
