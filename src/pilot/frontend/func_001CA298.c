extern unsigned char D_00097680[];
extern unsigned char D_00097698[];
extern int D_00097690;
extern int D_0009768C;
extern void SetTextColour(unsigned char, unsigned char, unsigned char);

int func_001CA298(void)
{
    if (D_0009768C == D_00097690) {
        SetTextColour(0, 0, 0);
        return 0;
    }
    SetTextColour(D_00097698[0] + (D_00097680[0] - D_00097698[0]) * D_0009768C / D_00097690,
                  D_00097698[1] + (D_00097680[1] - D_00097698[1]) * D_0009768C / D_00097690,
                  D_00097698[2] + (D_00097680[2] - D_00097698[2]) * D_0009768C / D_00097690);
    D_0009768C++;
    return 1;
}
