/* PSX GTE inline macros (PsyQ inline_c.h style) for matching: the exact register
 * sequences the PsyQ macros emit. Add macros here as functions need them -
 * check each against the target asm. */
#ifndef GTE_H
#define GTE_H

#define gte_SetRotMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $0;" "ctc2  $13, $1;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $2;" "ctc2  $13, $3;" "ctc2  $14, $4" : : "r"(r0) : "$12", "$13", "$14")

#define gte_SetLightMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $8;" "ctc2  $13, $9;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $10;" "ctc2  $13, $11;" "ctc2  $14, $12" : : "r"(r0) : "$12", "$13", "$14")

#define gte_SetColorMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $16;" "ctc2  $13, $17;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $18;" "ctc2  $13, $19;" "ctc2  $14, $20" : : "r"(r0) : "$12", "$13", "$14")

#define gte_SetTransVec0(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")

#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))

#define gte_rtv0tr() __asm__ volatile ( \
    "nop;" "nop;" "mvmva 1, 0, 0, 0, 0")

#define gte_stlvnl(r0) __asm__ volatile ( \
    "swc2  $25, 0( %0 );" "swc2  $26, 4( %0 );" "swc2  $27, 8( %0 )" : : "r"(r0) : "memory")

#endif

/* store SZ3 (from b048) */
#define gte_stsz(r) __asm__ volatile("swc2 $19, 0(%0)" : : "r"(r) : "memory")

/* load VECTOR into IR1-IR3 (hw-08) */
#define gte_ldlvl(r0) __asm__ volatile ( \
    "lwc2  $9, 0( %0 );" "lwc2  $10, 4( %0 );" "lwc2  $11, 8( %0 )" : : "r"(r0))

/* square IR1-IR3 (hw-08) */
#define gte_sqr0() __asm__ volatile ( \
    "nop;" "nop;" ".word 0x4AA00428")
