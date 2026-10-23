/* func_001CB95C: goto-built loop (loop.c ignores it: no &p->s giv), param tested before its copy to p, `case 0: default:` first. */
typedef struct { short x, y; char c4; unsigned char type; char *s; unsigned char r, g, b, p; } E_001CB95C;
extern void SetTextColour();
extern void PrintString();
extern void func_0005C8F4();
extern void PrintStringRightAligned();
void func_001CB95C(E_001CB95C *q)
{
    E_001CB95C *p;
    if (q->s == 0) return;
    p = q;
loop:
    SetTextColour(p->r, p->g, p->b);
    switch (p->type) {
    case 0:
    default:
        PrintString(p->s, p->x, p->y);
        break;
    case 1:
        func_0005C8F4(p->s, p->y);
        break;
    case 2:
        PrintStringRightAligned(p->s, p->x, p->y);
        break;
    }
    p++;
    if (p->s != 0) goto loop;
}
