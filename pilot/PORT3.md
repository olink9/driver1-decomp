# PORT3 - Driver 1 PC port, phase 3: saves + proof against an emulator (night 2026-10-26)

State: commit fb2b13f. The PC build is playable (it was played by hand: audio + controller work) and installed in Steam
(~/Applications/Driver1PC - coordinator-only, never touch it). Read port/README.md, pilot/PORT1.md hard rules
(all apply: PS1 build byte-identical via #ifdef PSX_PC / port/pc, work ALONE - no subagents, own build dir
~/dev/driver1-work/pcbuild/<you>, scratch ~/dev/driver1-work/pcbuild/<you>-scratch, no commits, no git state
changes, no PS1 `make`). Headless runs only: `ALSOFT_DRIVERS=null SDL_AUDIODRIVER=dummy SDL_VIDEODRIVER=offscreen`,
always `D1_PAD=...` (else the real controller leaks in), D1_QUIT on every run, `nice -n 10`, builds -j4 max.
Kill hung runs by PID. Search the web for PS1 specifics.

## F - memory card (owner: port/pc/pc_mcard.c (new), the memcard parts of port/pc/pc_psyq.c, src/mcard/mcard.c PC
## guards if needed, port/pc/pc_main.c only for an init call)
Saves don't work on PC: the 12 MemCard* PsyQ calls (libmcrd: MemCardInit/Start/Stop/Exist/Accept/ReadFile/WriteFile/
GetDirentry/Sync/CreateFile/DeleteFile/Format, see the README table) are "no card" stubs; PsyCross LIBMCRD.C is bypassed
(buggy). Implement libmcrd on top of a real 128 KB PS1 memory-card image file (standard raw .mcd/.mcr format: 16
blocks of 8 KB, directory frame layout per psx-spx) so saves are byte-compatible with emulators (DuckStation can
read the same file). Card file path: env `D1_MCARD` or default `card1.mcd` in the working dir (the Steam launcher
cds into ~/Applications/Driver1PC); create a formatted card if missing. Match libmcrd semantics the game relies on
(async calls + MemCardSync results, event/return codes, the "new card"/unformatted flows, slot 2 = no card).
Verify with scripted pad runs: save a game (career save from the apartment "Save Game" after the garage skip, and a
replay save if the menus allow), quit, relaunch, load it, and show it's there (frames). Validate the card file
with an independent parser (write a small python checker per psx-spx; checksum bytes). Report frames + how verified.

## G - lockstep comparison against an emulator (owner: new port/pc/pc_trace.c, tools/pc/compare/*, a one-line
## hook in port/pc/pc_main.c's VSync wrapper; no other src/ edits - write Requests instead)
Goal: prove the PC build simulates the same as the PS1 game, and find the first divergence when it doesn't.
1. Emulator: PCSX-Redux (scriptable with Lua, has a headless/no-UI mode and a GDB server) - build it in distrobox
   `build` (or use its AppImage), keep it in ~/dev/driver1-work/emu. BIOS: look in the EmuDeck tree on the SD card
   (`/run/media/deck/*/Emulation/bios`) - copy, never move/modify. Disc: ~/dev/driver1-work/driver1.bin (make a .cue).
2. Deterministic input: drive both sides with the same per-frame pad sequence. Prefer the game's own attract-mode
   demos / saved replays (Driver's replay system records pad input) - they need no injection. Otherwise inject a
   scripted pad on both sides (PC: D1_PAD; emulator: Lua).
3. Trace: at a fixed point each game frame (e.g. entry of the main loop step / VSync), dump a set of game-state
   variables on both sides: car_data[] positions/velocities/orientation, player state, camera, cop/felony state,
   mission timer, RNG state, frame counter. On PC, PS1-image variables live at their PS1 address; C-owned objects
   are elsewhere - use pc_gendata's symbol map (<build>/pc_syms.txt) or `&symbol` in pc_trace.c. Emulator side:
   read RAM at the PS1 addresses (build/slus_008.42.map has every symbol).
4. tools/pc/compare/: run both, align frames (sync on a game frame counter, not vblanks), diff, report the first
   divergent frame + variable. Then bisect the cause: which function computed the first wrong value (watchpoints in
   the emulator GDB server vs PC gdb), and fix it if it's a port bug (if it's in a file you don't own, write the fix
   as a Request with the exact patch). Expect the RNG seed (BIOS rand) and CD timing to matter.
Done = at least the attract demos run in lockstep for N frames (state N), each divergence explained or fixed, the
tool documented in port/README.md ("Lockstep comparison") so it can be rerun after any change.

Report format: files changed, what's verified and how, open issues, Requests (exact patches for files you don't own).

## H - FMV and MDEC (owner: src/fmvplay.c, src/nfmv/*.c, src/cred/cred.c PC guards, new port/pc/pc_mdec.c and
## port/pc/pc_fmv.c, the Exec + libpress (DecDCT*) block of port/pc/pc_psyq.c (lines ~66-160 only - F owns libmcrd
## in pc_mcard.c), port/CMakeLists.txt for adding NFMV sources, port/pc/pc_xa.c additions for STR audio)
Today: fmvplay.c runs NFMV.EXE through BIOS Exec (stubbed: "movie skipped"), and the libpress/MDEC calls the
credits (CRED.BIN portraits) use are stubs. Goal: the game's FMVs play in-process on PC, as REDRIVER2 did
(C/fmvplay.c calls FMV_main() instead of loading FMV.EXE):
1. MDEC: implement DecDCTvlc / DecDCTin / DecDCTout (+ Reset, callback) in pc_mdec.c - VLC decode of the PS1
   bitstream (version 2/3 frames; psx-spx "MDEC" + "CDROM - STR"), IDCT, YUV->RGB 15/24-bit exactly as the MDEC does,
   with the out-callback chain timing the game expects. Check the decoded credits portraits by frame dump.
2. NFMV.EXE (src/nfmv: main/movie/pad, its own TUs, 27 functions, one wip `main` asm-only - write its C for PC) linked
   into the PC exe under a renamed entry (e.g. nfmv_main), called by fmvplay.c's Exec path with the same argv the
   game passes; its CD streaming (CdlReadS of STR sectors - raw 2352-byte sectors are kept in pcdata for STR/XA,
   see README) through pc_cd.c's interfaces or a direct reader in pc_fmv.c, its XA audio through pc_xa.c (decode the
   interleaved audio sectors; add an entry point there if needed), video via MDEC -> LoadImage -> display.
3. Which movies: find every FMV the game plays (intro, mission cutscenes, credits, ending) and check each starts,
   plays at the right speed (15/30 fps per STR header), with sound, and returns to the game. Skippable with the
   button the original uses.
Verify by frame dumps (D1_SHOT) at several points of a movie and a WAV capture of the audio (see E's method in
port/README.md: ALSOFT wave backend). Report frames.
