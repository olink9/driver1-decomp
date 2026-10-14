typedef struct { short x0, a, lim, cnt; } ENT;
typedef short (*FN)();
extern char D_0008C380[];
extern char D_000BC9BC[];
extern int func_0004FFE8();
extern void func_0004C96C();

short func_0006B9D8(char *obj, int arg, short total) {
    FN *p;
    int ua;
    FN *end = (FN *)D_0008C380;
    ENT *e = (ENT *)obj;
    for (p = end - 11; p < end; p++, e++) {
        if ((*p)(arg, obj)) {
            e->cnt++;
            if (e->cnt < e->lim) continue;
            ua = (unsigned short)e->a;
            if (e->a > 10) {
                total += ua;
                func_0004C96C(D_000BC9BC, func_0004FFE8(total) % 7 + 100);
                e->a = ((int)((unsigned short)e->a << 16)) >> 17;
            }
        }
        e->cnt = 0;
    }
    obj[0x58] = 0;
    obj[0x59] = 0;
    obj[0x5A] = 0;
    return total;
}
