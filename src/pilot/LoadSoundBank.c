/* LoadSoundBank (Driver 2 name). SpuSetTransferMode = SpuSetTransferMode, SpuRead here is D2's SpuWrite slot. The fix: "len = length - slength" into a fresh single-set variable (sched1 birthing boost puts the subu next to the jal), computed before the bankaddr load. */
typedef struct {
    unsigned int address;
    int a, b, c;
} SAMPLE_DATA;

extern SAMPLE_DATA D_000BCA74[][32];
extern int D_000BD484[];
extern int D_00096C8C;
extern void func_00079DB0();
extern void SpuSetTransferMode();
extern void SpuSetTransferStartAddr();
extern void SpuRead();
extern void SpuIsTransferCompleted();
extern void func_00073840();

int LoadSoundBank(char *address, int length, int bank)
{
    int slength;
    int spuaddress;
    int num_samples;
    int ct;
    int len;

    num_samples = *(int *)address;
    slength = num_samples * 16;
    { SAMPLE_DATA (*s)[32] = D_000BCA74; func_00079DB0(s[bank], address + 4, slength); }
    slength += 4;
    address += slength;
    len = length - slength;
    spuaddress = D_000BD484[bank];
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(spuaddress);
    SpuRead(address, len);
    SpuIsTransferCompleted(1);

    for (ct = 0; ct < num_samples; ct++) {
        D_000BCA74[bank][ct].address += spuaddress;
        if (bank == 2)
            func_00073840(D_00096C8C, ct, D_000BCA74[bank][ct].address);
    }
    return num_samples;
}

