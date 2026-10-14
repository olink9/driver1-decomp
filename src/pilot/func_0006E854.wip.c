extern char D_000868EC[], D_000869DC[], D_00086BBC[], D_00086D9C[];
extern int func_0006E4C0();
extern int func_0006E4CC();
extern int func_0006E558();
extern int func_0006E598();
extern int func_0006E604();

static inline char *getent(int type, int idx)
{
    char *p;

    switch (type) {
    case 0x11:
        p = D_000869DC + idx * 60;
        break;
    case 0x12:
        p = D_00086BBC + idx * 60;
        break;
    case 0x13:
        p = D_000868EC + idx * 60;
        break;
    case 0x16:
        p = D_00086D9C + idx * 72;
        break;
    default:
        return 0;
    }
    return p;
}

static inline int dispatch(int **a0, int type, char *s, int *pl)
{
    switch (type) {
    case 0x11:
        pl[0] = func_0006E4C0(a0[0], *a0[1]);
        return func_0006E4CC(pl[0], s);
    case 0x12:
        pl[0] = (int)a0[0];
        return func_0006E604(pl[0], s);
    case 0x13:
        pl[0] = (int)a0[0];
        return func_0006E558(pl[0], s);
    case 0x16:
        return func_0006E598(a0[0], a0[1][7], s);
    }
    return -1;
}

int func_0006E854(int **a0, int type, int idx)
{
    char *s;
    int loc[6];
    int r;

    s = getent(type, idx);
    if (s == 0)
        return 0;
    loc[2] = -1;
    r = dispatch(a0, type, s, loc);
    if (r == -1 && loc[2] == r)
        return 0;
    return 1;
}
