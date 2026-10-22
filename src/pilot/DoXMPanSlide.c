/* Driver 2: DoXMPanSlide */
typedef struct { char pad[0x4C]; unsigned char panning; unsigned char pansspd; } XMC_T;
typedef struct { char pad[0x10]; unsigned short vbtick; } MS_T;
extern MS_T *D_00097814;
extern XMC_T *XMC;

void DoXMPanSlide(unsigned char inf) {
    unsigned char lo, hi;
    short pan;
    if (inf != 0) XMC->pansspd = inf;
    else inf = XMC->pansspd;
    if (D_00097814->vbtick == 0) return;
    lo = inf & 0xf;
    hi = inf >> 4;
    if (hi) lo = 0;
    pan = XMC->panning;
    pan -= lo;
    pan += hi;
    if (pan < 0) pan = 0;
    if (pan > 255) pan = 255;
    XMC->panning = pan;
}
