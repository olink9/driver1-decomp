extern int D_00096CB4, D_00096CB8, D_00096CBC, D_00096CC0, D_00096CC4;

/* Driver 2: SetupScreenFade */
void func_0005071C(int start, int end, int speed)
{
    D_00096CB4 = start;
    D_00096CB8 = start;
    D_00096CBC = end;
    D_00096CC0 = speed;
    D_00096CC4 = 1;
}
