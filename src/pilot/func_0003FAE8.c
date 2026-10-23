typedef struct { char pad0[0x114]; int dmg[6]; char pad1[0x12E - 0x12C]; unsigned char mode; char pad2[0x17C - 0x12F]; short total; } CP;
extern CP car_data;
extern long long D_000977B0, D_00096870;
extern char D_000C6F00[];

void func_0003FAE8(CP *cp, signed char zone, int damage, unsigned char flag)
{
    int *dp;

    dp = &cp->dmg[zone];
    if (cp == &car_data) {
        D_000C6F00[0] = 1;
        damage = (damage * *(int *)&D_000977B0 + 0x800) >> 12;
    }
    if (cp->mode == 1) {
        switch (*(int *)&D_00096870) {
        case 1:
            damage = damage * 12 / 16;
            break;
        case 2:
            damage = damage * 7 / 16;
            break;
        }
    }
    if (flag == 0)
        cp->total += damage;
    *dp += damage;
    if (*dp >= 0x1000)
        *dp = 0xFFF;

    switch (zone) {
    case 0: zone = 1; break;
    case 1: zone = 0; break;
    case 3: zone = 4; break;
    case 4: zone = 3; break;
    default: zone = -1; break;
    }
    if (zone != -1) {
        dp = &cp->dmg[zone];
        damage >>= 1;
        if (*dp < damage) {
            if (flag == 0)
                cp->total += damage - *(unsigned short *)dp;
            *dp = damage;
        }
    }
    if (cp->total < 0)
        cp->total = 0x7FFF;
}
