/* q is a single-set local so its p+11 gets sched1's birthing boost and stays after the store
   (a twice-set p += 11 has no boost and loses the tie to the store). */
typedef struct { char pad[12]; unsigned char r, g, b, pad2; } T_001CF468;
extern T_001CF468 D_001CF468[];
extern int D_001CDECC;

void func_001CD554(void)
{
    T_001CF468 *p = D_001CF468;
    T_001CF468 *q;
    D_001CDECC = 0;
    q = p + 11;
    q->r = 255;
    q->g = 255;
    q->b = 255;
}
