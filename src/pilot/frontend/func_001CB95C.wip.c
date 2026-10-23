/* WIP 73%: gcc strength-reduces &p->s into s0 and copies a0 to s1; target keeps plain p in s0 (frame 0x18, lbu 0xC(s0)). Cause unknown after 6 attempts. */
typedef struct { short x, y; char c4; unsigned char type; char *s; unsigned char r, g, b, p; } E_001CB95C;
extern void SetTextColour();
extern void PrintString();
extern void func_0005C8F4();
extern void PrintStringRightAligned();

void func_001CB95C(E_001CB95C *p)
{
    while (p->s != 0) {
        SetTextColour(p->r, p->g, p->b);
        switch (p->type) {
        case 1:
            func_0005C8F4(p->s, p->y);
            p++;
            break;
        case 2:
            PrintStringRightAligned(p->s, p->x, p->y);
            p++;
            break;
        default:
            PrintString(p->s, p->x, p->y);
            p++;
            break;
        }
    }
}
