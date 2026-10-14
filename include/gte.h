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

#endif
