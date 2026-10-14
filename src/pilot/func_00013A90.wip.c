typedef struct {
    char pad0;
    char used;
    char pad2[0x26];
    int id;
    char pad[0x50 - 0x2C];
} ENT;
extern ENT D_00098950[];
extern int D_00099AE0[];

int func_00013A90(int a, int b, int c, int d)
{
    int i;
    D_00099AE0[3] = 0;
    D_00099AE0[2] = 0;
    D_00099AE0[1] = 0;
    D_00099AE0[0] = 0;
    for (i = 0; i < 22; i++) {
        if (D_00098950[i].used) {
            if (D_00098950[i].id == a)
                D_00099AE0[0]++;
            else if (D_00098950[i].id == b)
                D_00099AE0[1]++;
            else if (D_00098950[i].id == c)
                D_00099AE0[2]++;
            else if (D_00098950[i].id == d)
                D_00099AE0[3]++;
        }
    }
    return D_00099AE0[0] + D_00099AE0[1] + D_00099AE0[2] + D_00099AE0[3];
}
