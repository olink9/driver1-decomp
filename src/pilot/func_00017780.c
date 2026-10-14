typedef struct {
    char pad0[0x18];
    int x18;
    int pad1c;
    int x20;
    char pad24[0xEA - 0x24];
    short xEA;
    char padEC[0xF4 - 0xEC];
    int xF4;
    char padF8[0x18C - 0xF8];
} ENTRY;

typedef struct {
    unsigned char id;
    char pad1[0x16];
    unsigned char flags;
    char pad18[0x24 - 0x18];
    int x24;
    char pad28[0x38 - 0x28];
    int x38;
    int pad3c;
    int x40;
    int x44;
} OBJ;

extern ENTRY D_0009B230[];

unsigned char func_00017780(OBJ *p, int a1, int a2, unsigned char mode)
{
    unsigned char res = 1;
    ENTRY *e = &D_0009B230[p->id];
    int d;

    if (p->x40 != 0x7FFFFFFF && p->x38 != 0 && (mode == 0 || mode == 2))
    {
        d = p->x40 - e->x18;
        if (abs(d) <= 32)
        {
            if (p->flags & 1)
            {
                e->x18 = p->x40;
                p->x40 = 0x7FFFFFFF;
                p->flags &= 0xDE;
                e->xF4 = 0;
                e->xEA = 0;
                if (p->flags & 0x10)
                    p->flags = (p->flags & 0xEF) | 8;
            }
        }
        else if (p->x38 >= 11)
        {
            if (p->flags & 1)
            {
                if (p->x24 == 2)
                {
                    if (p->flags & 0x30)
                        e->xF4 = -(d / 8);
                    else
                        e->xF4 = -(d / 32);
                }
                else
                {
                    if (p->flags & 0x30)
                        e->xF4 = d / 8;
                    else
                        e->xF4 = d / 32;
                }
            }
            res = 0;
        }
    }

    if (p->x44 != 0x7FFFFFFF && p->x38 != 0 && (mode == 1 || mode == 3))
    {
        d = p->x44 - e->x20;
        if (abs(d) <= 32)
        {
            if (p->flags & 1)
            {
                e->x20 = p->x44;
                p->x44 = 0x7FFFFFFF;
                p->flags &= 0xDE;
                e->xF4 = 0;
                e->xEA = 0;
                if (p->flags & 0x10)
                    p->flags = (p->flags & 0xEF) | 8;
            }
        }
        else if (p->x38 >= 11)
        {
            if (p->flags & 1)
            {
                if (p->x24 == 1)
                {
                    if (p->flags & 0x30)
                        e->xF4 = -(d / 8);
                    else
                        e->xF4 = -(d / 32);
                }
                else
                {
                    if (p->flags & 0x30)
                        e->xF4 = d / 8;
                    else
                        e->xF4 = d / 32;
                }
            }
            res = 0;
        }
    }

    if (res)
    {
        p->x40 = 0x7FFFFFFF;
        p->x44 = 0x7FFFFFFF;
    }
    return res;
}
