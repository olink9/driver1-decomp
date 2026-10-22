/* SetInstr (Driver 2) */
typedef struct { char p0[0x35]; unsigned char vol; char p1[0xB]; unsigned char kick; unsigned char sample; char p2[9]; unsigned char panning; char pp; unsigned char volume; char p3[5]; unsigned char transpose; char p4[7]; unsigned char retrig; char p5; unsigned short c2spd; unsigned char tmpvolume; } XMCH;
typedef struct { char p0[0xA]; unsigned short numins; char p1[0xC18-0xC]; unsigned char *samp[1]; } MH;
extern int D_0009783C;
extern XMCH *XMC;
extern MH *D_00097818;
void SetInstr(unsigned char inst)
{
    unsigned char *j;
    unsigned char j2;
    if (D_0009783C > 23) return;
    if (!XMC->kick) return;
    if (inst == 255) inst = XMC->sample;
    if (inst >= D_00097818->numins) return;
    XMC->sample = inst;
    j = *(unsigned char **)((char *)D_00097818 + 0xC18 + inst * 4);
    j2 = j[12];
    XMC->tmpvolume = j2 + 64;
    XMC->volume = j2 + 64;
    XMC->vol = j2 + 0x10;
    XMC->retrig = 0;
    if (*(int *)((char *)D_00097818 + 0xF18) == 0) XMC->panning = j[15];
    XMC->transpose = j[16];
    XMC->c2spd = (unsigned char)(j[13] + 128);
}
