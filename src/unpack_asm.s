/* unpack_asm.s: original assembler module, 2 functions func_00026140..func_00026194 (0x26140-0x26794).
 * A decompressor: func_00026140 is the C-callable wrapper (saves s0-s3, keeps a0->4 in D_0002613C and returns
 * it), func_00026194 the unpacker itself. Not compiler output (see each function's note).
 * Module extent: the original module also owns the word D_0002613C at 0x2613C (just before func_00026140), which
 * the split leaves at the tail of glaunch.c's last asm block (glaunch.c links with +0x4 zero padding there), and
 * the words up to 0x26794 after the unpacker's last instruction, which it uses as scratch (it writes the packed
 * header word to 0x26790 and a halfword to 0x2678E through a literal address) - they are .word 0 here.
 * The cut at 0x26794 (here vs tile_asm.s) is by function: the unpacker and its scratch end there, and the
 * GTE helpers after it (tile_asm.s) never touch them. Medium confidence; one module 0x2613C-0x270D4 would
 * link the same. REDRIVER2 keeps its unpacker apart from the render asm too (rnc_2.c vs rndrasm.c). */

.include "macro.inc"

.set noreorder

.section .text, "ax"

.include "macro.inc"

.set noreorder

.section .text, "ax"

/* ---------------- func_00026140 (HANDWRITTEN ASM) ---------------- */
/* saves s0-s3/ra in a hand-built 0x14 frame, stores a0->4 to D_0002613C, calls func_00026194, returns D_0002613C */
glabel func_00026140
    addi $sp, $sp, -0x14
    sw $s0, 0x0($sp)
    sw $s1, 0x4($sp)
    sw $s2, 0x8($sp)
    sw $s3, 0xC($sp)
    sw $ra, 0x10($sp)
    lui $a2, %hi(D_0002613C)
    addiu $a2, $a2, %lo(D_0002613C)
    lw $a3, 0x4($a0)
    jal func_00026194
    sw $a3, 0x0($a2)
    lw $s0, 0x0($sp)
    lw $s1, 0x4($sp)
    lw $s2, 0x8($sp)
    lw $s3, 0xC($sp)
    lw $ra, 0x10($sp)
    addi $sp, $sp, 0x14
    lui $v0, %hi(D_0002613C)
    addiu $v0, $v0, %lo(D_0002613C)
    jr $ra
    lw $v0, 0x0($v0)
endlabel func_00026140

/* ---------------- func_00026194 (HANDWRITTEN ASM) ---------------- */
/* LZ-style decompressor: a0 = packed src (header word stored to abs 0x26790), a1 = dst. No prologue, clobbers s0-s3 unsaved, addi/add forms, bit-flag state machine with cross-jumps - not compiler output. */
glabel func_00026194
    addu $t6, $a0, $zero
    addu $s1, $a1, $zero
    addiu $t5, $zero, 0x0
    lui $t3, 0x2
    ori $t3, $t3, 0x6790
    addiu $t6, $t6, 0x4
    lw $t5, 0x0($t6)
    addiu $t6, $t6, 0x4
    sw $t5, 0x0($t3)
    addu $v1, $t6, $zero
    addi $v1, $v1, 0xA
    addu $a1, $s1, $zero
    lw $t5, 0x0($t6)
    addiu $t6, $t6, 0x4
    addi $t3, $t3, -0x2
    sh $zero, 0x0($t3)
    lui $t2, 0xffff
    ori $t2, $t2, 0xff00
    addiu $t7, $zero, 0x1
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    j .L0002653C
    andi $t7, $t7, 0x1
  .L00026200:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L000262FC
    andi $t7, $t7, 0x1
  .L0002621C:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L0002636C
    andi $t7, $t7, 0x1
  .L00026238:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L0002638C
    andi $t7, $t7, 0x1
  .L00026254:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L000263B0
    andi $t7, $t7, 0x1
  .L00026270:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L000263E4
    andi $t7, $t7, 0x1
  .L0002628C:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L00026404
    andi $t7, $t7, 0x1
  .L000262A8:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L00026424
    andi $t7, $t7, 0x1
  .L000262C4:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L00026450
    andi $t7, $t7, 0x1
  .L000262E0:
    addiu $t0, $zero, 0x3
  .L000262E4:
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L00026200
    nop
  .L000262FC:
    addu $t1, $t1, $t1
    addu $t1, $t1, $t7
    srl $t7, $t1, 16
    andi $t7, $t7, 0x1
    addi $t0, $t0, -0x1
    bgez $t0, .L000262E4
    nop
    addi $t1, $t1, 0x2
  .L0002631C:
    lbu $t4, 0x0($v1)
    lbu $s2, 0x1($v1)
    sb $t4, 0x0($a1)
    sb $s2, 0x1($a1)
    lbu $t4, 0x2($v1)
    lbu $s2, 0x3($v1)
    sb $t4, 0x2($a1)
    sb $s2, 0x3($a1)
    addiu $v1, $v1, 0x4
    addi $t1, $t1, -0x1
    bgez $t1, .L0002631C
    addi $a1, $a1, 0x4
    j .L0002653C
    nop
  .L00026354:
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L0002621C
    nop
  .L0002636C:
    addu $t0, $t0, $t0
    addu $t0, $t0, $t7
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L00026238
    nop
  .L0002638C:
    beq $zero, $t7, .L000263CC
    nop
    addi $t0, $t0, -0x1
    add $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L00026254
    nop
  .L000263B0:
    addu $t0, $t0, $t0
    addu $t0, $t0, $t7
    srl $t7, $t0, 16
    andi $t7, $t7, 0x1
    addi $t4, $t0, -0x9
    beq $zero, $t4, .L000262E0
    nop
  .L000263CC:
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L00026270
    nop
  .L000263E4:
    beq $zero, $t7, .L00026470
    nop
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L0002628C
    nop
  .L00026404:
    addu $t1, $t1, $t1
    addu $t1, $t1, $t7
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L000262A8
    nop
  .L00026424:
    bne $zero, $t7, .L00026608
    nop
    bne $zero, $t1, .L00026460
    nop
    addi $t1, $t1, 0x1
  .L00026438:
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L000262C4
    nop
  .L00026450:
    addu $t1, $t1, $t1
    addu $t1, $t1, $t7
    srl $t7, $t1, 16
    andi $t7, $t7, 0x1
  .L00026460:
    sll $t4, $t1, 8
    andi $t4, $t4, 0xFF00
    srl $t1, $t1, 8
    or $t1, $t1, $t4
  .L00026470:
    andi $s3, $t1, 0xFF00
    lbu $t1, 0x0($v1)
    addiu $v1, $v1, 0x1
    or $t1, $t1, $s3
    addu $t6, $a1, $zero
    sub $t6, $t6, $t1
    addi $t6, $t6, -0x1
    andi $t7, $t0, 0x1
    srl $t0, $t0, 1
    beq $zero, $t7, .L000264AC
    nop
    lbu $t4, 0x0($t6)
    addi $t6, $t6, 0x1
    sb $t4, 0x0($a1)
    addi $a1, $a1, 0x1
  .L000264AC:
    addi $t0, $t0, -0x1
    bne $zero, $t1, .L000264E4
    nop
    andi $s3, $t1, 0xFF00
    lbu $t1, 0x0($t6)
    nop
    or $t1, $t1, $s3
  .L000264C8:
    sb $t1, 0x0($a1)
    addi $t0, $t0, -0x1
    sb $t1, 0x1($a1)
    bgez $t0, .L000264C8
    addi $a1, $a1, 0x2
    j .L0002653C
    nop
  .L000264E4:
    lbu $s2, 0x1($t6)
    lbu $t4, 0x0($t6)
    sb $s2, 0x1($a1)
    sb $t4, 0x0($a1)
    addi $t6, $t6, 0x2
    addi $t0, $t0, -0x1
    bgez $t0, .L000264E4
    addi $a1, $a1, 0x2
    j .L0002653C
    nop
  .L0002650C:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    bne $zero, $t7, .L00026580
    nop
  .L0002652C:
    lbu $t4, 0x0($v1)
    addi $v1, $v1, 0x1
    sb $t4, 0x0($a1)
    addi $a1, $a1, 0x1
  .L0002653C:
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    bne $zero, $t7, .L00026574
    nop
    lbu $t4, 0x0($v1)
    addi $v1, $v1, 0x1
    sb $t4, 0x0($a1)
    addi $a1, $a1, 0x1
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    beq $zero, $t7, .L0002652C
    nop
  .L00026574:
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L0002650C
    nop
  .L00026580:
    addiu $t0, $zero, 0x2
    addiu $t1, $zero, 0x0
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L0002665C
    nop
  .L000265A0:
    beq $zero, $t7, .L00026354
    nop
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L00026678
    nop
  .L000265C0:
    beq $zero, $t7, .L00026470
    nop
    addi $t0, $t0, 0x1
    add $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L00026694
    nop
  .L000265E4:
    beq $zero, $t7, .L000263CC
    nop
    lbu $t0, 0x0($v1)
    addi $v1, $v1, 0x1
    nop
    beq $zero, $t0, .L000266E8
    addi $t0, $t0, 0x8
    j .L000263CC
    nop
  .L00026608:
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L000266B0
    nop
  .L00026620:
    addu $t1, $t1, $t1
    addu $t1, $t1, $t7
    srl $t7, $t1, 16
    andi $t7, $t7, 0x1
    ori $t1, $t1, 0x4
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    beq $zero, $t2, .L000266CC
    nop
  .L0002664C:
    bne $zero, $t7, .L00026460
    nop
    j .L00026438
    nop
  .L0002665C:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L000265A0
    andi $t7, $t7, 0x1
  .L00026678:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L000265C0
    andi $t7, $t7, 0x1
  .L00026694:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L000265E4
    andi $t7, $t7, 0x1
  .L000266B0:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L00026620
    andi $t7, $t7, 0x1
  .L000266CC:
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    j .L0002664C
    andi $t7, $t7, 0x1
  .L000266E8:
    addu $t2, $t2, $t2
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
    andi $t2, $t2, 0xFF
    bne $zero, $t2, .L00026718
    nop
    lbu $t2, 0x0($v1)
    addi $v1, $v1, 0x1
    addu $t2, $t2, $t2
    addu $t2, $t2, $t7
    srl $t7, $t2, 8
    andi $t7, $t7, 0x1
  .L00026718:
    bne $zero, $t7, .L0002653C
    nop
    lhu $t5, 0x0($t3)
    addi $t3, $t3, 0x2
    beq $zero, $t5, .L0002674C
    nop
  .L00026730:
    lh $s0, 0x0($t3)
    addi $t3, $t3, 0x2
    sb $s0, 0x0($a1)
    addi $a1, $a1, 0x1
    addi $t5, $t5, -0x1
    bne $zero, $t5, .L00026730
    nop
  .L0002674C:
    j .L00026758
    nop
    sw $s0, 0x0($t3)
  .L00026758:
    jr $ra
    nop
    /* 0x26760-0x26793: 13 zero words after the code - the unpacker's scratch (it stores the packed header
     * word at 0x26790 and a halfword at 0x2678E through literal addresses), not padding */
    .fill 13, 4, 0
endlabel func_00026194
