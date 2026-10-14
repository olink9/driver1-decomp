extern void func_0007EA74();
typedef struct { int x, y, z; } V;
void func_0006EA0C(V *a, int n, int *src, int val)
{
    int i;
    for (i = 4; n < i; i--) {
        func_0007EA74(&a[i], &a[i - 1]);
        a[i].z = a[i - 1].z;
    }
    func_0007EA74(&a[n], src);
    a[n].z = val;
}
