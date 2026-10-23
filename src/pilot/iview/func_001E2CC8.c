extern int D_001E4018;
extern int D_00096854;
extern int D_0009745C;
extern void SetTextColour();
extern void PrintString();
extern int sprintf();

void func_001E2CC8(void)
{
    char buf[100];

    SetTextColour(0x7F, 0x7F, 0x7F);
    SetTextColour(0, 0x80, 0);
    sprintf(buf, "Score:  %d", D_001E4018);
    PrintString(buf, 0xFA, 0x19);
    SetTextColour(0x80, 0, 0);
    if (D_00096854 == 0)
        sprintf(buf, "Penalties:  %d", D_0009745C);
    PrintString(buf, 0x19, 0x26);
}
