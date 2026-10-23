extern int D_001CF0A8, D_001CF0AC, D_001CF09C, D_001CF0E0, D_001CF0E4, D_001CF0E8, D_001CF0EC, D_001CF0F0;
extern char D_001D3230[], D_001D3240[], D_001D3250[], D_001D3260[];
extern void func_001CA408();

void func_001CAA0C(void)
{
    D_001CF0A8 += D_001CF0AC;
    if (D_001CF0A8 > 255) {
        D_001CF0A8 = 255;
        D_001CF0AC = -D_001CF0AC;
    }
    if (D_001CF0A8 < 0) {
        D_001CF0A8 = 0;
        D_001CF0AC = -D_001CF0AC;
    }
    if (D_001CF09C == 0)
        func_001CA408(D_001CF0E0, D_001CF0F0, D_001D3240, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 1, 3);
    else
        func_001CA408(D_001CF0E0, D_001CF0F0, D_001D3240, 255, 255, 255, 1, 3);
    if (D_001CF09C == 1)
        func_001CA408(D_001CF0E4, D_001CF0F0, D_001D3230, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 0, 3);
    else
        func_001CA408(D_001CF0E4, D_001CF0F0, D_001D3230, 255, 255, 255, 0, 3);
    if (D_001CF09C == 2)
        func_001CA408(D_001CF0E8, D_001CF0F0, D_001D3260, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 0, 3);
    else
        func_001CA408(D_001CF0E8, D_001CF0F0, D_001D3260, 255, 255, 255, 0, 3);
    if (D_001CF09C == 3)
        func_001CA408(D_001CF0EC, D_001CF0F0, D_001D3250, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 0, 3);
    else
        func_001CA408(D_001CF0EC, D_001CF0F0, D_001D3250, 255, 255, 255, 0, 3);
}
