/* mdraw_asm.s: original assembler module (the plan's mdraw.s), 25 functions func_00030380..func_00031734
 * (0x30380-0x317B0). The model/primitive renderer: GTE prim emitters that run on the caller's registers, jump
 * into each other and keep state in t/s registers - not compiler output. func_00031048 (an empty jr ra; nop)
 * has no HANDWRITTEN tag because its pilot matched as an empty C function; it is part of this module all the
 * same. draw.c ends at 0x30380, models.c starts at 0x317B0. */

.include "macro.inc"

.set noreorder

.section .text, "ax"

/* ---------------- func_00030380 (HANDWRITTEN ASM) ---------------- */
/* GTE prim emitter fragment: no prologue, uses caller's v1/t1/t2/s2-s4 directly, `add`/`addi` (never compiler output) */
glabel func_00030380
    lw $12, 0x78($3)
    nop
    lw $13, 0x0($12)
    srl $18, $10, 6
    andi $18, $18, 0x1FC
    add $18, $18, $3
    lw $18, 0x1F0($18)
    sw $2, 0x0($12)
    srl $19, $10, 14
    andi $19, $19, 0x1FC
    add $19, $19, $3
    lw $19, 0x1F0($19)
    srl $20, $10, 22
    andi $20, $20, 0x1FC
    add $20, $20, $3
    lw $20, 0x1F0($20)
    lui $8, 0x400
    sw $9, 0x4($2)
    or $8, $8, $13
    sw $18, 0x8($2)
    addi $2, $2, 0x14
    sw $19, -0x8($2)
    addi $5, $5, 0x8
    sw $20, -0x4($2)
    jr $31
    sw $8, -0x14($2)
endlabel func_00030380

/* ---------------- func_000303E8 (HANDWRITTEN ASM) ---------------- */
/* GTE prim emitter fragment (sibling of func_00030380): no prologue, caller's v1/t1/s2-s4, `add`/`addi` */
glabel func_000303E8
    lw $12, 0x78($3)
    lw $10, 0x8($5)
    lw $13, 0x0($12)
    srl $18, $9, 6
    andi $18, $18, 0x1FC
    add $18, $18, $3
    lw $18, 0x1F0($18)
    srl $19, $9, 14
    andi $19, $19, 0x1FC
    add $19, $19, $3
    lw $19, 0x1F0($19)
    sw $2, 0x0($12)
    srl $20, $9, 22
    andi $20, $20, 0x1FC
    add $20, $20, $3
    lw $20, 0x1F0($20)
    sll $9, $9, 2
    andi $9, $9, 0x1FC
    add $9, $9, $3
    lw $9, 0x1F0($9)
    sw $10, 0x4($2)
    lui $8, 0x500
    sw $9, 0x8($2)
    or $8, $8, $13
    sw $18, 0xC($2)
    addi $5, $5, 0xC
    sw $20, 0x10($2)
    addi $2, $2, 0x18
    sw $19, -0x4($2)
    jr $31
    sw $8, -0x18($2)
endlabel func_000303E8

/* ---------------- func_00030464 (HANDWRITTEN ASM) ---------------- */
glabel func_00030464
    ori $8, $0, 0x8080
    mtc2 $8, $23
    j .L00030520
    nop
endlabel func_00030464

/* ---------------- func_00030474 (HANDWRITTEN ASM) ---------------- */
/* GTE prim helper (inputs in t0-t3/s5/s6/v1/a1); .L00030520 is a second entry point used by func_00030464 / func_000307E8 (j) and DrawMapPSX (address taken) */
glabel func_00030474
    srl $t4, $t2, 2
    andi $t4, $t4, 0x3FC0
    srl $t5, $t2, 15
    andi $t5, $t5, 0xFE
    add $t5, $t5, $t4
    add $t5, $t5, $s6
    srl $t4, $t4, 5
    add $t4, $t4, $s6
    lhu $t4, 0x0($t4)
    lhu $t5, 0x100($t5)
    andi $at, $s5, 0xF
    beqz $at, .L000304C0
    nop
    lw $s3, 0x78($v1)
    lw $t2, 0x8($a1)
    lw $t3, 0xC($a1)
    lw $at, 0x0($s3)
    j .L000304D0
    sw $v0, 0x0($s3)
  .L000304C0:
    lw $at, 0x0($s5)
    lw $t2, 0x8($a1)
    lw $t3, 0xC($a1)
    sw $v0, 0x0($s5)
  .L000304D0:
    sll $t5, $t5, 16
    or $t0, $t0, $at
    sll $t4, $t4, 16
    andi $at, $t2, 0xFFFF
    or $t5, $t5, $at
    srl $t2, $t2, 16
    or $t4, $t4, $t2
    sll $s2, $t1, 2
    andi $s2, $s2, 0x3FC
    add $s2, $s2, $v1
    lw $s2, 0x1F0($s2)
    srl $s3, $t1, 6
    andi $s3, $s3, 0x3FC
    add $s3, $s3, $v1
    lw $s3, 0x1F0($s3)
    srl $s4, $t1, 14
    andi $s4, $s4, 0x3FC
    add $s4, $s4, $v1
    jr $ra
    lw $s4, 0x1F0($s4)
  .L00030520:
    sw $ra, 0x7C($v1)
    jal func_00030474
    lui $t0, 0x700
    srl $t2, $t3, 16
    sw $t0, 0x0($v0)
    lw $ra, 0x7C($v1)
    sw $s2, 0x8($v0)
    lui $at, 0x2400
    sw $t5, 0xC($v0)
    lw $t2, 0xC($v1)
    sw $s3, 0x10($v0)
    or $at, $at, $t2
    sw $t4, 0x14($v0)
    addi $v0, $v0, 0x20
    sw $s4, -0x8($v0)
    addi $a1, $a1, 0x10
    sw $t3, -0x4($v0)
    jr $ra
    sw $at, -0x1C($v0)
endlabel func_00030474

/* ---------------- func_0003056C (HANDWRITTEN ASM) ---------------- */
/* GTE prim-builder fragment: uses scratchpad base in v1 from the caller (no prologue), saves ra at 0x7C(v1),
 * calls func_00030474, then writes a 10-word primitive at v0 (codes 0x09000000 / 0x2C000000) */
glabel func_0003056C
    sw $ra, 0x7C($v1)
    jal func_00030474
    lui $t0, 0x900
    srl $t2, $t3, 16
    lw $ra, 0x7C($v1)
    srl $t1, $t1, 22
    sw $t0, 0x0($v0)
    andi $t1, $t1, 0x3FC
    sw $s2, 0x8($v0)
    add $t1, $t1, $v1
    sw $t5, 0xC($v0)
    lw $t1, 0x1F0($t1)
    sw $s3, 0x10($v0)
    lui $at, 0x2C00
    sw $t4, 0x14($v0)
    lw $t0, 0xC($v1)
    sw $t1, 0x18($v0)
    or $t0, $t0, $at
    sw $t2, 0x1C($v0)
    addi $a1, $a1, 0x10
    sw $s4, 0x20($v0)
    addi $v0, $v0, 0x28
    sw $t3, -0x4($v0)
    jr $ra
    sw $t0, -0x24($v0)
endlabel func_0003056C

/* ---------------- func_000305D0 (HANDWRITTEN ASM) ---------------- */
glabel func_000305D0
    srl $t4, $t1, 5
    andi $t4, $t4, 0x7F8
    srl $t9, $t1, 13
    andi $t9, $t9, 0x7F8
    srl $t8, $t1, 21
    andi $t8, $t8, 0x7F8
    sll $t1, $t1, 3
    andi $t1, $t1, 0x7F8
    add $t1, $t1, $t3
    add $t4, $t4, $t3
    add $t9, $t9, $t3
    add $t8, $t8, $t3
    srl $at, $t0, 16
    srl $t3, $t2, 16
    andi $t0, $t0, 0xFEFF
    andi $at, $at, 0xFEFF
    andi $t2, $t2, 0xFEFF
    andi $t3, $t3, 0xFEFF
    sh $t0, 0xB0($v1)
    sh $at, 0xB4($v1)
    sh $t3, 0xBC($v1)
    sh $t2, 0xC0($v1)
    lh $t3, 0x4($t1)
    lw $t1, 0x0($t1)
    lh $t5, 0x4($t4)
    lw $t4, 0x0($t4)
    lh $at, 0x4($t9)
    lw $t9, 0x0($t9)
    sw $t1, 0xE4($v1)
    sh $t3, 0xE8($v1)
    sw $t4, 0xF4($v1)
    sh $t5, 0xF8($v1)
    sh $at, 0x128($v1)
    sw $t9, 0x124($v1)
    lw $at, 0x0($t8)
    lw $t9, 0x4($t8)
    lui $t4, 0x10
    ori $t4, $t4, 0xd04
    lui $t5, 0x8
    ori $t5, $t5, 0x6ea2
    sra $t2, $t1, 16
    sll $t1, $t1, 16
    sra $t1, $t1, 16
    sw $at, 0x114($v1)
    sw $t9, 0x118($v1)
    nop
    nop
    addiu $at, $zero, 0x5
  .L00030690:
    andi $s3, $t4, 0x1E
    add $t8, $s3, $v1
    lhu $t8, 0xB0($t8)
    andi $s2, $t5, 0x1E
    add $t0, $t0, $t8
    srl $t0, $t0, 1
    add $t9, $s2, $v1
    sh $t0, 0xB0($t9)
    addu $t0, $t8, $zero
    sll $s3, $s3, 2
    sll $s2, $s2, 2
    add $s3, $s3, $v1
    add $s2, $s2, $v1
    lh $t8, 0xE4($s3)
    lh $t9, 0xE6($s3)
    add $t1, $t1, $t8
    sra $t1, $t1, 1
    sh $t1, 0xE4($s2)
    addu $t1, $t8, $zero
    lh $t8, 0xE8($s3)
    add $t2, $t2, $t9
    sra $t2, $t2, 1
    sh $t2, 0xE6($s2)
    addu $t2, $t9, $zero
    add $t3, $t3, $t8
    sra $t3, $t3, 1
    sh $t3, 0xE8($s2)
    addu $t3, $t8, $zero
    srl $t4, $t4, 4
    addiu $at, $at, -0x1
    bgtz $at, .L00030690
    srl $t5, $t5, 4
    jr $ra
    nop
endlabel func_000305D0

/* ---------------- func_00030718 (HANDWRITTEN ASM) ---------------- */
glabel func_00030718
    sw $24, 0x64($3)
    sw $25, 0x68($3)
    sw $17, 0x6C($3)
    sw $14, 0x70($3)
    sw $31, 0x7C($3)
    jal func_00030BF0
    lw $11, 0x74($3)
    sll $13, $13, 16
    mtc2 $13, $20
    sll $12, $12, 16
    lw $8, 0x8($5)
    lw $10, 0xC($5)
    jal func_000305D0
    mtc2 $12, $21
    lw $10, 0xC($3)
    lui $1, (0x2C000000 >> 16)
    or $10, $10, $1
    addiu $8, $0, 0x4
    lui $9, %hi(D_00089444)
    addiu $9, $9, %lo(D_00089444)
    lw $11, 0x0($21)
    addiu $12, $0, 0x3
    addiu $19, $0, 0x0
    addi $18, $3, 0xE4
  .L00030778:
    lwc2 $0, 0x0($18)
    lwc2 $1, 0x4($18)
    lwc2 $2, 0x8($18)
    lwc2 $3, 0xC($18)
    lwc2 $4, 0x10($18)
    lwc2 $5, 0x14($18)
    nop
    nop
    rtpt
    nop
    swc2 $12, 0x0($18)
    swc2 $16, 0x4($18)
    swc2 $13, 0x8($18)
    swc2 $17, 0xC($18)
    swc2 $14, 0x10($18)
    swc2 $18, 0x14($18)
    addi $12, $12, -0x1
    bgtz $12, .L00030778
    addi $18, $18, 0x18
    jal func_00030C20
    nop
    lw $31, 0x7C($3)
    lw $24, 0x64($3)
    lw $25, 0x68($3)
    lw $17, 0x6C($3)
    lw $14, 0x70($3)
    jr $31
    addi $5, $5, 0x10
endlabel func_00030718

/* ---------------- func_000307E8 (HANDWRITTEN ASM) ---------------- */
glabel func_000307E8
    lui $8, 0x40
    ori $8, $8, 0x8080
    mtc2 $8, $23
    j .L00030520
    nop
endlabel func_000307E8

/* ---------------- func_000307FC (HANDWRITTEN ASM) ---------------- */
glabel func_000307FC
    lw $t0, 0x0($a0)
    lw $t1, 0x4($a0)
    lw $t2, 0x8($a0)
    lw $t3, 0xC($a0)
    cfc2 $t4, $21
    cfc2 $t5, $22
    cfc2 $t6, $23
    sub $t0, $t0, $t4
    sub $t1, $t1, $t5
    sub $t2, $t2, $t6
    mtc2 $t0, $9
    mtc2 $t1, $10
    mtc2 $t2, $11
    srl $t9, $t3, 3
    andi $t9, $t9, 0x7E0
    .word 0x4A4DE012 /* mvmva 1, 2, 3, 3, 0 */
    lui $a0, %hi(D_000A668C)
    addiu $a0, $a0, %lo(D_000A668C)
    add $a0, $a0, $t9
    lw $t4, 0x0($a0)
    lw $t5, 0x4($a0)
    lw $t6, 0x8($a0)
    lw $t7, 0xC($a0)
    lw $t8, 0x10($a0)
    mfc2 $at, $9
    mfc2 $v1, $10
    mfc2 $v0, $11
    ctc2 $at, $5
    ctc2 $v1, $6
    ctc2 $v0, $7
    ctc2 $t4, $8
    ctc2 $t5, $9
    ctc2 $t6, $10
    ctc2 $t7, $11
    ctc2 $t8, $12
    mtc2 $t0, $9
    mtc2 $t1, $10
    mtc2 $t2, $11
    lui $a0, %hi(CompoundMatrix)
    addiu $a0, $a0, %lo(CompoundMatrix)
    .word 0x4A4BE012 /* mvmva 1, 1, 3, 3, 0 */
    add $a0, $a0, $t9
    lw $t0, 0x0($a0)
    lw $t1, 0x4($a0)
    lw $t2, 0x8($a0)
    lw $t5, 0xC($a0)
    lhu $t6, 0x10($a0)
    ctc2 $t0, $0
    ctc2 $t1, $1
    ctc2 $t2, $2
    ctc2 $t5, $3
    mfc2 $t0, $9
    mfc2 $t1, $10
    andi $t0, $t0, 0xFFFF
    ctc2 $t6, $4
    sll $t1, $t1, 16
    or $t0, $t0, $t1
    mfc2 $t1, $11
    srl $t3, $t3, 14
    andi $t3, $t3, 0xFFFC
    lui $a0, %hi(modelpointers)
    addiu $a0, $a0, %lo(modelpointers)
    add $a0, $a0, $t3
    lw $a0, 0x0($a0)
    lui $v1, (0x1F800044 >> 16)
    mtc2 $t0, $0
    mtc2 $t1, $1
    srl $t9, $t9, 6
    sw $t9, (0x1F800008 & 0xFFFF)($v1)
    lhu $t8, 0x8($a0)
    lhu $a3, 0x0($a0)
    sw $t0, (0x1F800010 & 0xFFFF)($v1)
    sw $t1, (0x1F800014 & 0xFFFF)($v1)
    sub $t8, $v0, $t8
    addiu $t7, $zero, 0x2328
    slt $at, $t8, $t7
    bnez $at, .L0003093C
    sra $v0, $v0, 1
    j .L00030954
    addiu $t6, $zero, 0x0
  .L0003093C:
    addiu $t7, $zero, 0x1770
    slt $at, $t8, $t7
    bnez $at, .L00030954
    addiu $t6, $zero, 0x20
    j .L00030954
    addiu $t6, $zero, 0x10
  .L00030954:
    sw $t6, (0x1F800044 & 0xFFFF)($v1)
    addiu $t7, $zero, 0x80
    slt $at, $v0, $t7
    beqz $at, .L00030970
    nop
    j .L00030984
    addu $v0, $t7, $zero
  .L00030970:
    addiu $t7, $zero, 0x3FFC
    slt $at, $t7, $v0
    beqz $at, .L00030984
    andi $v0, $v0, 0xFFFC
    addu $v0, $t7, $zero
  .L00030984:
    addi $t9, $v0, 0x1000
    addiu $t7, $zero, 0x3FFC
    slt $at, $t7, $t9
    beqz $at, .L0003099C
    andi $t9, $t9, 0xFFFC
    addu $t9, $t7, $zero
  .L0003099C:
    add $t9, $t9, $a2
    lw $at, 0x0($t9)
    sw $t9, (0x1F8001EC & 0xFFFF)($v1)
    sw $at, (0x1F800000 & 0xFFFF)($v1)
    add $a2, $a2, $v0
    andi $a3, $a3, 0x8000
    bnez $a3, .L000309CC
    addu $v0, $a1, $zero
    beqz $t6, .L000309CC
    nop
    bgez $zero, .L000309D0
    lbu $t7, 0x6($a0)
  .L000309CC:
    lhu $t7, 0xC($a0)
  .L000309D0:
    lw $a3, 0x1C($a0)
    lw $at, 0x0($a2)
    lw $a1, 0x14($a0)
    lw $t9, 0x0($a3)
    lw $a0, 0x10($a0)
    addi $a3, $a3, 0x4
    srl $t8, $t9, 8
    andi $t8, $t8, 0xFF
    sw $a2, (0x1F800078 & 0xFFFF)($v1)
    srl $a2, $t9, 16
    sw $ra, (0x1F800048 & 0xFFFF)($v1)
    sw $a0, (0x1F800074 & 0xFFFF)($v1)
    jal func_000310B8
    add $a2, $a2, $a3
    sw $at, 0x4($v1)
    andi $t9, $t9, 0xFF
    sw $s0, 0x4C($v1)
    sw $s1, 0x50($v1)
    sw $s2, 0x54($v1)
    addi $s0, $v1, 0x1F0
    addi $s5, $v1, 0x0
    lui $s6, %hi(texture_pages)
    addiu $s6, $s6, %lo(texture_pages)
  .L00030A2C:
    addi $s1, $v1, 0x18
    mfc2 $t0, $25
    swc2 $26, 0x0($s1)
    swc2 $27, 0x4($s1)
    bgezal $t7, func_00031094
    nop
  .L00030A44:
    bgezal $t7, func_00031074
    nop
    bgez $t7, .L00030A44
    nop
    addiu $s7, $zero, 0x3
  .L00030A58:
    lw $t1, 0x0($a2)
    bgtz $t0, .L00030A8C
    addi $a2, $a2, 0x4
    srl $at, $t1, 8
    andi $at, $at, 0xFF
    add $a2, $a2, $at
    srl $t1, $t1, 16
    andi $at, $t1, 0x8000
    beqz $at, .L00030A84
    andi $t1, $t1, 0x3FF
    addi $s5, $s5, 0x4
  .L00030A84:
    j .L00030B98
    add $a1, $a1, $t1
  .L00030A8C:
    lw $t2, 0x0($a2)
    srl $at, $t1, 16
    andi $t3, $at, 0x8000
    beqz $t3, .L00030AA4
    andi $t6, $t1, 0xFF
    addi $s5, $s5, 0x4
  .L00030AA4:
    andi $t3, $at, 0x4000
    bnez $t3, .L00030ABC
    srl $at, $at, 8
    lw $t3, 0x8($v1)
    nop
    add $at, $at, $t3
  .L00030ABC:
    andi $at, $at, 0x1C
    add $at, $at, $v1
    lw $t3, 0x24($at)
    nop
    sw $t3, 0xC($v1)
  .L00030AD0:
    sll $at, $t2, 16
    sra $at, $at, 17
    add $at, $t0, $at
    bgtz $at, .L00030B08
    andi $t3, $t2, 0x1F
    srl $t2, $t2, 14
    andi $t1, $t2, 0x3FC
    sub $t6, $t6, $t3
    addi $a2, $a2, 0x4
    lw $t2, 0x0($a2)
    bgtz $t6, .L00030AD0
    add $a1, $a1, $t1
  .L00030B00:
    j .L00030B00
    .word 0x0001000D /* break 1 */
  .L00030B08:
    lw $t3, 0x44($v1)
    srl $t2, $t2, 24
    beqz $t3, .L00030B6C
    add $a2, $a2, $t2
  .L00030B18:
    lw $t2, 0x0($a1)
    lw $t1, 0x4($a1)
    addiu $t4, $zero, 0x60
    andi $at, $t2, 0x7
    sll $at, $at, 2
    and $t4, $t4, $t2
    beqz $t4, .L00030B4C
    add $at, $at, $v1
    andi $t4, $t4, 0x40
    beqz $t4, .L00030B48
    nop
    addiu $t3, $zero, 0x10
  .L00030B48:
    add $at, $at, $t3
  .L00030B4C:
    lw $t0, 0x80($at)
    nop
    jalr $t0
    addi $t6, $t6, -0x1
    bgtz $t6, .L00030B18
    lw $t3, 0x44($v1)
    j .L00030B98
    nop
  .L00030B6C:
    lw $t2, 0x0($a1)
    lw $t1, 0x4($a1)
    andi $at, $t2, 0x7
    sll $at, $at, 2
    add $at, $at, $v1
    lw $at, 0x80($at)
    nop
    jalr $at
    addi $t6, $t6, -0x1
    bgtz $t6, .L00030B6C
    nop
  .L00030B98:
    addi $t9, $t9, -0x1
    blez $t9, .L00030BCC
    addi $s7, $s7, -0x1
    blez $s7, .L00030BB4
    addi $s1, $s1, 0x4
    j .L00030A58
    lw $t0, -0x4($s1)
  .L00030BB4:
    bgezal $t7, func_00031050
    lwc2 $0, 0x10($v1)
    jal func_000310B8
    lwc2 $1, 0x14($v1)
    j .L00030A2C
    nop
  .L00030BCC:
    lw $t0, 0x1EC($v1)
    lw $t1, 0x0($v1)
    nop
    sw $t1, 0x0($t0)
    lw $ra, 0x48($v1)
    lw $s0, 0x4C($v1)
    lw $s1, 0x50($v1)
    jr $ra
    lw $s2, 0x54($v1)
endlabel func_000307FC

/* ---------------- func_00030BF0 (HANDWRITTEN ASM) ---------------- */
/* table lookup helper: index in $t2, table base in $s6 -> unsigned halfwords in $t5/$t4 (non-ABI registers) */
glabel func_00030BF0
    srl $12, $10, 2
    andi $12, $12, 0x3FC0
    srl $13, $10, 15
    andi $13, $13, 0xFE
    add $13, $13, $12
    add $13, $13, $22
    srl $12, $12, 5
    add $12, $12, $22
    lhu $13, 0x100($13)
    lhu $12, 0($12)
    jr $31
    nop
endlabel func_00030BF0

/* ---------------- func_00030C20 (HANDWRITTEN ASM) ---------------- */
glabel func_00030C20
    .set noat
    lw $19, 0x0($9)
    addi $9, $9, 0x4
    mfc2 $1, $20
    srl $12, $19, 5
    srl $13, $19, 10
    srl $18, $19, 15
    andi $19, $19, 0x3E
    andi $12, $12, 0x3E
    andi $13, $13, 0x3E
    andi $18, $18, 0x3E
    add $24, $19, $3
    lhu $24, 0xB0($24)
    add $25, $12, $3
    or $24, $1, $24
    lhu $25, 0xB0($25)
    mfc2 $1, $21
    mtc2 $24, $22
    or $25, $25, $1
    mtc2 $25, $23
    add $24, $13, $3
    add $25, $18, $3
    sll $19, $19, 2
    add $19, $19, $3
    lw $17, 0xE8($19)
    lw $19, 0xE4($19)
    sll $12, $12, 2
    add $12, $12, $3
    lw $1, 0xE8($12)
    lw $12, 0xE4($12)
    add $17, $17, $1
    sll $13, $13, 2
    add $13, $13, $3
    lw $1, 0xE8($13)
    lw $13, 0xE4($13)
    add $17, $17, $1
    sll $18, $18, 2
    add $18, $18, $3
    lw $1, 0xE8($18)
    lw $18, 0xE4($18)
    add $17, $17, $1
    sra $17, $17, 3
    addiu $1, $0, 0x20
    sub $1, $17, $1
    bltz $1, .L00030EBC
    nop
    lui $1, (0x80008000 >> 16)
    ori $1, $1, (0x80008000 & 0xFFFF)
    and $1, $1, $19
    and $1, $1, $12
    and $1, $1, $13
    and $1, $1, $18
    bnez $1, .L00030EBC
    nop
    lui $20, (0x80008000 >> 16)
    ori $20, $20, (0x80008000 & 0xFFFF)
    or $1, $19, $12
    or $1, $1, $13
    or $1, $1, $18
    and $1, $1, $20
    beqz $1, .L00030D4C
    andi $20, $1, 0x8000
    beqz $20, .L00030D28
    srl $1, $1, 16
    beqz $1, .L00030D54
    lui $1, (0x1000000 >> 16)
    bgez $0, .L00030D6C
  .L00030D28:
    addiu $1, $0, 0x200
    and $1, $1, $19
    and $1, $1, $12
    and $1, $1, $13
    and $1, $1, $18
    bnez $1, .L00030EBC
    nop
    j .L00030D6C
    nop
  .L00030D4C:
    lui $1, (0x1000200 >> 16)
    ori $1, $1, (0x1000200 & 0xFFFF)
  .L00030D54:
    and $1, $1, $19
    and $1, $1, $12
    and $1, $1, $13
    and $1, $1, $18
    bnez $1, .L00030EBC
    nop
  .L00030D6C:
    lhu $24, 0xB0($24)
    lhu $25, 0xB0($25)
    addiu $20, $0, 0x140
    sub $1, $17, $20
    bgtz $1, .L00030E48
    nop
    sw $21, 0x58($3)
    addiu $11, $0, 0x280
    addiu $21, $0, -0x80
    sll $20, $19, 16
    sra $20, $20, 16
    sub $1, $20, $21
    bltz $1, .L00030DF0
    sub $1, $11, $20
    bltz $1, .L00030DF0
    sll $20, $12, 16
    sra $20, $20, 16
    sub $1, $20, $21
    bltz $1, .L00030DF0
    sub $1, $11, $20
    bltz $1, .L00030DF0
    sll $20, $13, 16
    sra $20, $20, 16
    sub $1, $20, $21
    bltz $1, .L00030DF0
    sub $1, $11, $20
    bltz $1, .L00030DF0
    sll $20, $18, 16
    sra $20, $20, 16
    sub $1, $20, $21
    bltz $1, .L00030DF0
    sub $1, $11, $20
    bgez $1, .L00030E44
  .L00030DF0:
    lw $21, 0x58($3)
    sw $19, 0x1D0($3)
    sw $12, 0x1D4($3)
    sw $18, 0x1D8($3)
    sw $13, 0x1DC($3)
    swc2 $22, 0x1E0($3)
    swc2 $23, 0x1E4($3)
    sh $25, 0x1E8($3)
    sh $24, 0x1EA($3)
    lui $1, (0xFFFFFF >> 16)
    ori $1, $1, (0xFFFFFF & 0xFFFF)
    and $1, $10, $1
    sw $31, 0x58($3)
    swc2 $20, 0x5C($3)
    jal func_00031550
    swc2 $21, 0x60($3)
    lw $31, 0x58($3)
    lwc2 $20, 0x5C($3)
    lwc2 $21, 0x60($3)
    bgez $0, .L00030EBC
    nop
  .L00030E44:
    nop
  .L00030E48:
    andi $1, $21, 0xF
    bnez $1, .L00030E60
    nop
    lw $1, 0x0($3)
    j .L00030E78
    sw $2, 0x0($3)
  .L00030E60:
    lw $20, 0x1AC($3)
    andi $17, $17, 0x3FFC
    add $20, $20, $17
    lw $1, 0x0($20)
    nop
    sw $2, 0x0($20)
  .L00030E78:
    sw $19, 0x8($2)
    sw $12, 0x10($2)
    sw $13, 0x18($2)
    sw $18, 0x20($2)
    sw $10, 0x4($2)
    lui $20, (0x9000000 >> 16)
    swc2 $22, 0xC($2)
    or $20, $20, $1
    swc2 $23, 0x14($2)
    addi $2, $2, 0x28
    sw $20, -0x28($2)
    addi $8, $8, -0x1
    sw $24, -0xC($2)
    bgtz $8, func_00030C20
    sw $25, -0x4($2)
    jr $31
    nop
  .L00030EBC:
    addi $8, $8, -0x1
    bgtz $8, func_00030C20
    nop
    jr $31
    nop
    .purgem XBEQZ
    .purgem XBGEZ
    .purgem XBGTZ
    .purgem XBLTZ
    .purgem XBNEZ
    .purgem XJ
    .purgem XJAL
    .set at
endlabel func_00030C20

/* ---------------- func_00030ED0 (HANDWRITTEN ASM) ---------------- */
/* Saves ra/s0-s7 to the static block D_00089494 instead of the stack, loops func_000307FC over a0[0..a1) */
glabel func_00030ED0
    lui $13, %hi(D_00089494)
    addiu $13, $13, %lo(D_00089494)
    sw $31, 0x0($13)
    sw $16, 0x4($13)
    sw $17, 0x8($13)
    sw $18, 0xC($13)
    sw $19, 0x10($13)
    sw $20, 0x14($13)
    sw $21, 0x18($13)
    sw $22, 0x1C($13)
    sw $23, 0x20($13)
    addu $16, $4, $0
    addu $17, $5, $0
    lw $18, 0x4($6)
    lw $5, 0x0($6)
  .L00030F0C:
    lw $4, 0x0($16)
    addu $6, $18, $0
    nop
    jal func_000307FC
    addi $17, $17, -0x1
    addu $5, $2, $0
    bgtz $17, .L00030F0C
    addi $16, $16, 0x4
    lui $13, %hi(D_00089494)
    addiu $13, $13, %lo(D_00089494)
    lw $31, 0x0($13)
    lw $16, 0x4($13)
    lw $17, 0x8($13)
    lw $18, 0xC($13)
    lw $19, 0x10($13)
    lw $20, 0x14($13)
    lw $21, 0x18($13)
    lw $22, 0x1C($13)
    lw $23, 0x20($13)
    jr $31
    nop
endlabel func_00030ED0

/* ---------------- func_00030F60 (HANDWRITTEN ASM) ---------------- */
glabel func_00030F60
    sw $t8, 0x64($v1)
    sw $t9, 0x68($v1)
    sw $s1, 0x6C($v1)
    sw $t6, 0x70($v1)
    sw $ra, 0x7C($v1)
    jal func_00030BF0
    lw $t6, 0x74($v1)
    sll $t5, $t5, 16
    mtc2 $t5, $20
    sll $t4, $t4, 16
    lw $t0, 0x8($a1)
    lw $t2, 0xC($a1)
    jal func_0003110C
    mtc2 $t4, $21
    lw $t2, 0xC($v1)
    lui $at, 0x2c00
    or $t2, $t2, $at
    addiu $t0, $zero, 0x10
    lui $t1, %hi(D_00089454)
    addiu $t1, $t1, %lo(D_00089454)
    lw $t3, 0x0($s5)
    addiu $t4, $zero, 0x8
    addi $s2, $v1, 0xE4
  .L00030FBC:
    lwc2 $0, 0x0($s2)
    lwc2 $1, 0x4($s2)
    lwc2 $2, 0x8($s2)
    lwc2 $3, 0xC($s2)
    lwc2 $4, 0x10($s2)
    lwc2 $5, 0x14($s2)
    nop
    nop
    rtpt
    swc2 $12, 0x0($s2)
    swc2 $17, 0x4($s2)
    swc2 $13, 0x8($s2)
    swc2 $18, 0xC($s2)
    swc2 $14, 0x10($s2)
    swc2 $19, 0x14($s2)
    addi $t4, $t4, -0x1
    bgtz $t4, .L00030FBC
    addi $s2, $s2, 0x18
    lwc2 $0, 0x0($s2)
    lwc2 $1, 0x4($s2)
    nop
    nop
    rtps
    nop
    swc2 $14, 0x0($s2)
    swc2 $19, 0x4($s2)
    jal func_00030C20
    nop
    lw $ra, 0x7C($v1)
    lw $t8, 0x64($v1)
    lw $t9, 0x68($v1)
    lw $s1, 0x6C($v1)
    lw $t6, 0x70($v1)
    jr $ra
    addi $a1, $a1, 0x10
endlabel func_00030F60

/* ---------------- func_00031048 (matched by a C pilot: void func_00031048(void) { }) ---------------- */
glabel func_00031048
    jr $ra
    nop
endlabel func_00031048

/* ---------------- func_00031050 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: store SXY0-2 to s0, s0 += 12, t7 -= 3; t7 = -1 once it runs out */
glabel func_00031050
    addi $15, $15, -3
    swc2 $12, 0x0($16)
    swc2 $13, 0x4($16)
    swc2 $14, 0x8($16)
    bgtz $15, .L0003106C
    addi $16, $16, 0xC
    addiu $15, $0, -1
  .L0003106C:
    jr $31
    nop
endlabel func_00031050

/* ---------------- func_00031074 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: store SXY0-2 to s0, t7 -= 3, loop into func_00031094 while t7 > 0 */
glabel func_00031074
    addi $15, $15, -3
    swc2 $12, 0x0($16)
    swc2 $13, 0x4($16)
    swc2 $14, 0x8($16)
    bgtz $15, func_00031094
    addi $16, $16, 0xC
    jr $31
    addiu $15, $0, -1
endlabel func_00031074

/* ---------------- func_00031094 (HANDWRITTEN ASM) ---------------- */
/* GTE fragment: load 3 SVECTORs (VXY0..VZ2) from a0, a0 += 24, rtpt in the jr delay slot */
glabel func_00031094
    lwc2 $0, 0x0($4)
    lwc2 $1, 0x4($4)
    lwc2 $2, 0x8($4)
    lwc2 $3, 0xC($4)
    lwc2 $4, 0x10($4)
    lwc2 $5, 0x14($4)
    addi $4, $4, 0x18
    jr $31
    .word 0x4A280030
endlabel func_00031094

/* ---------------- func_000310B8 (HANDWRITTEN ASM) ---------------- */
/* Handwritten GTE function (matrix load from a3, MVMVA in the jr delay slot). */
glabel func_000310B8
    lw $8, 0($7)
    lw $9, 4($7)
    lw $10, 8($7)
    lw $11, 12($7)
    lw $12, 16($7)
    lw $13, 20($7)
    ctc2 $8, $8
    ctc2 $9, $9
    ctc2 $10, $10
    ctc2 $11, $11
    ctc2 $12, $12
    sra $12, $12, 16
    ctc2 $12, $13
    sra $12, $13, 16
    ctc2 $12, $15
    sll $13, $13, 16
    sra $13, $13, 16
    ctc2 $13, $14
    addi $7, $7, 24
    jr $31
    mvmva 1, 1, 0, 1, 0
endlabel func_000310B8

/* ---------------- func_0003110C (HANDWRITTEN ASM) ---------------- */
glabel func_0003110C
    srl $at, $t0, 16
    srl $t3, $t2, 16
    andi $t0, $t0, 0xFEFF
    andi $at, $at, 0xFEFF
    andi $t2, $t2, 0xFEFF
    andi $t3, $t3, 0xFEFF
    sh $t0, 0xB0($v1)
    sh $at, 0xBE($v1)
    sh $t3, 0xD6($v1)
    sh $t2, 0xDC($v1)
    srl $t4, $t1, 5
    andi $t4, $t4, 0x7F8
    srl $t9, $t1, 13
    andi $t9, $t9, 0x7F8
    srl $t8, $t1, 21
    andi $t8, $t8, 0x7F8
    sll $t1, $t1, 3
    andi $t1, $t1, 0x7F8
    add $t1, $t1, $t6
    add $t4, $t4, $t6
    add $t9, $t9, $t6
    add $t8, $t8, $t6
    lh $t3, 0x4($t1)
    lw $t1, 0x0($t1)
    lh $t5, 0x4($t4)
    lw $t4, 0x0($t4)
    lh $at, 0x4($t9)
    lw $t9, 0x0($t9)
    sw $t1, 0xE4($v1)
    sh $t3, 0xE8($v1)
    sw $t4, 0x11C($v1)
    sh $t5, 0x120($v1)
    sh $at, 0x198($v1)
    sw $t9, 0x194($v1)
    lw $at, 0x0($t8)
    lw $t9, 0x4($t8)
    sra $t2, $t1, 16
    sll $t1, $t1, 16
    sra $t1, $t1, 16
    sw $at, 0x17C($v1)
    sw $t9, 0x180($v1)
    addiu $t6, $zero, 0x15
    addi $s3, $v1, 0x1B0
    addiu $s4, $zero, 0x0
  .L000311BC:
    bgtz $s4, .L000311D4
    addiu $s4, $s4, -0x1
    addiu $s4, $zero, 0x5
    lw $t4, 0x0($s3)
    lw $t5, 0x10($s3)
    addi $s3, $s3, 0x4
  .L000311D4:
    andi $s1, $t4, 0x3E
    add $t8, $s1, $v1
    lhu $t8, 0xB0($t8)
    andi $s2, $t5, 0x3E
    add $t0, $t0, $t8
    srl $t0, $t0, 1
    andi $t0, $t0, 0xFEFF
    add $t9, $s2, $v1
    sh $t0, 0xB0($t9)
    addu $t0, $t8, $zero
    sll $s1, $s1, 2
    sll $s2, $s2, 2
    add $s1, $s1, $v1
    add $s2, $s2, $v1
    lh $t8, 0xE4($s1)
    lh $t9, 0xE6($s1)
    add $t1, $t1, $t8
    sra $t1, $t1, 1
    sh $t1, 0xE4($s2)
    addu $t1, $t8, $zero
    lh $t8, 0xE8($s1)
    add $t2, $t2, $t9
    sra $t2, $t2, 1
    sh $t2, 0xE6($s2)
    addu $t2, $t9, $zero
    add $t3, $t3, $t8
    sra $t3, $t3, 1
    sh $t3, 0xE8($s2)
    addu $t3, $t8, $zero
    srl $t4, $t4, 5
    addiu $t6, $t6, -0x1
    bgtz $t6, .L000311BC
    srl $t5, $t5, 5
    jr $ra
    nop
endlabel func_0003110C

/* ---------------- func_00031260 (HANDWRITTEN ASM) ---------------- */
glabel func_00031260
    .set noat
    mtc2 $8, $30
    lui $11, %hi(D_00094430)
    addiu $11, $11, %lo(D_00094430)
    mfc2 $9, $31
    addiu $1, $0, -2
    and $10, $9, $1
    addiu $9, $0, 0x1F
    sub $9, $9, $10
    sra $9, $9, 1
    addi $1, $10, -0x18
    bltz $1, 1f
    nop
    sllv $8, $8, $1
    .word 0x04010003
    1:
    addiu $1, $0, 0x18
    sub $1, $1, $10
    srav $8, $8, $1
    2:
    addi $8, $8, -0x40
    sll $8, $8, 1
    add $11, $11, $8
    lh $11, 0($11)
    nop
    sllv $11, $11, $9
    jr $31
    srl $8, $11, 12
    .set at
endlabel func_00031260

/* ---------------- func_000312C4 (HANDWRITTEN ASM) ---------------- */
glabel func_000312C4
    .set noat
    mtc2 $8, $0
    mtc2 $9, $2
    mtc2 $10, $4
    mtc2 $11, $6
    mtc2 $31, $12
    sub $8, $9, $8
    sra $9, $12, 16
    sra $1, $24, 16
    sub $9, $1, $9
    mtc2 $8, $9
    mtc2 $9, $10
    sw $8, 0x320($3)
    sw $9, 0x324($3)
    .word 0x4AA00428
    srl $1, $13, 8
    srl $11, $25, 8
    sub $1, $11, $1
    andi $8, $13, 0xFF
    andi $9, $25, 0xFF
    sub $11, $9, $8
    sw $11, 0x328($3)
    sw $1, 0x32C($3)
    mfc2 $8, $25
    mfc2 $9, $26
    .reloc ., R_MIPS_26, func_00031260
    .word 0x0C000000
    add $8, $9, $8
    mfc2 $10, $4
    sw $8, 0x330($3)
    .word 0x0148001A
    mfc2 $31, $12
    mfc2 $8, $0
    mfc2 $9, $2
    jr $31
    mfc2 $11, $6
    .set at
endlabel func_000312C4

/* ---------------- func_0003134C (HANDWRITTEN ASM) ---------------- */
glabel func_0003134C
    .set noat
    sll $8, $12, 16
    sra $8, $8, 16
    sll $9, $24, 16
    sra $9, $9, 16
    addiu $1, $0, -0x80
    sub $10, $8, $1
    bltz $10, .L00031390
    nop
    addiu $1, $0, 0x280
    sub $10, $1, $8
    bgez $10, .L000313A4
    nop
    sub $1, $1, $9
    bgez $1, .L000313F8
    nop
    jr $31
    nop
  .L00031390:
    sub $1, $9, $1
    bgez $1, .L000313F8
    nop
    jr $31
    nop
  .L000313A4:
    sw $12, 0x0($19)
    sw $13, 0x4($19)
    addi $19, $19, 0x8
    addi $18, $18, 0x1
    addiu $1, $0, 0x280
    sub $10, $1, $9
    bltz $10, .L000313DC
    nop
    addiu $1, $0, -0x80
    sub $10, $9, $1
    bltz $10, .L000313DC
    nop
    jr $31
    nop
  .L000313DC:
    sw $31, 0x394($3)
    neg $10, $10
    jal func_000312C4
    sll $10, $10, 12
    lw $31, 0x394($3)
    bgez $0, .L000314C4
    nop
  .L000313F8:
    sw $31, 0x394($3)
    neg $10, $10
    jal func_000312C4
    sll $10, $10, 12
    lw $31, 0x394($3)
    lwc2 $9, 0x324($3)
    lwc2 $10, 0x328($3)
    lwc2 $11, 0x32C($3)
    sra $1, $12, 16
    mtc2 $1, $25
    andi $1, $13, 0xFF
    mtc2 $1, $26
    srl $1, $13, 8
    mtc2 $1, $27
    mflo $1
    mtc2 $1, $8
    nop
    nop
    gpl 1
    mfc2 $1, $25
    mfc2 $10, $26
    mfc2 $11, $27
    sll $1, $1, 16
    andi $10, $10, 0xFF
    andi $11, $11, 0xFF
    sll $11, $11, 8
    or $10, $11, $10
    bgtz $8, .L00031478
    nop
    ori $1, $1, 0xFF80
    bgez $0, .L0003147C
    nop
  .L00031478:
    ori $1, $1, 0x280
  .L0003147C:
    sw $1, 0x0($19)
    sw $10, 0x4($19)
    addi $19, $19, 0x8
    addi $18, $18, 0x1
    addiu $1, $0, 0x280
    sub $10, $1, $9
    bltz $10, .L000314B4
    nop
    addiu $1, $0, -0x80
    sub $10, $9, $1
    bltz $10, .L000314B4
    nop
    jr $31
    nop
  .L000314B4:
    sw $8, 0x330($3)
    neg $10, $10
    sll $10, $10, 12
    .word 0x0148001A
  .L000314C4:
    lwc2 $9, 0x324($3)
    lwc2 $10, 0x328($3)
    lwc2 $11, 0x32C($3)
    sra $1, $24, 16
    mtc2 $1, $25
    andi $1, $25, 0xFF
    mtc2 $1, $26
    srl $1, $25, 8
    mtc2 $1, $27
    mflo $1
    neg $1, $1
    mtc2 $1, $8
    nop
    nop
    gpl 1
    mfc2 $1, $25
    mfc2 $10, $26
    mfc2 $11, $27
    sll $1, $1, 16
    andi $10, $10, 0xFF
    andi $11, $11, 0xFF
    sll $11, $11, 8
    or $10, $11, $10
    bgtz $9, .L00031534
    nop
    ori $1, $1, 0xFF80
    bgez $0, .L00031538
    nop
  .L00031534:
    ori $1, $1, 0x280
  .L00031538:
    sw $1, 0x0($19)
    sw $10, 0x4($19)
    addi $19, $19, 0x8
    addi $18, $18, 0x1
    jr $31
    nop
endlabel func_0003134C

/* ---------------- func_00031550 (HANDWRITTEN ASM) ---------------- */
glabel func_00031550
    sw $31, 0x380($3)
    sw $8, 0x384($3)
    sw $9, 0x388($3)
    sw $10, 0x38C($3)
    sw $11, 0x390($3)
    addiu $18, $0, 0x0
    addi $19, $3, 0x3C0
    lw $12, 0x1D0($3)
    lhu $13, 0x1E0($3)
    lw $24, 0x1D4($3)
    jal func_0003134C
    lhu $25, 0x1E4($3)
    lw $12, 0x1D4($3)
    lhu $13, 0x1E4($3)
    lw $24, 0x1D8($3)
    jal func_0003134C
    lhu $25, 0x1E8($3)
    lw $12, 0x1D8($3)
    lhu $13, 0x1E8($3)
    lw $24, 0x1DC($3)
    jal func_0003134C
    lhu $25, 0x1EA($3)
    lw $12, 0x1DC($3)
    lhu $13, 0x1EA($3)
    lw $24, 0x1D0($3)
    jal func_0003134C
    lhu $25, 0x1E0($3)
    andi $1, $21, 0xF
    bnez $1, .L000315D4
    nop
    lw $19, 0x0($3)
    j .L000315E8
    mtc2 $3, $0
  .L000315D4:
    lw $20, 0x1AC($3)
    andi $17, $17, 0x3FFC
    add $20, $20, $17
    lw $19, 0x0($20)
    mtc2 $20, $0
  .L000315E8:
    lhu $24, 0x1E2($3)
    lhu $25, 0x1E6($3)
    sll $24, $24, 16
    sll $25, $25, 16
    addi $1, $18, -0x3
    bnez $1, .L0003161C
    addi $1, $18, -0x4
    addi $8, $3, 0x3C0
    addi $9, $3, 0x3C8
    jal func_000316C8
    addi $10, $3, 0x3D0
    bgez $0, .L000316A4
    nop
  .L0003161C:
    bnez $1, .L00031640
    addi $1, $18, -0x5
    addi $8, $3, 0x3C0
    addi $9, $3, 0x3C8
    addi $10, $3, 0x3D0
    jal func_00031734
    addi $11, $3, 0x3D8
    bgez $0, .L000316A4
    nop
  .L00031640:
    bnez $1, .L00031674
    addi $1, $18, -0x6
    addi $8, $3, 0x3C0
    addi $9, $3, 0x3C8
    jal func_000316C8
    addi $10, $3, 0x3E0
    addi $8, $3, 0x3C8
    addi $9, $3, 0x3D0
    addi $10, $3, 0x3D8
    jal func_00031734
    addi $11, $3, 0x3E0
    bgez $0, .L000316A4
    nop
  .L00031674:
    bnez $1, .L000316A4
    nop
    addi $8, $3, 0x3C0
    addi $9, $3, 0x3C8
    addi $10, $3, 0x3D0
    jal func_00031734
    addi $11, $3, 0x3E8
    addi $8, $3, 0x3E8
    addi $9, $3, 0x3D0
    addi $10, $3, 0x3D8
    jal func_00031734
    addi $11, $3, 0x3E0
  .L000316A4:
    mfc2 $8, $0
    lw $31, 0x380($3)
    sw $19, 0x0($8)
    lw $8, 0x384($3)
    lw $9, 0x388($3)
    lw $10, 0x38C($3)
    lw $11, 0x390($3)
    jr $31
    nop
endlabel func_00031550

/* ---------------- func_000316C8 (HANDWRITTEN ASM) ---------------- */
glabel func_000316C8
    .set noat
    lhu $12, 4($8)
    lhu $13, 4($9)
    or $12, $12, $24
    or $13, $13, $25
    mtc2 $12, $2
    mtc2 $13, $4
    lhu $12, 4($10)
    lw $8, 0($8)
    lw $9, 0($9)
    lw $10, 0($10)
    lw $18, 12($3)
    lui $1, (0x24000000 >> 16)
    or $1, $1, $18
    lui $18, (0x7000000 >> 16)
    or $18, $18, $19
    sw $18, 0($2)
    sw $1, 4($2)
    sw $8, 8($2)
    swc2 $2, 12($2)
    sw $9, 16($2)
    swc2 $4, 20($2)
    sw $10, 24($2)
    sw $12, 28($2)
    addu $19, $2, $0
    addi $2, $2, 0x20
    jr $31
    nop
    .set at
endlabel func_000316C8

/* ---------------- func_00031734 (HANDWRITTEN ASM) ---------------- */
glabel func_00031734
    .set noat
    lhu $12, 4($8)
    lhu $13, 4($9)
    or $12, $12, $24
    or $13, $13, $25
    mtc2 $12, $2
    mtc2 $13, $4
    lhu $12, 4($10)
    lhu $13, 4($11)
    lw $8, 0($8)
    lw $9, 0($9)
    lw $10, 0($10)
    lw $11, 0($11)
    lw $18, 12($3)
    lui $1, 0x2C00
    or $1, $1, $18
    lui $18, 0x0900
    or $18, $18, $19
    sw $18, 0($2)
    sw $1, 4($2)
    sw $8, 8($2)
    swc2 $2, 12($2)
    sw $9, 16($2)
    swc2 $4, 20($2)
    sw $10, 32($2)
    sw $12, 36($2)
    sw $11, 24($2)
    sw $13, 28($2)
    addu $19, $2, $0
    addi $2, $2, 0x28
    jr $31
    nop
    .set at
endlabel func_00031734
