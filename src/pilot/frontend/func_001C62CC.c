/* func_001C62CC - refresh menu item texts (D_001CE534: 16-byte menu items, text pointer at +8) and two value positions.
   D_001CE794 as an unsized array (MEM_IN_STRUCT) keeps its load below the item stores; 79C/7A0 are the plain ints the
   sibling inc/dec callbacks (func_001C6370/63A4) use; D_001CE4D0 = 0x28-byte records with a short at +0x14. */
extern int D_00096A88, D_00096ABC, D_00096AC0;
extern char *D_001CE784[];
typedef struct { int a; unsigned char f; char pad[3]; char *s; int c; } MI;
extern MI D_001CE534[];
extern char *D_001CE794[];
extern int D_001CE79C, D_001CE7A0;
typedef struct { char pad[0x14]; short x; char pad2[0x12]; } TX;
extern TX D_001CE4D0[];

void func_001C62CC(void)
{
    D_001CE534[2].s = D_001CE784[1];
    D_001CE534[4].s = D_001CE784[D_00096ABC];
    D_001CE534[6].s = D_001CE784[D_00096AC0];
    D_001CE534[8].s = D_001CE784[1];
    D_001CE534[10].s = D_001CE794[0];
    D_001CE534[12].s = D_001CE784[D_00096A88];
    D_001CE534[16].s = D_001CE784[1];
    D_001CE534[18].s = D_001CE784[1];
    D_001CE4D0[0].x = D_001CE79C + 0x1C6;
    D_001CE4D0[1].x = D_001CE7A0 + 0x1C6;
}
