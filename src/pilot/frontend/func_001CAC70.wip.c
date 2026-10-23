/* WIP 83.8%: switch tree starts with ==2 instead of ==1/slti 2, and extra j/nop (ov1). */
extern int D_001CF09C;
extern int D_000BFA54, D_000BFA58, D_000BFA5C, D_000BFA60;
extern int D_001CF0C0;

void func_001CAC70(void)
{
    switch (D_001CF09C) {
    case 1: D_001CF0C0 = D_000BFA58; break;
    case 2: D_001CF0C0 = D_000BFA5C; break;
    case 3: D_001CF0C0 = D_000BFA60; break;
    case 0:
    default: D_001CF0C0 = D_000BFA54; break;
    }
}
