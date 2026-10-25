| # | file | idx | address range | fns | D2 anchors | conf | sdata / sbss (local syms) | evidence |
|---|---|---|---|---|---|---|---|---|
| 1 | clearmem.c | 0-0 | 00012498-0001250C | 1 | 1 | low | - / - | ClearMem (D2 SYSTEM.C) alone at the exe start; may belong to the next file |
| 2 | ai.c | 1-62 | 0001250C-0001B380 | 62 | 0 | low | 96774+9 / - | 62 unnamed AI fns (state handlers via fn-pointer tables at 30-39, "Driving on road %d" debug at 57); no D2 anchors; likely 2-3 files (cop/civ/route), best inner cuts at idx 56 (cut=2) and 14 |
| 3 | handling.c | 63-85 | 0001B380-000212F0 | 23 | 9 | medium | 96788+7 / 97000+9 | D2 HANDLING x13 + WHEELFORCES StepCars/StepOneCar + DR2ROADS MapHeight interleaved (D2 split these later); end at 86 is a weak minimum (cut 4) |
| 4 | roadbits.c | 86-97 | 000212F0-00022168 | 12 | 1 | low | 967B4+1 / - | ROADS_GetRouteData (D2 ROADBITS) + route helpers used by AI; boundaries 86 and 98 are soft |
| 5 | main.c | 98-109 | 00022168-00024754 | 12 | 3 | high | 967B8+20 / 9705C+10 | D2 MAIN.C LoadGameLevel/InitModelNames/main; level load + main loop func_0002360C |
| 6 | glaunch.c | 110-117 | 00024754-00026140 | 8 | 1 | medium | 96778+13 / 97068+8 | ReInitFrontend (D2 GLAUNCH) + FRONTEND.BIN/GLOAD launch code; 113 is the per-frame draw (could be main.c) |
| 7 | unpack.c | 118-119 | 00026140-00026794 | 2 | 0 | low | - / - | 2 fns, zero call/data crossings both sides (118 & 120); 119 ends with 13 nops of padding |
| 8 | tile.c | 120-149 | 00026794-000298B0 | 30 | 0 | low | 96938+7 / 97130+8 | DrawMapPSX helper cluster 120-149 (rooted at 140); D2 TILE/MDRAW analog |
| 9 | shadow.c | 150-162 | 000298B0-0002BDB0 | 13 | 6 | high | 96954+1 / 97160+6 | D2 SHADOW x7 (DrawTyreTracks..sQuad) + tyre-track adders 153-155 + CARSHAD init 157; zero cut at 163 |
| 10 | draw.c | 163-185 | 0002BDB0-00030198 | 23 | 6 | high | 96984+7 / 9707C+7 | D2 DRAW x9 (DrawMapPSX, InitFrustrumMatrix, ProcessMapLump...); 167 BCOLL3D match is weak (0.32) |
| 11 | mdraw.c | 186-211 | 00030198-000317B0 | 26 | 0 | low | - / - | 26 fns, mostly reached only by fn-pointer (poly/primitive handlers); split from draw.c at cut minimum 186 |
| 12 | models.c | 212-215 | 000317B0-00032DEC | 4 | 0 | low | 969B4+3 / 971CC+3 | DLOGO/GT/REFLEC lookup, "Model %s was not found", 2 more; zero cuts at 212 and 216 |
| 13 | spool.c | 216-247 | 00032DEC-000357B4 | 32 | 21 | high | 969E8+3 / 971E0+46 | 28 D2 SPOOL anchors in source-line order; zero cut at 216 |
| 14 | pad.c | 248-252 | 000357B4-00035B50 | 5 | 0 | medium | 967EC+4 / 972E0+2 | PadInitDirect + pad readers (D2 PAD.C) |
| 15 | misc_253.c | 253-254 | 00035B50-00035BC8 | 2 | 0 | low | - / - | 2 tiny fns with zero crossings each side |
| 16 | motion_c.c | 255-275 | 00035BC8-00039450 | 21 | 1 | medium | - / - | 21 NormalClip draw handlers incl. wjmDraw3 (D2 MOTION_C) |
| 17 | map.c | 276-288 | 00039450-0003A100 | 13 | 1 | medium | - / 972FC+2 | road/junction lump readers (MAX_ROADS), cell lookups, CheckUnpackNewRegions (D2 MAP.C) |
| 18 | map2.c | 289-291 | 0003A100-0003A698 | 3 | 0 | low | 96A3C+1 / - | 3 fns, zero cuts at 289 and 291; may be the tail of map.c or head of cars.c |
| 19 | cars.c | 292-300 | 0003A698-0003C998 | 9 | 3 | high | 96A44+4 / 973AC+2 | D2 CARS DrawCarWheels/DrawWheelObject/MangleWheelModels + DrawCar 292 |
| 20 | camera.c | 301-310 | 0003C998-0003E540 | 10 | 3 | high | 96A74+5 / 973BC+5 | D2 CAMERA x4 in line order |
| 21 | denting.c | 311-319 | 0003E540-0003F2E0 | 9 | 4 | high | 96A8C+3 / 973D0+5 | D2 DENTING x4; zero cut at 320 |
| 22 | bcollide.c | 320-325 | 0003F2E0-00040BAC | 6 | 4 | high | - / 977B0+1 | D2 BCOLLIDE x4; zero cut at 320 |
| 23 | debris.c | 326-380 | 00040BAC-0004A9F4 | 55 | 30 | high | 96AA4+58 / 973F0+30 | 48 D2 DEBRIS anchors; D2 SKY DrawLensFlare/DisplaySun sit INSIDE (D1 kept them in debris.c); zero cut at 381 |
| 24 | objcoll.c | 381-385 | 0004A9F4-0004C1C8 | 5 | 0 | medium | 96C50+1 / 974B0+1 | CheckScenaryCollisions-sized fn 383 + helpers (D2 OBJCOLL); zero cuts at 381 and 386 |
| 25 | gamesnd.c | 386-398 | 0004C1C8-0004D3DC | 13 | 2 | medium | 96C5C+6 / 97298+7 | D2 GAMESND x2 + VOICES.BLK, engine sounds; 386-388 have zero cuts (start uncertain by 1-3 fns) |
| 26 | sound.c | 399-431 | 0004D3DC-0004E948 | 33 | 14 | high | 96C74+4 / - | 32 D2 SOUND anchors in line order; ends at 431 (zero cut) |
| 27 | convert.c | 432-439 | 0004E948-0004ED34 | 8 | 2 | medium | - / - | D2 CONVERT x6; zero cuts 432-440 |
| 28 | sky.c | 440-449 | 0004ED34-000503F4 | 10 | 3 | low | - / 97090+7 | SKY.BIN load + PlotSkyPoly (D2 SKY) but ScaleCamera/Getlong (D2 CONVERT) are inside: either one file 432-449 or D1 moved them |
| 29 | loadview.c | 450-455 | 000503F4-00050E28 | 6 | 5 | medium | 96CB4+4 / 97504+3 | D2 LOADVIEW x5; 450 (CdInit, \DRIVER\%s;1) could be system.c |
| 30 | system.c | 456-466 | 00050E28-000516AC | 11 | 6 | high | 96CC8+1 / 974FC+7 | D2 SYSTEM Loadfile/LoadfileSeg/sector_ready/loadsectors; 462 LOADSAVE match weak (0.25) |
| 31 | texture.c | 467-481 | 000516AC-00052C10 | 15 | 5 | high | 96CD8+1 / 9712C+20 | D2 TEXTURE x8; 475-481 car/model texture setup kept here (cut 1) |
| 32 | objanim.c | 482-491 | 00052C10-000539C8 | 10 | 2 | high | 96D18+1 / - | D2 OBJANIM x2 + its callees |
| 33 | overmap.c | 492-524 | 000539C8-00058814 | 33 | 11 | medium | 96D1C+12 / 975B0+7 | D2 OVERMAP x9; OVERLAY DrawPercentageBar (514) and TARGETS DrawStopZone (519) are INSIDE (520-524 is the big map screen) |
| 34 | director.c | 525-553 | 00058814-0005C68C | 29 | 15 | medium | 96DA0+15 / 97604+21 | D2 DIRECTOR x20 with REPLAYS cjpRecord + REPLAY.BLK loader (547-549) inside; 525-527 uncertain |
| 35 | pres.c | 554-562 | 0005C68C-0005D164 | 9 | 6 | high | 96E30+1 / 97688+3 | D2 PRES x6 (fonts/strings) |
| 36 | civ_draw.c | 563-572 | 0005D164-0005F0EC | 10 | 1 | low | 96E34+3 / - | DrawCiv (D2 MOTION_C) + callers/helpers; second MOTION_C group, far from the first |
| 37 | pedest.c | 573-595 | 0005F0EC-00061214 | 23 | 3 | medium | - / 976BC+2 | D2 PEDEST x4 |
| 38 | players.c | 596-604 | 00061214-00061B9C | 9 | 0 | low | - / - | 9 fns used by DMS/cops (SquareRoot0 math); unknown |
| 39 | felony.c | 605-631 | 00061B9C-00066224 | 27 | 1 | low | 96E5C+4 / 976DC+24 | FELONY GetCarHeading-like (612), cop speech; 605-631 is the cop/felony cluster, inner cut unknown |
| 40 | job_fx.c | 632-635 | 00066224-00067838 | 4 | 1 | medium | 96E6C+2 / - | D2 JOB_FX DrawExplosion + explosion setup |
| 41 | misc_636.c | 636-643 | 00067838-000682C0 | 8 | 1 | low | - / 97784+2 | 636-643: GlobalTimeStep helpers and overmap Long2DDistance (D2 OVERMAP) - flagged |
| 42 | mission.c | 644-657 | 000682C0-0006ACC8 | 14 | 0 | medium | 967DC+2 / 9778C+3 | MISSION.BLK + DMS script interpreter func_00068C6C |
| 43 | overlay.c | 658-665 | 0006ACC8-0006BB24 | 8 | 1 | medium | 96E80+8 / - | D2 OVERLAY SetColourByValue + timer/HUD printers |
| 44 | handlers.c | 666-678 | 0006BB24-0006BDA4 | 13 | 0 | low | - / - | 13 tiny fns only reached via pointer table, zero cuts throughout |
| 45 | pause.c | 679-703 | 0006BDA4-0006D56C | 25 | 0 | medium | 96EFC+14 / 977B4+7 | pause/menu item handlers (no direct callers) + MC/memcard launch + pause loop 703 |
| 46 | fmvplay.c | 704-707 | 0006D56C-0006DD3C | 4 | 2 | high | 96F74+1 / 977D4+1 | D2 FMVPLAY ReInitSystem/PlayRender |
| 47 | ladder.c | 708-726 | 0006DD3C-0006F2DC | 19 | 0 | low | 96F88+2 / 977E0+3 | FB\%d\FB%d.FB, mladder.psx (D2 GLAUNCH mission ladder analog), 718 calls FRONTEND |
| 48 | xaplay.c | 727-735 | 0006F2DC-0006F81C | 9 | 2 | high | 96F94+3 / 977F4+6 | D2 XAPLAY x2 + XA file table |
| 49 | xmplay.c | 736-798 | 0006F81C-000738F0 | 63 | 52 | high | 96FA0+5 / 97810+14 | 59 D2 XMPLAY anchors in line order; zero cut at 736 |
