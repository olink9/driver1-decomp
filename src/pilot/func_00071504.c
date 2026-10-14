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
extern XMCHANNEL *D_0009781C;

void func_00071504(unsigned char inf)
{
    unsigned char hi, lo;

    hi = inf >> 4;
    lo = inf & 0xf;

    if (lo) {
        D_0009781C->s3mrtgslide = hi;
        D_0009781C->s3mrtgspeed = lo;
    }

    if (hi) {
        D_0009781C->s3mrtgslide = hi;
    }

    if (D_0009781C->s3mrtgspeed > 0) {
        if (D_0009781C->retrig == 0) {
            D_0009781C->kick = 1;
            D_0009781C->retrig = D_0009781C->s3mrtgspeed;

            if (D_00097814->vbtick) {
                switch (D_0009781C->s3mrtgslide) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    D_0009781C->tmpvolume -= (1 << (D_0009781C->s3mrtgslide - 1));
                    break;
                case 6:
                    D_0009781C->tmpvolume = (2 * D_0009781C->tmpvolume) / 3;
                    break;
                case 7:
                    D_0009781C->tmpvolume = D_0009781C->tmpvolume >> 1;
                    if (D_0009781C->tmpvolume < 64)
                        D_0009781C->tmpvolume = 64;
                    break;
                case 9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                    D_0009781C->tmpvolume += (1 << (D_0009781C->s3mrtgslide - 9));
                    break;
                case 0xe:
                    D_0009781C->tmpvolume = (3 * D_0009781C->tmpvolume) / 2;
                    break;
                case 0xf:
                    D_0009781C->tmpvolume = D_0009781C->tmpvolume << 1;
                    break;
                }
                if (D_0009781C->tmpvolume < 64) D_0009781C->tmpvolume = 64;
                if (D_0009781C->tmpvolume > 128) D_0009781C->tmpvolume = 128;
            }
        }
        D_0009781C->retrig--;
    }
}
