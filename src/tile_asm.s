/* tile_asm.s: original assembler module, 20 functions func_00026794..func_00027098 (0x26794-0x270D4).
 * GTE helpers under the map/tile drawer (DrawMapPSX and tile.c call them; REDRIVER2's rndrasm.c/d2mapasm.c are the
 * D2 counterparts): register-convention entry points that share GTE state, fall through into each other and
 * pass values in non-ABI registers - not compiler output. func_00026B94 has no HANDWRITTEN tag because its
 * pilot matched as C (two loads into t2/t3 through inline asm); it is part of this module all the same.
 * Starts where unpack_asm.s ends (see the cut note there); tile.c follows at 0x270D4. */

.include "macro.inc"

.set noreorder

.section .text, "ax"

/* ---------------- func_00026794 (HANDWRITTEN ASM) ---------------- */
glabel func_00026794
    lw $8, 0($4)
    lw $9, 4($4)
    lw $10, 8($4)
    cfc2 $11, $21
    cfc2 $12, $22
    cfc2 $13, $23
    sub $8, $8, $11
    sub $9, $9, $12
    sub $10, $10, $13
    mtc2 $8, $9
    mtc2 $9, $10
    mtc2 $10, $11
    lw $8, 0($5)
    lw $9, 4($5)
    mvmva 1, 2, 3, 3, 0
    lw $10, 8($5)
    ctc2 $8, $0
    lw $11, 12($5)
    ctc2 $9, $1
    lw $12, 16($5)
    ctc2 $10, $2
    ctc2 $11, $3
    ctc2 $12, $4
    mfc2 $8, $9
    mfc2 $9, $10
    mfc2 $2, $11
    ctc2 $8, $5
    ctc2 $9, $6
    jr $31
    ctc2 $2, $7
endlabel func_00026794

/* ---------------- func_0002680C (HANDWRITTEN ASM) ---------------- */
/* GTE: subtract translation (cfc2 21-23) from a vector, rotate by mvmva, store result as new TR (ctc2 5-7) */
glabel func_0002680C
    lw $8, 0($4)
    lw $9, 4($4)
    lw $10, 8($4)
    cfc2 $11, $21
    cfc2 $12, $22
    cfc2 $13, $23
    sub $8, $8, $11
    sub $9, $9, $12
    sub $10, $10, $13
    mtc2 $8, $9
    mtc2 $9, $10
    mtc2 $10, $11
    nop
    nop
    .word 0x4A4DE012
    nop
    nop
    mfc2 $8, $9
    mfc2 $9, $10
    mfc2 $2, $11
    ctc2 $8, $5
    ctc2 $9, $6
    jr $31
    ctc2 $2, $7
endlabel func_0002680C

/* ---------------- func_0002686C (HANDWRITTEN ASM) ---------------- */
glabel func_0002686C
    swc2 $12, 0($2)
    swc2 $13, 4($2)
    swc2 $14, 8($2)
endlabel func_0002686C

/* ---------------- func_00026878 (HANDWRITTEN ASM) ---------------- */
glabel func_00026878
    lwc2 $0, 0($12)
    lwc2 $1, 4($12)
    lwc2 $2, 8($12)
    lwc2 $3, 12($12)
    lwc2 $4, 16($12)
    jr $31
    lwc2 $5, 20($12)
endlabel func_00026878

/* ---------------- func_00026894 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: store SXY0-2 to t1, t1 += 12, t6 -= 3; t6 = 0 once it runs out (sibling of func_000268B8) */
glabel func_00026894
    addi $14, $14, -3
    swc2 $12, 0x0($9)
    swc2 $13, 0x4($9)
    swc2 $14, 0x8($9)
    bgtz $14, .L000268B0
    addi $9, $9, 0xC
    addu $14, $0, $0
  .L000268B0:
    jr $31
    nop
endlabel func_00026894

/* ---------------- func_000268B8 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: store SXY0-2 to t1, t6 -= 3, loop into func_000268D8 while t6 > 0 */
glabel func_000268B8
    addi $14, $14, -3
    swc2 $12, 0x0($9)
    swc2 $13, 0x4($9)
    swc2 $14, 0x8($9)
    bgtz $14, func_000268D8
    addi $9, $9, 0xC
    jr $31
    addiu $14, $0, -1
endlabel func_000268B8

/* ---------------- func_000268D8 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: load 3 SVECTORs (VXY0..VZ2) from t0, t0 += 24, rtpt in the jr delay slot */
glabel func_000268D8
    lwc2 $0, 0x0($8)
    lwc2 $1, 0x4($8)
    lwc2 $2, 0x8($8)
    lwc2 $3, 0xC($8)
    lwc2 $4, 0x10($8)
    lwc2 $5, 0x14($8)
    addi $8, $8, 0x18
    jr $31
    .word 0x4A280030
endlabel func_000268D8

/* ---------------- func_000268FC (HANDWRITTEN ASM) ---------------- */
glabel func_000268FC
    lwc2 $0, 0x0($a0)
    lwc2 $1, 0x4($a0)
    lwc2 $2, 0x8($a0)
    lwc2 $3, 0xC($a0)
    lwc2 $4, 0x10($a0)
    lwc2 $5, 0x14($a0)
    lui $v1, 0x1f80
    lw $v1, 0x1f0($v1)
    rtpt
    lw $t0, 0x0($a1)
    lui $at, %hi(texture_pages)
    addiu $at, $at, %lo(texture_pages)
    srl $t1, $t0, 2
    andi $t1, $t1, 0x3FC0
    srl $t2, $t0, 15
    andi $t2, $t2, 0xFE
    add $t2, $t2, $t1
    add $t3, $t2, $at
    srl $t1, $t1, 5
    add $at, $t1, $at
    lh $t4, 0x100($t3)
    lh $t5, 0x0($at)
    lui $at, 0x1f80
    add $a3, $a3, $at
    mfc2 $at, $12
    mfc2 $t1, $13
    mfc2 $t6, $14
    lwc2 $0, 0x18($a0)
    lwc2 $1, 0x1C($a0)
    sll $t4, $t4, 16
    sll $t5, $t5, 16
    rtps
    lw $t2, 0x8($a1)
    lw $t7, 0xC($a1)
    srl $t3, $t2, 16
    sw $v1, 0x4($a2)
    lw $a1, 0x0($a3)
    sw $at, 0x8($a2)
    sw $a2, 0x0($a3)
    sw $t1, 0x18($a2)
    andi $t2, $t2, 0xFFFF
    sw $t6, 0x20($a2)
    srl $t0, $t7, 16
    swc2 $14, 0x10($a2)
    or $t4, $t4, $t2
    sw $t3, 0x1C($a2)
    or $t5, $t5, $t0
    sw $t7, 0x24($a2)
    lui $t0, 0x900
    sw $t4, 0xC($a2)
    or $t0, $t0, $a1
    sw $t5, 0x14($a2)
    jr $ra
    sw $t0, 0x0($a2)
endlabel func_000268FC

/* ---------------- func_000269D4 (HANDWRITTEN ASM) ---------------- */
glabel func_000269D4
    lui $v1, 0x1F80
    sw $ra, 0x38($v1)
    addi $t0, $a0, 0x2C
    lw $t9, 0x0($t0)
    lui $t3, %hi(D_0009714C)
    addiu $t3, $t3, %lo(D_0009714C)
    srl $t8, $t9, 16
    lw $t3, 0x0($t3)
    andi $t9, $t9, 0xFFFF
    jal func_00026878
    add $t4, $t9, $t3
    add $t8, $t8, $t3
    addi $t4, $t4, 0x18
    rtpt
    sw $s0, 0x3C($v1)
    sw $s1, 0x40($v1)
    sw $s2, 0x44($v1)
    sw $s3, 0x48($v1)
    sw $s4, 0x4C($v1)
    sw $s6, 0x50($v1)
    add $s0, $a2, $v1
    lw $a3, 0x14($a0)
    andi $at, $t8, 0x3
    sub $t8, $t8, $at
    add $a2, $v1, $at
    lw $t1, 0x0($t8)
    lw $t6, 0x4($t8)
    lw $t5, 0x8($t8)
    sw $t1, 0xCC($v1)
    sw $t6, 0xD0($v1)
    jal func_0002686C
    addi $v0, $v1, 0xE0
    sw $t5, 0xD4($v1)
    addi $t4, $t4, 0x18
    addi $v0, $v0, 0xC
    rtpt
    lw $t1, 0xC($t8)
    lw $t5, 0x10($t8)
    sw $t1, 0xD8($v1)
    sw $t5, 0xDC($v1)
    lw $t1, 0x0($s0)
    addi $t5, $a1, 0x78
    sw $t5, 0x0($s0)
    lw $t2, 0x0($a3)
    jal func_0002686C
    lui $t8, 0x900
    addi $t4, $t4, 0x18
    addi $v0, $v0, 0xC
    rtpt
    or $t8, $t8, $t1
    lui $s6, %hi(texture_pages)
    addiu $s6, $s6, %lo(texture_pages)
    jal func_00026BB8
    mtc2 $t8, $20
    lw $t0, 0x1F0($v1)
    swc2 $12, 0x0($v0)
    swc2 $13, 0x4($v0)
    swc2 $14, 0x8($v0)
    mtc2 $t0, $21
    sll $t8, $t8, 16
    sll $t9, $t9, 16
    addiu $t3, $zero, 0x4
    lui $t2, %hi(D_00086F20)
    addiu $t2, $t2, %lo(D_00086F20)
  .L00026AD4:
    lw $a0, 0x0($t2)
    addi $t2, $t2, 0x4
    andi $t5, $a0, 0x1E
    add $at, $t5, $t5
    add $t5, $t5, $a2
    add $at, $at, $v1
    lhu $t0, 0xCC($t5)
    lw $s1, 0xE0($at)
    or $t0, $t0, $t8
    mtc2 $t0, $22
    srl $a0, $a0, 8
    andi $t5, $a0, 0x1E
    add $at, $t5, $t5
    add $t5, $t5, $a2
    add $at, $at, $v1
    lhu $a3, 0xCC($t5)
    lw $s4, 0xE0($at)
    or $a3, $a3, $t9
    mtc2 $a3, $23
    srl $a0, $a0, 8
    andi $t5, $a0, 0x1E
    add $at, $t5, $t5
    add $t5, $t5, $a2
    add $at, $at, $v1
    lhu $t4, 0xCC($t5)
    lw $s2, 0xE0($at)
    srl $a0, $a0, 8
    add $at, $a0, $a0
    add $a0, $a0, $a2
    add $at, $at, $v1
    lhu $t5, 0xCC($a0)
    lw $s3, 0xE0($at)
    jal func_00026BE8
    addu $at, $a1, $zero
    lui $s3, 0x900
    or $s3, $s3, $at
    addi $t3, $t3, -0x1
    bnez $t3, .L00026AD4
    mtc2 $s3, $20
    lw $ra, 0x38($v1)
    lw $s0, 0x3C($v1)
    lw $s1, 0x40($v1)
    lw $s2, 0x44($v1)
    lw $s3, 0x48($v1)
    lw $s4, 0x4C($v1)
    lw $s6, 0x50($v1)
    jr $ra
    addu $v0, $a1, $zero
endlabel func_000269D4

/* ---------------- func_00026B94 (matched by a C pilot: void func_00026B94(void) { __asm__ volatile("lw $10, 4($7)"); __asm__ volatile("lw $11, 4($7)"); }) ---------------- */
glabel func_00026B94
    lw $t2, 0x4($a3)
    lw $t3, 0x4($a3)
    jr $ra
    nop
endlabel func_00026B94

/* ---------------- func_00026BA4 (HANDWRITTEN ASM) ---------------- */
/* GTE helper fragment: loads four words from a3 into t2-t5, advances a3 by 0x14, falls through (no jr) */
glabel func_00026BA4
    lw $10, 0x0($7)
    lw $11, 0x4($7)
    lw $12, 0x8($7)
    lw $13, 0xC($7)
    addi $7, $7, 0x14
endlabel func_00026BA4

/* ---------------- func_00026BB8 (HANDWRITTEN ASM) ---------------- */
/* table lookup helper: index in $t2, table base in $s6 -> signed halfwords in $t8/$t9 (non-ABI registers) */
glabel func_00026BB8
    srl $25, $10, 2
    andi $25, $25, 0x3FC0
    srl $24, $10, 15
    andi $24, $24, 0xFE
    add $24, $24, $25
    add $24, $24, $22
    srl $25, $25, 5
    add $25, $25, $22
    lh $24, 0x100($24)
    lh $25, 0($25)
    jr $31
    nop
endlabel func_00026BB8

/* ---------------- func_00026BE8 (HANDWRITTEN ASM) ---------------- */
/* store GTE SXY0-2 / SXYP plus caller s-/t-registers as a 0x28-byte record at $a1, advance $a1 (non-ABI) */
glabel func_00026BE8
    swc2 $20, 0($5)
    swc2 $21, 4($5)
    sw $17, 8($5)
    swc2 $22, 0xC($5)
    sw $20, 0x10($5)
    swc2 $23, 0x14($5)
    sw $18, 0x18($5)
    sw $12, 0x1C($5)
    sw $19, 0x20($5)
    sw $13, 0x24($5)
    jr $31
    addi $5, $5, 0x28
endlabel func_00026BE8

/* ---------------- func_00026C18 (HANDWRITTEN ASM) ---------------- */
/* stores t3,t4,s1,s2,s4,s3 to a1 and advances a1 by 0x18 */
glabel func_00026C18
    sw $11, 0x0($5)
    sw $12, 0x4($5)
    sw $17, 0x8($5)
    sw $18, 0xC($5)
    sw $20, 0x10($5)
    sw $19, 0x14($5)
    jr $31
    addi $5, $5, 0x18
endlabel func_00026C18

/* ---------------- func_00026C38 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: no prologue, uses the v1 scratch block and bgezal into func_00026894/func_000268D8/func_000268B8; rtpt x3 + 4-iteration face loop over D_00086F20 */
glabel func_00026C38
    sw $ra, 0x18($v1)
    bgezal $t6, func_00026894
    lw $t9, 0x0($s5)
    lui $t3, %hi(D_0009714C)
    addiu $t3, $t3, %lo(D_0009714C)
    srl $t8, $t9, 16
    lw $t3, 0x0($t3)
    andi $t9, $t9, 0xFFFF
    add $t4, $t9, $t3
    jal func_00026878
    addi $s5, $s5, 0x4
    add $t8, $t8, $t3
    sw $s5, 0x1C($v1)
    .word 0x4A280030
    addi $t4, $t4, 0x18
    andi $at, $t8, 0x3
    sub $t8, $t8, $at
    add $a2, $v1, $at
    addi $v0, $v1, 0xE0
    lw $s1, 0x0($t8)
    lw $s2, 0x4($t8)
    lw $t5, 0x8($t8)
    sw $s1, 0xCC($v1)
    sw $s2, 0xD0($v1)
    and $at, $a1, $s7
    jal func_0002686C
    sw $t5, 0xD4($v1)
    addi $t4, $t4, 0x18
    addi $v0, $v0, 0xC
    .word 0x4A280030
    lw $s1, 0xC($t8)
    lw $s2, 0x10($t8)
    sw $s1, 0xD8($v1)
    sw $s2, 0xDC($v1)
    lw $s1, 0x4($v1)
    addi $t5, $at, 0x78
    sw $t5, 0x4($v1)
    lw $t2, 0x0($a3)
    jal func_0002686C
    lw $s3, 0x1F0($v1)
    addi $t4, $t4, 0x18
    addi $v0, $v0, 0xC
    .word 0x4A280030
    lui $t5, 0x900
    or $t5, $t5, $s1
    jal func_00026BB8
    mtc2 $t5, $20
    swc2 $12, 0x0($v0)
    swc2 $13, 0x4($v0)
    swc2 $14, 0x8($v0)
    lw $s3, 0x1F0($v1)
    bgezal $t6, func_000268D8
    mtc2 $s3, $21
    sll $t8, $t8, 16
    sll $t9, $t9, 16
    addiu $t3, $zero, 0x4
    lui $t2, %hi(D_00086F20)
    addiu $t2, $t2, %lo(D_00086F20)
  .L00026D20:
    lw $s3, 0x0($t2)
    addi $t2, $t2, 0x4
    andi $t5, $s3, 0x1E
    add $at, $t5, $t5
    add $t5, $t5, $a2
    add $at, $at, $v1
    lhu $s4, 0xCC($t5)
    lw $s1, 0xE0($at)
    or $s4, $s4, $t8
    mtc2 $s4, $22
    srl $s3, $s3, 8
    andi $t5, $s3, 0x1E
    add $at, $t5, $t5
    add $t5, $t5, $a2
    add $at, $at, $v1
    bgezal $t6, func_000268B8
    lhu $t4, 0xCC($t5)
    lw $s4, 0xE0($at)
    or $t4, $t4, $t9
    mtc2 $t4, $23
    srl $s3, $s3, 8
    andi $t5, $s3, 0x1E
    add $at, $t5, $t5
    add $t5, $t5, $a2
    add $at, $at, $v1
    lhu $t4, 0xCC($t5)
    lw $s2, 0xE0($at)
    srl $s3, $s3, 8
    add $at, $s3, $s3
    add $s3, $s3, $a2
    add $at, $at, $v1
    lhu $t5, 0xCC($s3)
    lw $s3, 0xE0($at)
    jal func_00026BE8
    and $at, $a1, $s7
    lui $s3, 0x900
    or $s3, $s3, $at
    addi $t3, $t3, -0x1
    bnez $t3, .L00026D20
    mtc2 $s3, $20
    lw $ra, 0x18($v1)
    lw $s5, 0x1C($v1)
    jr $ra
    addi $a3, $a3, 0x14
endlabel func_00026C38

/* ---------------- func_00026DD0 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: saves ra in the v1 scratch block, bgezal into func_000268B8, tail-jumps into func_00026C18 */
glabel func_00026DD0
    lw $17, 0x8($3)
    and $18, $5, $23
    sw $18, 0x8($3)
    sw $31, 0x18($3)
    lui $18, 0x500
    bgezal $14, func_000268B8
    or $11, $17, $18
    lw $20, 0x4($7)
    lbu $19, 0x1($7)
    sll $17, $20, 2
    andi $17, $17, 0x3FC
    add $17, $17, $3
    srl $18, $20, 6
    andi $18, $18, 0x3FC
    add $18, $18, $3
    andi $19, $19, 0x1
    sll $19, $19, 2
    add $19, $19, $3
    lw $12, 0x1F4($19)
    srl $19, $20, 14
    andi $19, $19, 0x3FC
    add $19, $19, $3
    srl $20, $20, 22
    andi $20, $20, 0x3FC
    add $20, $20, $3
    lw $17, 0x5C($17)
    lw $18, 0x5C($18)
    bgezal $14, func_000268B8
    lw $19, 0x5C($19)
    lw $31, 0x18($3)
    lw $20, 0x5C($20)
    j func_00026C18
    addi $7, $7, 0xC
endlabel func_00026DD0

/* ---------------- func_00026E54 (HANDWRITTEN ASM) ---------------- */
glabel func_00026E54
    lw $t3, 0x4($a3)
    lw $s1, 0xC($v1)
    sll $at, $t3, 2
    andi $at, $at, 0x3FC
    add $at, $at, $v1
    lwc2 $12, 0x5C($at)
    srl $at, $t3, 6
    andi $at, $at, 0x3FC
    add $at, $at, $v1
    lwc2 $13, 0x5C($at)
    srl $at, $t3, 14
    andi $at, $at, 0x3FC
    add $at, $at, $v1
    lwc2 $14, 0x5C($at)
    srl $t8, $t3, 22
    andi $t8, $t8, 0x3FC
    nclip
    lui $s2, 0x900
    or $s1, $s1, $s2
    and $s2, $a1, $s7
    lw $t4, 0x8($a3)
    mfc2 $at, $24
    add $t8, $t8, $v1
    slt $at, $at, $zero
    bnez $at, .L00026F20
    sw $ra, 0x18($v1)
    sw $s2, 0xC($v1)
    lw $t5, 0xC($a3)
    lw $t2, 0x0($a3)
    jal func_00026BB8
    lw $s4, 0x5C($t8)
    lw $ra, 0x18($v1)
    sll $t8, $t8, 16
    sll $t9, $t9, 16
    andi $at, $t4, 0xFFFF
    or $t8, $t8, $at
    srl $at, $t5, 16
    or $t9, $t9, $at
    srl $t4, $t4, 16
    lw $s2, 0x1F0($v1)
    sw $s1, 0x0($a1)
    sw $s2, 0x4($a1)
    swc2 $12, 0x8($a1)
    sw $t8, 0xC($a1)
    sw $s4, 0x10($a1)
    sw $t9, 0x14($a1)
    swc2 $13, 0x18($a1)
    sw $t4, 0x1C($a1)
    swc2 $14, 0x20($a1)
    sw $t5, 0x24($a1)
    addi $a1, $a1, 0x28
  .L00026F20:
    jr $ra
    addi $a3, $a3, 0x14
endlabel func_00026E54

/* ---------------- func_00026F28 (HANDWRITTEN ASM) ---------------- */
glabel func_00026F28
    lw $17, 0x8($3)
    and $18, $5, $23
    j .L00026F54
    sw $18, 0x8($3)
    lw $17, 0x8($3)
    and $18, $5, $23
    j .L00026F54
    sw $18, 0x8($3)
    lw $17, 0x4($3)
    and $18, $5, $23
    sw $18, 0x4($3)
  .L00026F54:
    sw $31, 0x18($3)
    lui $18, (0x9000000 >> 16)
    or $17, $17, $18
    lui $18, (0x2C808080 >> 16)
    bgezal $14, func_000268B8
    ori $18, $18, (0x2C808080 & 0xFFFF)
    jal func_00026BA4
    mtc2 $17, $20
    lw $18, 0x1F0($3)
    bgezal $14, func_000268B8
    mtc2 $18, $21
    sll $24, $24, 16
    sll $25, $25, 16
    andi $1, $12, 0xFFFF
    or $24, $24, $1
    mtc2 $24, $22
    srl $1, $13, 16
    or $25, $25, $1
    mtc2 $25, $23
    srl $12, $12, 16
    sll $17, $11, 2
    andi $17, $17, 0x3FC
    add $17, $17, $3
    srl $18, $11, 6
    andi $18, $18, 0x3FC
    add $18, $18, $3
    srl $19, $11, 14
    andi $19, $19, 0x3FC
    add $19, $19, $3
    srl $20, $11, 22
    andi $20, $20, 0x3FC
    add $20, $20, $3
    lw $17, 0x5C($17)
    lw $18, 0x5C($18)
    bgezal $14, func_000268B8
    lw $19, 0x5C($19)
    lw $31, 0x18($3)
    b func_00026BE8
    lw $20, 0x5C($20)
endlabel func_00026F28

/* ---------------- func_00026FF0 (HANDWRITTEN ASM) ---------------- */
glabel func_00026FF0
    lui $v1, 0x1f80
    sw $ra, 0x38($v1)
    lw $t7, 0xC($a0)
    lw $t0, 0x10($a0)
    andi $t6, $t7, 0xFFFF
    jal func_000268D8
    srl $t7, $t7, 16
    sw $s0, 0x3C($v1)
    sw $s1, 0x40($v1)
    sw $s5, 0x50($v1)
    lw $s0, 0x24($a0)
    lui $s1, %hi(func_00026F28 + 0x20)
    addiu $s1, $s1, %lo(func_00026F28 + 0x20)
  .L00027024:
    sw $s2, 0x44($v1)
    sw $s3, 0x48($v1)
    sw $s4, 0x4C($v1)
    sw $s6, 0x54($v1)
    sw $s7, 0x58($v1)
    lw $a3, 0x14($a0)
    lui $s6, %hi(texture_pages)
    addiu $s6, $s6, %lo(texture_pages)
    lui $s7, 0xff
    ori $s7, $s7, 0xffff
    addi $t1, $v1, 0x5C
  .L00027050:
    jalr $s1
    addi $t7, $t7, -0x1
    andi $s1, $s0, 0x1C
    add $s1, $s1, $v1
    lw $s1, 0x20($s1)
    bnez $t7, .L00027050
    srl $s0, $s0, 3
    lw $ra, 0x38($v1)
    lw $s0, 0x3C($v1)
    lw $s1, 0x40($v1)
    lw $s2, 0x44($v1)
    lw $s3, 0x48($v1)
    lw $s4, 0x4C($v1)
    lw $s5, 0x50($v1)
    lw $s6, 0x54($v1)
    lw $s7, 0x58($v1)
    jr $ra
    addu $v0, $a1, $zero
endlabel func_00026FF0

/* ---------------- func_00027098 (HANDWRITTEN ASM) ---------------- */
/* GTE renderer entry stub: save ra/s-regs in scratchpad, call func_000268D8, then jump into the loop at .L00027024 */
glabel func_00027098
    lui $v1, 0x1F80
    sw $ra, 0x38($v1)
    lw $t7, 0xC($a0)
    lw $t0, 0x10($a0)
    andi $t6, $t7, 0xFFFF
    jal func_000268D8
    srl $t7, $t7, 16
    sw $s0, 0x3C($v1)
    sw $s1, 0x40($v1)
    sw $s5, 0x50($v1)
    lw $s0, 0x28($a0)
    lui $s1, %hi(func_00026C38)
    addiu $s1, $s1, %lo(func_00026C38)
    j .L00027024
    addi $s5, $a0, 0x2C
endlabel func_00027098
