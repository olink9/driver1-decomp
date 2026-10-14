typedef struct { int a, b, c, d; } V4;
typedef struct { char pad[0xC8]; V4 v; } G;
extern G D_0009B0A4;
extern short func_00021558();
int func_00060AD4(int x, int z) {
    V4 v;
    v = D_0009B0A4.v;
    if (x > v.a - 250 && x < v.a + 250 && z > v.c - 250 && z < v.c + 250) return 0;
    if (func_00021558(x, z) != -0x8000) return 0;
    if (func_00021558(x - 300, z - 300) > 0) return 1;
    if (func_00021558(x + 300, z - 300) > 0) return 1;
    if (func_00021558(x - 300, z + 300) > 0 || func_00021558(x - 300, z + 300) > 0) return 1;
    return 0;
}
