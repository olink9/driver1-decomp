/* rle_asm.s: original assembler module, 1 function func_00035B5C (0x35B5C-0x35BC8): an RLE unpacker
 * (texture.c and pad.c's neighbour code call it). Between pad.c (ends 0x35B5C) and motion_c.c (0x35BC8). */

.include "macro.inc"

.set noreorder

.section .text, "ax"

/* ---------------- func_00035B5C (HANDWRITTEN ASM) ---------------- */
/* RLE unpacker writing backwards from dest+0x7FFF (signed count byte: <0 = run of next byte, >=0 = literals).
   No C form: every branch delay slot is filled from the branch target while the independent insn right before
   the branch (addiu a1 / addiu a2) stays put - GCC 2.7 reorg's fill_simple_delay_slots would always take it,
   and `bnez a3; addiu a3,a3,1` tests the count before stepping it without a copy (see pilot/AGENT.md f-01). */
glabel func_00035B5C
    addiu $a2, $a0, 0x7FFF
  .L00035B60:
    lb $a3, 0x0($a1)
    addiu $a1, $a1, 0x1
    andi $v0, $a3, 0x80
    beqz $v0, .L00035B9C
    lbu $v1, 0x0($a1)
    addiu $a1, $a1, 0x1
    sb $v1, 0x0($a2)
    addiu $a2, $a2, -0x1
  .L00035B80:
    sb $v1, 0x0($a2)
    addiu $a2, $a2, -0x1
    bnez $a3, .L00035B80
    addiu $a3, $a3, 0x1
    j .L00035BB4
    sltu $v0,$a2,$a0
  .L00035B98:
    lbu $v1, 0x0($a1)
  .L00035B9C:
    addiu $a1, $a1, 0x1
    sb $v1, 0x0($a2)
    addiu $a2, $a2, -0x1
    bnez $a3, .L00035B98
    addiu $a3, $a3, -0x1
    sltu $v0,$a2,$a0
  .L00035BB4:
    beqz $v0, .L00035B60
    nop
    jr $ra
    addu $v0, $a1, $zero
endlabel func_00035B5C
    /* 0x35BC4: one zero word after the function (part of this block in the original; no code reaches it) */
    .word 0
