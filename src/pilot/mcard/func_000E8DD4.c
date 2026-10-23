typedef struct { char fname[0x18]; char title[0x24]; unsigned int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
extern H D_000EE930[];
extern char D_000E684C[];
extern char *strcpy();

/* reset card n's directory: no files, every slot "Empty" */
void func_000E8DD4(int n)
{
    int i;

    D_000EE930[n].s = 1;
    D_000EE930[n].c2 = 0;
    D_000EE930[n].c3 = 0;
    if (n == 0)
        D_000EE930[0].p4 = D_000E684C;
    else
        D_000EE930[n].p4 = D_000E684C;
    for (i = 0; i < 15; i++) {
        strcpy(D_000EE930[n].e[i].fname, "Empty");
        D_000EE930[n].e[i].idx = i;
        D_000EE930[n].e[i].size = 0;
        D_000EE930[n].e[i].n = 1;
        D_000EE930[n].e[i].f43 = 0;
    }
}
