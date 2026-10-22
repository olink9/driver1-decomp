typedef struct {
    char pad[0xE];
    unsigned short n;
    char pad2[4];
    unsigned char *p;
} HDR;
extern int D_00089F1C[];
extern long long gLightsOn;
extern long long D_00097128;

void func_000538E8(HDR *h) {
    unsigned char *p = h->p;
    int i = 0;
    int v;
    if (h->n == 0) return;
    do {
        if (p[0] == 9) {
            if (*(int *)&gLightsOn == 0) v = *(unsigned char *)&D_00097128 - 0x18;
            else v = 0xFF;
            p[8] = v; p[9] = v; p[10] = v;
        }
        if (p[0] == 8) {
            if (*(int *)&gLightsOn == 0) v = *(unsigned char *)&D_00097128;
            else v = 0xFF;
            p[4] = v; p[5] = v; p[6] = v;
        }
        if (p[0] > 0x10) break;
        i++;
        p += D_00089F1C[p[0]];
    } while (i < h->n);
}
