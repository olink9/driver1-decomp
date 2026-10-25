/* gte_asm.s: original assembler module, 8 functions func_0002C53C..func_0002C6DC (0x2C53C-0x2C8FC).
 * Small GTE/matrix helpers and a 32-byte block copy, called from draw.c, quad.c, cars.c, debris.c, models.c,
 * civ_draw.c, overmap.c, job_fx.c and glaunch.c; non-ABI register conventions, not compiler output. It sits
 * inside draw.c's range in the plan: quad.c (0x2BDB0-0x2C53C) before it, draw.c after it. */

.include "macro.inc"

.set noreorder

.section .text, "ax"

/* ---------------- func_0002C53C (HANDWRITTEN ASM) ---------------- */
glabel func_0002C53C
    lui $8, %hi(camera_position)
    lw $8, %lo(camera_position)($8)
    lui $9, %hi(D_0008A8F4)
    lw $9, %lo(D_0008A8F4)($9)
    lui $10, %hi(D_0008A8F8)
    lw $10, %lo(D_0008A8F8)($10)
    ctc2 $8, $21
    ctc2 $9, $22
    jr $31
    ctc2 $10, $23
endlabel func_0002C53C

/* ---------------- func_0002C564 (HANDWRITTEN ASM) ---------------- */
glabel func_0002C564
    lwc2 $9, 0($4)
    lwc2 $10, 4($4)
    lwc2 $11, 8($4)
    nop
    nop
    mvmva 1, 2, 3, 3, 0
    nop
    nop
    swc2 $9, 0($4)
    swc2 $10, 4($4)
    swc2 $11, 8($4)
    jr $31
    nop
endlabel func_0002C564

/* ---------------- func_0002C598 (HANDWRITTEN ASM) ---------------- */
/* 32-byte block copy: copies a2 bytes (multiple of 32) from a1 to a0; addi (trapping) and self-branch, no prologue */
glabel func_0002C598
    lw $t0, 0x0($a1)
    lw $t1, 0x4($a1)
    lw $t2, 0x8($a1)
    lw $t3, 0xC($a1)
    lw $t4, 0x10($a1)
    lw $t5, 0x14($a1)
    lw $t6, 0x18($a1)
    lw $t7, 0x1C($a1)
    sw $t0, 0x0($a0)
    sw $t1, 0x4($a0)
    sw $t2, 0x8($a0)
    sw $t3, 0xC($a0)
    sw $t4, 0x10($a0)
    sw $t5, 0x14($a0)
    sw $t6, 0x18($a0)
    sw $t7, 0x1C($a0)
    addi $a1, $a1, 0x20
    addi $a2, $a2, -0x20
    bgtz $a2, func_0002C598
    addi $a0, $a0, 0x20
    jr $ra
    nop
endlabel func_0002C598

/* ---------------- func_0002C5F0 (HANDWRITTEN ASM) ---------------- */
glabel func_0002C5F0
    lw $8, 0($4)
    lw $9, 4($4)
    lw $10, 8($4)
    cfc2 $11, $21
    cfc2 $12, $22
    cfc2 $13, $23
    sub $11, $8, $11
    sub $12, $9, $12
    sub $13, $10, $13
    sra $4, $11, 1
    sra $6, $12, 1
    sra $7, $13, 1
    mtc2 $4, $9
    mtc2 $6, $10
    mtc2 $7, $11
    lui $24, %hi(D_000A5E1C)
    addiu $24, $24, %lo(D_000A5E1C)
    mvmva 1, 1, 3, 3, 0
    lw $24, 0x14($24)
    sra $5, $5, 1
    sub $5, $0, $5
    mfc2 $4, $25
    mfc2 $6, $26
    add $5, $5, $24
    slt $2, $4, $5
    bnez $2, 1f
    addiu $2, $0, -1
    mfc2 $4, $27
    slt $2, $6, $5
    bnez $2, 1f
    addiu $2, $0, -1
    slt $2, $4, $5
    bnez $2, 1f
    addiu $2, $0, -1
    jr $31
    addu $2, $0, $0
    1:
    jr $31
    nop
endlabel func_0002C5F0

/* ---------------- func_0002C688 (HANDWRITTEN ASM) ---------------- */
glabel func_0002C688
    swc2 $12, 0($2)
    swc2 $13, 4($2)
    swc2 $14, 8($2)
endlabel func_0002C688

/* ---------------- func_0002C694 (HANDWRITTEN ASM) ---------------- */
glabel func_0002C694
    lwc2 $0, 0($3)
    lwc2 $1, 4($3)
    lwc2 $2, 8($3)
    lwc2 $3, 12($3)
    lwc2 $4, 16($3)
    jr $31
    lwc2 $5, 20($3)
endlabel func_0002C694

/* ---------------- func_0002C6B0 (HANDWRITTEN ASM) ---------------- */
/* table lookup helper: angle in $t5, table base in $t6 -> $t8 = tbl[0x100/2 + ...], $t9 = tbl[...] (non-ABI registers) */
glabel func_0002C6B0
    srl $25, $13, 2
    andi $25, $25, 0x3FC0
    srl $24, $13, 15
    andi $24, $24, 0xFE
    add $24, $24, $25
    add $24, $24, $14
    lhu $24, 0x100($24)
    srl $25, $25, 5
    add $25, $25, $14
    jr $31
    lhu $25, 0($25)
endlabel func_0002C6B0

/* ---------------- func_0002C6DC (HANDWRITTEN ASM) ---------------- */
glabel func_0002C6DC
    mtc2       $t3, $9
    mtc2       $t4, $10
    mtc2       $t5, $11
    lui        $t3, 0xFF00
    lui        $v0, 0x1F80
    mvmva      1, 2, 3, 3, 0
    mtc2       $ra, $23
    lw         $v1, 0x10($a0)
    lw         $t0, 0x1F0($v0)
    mfc2       $t4, $9
    mfc2       $t2, $10
    ctc2       $t4, $5
    mfc2       $t4, $11
    ctc2       $t2, $6
    addiu      $at, $zero, 0x64
    sub        $at, $t4, $at
    bgtz $at, .L0002C730
    nop
    ctc2       $t4, $7
    jr $ra
    addu      $v0, $a1, $zero
  .L0002C730:
    jal func_0002C694
    ctc2      $t4, $7
    not        $t5, $t3
    srl        $at, $t4, 1
    rtpt
    lw         $t7, 0xC($a0)
    addi       $a3, $a3, -0x30
    bnez $a3, .L0002C758
    mtc2      $t0, $21
    addiu      $at, $zero, 0x3E68
  .L0002C758:
    andi       $at, $at, 0xFFFC
    add        $a2, $a2, $at
    lw         $at, 0x0($a2)
    lw         $a3, 0x14($a0)
    lui        $t9, 0x0900
    and        $t6, $t3, $at
    and        $t8, $t5, $at
    andi       $t7, $t7, 0x4
    beqz $t7, .L0002C810
    or        $t7, $t8, $t9
    mfc2       $t0, $12
    mfc2       $t1, $13
    mfc2       $t2, $14
    lwc2       $0, 0x18($v1)
    lwc2       $1, 0x1C($v1)
    and        $at, $t5, $a1
    or         $t6, $t6, $at
    rtps
    sw         $t6, 0x0($a2)
  .L0002C7A4:
    lui        $t6, %hi(texture_pages)
    addiu      $t6, $t6, %lo(texture_pages)
    lw         $t5, 0x0($a3)
    jal func_0002C6B0
    lw        $t4, 0x8($a3)
    lw         $t5, 0xC($a3)
    sll        $t8, $t8, 16
    sll        $t9, $t9, 16
    srl        $at, $t5, 16
    andi       $t6, $t5, 0xFFFF
    or         $t8, $t8, $at
    or         $t9, $t9, $t6
    srl        $t6, $t4, 16
    sw         $t7, 0x0($a1)
    swc2       $21, 0x4($a1)
    sw         $t0, 0x18($a1)
    sw         $t8, 0xC($a1)
    sw         $t1, 0x20($a1)
    sw         $t9, 0x14($a1)
    swc2       $14, 0x8($a1)
    sw         $t4, 0x1C($a1)
    sw         $t2, 0x10($a1)
    sw         $t6, 0x24($a1)
    mfc2       $ra, $23
    addi       $v0, $a1, 0x28
    jr $ra
    nop
  .L0002C810:
    mfc2       $t0, $12
    mfc2       $t1, $13
    mfc2       $t2, $14
    lwc2       $0, 0x20($v1)
    lwc2       $1, 0x24($v1)
    lwc2       $2, 0x28($v1)
    lwc2       $3, 0x2C($v1)
    lwc2       $4, 0x30($v1)
    lwc2       $5, 0x34($v1)
    sw         $s0, 0x0($v0)
    sw         $s1, 0x4($v0)
    rtpt
    sw         $t6, 0xC($v0)
    lui        $t6, %hi(texture_pages)
    addiu      $t6, $t6, %lo(texture_pages)
    lw         $t5, 0x14($a3)
    jal func_0002C6B0
    sw        $s2, 0x8($v0)
    mfc2       $s0, $12
    mfc2       $s1, $13
    mfc2       $s2, $14
    lwc2       $4, 0x18($v1)
    lwc2       $5, 0x1C($v1)
    lwc2       $0, 0x38($v1)
    lwc2       $1, 0x3C($v1)
    sll        $t8, $t8, 16
    sll        $t9, $t9, 16
    rtpt
    lw         $t3, 0x1C($a3)
    lw         $t6, 0x20($a3)
    srl        $at, $t3, 16
    andi       $t3, $t3, 0xFFFF
    or         $t8, $t8, $at
    or         $t9, $t9, $t3
    srl        $t4, $t6, 16
    sw         $t7, 0x0($a1)
    swc2       $21, 0x4($a1)
    sw         $s0, 0x10($a1)
    sw         $t8, 0xC($a1)
    sw         $s1, 0x8($a1)
    sw         $t9, 0x14($a1)
    swc2       $12, 0x20($a1)
    sw         $t6, 0x1C($a1)
    sw         $s2, 0x18($a1)
    sw         $t4, 0x24($a1)
    lui        $s0, 0x00FF
    ori        $s0, $s0, 0xFFFF
    and        $at, $s0, $a1
    lui        $s2, 0x0900
    or         $t7, $at, $s2
    addi       $a1, $a1, 0x28
    lw         $s1, 0xC($v0)
    and        $t6, $s0, $a1
    or         $t6, $t6, $s1
    sw         $t6, 0x0($a2)
    lw         $s0, 0x0($v0)
    lw         $s1, 0x4($v0)
    j .L0002C7A4
    lw        $s2, 0x8($v0)
endlabel func_0002C6DC
