/* Driver 1 PC port - force-included (-include) into every game file of the PC build.
 * The PS1 build never sees this file: everything PC-only in src/ sits behind #ifdef PSX_PC,
 * so `make` stays byte-identical. */
#ifndef PC_COMPAT_H
#define PC_COMPAT_H
#define PSX_PC 1
#define INCLUDE_ASM(FOLDER, NAME)
#define INCLUDE_RODATA(FOLDER, NAME)

/* common.h typedefs this too; gnu11 allows the repeat. gte.h (PsyCross inline_c.h) needs it in files without common.h */
typedef unsigned int uint;
#endif
