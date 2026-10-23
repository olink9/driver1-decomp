typedef struct { char name[16]; short s10; short pad; int x, y, w, h; } ENT;
typedef struct { int used; unsigned char slot; char pad; short tpage; int count; ENT e[10]; } TEX;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
    unsigned char idx, slot;
} OUT;
extern TEX D_001CFDF0[];
extern char D_001C0230[];
extern char D_001C0238[];
extern int sprintf();
extern int printf();
extern int func_0007EC88(); /* strlen */
extern int func_0007B218(); /* strncmp */

/* find a texture by name in the loaded sheets */
void func_001C334C(char *name, OUT *out)
{
    char buf[16];
    int k;
    int i;
    int n;

    for (k = 0; k < 32; k++) {
        n = D_001CFDF0[k].count;
        for (i = 0; i < n; i++) {
            sprintf(buf, D_001C0230, name);
            if (func_0007B218(buf, D_001CFDF0[k].e[i].name, func_0007EC88(buf)) == 0) {
                out->tpage = D_001CFDF0[k].tpage;
                out->slot = D_001CFDF0[k].slot;
                out->idx = i;
                out->u0 = D_001CFDF0[k].e[i].x * 4;
                out->v0 = D_001CFDF0[k].e[i].y;
                out->u1 = (D_001CFDF0[k].e[i].x + D_001CFDF0[k].e[i].w) * 4 - 1;
                out->v1 = D_001CFDF0[k].e[i].y;
                out->u2 = D_001CFDF0[k].e[i].x * 4;
                out->v2 = D_001CFDF0[k].e[i].y + D_001CFDF0[k].e[i].h - 1;
                out->u3 = (D_001CFDF0[k].e[i].x + D_001CFDF0[k].e[i].w) * 4 - 1;
                out->v3 = D_001CFDF0[k].e[i].y + D_001CFDF0[k].e[i].h - 1;
                out->clut = D_001CFDF0[k].e[i].s10;
                return;
            }
        }
    }
    printf(D_001C0238, name);
    for (;;)
        __asm__ volatile("break 1024");
}
