# Lockstep trace spec (port/README.md "Lockstep comparison"): what both sides snapshot once per game frame.
# label        ps1addr   pc symbol           bytes   (word-aligned, multiple of 4)
# ps1addr is where the emulator reads it; the PC build (port/pc/pc_trace.c) finds the symbol in its own ELF
# symbol table (a C-owned object, or a PS1-image name pc_gendata PROVIDEs at its PS1 address). A "*" symbol is a
# PS1 address range: the PC assembles it from PC_RAM plus the C-owned objects the PS1 map puts inside it.
# At most 63 entries and 34 KB in all (lrhost's buffer in PS1 RAM).
frame          97534     D_00097534          4       # game frame counter (main.c func_0002360C, ++ after the wait)
CameraCnt      970D8     CameraCnt           4       # replay frame counter
randseed       09010     s_rand_seed         4       # BIOS rand() seed (SCPH-5501 A(2Fh) keeps it at 0x9010; PC: pc_psyq.c)
camera_pos     8A8F0     camera_position     16
rnd2tab        BDD40     D_000BDD40          256     # Random2 (sky.c func_0004FFE8) table
car_data       9B0A4     car_data            7920    # CAR_DATA[20] (0x18C each)
pursuer        C4690     D_000C4690          9800    # felony.c .bss 0xC4690-0xC6CD8 (PURSUER[5] + views)
mission        C6F04     D_000C6F04          744     # MISSION
civslots       98950     D_00098950          1760    # CIV_SLOT[] (civ_ai.c / cars.c)
smalldata      96770     *                   4312    # the whole .sdata+.sbss (0x96770-0x97848): every small global

# Masks: bytes lockstep.py leaves out of the comparison, each with the reason it may differ.
# mask label offset(hex) bytes [stride(hex) count]
# felony.c func_00065424 builds the pursuer's steering target in a stack VEC whose vy/pad it never sets (only vx/vz;
# the path branch's func_00064DEC writes vx/vz too) and smooths it into cur/prev (OBJ +0x76C/+0x77C): their vy and pad
# words are uninitialised stack garbage on both machines, and nothing reads them.
mask pursuer 770 4 7A8 5
mask pursuer 778 4 7A8 5
mask pursuer 780 4 7A8 5
mask pursuer 788 4 7A8 5
# smalldata (0x96770 + offset): state that is timing or platform, not simulation -
# vblank clocks: D_00096924 (the loop's last VSync(-1)), vblcounter (system.c)
mask smalldata 1B4 4
mask smalldata DD0 4
# GetSp() results the game saves around scratchpad-stack code (main.c D_00097064, cars.c D_000973AC): PS1 stack
# addresses vs the PC's token (pc_psyq.c GetSp/SetSp)
mask smalldata 8F4 4
mask smalldata C3C 4
# pres.c D_00096E30 = (int)"DATA\LANG.ENG": the address of a string literal (PS1 .rodata 0x10EF4 / PC .rodata)
mask smalldata 6C0 4
# CD streaming (spool.c .sdata 0x969E8 + .sbss 0x971E0-0x972DC): the PC serves the stream synchronously inside the
# frame's VSync (port/pc/pc_cd.c), the PS1 over several frames from the CD interrupt, so the spooler's progress
# counters and buffer pointers differ while a region streams in (a few frames per area change); what it loads
# compares equal (the sim regions stay identical)
mask smalldata 278 12
mask smalldata A70 252
# music and XA: the XM player runs in the VSync callback (xmplay.c .sdata 0x96FA0 / .sbss 0x97810-0x97848, incl.
# D_00096FA0's VSync(1) timing) - per vblank, not per game frame, and on PC on PsyCross's interrupt thread at wall
# clock; xaplay.c .sbss 0x977F4 keeps the old CdReadyCallback (a PsyQ-library address on the PS1)
mask smalldata 830 24
mask smalldata 10A0 56
mask smalldata 1084 28
# gamesnd.c D_000974C0: frames since the speech queue (police radio) last waited on voice 9, which frees when the SPU
# finishes the sample - audio-hardware timing (PsyCross's OpenAL voice vs the PS1 SPU) drifts it by a few frames
# per message. Read only by felony.c func_000651F0 (>= 0xF1 before the next radio call); no sim divergence follows
mask smalldata D50 4
# ... and so the radio's "suspect heading north/east/.." call can sample the player's heading a few frames apart:
# D_00097728 (felony.c, the last direction called) then holds another quadrant. Only compared to choose whether to
# queue the next call (speech only).
mask smalldata FB8 4
