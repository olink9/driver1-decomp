/* Driver 2: damage_lamp */
struct L { int index; unsigned char damage; char pad[3]; };
extern int D_00097478;
extern struct L D_000B8190[];

int func_000431CC(int idx)
{
    int i;
    int od;
    for (i = 0; i < 4; i++) {
        if (idx == D_000B8190[i].index) {
            od = D_000B8190[i].damage;
            D_000B8190[i].damage = od + 1;
            idx = 0;
            if (od < 3)
                idx = ((unsigned char)(od + 1) < 3) ^ 1;
            return idx;
        }
    }
    D_000B8190[D_00097478].damage = 0;
    D_000B8190[D_00097478++].index = idx;
    if (D_00097478 > 4) D_00097478 = 0;
    return 0;
}
