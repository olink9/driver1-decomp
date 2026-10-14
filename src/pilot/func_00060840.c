/* Driver 2: CheckForPlayerCar */
int func_00060840(int *a, int *b)
{
    int x = a[0];
    int z;
    if (x < b[0]) return 0;
    if (x > b[1]) return 0;
    z = a[2];
    if (z < b[2]) return 0;
    return z <= b[3];
}
