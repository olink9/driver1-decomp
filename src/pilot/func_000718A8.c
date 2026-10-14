/* Driver 2: DoTremolo */
typedef struct { char p0[0x4E]; unsigned char volume; char p1[0x11]; unsigned char tmpvolume; char p2[0xE]; unsigned char wavecontrol; char p3[3]; unsigned char trmpos; unsigned char trmspd; unsigned char trmdepth; } XM;
typedef struct { char p[0x10]; unsigned short vbtick; } MS;
extern XM *D_0009781C;
extern MS *D_00097814;
extern unsigned char D_0008D948[];

void func_000718A8(void)
{
    unsigned char q;
    unsigned short temp = 0;
    q = (D_0009781C->trmpos >> 2) & 0x1f;
    switch ((D_0009781C->wavecontrol >> 4) & 3) {
    case 0: temp = D_0008D948[q]; break;
    case 1:
        q <<= 3;
        if (D_0009781C->trmpos > 127) q = 255 - q;
        temp = q;
        break;
    case 2: temp = 255; break;
    }
    temp *= D_0009781C->trmdepth;
    temp >>= 6;
    if (D_0009781C->trmpos < 128) {
        D_0009781C->volume = D_0009781C->tmpvolume + temp;
        if (D_0009781C->volume > 128) D_0009781C->volume = 128;
    } else {
        D_0009781C->volume = D_0009781C->tmpvolume - temp;
        if (D_0009781C->volume < 64) D_0009781C->volume = 64;
    }
    if (D_00097814->vbtick) D_0009781C->trmpos += D_0009781C->trmspd;
}
