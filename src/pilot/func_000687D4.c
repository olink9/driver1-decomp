/* Driver 2 name unknown (script opcode skip: returns pointer past the instruction) */
typedef struct { int type; int pad[3]; } E16;
extern signed char D_0008BCE4[];
extern E16 D_0008BD58[];

int *func_000687D4(int *p)
{
    int op = *p;
    int n = D_0008BCE4[op];
    int off;
    if (n > 0)
        return (int *)((char *)p + n);
    switch (op) {
    case 2:
        if (D_0008BD58[p[1]].type != 1)
            goto r12;
    case 0x16:
        off = p[2] + 12;
        break;
    r12:
        return (int *)((char *)p + 12);
    case 0x14:
        off = p[1] * 3;
    words:
        off = off * 4 + 8;
        break;
    case 0x46:
        off = p[3] + 16;
        break;
    case 0x56:
        p++;
        do {
        } while (*p++ != 0x7FFFFFFF);
        return p;
    case 0x5E:
        off = p[1] + 8;
        break;
    case 0x68:
        off = p[1];
        goto words;
    default:
        return p;
    }
    return (int *)((char *)p + off);
}
