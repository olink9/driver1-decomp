typedef struct { short x, y; int p; int type; int *args; } ITEM;
extern int func_0005C8F4(), func_0005C864(), func_0005CC68();
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
        return func_0005C864(it->p, it->x, it->y);
    case 0:
        return func_0005CC68(it->p, it->x, it->y);
    case 3:
        a = it->y;
        b = *it->args;
        c = func_0005CC68(it->p, it->x, a);
        func_0006ACC8(buf2, b, 1);
        return func_0005CC68(buf2, (short)c, a);
    case 4:
        s = buf;
        sprintf(s, it->p, *it->args);
        func_0005CC68(s, it->x, it->y);
    }
    return 0;
}
