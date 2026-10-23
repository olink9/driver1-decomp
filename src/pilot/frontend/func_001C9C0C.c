typedef struct { int x, y, z; } V3s;
extern V3s camera_position, D_001D31C0;
extern int D_001CEF88, D_001D3200, D_001D3204, D_001D3208, D_001D3210, D_001D3214, D_001D3218, D_001D321C;
extern char D_001D31D0[], D_001D31F0[];
extern void func_001C9CF0();
extern void func_001C9E58();

void func_001C9C0C(void)
{
    if (D_001CEF88 == 0) {
        camera_position.x = D_001D31C0.x;
        camera_position.y = D_001D31C0.y;
        camera_position.z = D_001D31C0.z;
        func_001C9E58(D_001D31D0, D_001D31F0);
    } else if (D_001D3208 >= D_001D321C) {
        int t;
        D_001D321C = 0;
        t = D_001D3204;
        D_001D3210 = 0;
        D_001D3214 = 0;
        D_001D3218 = 0;
        D_001D3204 = -1;
        D_001CEF88 = 0;
        D_001D3208 = 0;
        D_001D3200 = t;
    } else {
        func_001C9CF0();
        D_001D3208++;
    }
}
