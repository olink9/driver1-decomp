/* Driver 2: DoTremolo */
typedef struct { char p0[0x4E]; unsigned char volume; char p1[0x11]; unsigned char tmpvolume; char p2[0xE]; unsigned char wavecontrol; char p3[3]; unsigned char trmpos; unsigned char trmspd; unsigned char trmdepth; } XM;
typedef struct { char p[0x10]; unsigned short vbtick; } MS;
extern XM *XMC;
extern MS *D_00097814;
extern unsigned char D_0008D948[];

void DoTremolo(void)
{
    unsigned char q;
    unsigned short temp = 0;
    q = (XMC->trmpos >> 2) & 0x1f;
    switch ((XMC->wavecontrol >> 4) & 3) {
    case 0: temp = D_0008D948[q]; break;
    case 1:
        q <<= 3;
        if (XMC->trmpos > 127) q = 255 - q;
        temp = q;
        break;
    case 2: temp = 255; break;
    }
    temp *= XMC->trmdepth;
    temp >>= 6;
    if (XMC->trmpos < 128) {
        XMC->volume = XMC->tmpvolume + temp;
        if (XMC->volume > 128) XMC->volume = 128;
    } else {
        XMC->volume = XMC->tmpvolume - temp;
        if (XMC->volume < 64) XMC->volume = 64;
    }
    if (D_00097814->vbtick) XMC->trmpos += XMC->trmspd;
}
