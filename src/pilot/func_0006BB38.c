int abs(int);

typedef struct {
    char pad[0x50];
    int x;
} S;

int func_0006BB38(S *p)
{
    return abs(p->x) > 1024000;
}
