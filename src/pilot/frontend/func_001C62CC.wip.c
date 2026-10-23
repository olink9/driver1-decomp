/* WIP 83.17 (was 69.05): D_001CE794/79C/7A0 as unsized arrays (MEM_IN_STRUCT, so their loads stay below the
   D_001CE534 stores as in the target). Left: target issues lhu 79C/7A0 BEFORE the [66]/[74] stores (sw 0x108/0x128)
   and lw 794 before lw A88; with arrays the lhu's wait for all D_001CE534 stores, with scalars they float to the top.
   Tried: all 90 placements of the two short statements (end is best), 794 scalar (70), one struct 794..7A0 (80.6),
   struct for 79C/7A0 (82.8), locals for the short sums before [66] (78.7). Untried: other D_001CE534/4D0 types
   (0x20-stride struct for 534: fields at +8 of entries 1..9). */
extern int D_00096A88, D_00096ABC, D_00096AC0;
extern int D_001CE534[];
extern int D_001CE784[];
extern int D_001CE794[];
extern unsigned short D_001CE79C[], D_001CE7A0[];
extern short D_001CE4D0[];

void func_001C62CC(void)
{
    D_001CE534[10] = D_001CE784[1];
    D_001CE534[18] = D_001CE784[D_00096ABC];
    D_001CE534[26] = D_001CE784[D_00096AC0];
    D_001CE534[34] = D_001CE784[1];
    D_001CE534[42] = D_001CE794[0];
    D_001CE534[50] = D_001CE784[D_00096A88];
    D_001CE534[66] = D_001CE784[1];
    D_001CE534[74] = D_001CE784[1];
    D_001CE4D0[10] = D_001CE79C[0] + 0x1C6;
    D_001CE4D0[30] = D_001CE7A0[0] + 0x1C6;
}
