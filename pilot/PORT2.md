# PORT2 - Driver 1 PC port, phase 2: in game

State (commit 54598c1): the PC build boots to the title screen and, left idle, starts the attract-mode demo.
Read port/README.md (how it is built and run, memory model, CD layer, debug env vars) and pilot/PORT1.md
(hard rules - they all still apply: PS1 build byte-identical via #ifdef PSX_PC, work ALONE, no subagents, -j2,
own build dir ~/dev/driver1-work/pcbuild/<you>, scratch in ~/dev/driver1-work/pcbuild/<you>-scratch, no commits,
no git state changes, no PS1 `make` - the coordinator runs it). Search the web for PS1 specifics (psx-spx,
PsyQ docs, REDRIVER2/OpenDriver2 sources and issues) instead of guessing; ~/dev/redriver2-ref has D2's PC code.

Run headless: `SDL_VIDEODRIVER=offscreen D1_DATA=~/dev/driver1-work/pcdata D1_SHOT=... D1_QUIT=... driver1`
inside the `build` distrobox (gdb is installed there). Look at the frames you dump (convert .ppm with ffmpeg).
Never open a visible window; keep runs short.

Method for every bug: find the root cause (compare with the original asm in asm_fn/ / tools/fnasm.py and the
D2 twin), fix it at the cause, and note it in port/README.md "Port notes" with one line. The classic causes so
far: a call that relied on a leftover register (sound.c func_0004E67C(idx)), PS1 stack/layout assumptions,
pointers into data that splat didn't mark, PsyCross gaps. When you find one cause, grep for the same pattern
elsewhere and fix those too.

## D - crash-fix into the game (owner: everything in src/ and port/pc/ except E's files)
Current crash: attract-mode demo, FindSeated (src/pedest.c:936) <- func_0005F0EC <- func_0005F3F8 <- main.c
func_000230B0, fault address 0x3F02371 (looks like a bad pointer, maybe from level data or an unmarked pointer).
Goal 1: the attract demo renders in-game frames (city, cars) and runs for 5+ minutes without crashing.
Goal 2: from the title screen, start a game (feed pad input - find how PsyCross reads pads, or add a D1_PAD
script env var in pc_main.c that injects button presses at given vblanks) and drive in Miami for a minute.
Report frames (paths to PNGs) at each milestone. Also check: "[Psy-X] Failed to map the buffer" (render).

## E - audio (owner: src/sound.c, src/gamesnd.c, src/xaplay.c, src/xmplay.c, port/psycross/src/audio/**,
## new port/pc/pc_xa.c; in port/pc/pc_cd.c only the XA branch that currently prints "XA audio stream ... ignored")
1. In attract mode 17 samples hit "SPU WARNING: sample has no end flag within the buffer" (PsyCross
   decodeSound, bound added by the coordinator). Find out why: wrong voice start addresses (SpuSetVoiceAttr
   use / address units / mask), wrong SpuWrite destination for the sound banks, or a PsyCross bug. Fix at the cause.
2. XA audio (speech/music streamed from the disc, CdlReadS + XA filter) is silent: implement it. Raw 2352-byte
   sectors are kept in pcdata for XA/STR files (see README). Decode XA ADPCM (psx-spx) and play through OpenAL
   (PsyCross already owns the OpenAL context) honouring the channel filter the game sets.
3. XM music (xmplay.c) runs on PsyCross's SPU emulation - check it plays (dump audio to a WAV under
   SDL_AUDIODRIVER=disk or an OpenAL loopback if you need to verify headlessly).
Report what you verified and how.

Report format: files changed, what each change is, what's verified, open issues, Requests (changes you need
in files you don't own - write them, don't make them).
