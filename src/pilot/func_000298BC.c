extern long long D_00096834, D_00097370;
extern int D_00097138, D_00097150;
extern short *D_0009713C, *D_00097140;
void func_000298BC(short *p)
{
    int a, b;
    D_0009713C = p;
    D_00097140 = p + *(int *)&D_00097370;
    switch (*(int *)&D_00096834) {
    default: a = 0xEFEFEF; b = 0x5FC0DF; break;
    case 1: a = 0xB0B4BC; b = 0x6FA6BA; break;
    case 3: D_00097138 = 0x454548; b = 0x3243; goto st;
    case 5: a = 0x74777E; b = 0x296378; break;
    case 6: a = 0x657389; b = 0x303A69; break;
    }
    D_00097138 = a;
st:
    D_00097150 = b;
}
