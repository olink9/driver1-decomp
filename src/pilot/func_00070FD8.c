/* SetInstr (Driver 2) */
typedef struct { char p0[0x35]; unsigned char vol; char p1[0xB]; unsigned char kick; unsigned char sample; char p2[9]; unsigned char panning; char pp; unsigned char volume; char p3[5]; unsigned char transpose; char p4[7]; unsigned char retrig; char p5; unsigned short c2spd; unsigned char tmpvolume; } XMCH;
typedef struct { char p0[0xA]; unsigned short numins; char p1[0xC18-0xC]; unsigned char *samp[1]; } MH;
extern int D_0009783C;
extern XMCH *D_0009781C;
extern MH *D_00097818;
void func_00070FD8(unsigned char inst)
{
    unsigned char *j;
    unsigned char j2;
    if (D_0009783C > 23) return;
    if (!D_0009781C->kick) return;
    if (inst == 255) inst = D_0009781C->sample;
    if (inst >= D_00097818->numins) return;
    D_0009781C->sample = inst;
    j = *(unsigned char **)((char *)D_00097818 + 0xC18 + inst * 4);
    j2 = j[12];
    D_0009781C->tmpvolume = j2 + 64;
    D_0009781C->volume = j2 + 64;
    D_0009781C->vol = j2 + 0x10;
    D_0009781C->retrig = 0;
    if (*(int *)((char *)D_00097818 + 0xF18) == 0) D_0009781C->panning = j[15];
    D_0009781C->transpose = j[16];
    D_0009781C->c2spd = (unsigned char)(j[13] + 128);
}
