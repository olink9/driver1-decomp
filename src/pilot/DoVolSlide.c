/* Driver 2: DoVolSlide */
typedef struct { char pad[0x10]; unsigned short vbtick; } MS;
typedef struct { char pad[0x60]; unsigned char tmpvolume; } XM;
extern MS *D_00097814;
extern XM *XMC;

void DoVolSlide(unsigned char dat) {
    if (!D_00097814->vbtick) return;
    XMC->tmpvolume += dat >> 4;
    if (XMC->tmpvolume > 128) XMC->tmpvolume = 128;
    XMC->tmpvolume -= dat & 0xf;
    if (XMC->tmpvolume < 64) XMC->tmpvolume = 64;
}
