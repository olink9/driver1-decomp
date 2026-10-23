typedef struct { short a, b; } CNT;
typedef struct { short x, y; } XY;
extern CNT D_001D3298[5];
extern XY *D_001CF320;
extern unsigned int *D_001CF324;

int func_001CBEFC(int n)
{
    int i;

    D_001D3298[0].a = D_001D3298[1].a = D_001D3298[2].a = D_001D3298[3].a = D_001D3298[4].a = 0;
    D_001D3298[0].b = D_001D3298[1].b = D_001D3298[2].b = D_001D3298[3].b = D_001D3298[4].b = 0;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].x < 0)
            D_001D3298[0].a++;
        if (D_001CF320[i].x < -254)
            D_001D3298[0].b++;
    }
    if (D_001D3298[0].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].y < 0)
            D_001D3298[1].a++;
        if (D_001CF320[i].y < -254)
            D_001D3298[1].b++;
    }
    if (D_001D3298[1].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].x > 0x200)
            D_001D3298[2].a++;
        if (D_001CF320[i].x > 0x2FE)
            D_001D3298[2].b++;
    }
    if (D_001D3298[2].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF320[i].y > 0x100)
            D_001D3298[3].a++;
        if (D_001CF320[i].y > 0x1FE)
            D_001D3298[3].b++;
    }
    if (D_001D3298[3].a == n)
        return 1;
    for (i = 0; i < n; i++) {
        if (D_001CF324[i] < 10)
            D_001D3298[4].a++;
        if (D_001CF324[i] < 200)
            D_001D3298[4].b++;
    }
    return D_001D3298[4].a == n;
}
