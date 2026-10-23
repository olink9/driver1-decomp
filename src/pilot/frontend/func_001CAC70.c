extern int D_001CF09C;
extern int D_000BFA54, D_000BFA58, D_000BFA5C, D_000BFA60;
extern int D_001CF0C0;

void func_001CAC70(void)
{
    int v;
    if (D_001CF09C == 1) v = D_000BFA58;
    else if (D_001CF09C >= 2) {
        if (D_001CF09C == 2) v = D_000BFA5C;
        else if (D_001CF09C == 3) v = D_000BFA60;
        else v = D_000BFA54;
    } else v = D_000BFA54;
    D_001CF0C0 = v;
}
