/* DoS3MRetrig (Driver 2 name) */
typedef struct {
    char pad0[0x10];
    unsigned short vbtick;
} XMSONG;
typedef struct {
    char pad0[0x41];
    unsigned char kick;
    char pad42[0x5C - 0x42];
    unsigned char retrig;
    char pad5D[0x60 - 0x5D];
    unsigned char tmpvolume;
    char pad61[0x6C - 0x61];
    unsigned char s3mrtgspeed;
    unsigned char s3mrtgslide;
} XMCHANNEL;

extern XMSONG *D_00097814;
extern XMCHANNEL *XMC;

void DoS3MRetrig(unsigned char inf)
{
    unsigned char hi, lo;

    hi = inf >> 4;
    lo = inf & 0xf;

    if (lo) {
        XMC->s3mrtgslide = hi;
        XMC->s3mrtgspeed = lo;
    }

    if (hi) {
        XMC->s3mrtgslide = hi;
    }

    if (XMC->s3mrtgspeed > 0) {
        if (XMC->retrig == 0) {
            XMC->kick = 1;
            XMC->retrig = XMC->s3mrtgspeed;

            if (D_00097814->vbtick) {
                switch (XMC->s3mrtgslide) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    XMC->tmpvolume -= (1 << (XMC->s3mrtgslide - 1));
                    break;
                case 6:
                    XMC->tmpvolume = (2 * XMC->tmpvolume) / 3;
                    break;
                case 7:
                    XMC->tmpvolume = XMC->tmpvolume >> 1;
                    if (XMC->tmpvolume < 64)
                        XMC->tmpvolume = 64;
                    break;
                case 9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                    XMC->tmpvolume += (1 << (XMC->s3mrtgslide - 9));
                    break;
                case 0xe:
                    XMC->tmpvolume = (3 * XMC->tmpvolume) / 2;
                    break;
                case 0xf:
                    XMC->tmpvolume = XMC->tmpvolume << 1;
                    break;
                }
                if (XMC->tmpvolume < 64) XMC->tmpvolume = 64;
                if (XMC->tmpvolume > 128) XMC->tmpvolume = 128;
            }
        }
        XMC->retrig--;
    }
}
