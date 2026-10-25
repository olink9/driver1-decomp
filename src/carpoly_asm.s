/* carpoly_asm.s: original assembler module, 2 functions func_0003DF94..func_0003E244 (0x3DF94-0x3E540).
 * GTE poly builders called from cars.c and camera.c (hand-scheduled rtpt/nclip/avsz3 pipelines, frames that
 * save registers they never use) - not compiler output. camera.c ends at 0x3DF94, denting.c starts at 0x3E540. */

.include "macro.inc"

.set noreorder

.section .text, "ax"

/* ---------------- func_0003DF94 (HANDWRITTEN ASM) ---------------- */
/* GTE poly builder: saves s5-fp at sp+0..0xC though they are never used, hand-scheduled t-register pipeline (rtpt/nclip/avsz3), loop counter in t5 - not compiler output */
glabel func_0003DF94
    addiu $sp, $sp, -0x30
    addu $t6, $a1, $zero
    lui $v1, 0x2400
    sw $s5, 0x0($sp)
    sw $s6, 0x4($sp)
    sw $s7, 0x8($sp)
    sw $fp, 0xC($sp)
    sw $s4, 0x28($sp)
    sw $s3, 0x24($sp)
    sw $s2, 0x20($sp)
    sw $s1, 0x1C($sp)
    sw $s0, 0x18($sp)
    lw $t0, 0x0($a3)
    lw $v0, 0x8($a3)
    lw $s1, 0x40($sp)
    lw $s4, 0x44($sp)
    lw $s2, 0x4($a3)
    or $s3, $v0, $v1
    addiu $v0, $zero, 0x1
    lw $v1, 0xC($a3)
    beq $s1, $v0, .L0003DFFC
    nop
    addiu $v0, $s1, -0x2
    sll $v0, $v0, 2
    j .L0003E000
    addu $t8, $v1, $v0
  .L0003DFFC:
    addiu $t8, $v1, 0x10
  .L0003E000:
    addu $t5, $a0, $zero
    lui $t7, 0x1f80
    lw $t7, 0x38($t7)
    blez $t5, .L0003E210
    nop
    lui $s0, 0x700
  .L0003E018:
    lw $v0, 0x0($t6)
    nop
    andi $a0, $v0, 0xFF
    sll $a0, $a0, 3
    addu $a0, $a2, $a0
    sra $v1, $v0, 5
    andi $v1, $v1, 0x7F8
    addu $v1, $a2, $v1
    sw $v0, 0x10($sp)
    sra $v0, $v0, 13
    andi $v0, $v0, 0x7F8
    addu $v0, $a2, $v0
    lwc2 $0, 0x0($a0)
    lwc2 $1, 0x4($a0)
    lwc2 $2, 0x0($v1)
    lwc2 $3, 0x4($v1)
    lwc2 $4, 0x0($v0)
    lwc2 $5, 0x4($v0)
    nop
    nop
    .word 0x4A280030
    lw $t2, 0x8($t6)
    lw $t3, 0xC($t6)
    beqz $s1, .L0003E098
    lw $t4, 0x10($t6)
    sra $v0, $t2, 16
    sll $v0, $v0, 2
    addu $v0, $v0, $t8
    lw $v0, 0x0($v0)
    andi $v1, $t2, 0xFFFF
    sll $v0, $v0, 16
    or $t2, $v1, $v0
  .L0003E098:
    lw $v0, 0x18($a3)
    lw $v1, 0x14($t6)
    .word 0x4B400006
    nop
    addu $v0, $v0, $v1
    lbu $v0, 0x0($v0)
    nop
    beqz $v0, .L0003E0C4
    addu $t2, $t2, $v0
    addu $t3, $t3, $v0
    addu $t4, $t4, $v0
  .L0003E0C4:
    swc2 $24, 0x10($sp)
    nop
    nop
    .word 0x4B58002D
    lw $v0, 0x10($sp)
    nop
    bltz $v0, .L0003E204
    nop
    swc2 $12, 0x8($t0)
    swc2 $13, 0x10($t0)
    swc2 $14, 0x18($t0)
    nop
    mfc2 $v0, $7
    nop
    sra $v0, $v0, 1
    sll $v0, $v0, 2
    addu $v0, $v0, $s2
    beqz $s4, .L0003E1CC
    sw $v0, 0x10($sp)
    swc2 $12, 0x28($t0)
    swc2 $13, 0x30($t0)
    swc2 $14, 0x38($t0)
    lw $v0, 0x1C($a3)
    lw $v1, 0x14($t6)
    nop
    addu $v0, $v0, $v1
    lbu $a1, 0x0($v0)
    lui $v1, 0x2600
    sll $a0, $a1, 16
    sll $v0, $a1, 8
    or $v0, $v0, $v1
    or $a0, $a0, $v0
    or $a0, $a0, $a1
    sw $a0, 0x4($t0)
    lw $a0, 0x4($t6)
    lw $v1, 0x14($a3)
    andi $v0, $a0, 0xFF
    sll $v0, $v0, 1
    addu $v0, $v0, $t7
    lhu $v0, 0x0($v0)
    nop
    or $v0, $v0, $v1
    sw $v0, 0xC($t0)
    sra $v0, $a0, 7
    andi $v0, $v0, 0x1FE
    addu $v0, $v0, $t7
    sra $a0, $a0, 15
    andi $a0, $a0, 0x1FE
    lhu $v0, 0x0($v0)
    lw $v1, 0x10($a3)
    addu $a0, $a0, $t7
    or $v0, $v0, $v1
    sw $v0, 0x14($t0)
    lhu $v0, 0x0($a0)
    nop
    sw $v0, 0x1C($t0)
    lw $v0, 0x10($sp)
    nop
    lw $v0, 0x0($v0)
    nop
    or $v0, $v0, $s0
    sw $v0, 0x0($t0)
    lw $v0, 0x10($sp)
    nop
    sw $t0, 0x0($v0)
    addiu $t0, $t0, 0x20
  .L0003E1CC:
    sw $s3, 0x4($t0)
    lw $v0, 0x10($sp)
    sw $t2, 0xC($t0)
    lw $v0, 0x0($v0)
    sw $t3, 0x14($t0)
    sw $t4, 0x1C($t0)
    or $v0, $v0, $s0
    sw $v0, 0x0($t0)
    lw $t3, 0x20($a3)
    lw $v0, 0x10($sp)
    bnez $t3, .L0003E200
    nop
    sw $t0, 0x0($v0)
  .L0003E200:
    addiu $t0, $t0, 0x20
  .L0003E204:
    addiu $t5, $t5, -0x1
    bgtz $t5, .L0003E018
    addiu $t6, $t6, 0x18
  .L0003E210:
    sw $t0, 0x0($a3)
    lw $s4, 0x28($sp)
    lw $s3, 0x24($sp)
    lw $s2, 0x20($sp)
    lw $s1, 0x1C($sp)
    lw $s0, 0x18($sp)
    lw $s5, 0x0($sp)
    lw $s6, 0x4($sp)
    lw $s7, 0x8($sp)
    lw $fp, 0xC($sp)
    addiu $sp, $sp, 0x30
    jr $ra
    nop
endlabel func_0003DF94

/* ---------------- func_0003E244 (HANDWRITTEN ASM) ---------------- */
glabel func_0003E244
    .set noat
    addiu $sp, $sp, -0x30
    lui $v1, 0x2c00
    sw $s5, 0x0($sp)
    sw $s6, 0x4($sp)
    sw $s7, 0x8($sp)
    sw $fp, 0xC($sp)
    sw $s4, 0x28($sp)
    sw $s3, 0x24($sp)
    sw $s2, 0x20($sp)
    sw $s1, 0x1C($sp)
    sw $s0, 0x18($sp)
    lw $t1, 0x0($a3)
    lw $v0, 0x8($a3)
    lw $t0, 0x40($sp)
    lw $s1, 0x44($sp)
    lw $s3, 0x4($a3)
    or $s2, $v0, $v1
    lw $v1, 0xC($a3)
    addiu $v0, $zero, 0x1
    beq $t0, $v0, .L0003E2A8
    addu $t8, $a1, $zero
    addiu $v0, $t0, -0x2
    sll $v0, $v0, 2
    j .L0003E2AC
    addu $t9, $v1, $v0
  .L0003E2A8:
    addiu $t9, $v1, 0x10
  .L0003E2AC:
    bnez $t0, .L0003E2B8
    lui $t6, 0x1f80
    addu $t9, $zero, $zero
  .L0003E2B8:
    lw $t6, 0x38($t6)
    blez $a0, .L0003E50C
    nop
    lui $s0, 0x900
    nop
    lw $at, 0x0($t8)
    nop
    andi $s4, $at, 0xFF
    sll $s4, $s4, 3
    addu $s4, $a2, $s4
  .L0003E2E0:
    lw $v0, 0x0($s4)
    sra $s5, $at, 5
    andi $s5, $s5, 0x7F8
    addu $s5, $a2, $s5
    lw $s4, 0x4($s4)
    sra $s6, $at, 13
    andi $s6, $s6, 0x7F8
    addu $s6, $a2, $s6
    lw $v1, 0x0($s5)
    sra $s7, $at, 21
    andi $s7, $s7, 0x7F8
    addu $s7, $a2, $s7
    lw $s5, 0x4($s5)
    lw $t7, 0x0($s6)
    mtc2 $v0, $0
    mtc2 $s4, $1
    mtc2 $v1, $2
    lw $s6, 0x4($s6)
    mtc2 $s5, $3
    mtc2 $t7, $4
    mtc2 $s6, $5
    nop
    nop
    rtpt
    lw $at, 0x18($t8)
    lw $t3, 0x8($t8)
    lw $t5, 0xC($t8)
    andi $s4, $at, 0xFF
    sll $s4, $s4, 3
    addu $s4, $a2, $s4
    lw $t4, 0x10($t8)
    nclip
    swc2 $12, 0x8($t1)
    swc2 $13, 0x10($t1)
    swc2 $14, 0x18($t1)
    mfc2 $t7, $24
    beqz $s1, .L0003E384
    addiu $v0, $t1, 0x28
    swc2 $12, 0x8($v0)
    swc2 $13, 0x10($v0)
    swc2 $14, 0x18($v0)
  .L0003E384:
    bltz $t7, .L0003E500
    nop
    lwc2 $0, 0x0($s7)
    lwc2 $1, 0x4($s7)
    nop
    nop
    rtps
    beqz $t9, .L0003E3C0
    sra $v0, $t3, 16
    sll $v0, $v0, 2
    addu $v0, $v0, $t9
    lw $v0, 0x0($v0)
    andi $v1, $t3, 0xFFFF
    sll $v0, $v0, 16
    or $t3, $v1, $v0
  .L0003E3C0:
    lw $v0, 0x18($a3)
    lw $v1, 0x14($t8)
    nop
    addu $v0, $v0, $v1
    lbu $v1, 0x0($v0)
    nop
    beqz $v1, .L0003E3F0
    sll $v0, $v1, 16
    addu $t3, $t3, $v1
    addu $t5, $t5, $v1
    addu $v0, $v0, $v1
    addu $t4, $t4, $v0
  .L0003E3F0:
    avsz4
    addiu $v0, $t1, 0x20
    swc2 $14, 0x0($v0)
    mfc2 $v0, $7
    nop
    sra $v0, $v0, 1
    sll $v0, $v0, 2
    beqz $s1, .L0003E4CC
    addu $s5, $v0, $s3
    addiu $v0, $t1, 0x48
    swc2 $14, 0x0($v0)
    lw $v0, 0x1C($a3)
    lw $v1, 0x14($t8)
    nop
    addu $v0, $v0, $v1
    lbu $a1, 0x0($v0)
    lui $v1, 0x2e00
    sll $t7, $a1, 16
    sll $v0, $a1, 8
    or $v0, $v0, $v1
    or $t7, $t7, $v0
    or $t7, $t7, $a1
    sw $t7, 0x4($t1)
    lw $t7, 0x4($t8)
    lw $v1, 0x14($a3)
    andi $v0, $t7, 0xFF
    sll $v0, $v0, 1
    addu $v0, $v0, $t6
    lhu $v0, 0x0($v0)
    sra $t0, $t7, 7
    andi $t0, $t0, 0x1FE
    addu $t0, $t0, $t6
    lhu $t0, 0x0($t0)
    lw $t2, 0x10($a3)
    or $v0, $v0, $v1
    sw $v0, 0xC($t1)
    or $t0, $t0, $t2
    sw $t0, 0x14($t1)
    sra $v0, $t7, 15
    andi $v0, $v0, 0x1FE
    addu $v0, $v0, $t6
    srl $t7, $t7, 24
    sll $t7, $t7, 1
    lhu $v0, 0x0($v0)
    addu $t7, $t7, $t6
    sw $v0, 0x1C($t1)
    lhu $v0, 0x0($t7)
    nop
    sw $v0, 0x24($t1)
    lw $v0, 0x0($s5)
    nop
    or $v0, $v0, $s0
    sw $v0, 0x0($t1)
    sw $t1, 0x0($s5)
    addiu $t1, $t1, 0x28
  .L0003E4CC:
    lw $v0, 0x0($s5)
    srl $v1, $t4, 16
    or $v0, $v0, $s0
    sw $s2, 0x4($t1)
    sw $t3, 0xC($t1)
    sw $t5, 0x14($t1)
    lw $t3, 0x20($a3)
    sw $t4, 0x1C($t1)
    sw $v1, 0x24($t1)
    bnez $t3, .L0003E4FC
    sw $v0, 0x0($t1)
    sw $t1, 0x0($s5)
  .L0003E4FC:
    addiu $t1, $t1, 0x28
  .L0003E500:
    addiu $a0, $a0, -0x1
    bgtz $a0, .L0003E2E0
    addiu $t8, $t8, 0x18
  .L0003E50C:
    sw $t1, 0x0($a3)
    lw $s4, 0x28($sp)
    lw $s3, 0x24($sp)
    lw $s2, 0x20($sp)
    lw $s1, 0x1C($sp)
    lw $s0, 0x18($sp)
    lw $s5, 0x0($sp)
    lw $s6, 0x4($sp)
    lw $s7, 0x8($sp)
    lw $fp, 0xC($sp)
    addiu $sp, $sp, 0x30
    jr $ra
    nop
    .purgem XBEQ
    .purgem XBEQZ
    .purgem XBGTZ
    .purgem XBLEZ
    .purgem XBLTZ
    .purgem XBNEZ
    .purgem XJ
    .set at
endlabel func_0003E244
