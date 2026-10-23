/* DoLightning (Driver 2) */
extern int D_00096AAC;
extern long long D_00097128, D_00096980;
extern int func_000776E4();
extern void add_haze();
void DoLightning(void)
{
    if (D_00096AAC < -10)
        D_00096AAC = func_000776E4() & 0xff;
    else
        D_00096AAC--;
    if (D_00096AAC < 0 && (func_000776E4() & 1) == 0) {
        if (func_000776E4() & 1) {
            add_haze(50, 0, 0x107E);
        } else {
            *(int *)&D_00097128 = *(int *)&D_00097128 << 1;
            if (*(int *)&D_00097128 > 128) *(int *)&D_00097128 = 128;
            *(int *)&D_00096980 = 12;
        }
    }
}
