typedef struct {
    char pad0[0xC8];
    int xC8;
    int padCC;
    int xD0;
    char padD4[0x18C - 0xD4];
} ENTRY;

typedef struct {
    unsigned char id;
    char pad1[0x16];
    unsigned char flags;
    char pad18[0x24 - 0x18];
    int x24;
    int x28;
    char pad2c[0x38 - 0x2C];
    int x38;
    int pad3c;
    int x40;
    int x44;
    int x48;
    int x4C;
} OBJ;

extern ENTRY D_0009B230[];
extern OBJ D_00098950[];
extern int func_0004FFE8();
extern void func_00014FC4();

void func_00018C74(OBJ *p)
{
    OBJ *o = D_00098950;
    int pos[2];
    int i, d, tx, tz;

    if (p->flags & 9)
        return;
    if ((func_0004FFE8(p->id) - p->id) & 7)
        return;

    func_00014FC4(p->x28, p->x24, 0, 2, 0xFF, pos);
    tx = pos[0] / 1500;
    tz = pos[1] / 1500;
    p->x48 = D_0009B230[p->id].xC8 / 1500;
    p->x4C = D_0009B230[p->id].xD0 / 1500;

    if (p->x24 & 1)
    {
        d = p->x4C - tz;
        if (d >= 0)
        {
            if (d != 1)
                return;
        }
        else if (p->x4C != tz - 1)
            return;

        for (i = 0; i < 22; i++, o++)
        {
            if (p->id != o->id && p->x28 == o->x28 && o->x4C == tz && (p->x48 - o->x48 >= 0 ? p->x48 - o->x48 : o->x48 - p->x48) < 5)
                return;
        }
        p->flags |= 1;
        p->x44 = pos[1];
    }
    else
    {
        d = p->x48 - tx;
        if (d >= 0)
        {
            if (d != 1)
                return;
        }
        else if (p->x48 != tx - 1)
            return;

        for (i = 0; i < 22; i++, o++)
        {
            if (p->id != o->id && p->x28 == o->x28 && o->x48 == tx && (p->x4C - o->x4C >= 0 ? p->x4C - o->x4C : o->x4C - p->x4C) < 5)
                return;
        }
        p->flags |= 1;
        p->x40 = pos[0];
    }
}
