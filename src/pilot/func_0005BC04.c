typedef struct { char a, b; char pad[0x4e]; } E;
typedef struct { char pad[0xC8]; int cx; int pad2; int cy; char pad3[396 - 0xD4]; } B;
extern E D_00098950[];
extern B D_0009B0A4[];

int func_0005BC04(int i)
{
    int j = i + 1;
    int n;
    int dx, dy;

    if (i >= 25)
        return 0;
    for (n = 0; n < 24; n++) {
        if (D_00098950[j - 1].b == 4) {
            dx = D_0009B0A4[0].cx - D_0009B0A4[j].cx;
            dy = D_0009B0A4[0].cy - D_0009B0A4[j].cy;
            dx = abs(dx);
            dy = abs(dy);
            if (dx < 8000 && dy < 8000)
                return j;
        }
        if (j >= 25)
            return 0;
        j++;
    }
    return 0;
}
