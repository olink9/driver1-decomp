/* WIP 86.15: loop shape ok; pointer-init position (addiu a3,t2,8 after the hoisted luis) and the order of the D_001D3284 store vs 3288/3290 differ */
typedef struct { unsigned char pad0, a; unsigned short b; unsigned char idx[4]; unsigned char p[8]; } T;
typedef struct { unsigned short x, y, z, w; } SRC;
typedef struct { unsigned short x, y, z; unsigned char u, v; } DST;
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern SRC *D_001D328C;
extern DST *D_001CF318;
extern int D_001CF308, D_001D3280, D_001D3284, D_001D3288[], D_001D3290[];
extern void func_001CBAA4();

void func_001CBD08(T *t)
{
    unsigned char *ip = t->idx;
    int i = 0;
    unsigned char *pp = t->p;
    for (; i < 3; i++) {
        D_001CF318[i].x = D_001D328C[*ip].x;
        D_001CF318[i].y = D_001D328C[*ip].y;
        D_001CF318[i].z = D_001D328C[*ip].z;
        D_001CF318[i].u = pp[0];
        D_001CF318[i].v = pp[1];
        ip++;
        pp += 2;
    }
    D_001D3284 = texture_cluts[t->a][t->b] << 16;
    D_001D3288[0] = 3;
    D_001D3290[0] = 0;
    D_001D3280 = texture_pages[t->a] << 16;
    func_001CBAA4(D_001CF308);
}
