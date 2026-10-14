/* Driver 2: damage_lamp */
struct L { int index; unsigned char damage; char pad[3]; };
extern int D_00097478;
extern struct L D_000B8190[];

int func_000431CC(int a0)
{
    int od;
    struct L *p;
    int count;
    int idx;
    idx = a0;
    count = 0;
    p = D_000B8190;
    do {
        if (idx == p->index) {
            od = p->damage;
            p->damage++;
            idx = 0;
            if (od < 3)
                idx = ((unsigned char)(od + 1) < 3) ^ 1;
            return idx;
        }
        p++;
        count++;
    } while (count < 4);
    D_000B8190[D_00097478].damage = 0;
    D_000B8190[D_00097478++].index = idx;
    if (D_00097478 > 4) D_00097478 = 0;
    return 0;
}
