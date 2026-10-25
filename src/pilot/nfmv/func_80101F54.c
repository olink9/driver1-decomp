/* Caller func_80100DF8 declares this as returning short (sll 16 on the result); defined int here. */
#include "common.h"
typedef struct { unsigned char status; unsigned char len; unsigned short buttons; char pad[30]; } PADBUF;
extern unsigned short D_80126760;
extern volatile short D_80126762;
extern volatile PADBUF D_80126920[];

int func_80101F54(short mask)
{
    int r = 0;
    if (D_80126760)
        if ((D_80126920[D_80126762].buttons & mask) == 0) r = 1;
    return r;
}
