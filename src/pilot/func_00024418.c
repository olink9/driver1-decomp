typedef struct { int a, b, c, d, e, f; } S;
int func_00024418(int x, int y, S *s) {
    if (s->a < s->e) {
        if (x < s->a || s->e < x) return 0;
    } else {
        if (x < s->e || s->a < x) return 0;
    }
    {
        int b = s->b, f = s->f;
        if (b < f) {
            if (y < b || f < y) return 0;
        } else {
            if (y < f || b < y) return 0;
        }
    }
    return 1;
}
