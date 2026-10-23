typedef struct { short x, y; int p; int type; int *args; } ITEM;
extern int func_0005C8F4(), PrintStringRightAligned(), PrintString();
extern void sprintf();
extern int func_0006ACC8();

int func_0006B320(ITEM *it)
{
    char buf2[0x10];
    char buf[0x80];
    int a, b, c;
    char *s;
    switch (it->type) {
    case 2:
        return func_0005C8F4(it->p, it->y);
    case 1:
        return PrintStringRightAligned(it->p, it->x, it->y);
    case 0:
        return PrintString(it->p, it->x, it->y);
    case 3:
        a = it->y;
        b = *it->args;
        c = PrintString(it->p, it->x, a);
        func_0006ACC8(buf2, b, 1);
        return PrintString(buf2, (short)c, a);
    case 4:
        s = buf;
        sprintf(s, it->p, *it->args);
        PrintString(s, it->x, it->y);
    }
    return 0;
}
