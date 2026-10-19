typedef struct { char pad[0x12]; volatile unsigned short flags; } S1AECC;
extern long long D_00097668;
extern long long D_00096AA8;
extern long long D_00096AA9;

void func_0001AECC(void)
{
    int a = (*(S1AECC **)&D_00097668)->flags & 1;
    unsigned int b = (*(S1AECC **)&D_00097668)->flags & 2; b = b != 0;
    *(unsigned char *)&D_00096AA8 = a * 180;
    *(unsigned char *)&D_00096AA9 = b * 180;
}
