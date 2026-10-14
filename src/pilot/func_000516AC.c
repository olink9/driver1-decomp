typedef struct {
    short x, y, w, h;
} RECT16;

/* Driver 2: IncrementClutNum (texture.c) */
void func_000516AC(RECT16 *clut)
{
    clut->x += 16;
    if (clut->x == 1024) {
        clut->x = 960;
        clut->y += 1;
    }
}
