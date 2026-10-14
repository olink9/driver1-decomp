typedef struct { char pad[0x28]; int id; char pad2[0x1C]; int x; int z; } S;
extern S D_00098950[];
extern unsigned short func_00021820();

int func_00019E88(char *p, int x, int z, int id)
{
    int pos[6];
    int i;
    S *e = D_00098950;
    int ix, iz;

    if (func_00021820(x, z, *(unsigned char *)(p + 0x27), pos) != 0) return 0;
    if (*(short *)((char *)pos + 0xE) != id) return 0;
    if (pos[0] < 30000) return 0;
    ix = x / 1500;
    iz = z / 1500;
    for (i = 0; i < 22; i++, e++) {
        if (e->id == id) {
            if (*(unsigned char *)(p + 0x27) & 1) {
                if ((ix - e->x < 0 ? e->x - ix : ix - e->x) >= 3) continue;
                return 0;
            } else {
                if ((iz - e->z < 0 ? e->z - iz : iz - e->z) < 3) return 0;
            }
        }
    }
    return 1;
}
