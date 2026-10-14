/* SetColourByValue */
typedef struct { unsigned char r, g, b, pad; int value; int flags; } BAND;
typedef struct { unsigned char r, g, b; } COL;
extern long long D_000975E0;

void func_0006ADFC(BAND *hi, int value, COL *pOut) {
    BAND *lo;
    int temp, inv;

    while (hi->value < value) hi++;

    if (hi->flags == 2)
        value = hi[-1].value + *(unsigned char *)&D_000975E0 * (hi->value - hi[-1].value) / 8;
    lo = hi - 1;
    temp = ((value - lo->value) * 4096) / (hi->value - lo->value);
    inv = 4096 - temp;
    pOut->r = (inv * lo->r + temp * hi->r) >> 12;
    pOut->g = (inv * lo->g + temp * hi->g) >> 12;
    pOut->b = (inv * lo->b + temp * hi->b) >> 12;
}
