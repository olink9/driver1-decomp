/* Driver 1 decomp - common header. PsyQ SDK declarations come from PsyCross
 * (MIT, OpenDriver2 - include/psyq/LICENSE-PsyCross), configured for the real
 * PSX layout (32-bit tag, no PGXP). */
#ifndef COMMON_H
#define COMMON_H
#define static_assert(c, m)
#include "psx/types.h"
typedef unsigned int uint;
typedef unsigned long ulong;
#include "psx/libgte.h"
#include "psx/libgpu.h"
#include "psx/libetc.h"
#include "psx/libapi.h"
#include "psx/libcd.h"
#include "psx/libspu.h"
#include "psx/libpad.h"
#endif
