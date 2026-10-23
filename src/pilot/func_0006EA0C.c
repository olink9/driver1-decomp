extern void strcpy();
typedef struct { int x, y, z; } V;
void func_0006EA0C(V *a, int n, int *src, int val)
{
    int i;
    for (i = 4; n < i; i--) {
        strcpy(&a[i], &a[i - 1]);
        a[i].z = a[i - 1].z;
    }
    strcpy(&a[n], src);
    a[n].z = val;
}
