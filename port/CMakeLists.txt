# Driver 1 PC port (PsyCross). 32-bit x86 first: every struct keeps its PS1 layout (pointers are 4 bytes),
# so data read from the disc and the many int<->pointer casts work unchanged. 64-bit is a later step.
# Build: distrobox enter build -- cmake -S port -B ../driver1-work/pcbuild/b && make -C ../driver1-work/pcbuild/b
cmake_minimum_required(VERSION 3.16)
project(driver1_pc C CXX)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
if(NOT WIN32)
  set(ENV{PKG_CONFIG_PATH} "/usr/lib/i386-linux-gnu/pkgconfig")
  set(ENV{PKG_CONFIG_LIBDIR} "/usr/lib/i386-linux-gnu/pkgconfig:/usr/share/pkgconfig")
endif()

set(PSYCROSS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/psycross)
# PS1 primitive layout everywhere (game AND PsyCross must agree): 4-byte P_TAG with a 24-bit next pointer, no PGXP.
# This is the config of include/psyq/PsyX/PsyX_config.h; PsyCross's own default (extended 32-bit tags + PGXP)
# would change every primitive struct. 24-bit OT links mean every primitive must sit below 16 MB, see the link
# options below and port/pc/pc_main.c (PS1 RAM mapped at its own addresses).
add_compile_definitions(USE_EXTENDED_PRIM_POINTERS=0 USE_PGXP=0)
set(GAME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(OPENAL REQUIRED openal)

file(GLOB_RECURSE PSYCROSS_SRCS ${PSYCROSS_DIR}/src/*.c ${PSYCROSS_DIR}/src/*.cpp)
# the PsyQ library replacements are upper-case src/psx/*.C (LIBCD.C, LIBGPU.C ...). gcc compiles .C as C++ (their
# headers carry extern "C" blocks for exactly that), which is how REDRIVER2 builds them on Linux; the plain "*.c"
# glob above misses them on a case-sensitive file system. -fpermissive/-Wno-narrowing: rcossin_tbl.h etc.
file(GLOB_RECURSE PSYCROSS_SRCS_UC ${PSYCROSS_DIR}/src/*.C)
set_source_files_properties(${PSYCROSS_SRCS_UC} PROPERTIES LANGUAGE CXX COMPILE_OPTIONS "-fpermissive;-Wno-narrowing")
add_library(psycross STATIC ${PSYCROSS_SRCS} ${PSYCROSS_SRCS_UC})
target_include_directories(psycross PUBLIC ${PSYCROSS_DIR}/include ${PSYCROSS_DIR}/include/psx ${SDL2_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIRS})
target_link_directories(psycross PUBLIC ${SDL2_LIBRARY_DIRS} ${OPENAL_LIBRARY_DIRS})
if(WIN32)
  target_link_libraries(psycross PUBLIC ${SDL2_LIBRARIES} ${OPENAL_LIBRARIES} m)
else()
  target_link_libraries(psycross PUBLIC ${SDL2_LIBRARIES} ${OPENAL_LIBRARIES} dl m)
endif()

# game: main exe TUs + all overlays as plain code, + NFMV.EXE (the FMV player; its main() is nfmv_main on PC,
# run in-process by port/pc/pc_fmv.c)
file(GLOB GAME_SRCS ${GAME_DIR}/src/*.c
  ${GAME_DIR}/src/frontend/*.c ${GAME_DIR}/src/mcard/*.c ${GAME_DIR}/src/cred/*.c ${GAME_DIR}/src/iview/*.c
  ${GAME_DIR}/src/nfmv/*.c)
file(GLOB PC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/pc/*.c)

# Pointer fixups for the PS1 images loaded into PS1 RAM (port/pc/pc_gendata.py; README.md)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(PC_GEN ${CMAKE_CURRENT_SOURCE_DIR}/pc/pc_gendata.py)
if(WIN32)
  set(PC_GEN_PE --pe)   # PE/COFF objects: leading underscores, no symbol sizes, the header shadow
endif()
file(GLOB PS1_DATA_ASM ${GAME_DIR}/asm/data/*.s ${GAME_DIR}/asm/frontend/data/*.s ${GAME_DIR}/asm/mcard/data/*.s
  ${GAME_DIR}/asm/cred/data/*.s ${GAME_DIR}/asm/iview/data/*.s)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pc_fixups.s
  COMMAND ${Python3_EXECUTABLE} ${PC_GEN} fixups ${GAME_DIR} ${CMAKE_CURRENT_BINARY_DIR}/pc_fixups.s ${PC_GEN_PE}
  DEPENDS ${PC_GEN} ${PS1_DATA_ASM} VERBATIM)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/pc_fixups.s PROPERTIES LANGUAGE C COMPILE_OPTIONS "-xassembler")

add_library(d1obj OBJECT ${GAME_SRCS} ${PC_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/pc_fixups.s)
target_include_directories(d1obj PRIVATE ${GAME_DIR}/include ${GAME_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/pc
  ${PSYCROSS_DIR}/include ${PSYCROSS_DIR}/include/psx ${SDL2_INCLUDE_DIRS})
target_compile_options(d1obj PRIVATE -include ${CMAKE_CURRENT_SOURCE_DIR}/pc/pc_compat.h
  $<$<COMPILE_LANGUAGE:C>:-std=gnu11> -fgnu89-inline -funsigned-char -fno-strict-aliasing -fwrapv -w
  -fno-pie -fno-stack-protector -fno-delete-null-pointer-checks
  # each file's own data is defined in PS1 address order and the game reads across neighbours (xmplay.c:
  # D_0008D218[-1] is D_0008D208[7]): keep definition order, PS1 (psABI) alignment, `= 0` data in .data,
  # and no commons (the linker would scatter them)
  -fno-toplevel-reorder -malign-data=abi -fno-zero-initialized-in-bss -fno-common)
if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 14)
  # GCC 14 made the decompiled code's int<->pointer mixes errors; they are what the PS1 code does
  target_compile_options(d1obj PRIVATE $<$<COMPILE_LANGUAGE:C>:-fpermissive>)
endif()
if(WIN32)
  # COFF has no symbol sizes: one section per object gives pc_gendata.py --pe each object's size
  target_compile_options(d1obj PRIVATE -fdata-sections)
endif()

if(WIN32)
  set(LIBC_NAME libmsvcrt.a)
else()
  set(LIBC_NAME libc.so.6)
endif()
execute_process(COMMAND ${CMAKE_C_COMPILER} -m32 -print-file-name=${LIBC_NAME} OUTPUT_VARIABLE LIBC_SO
  OUTPUT_STRIP_TRAILING_WHITESPACE)
find_library(SDL2_SO NAMES SDL2 SDL2-2.0 HINTS ${SDL2_LIBRARY_DIRS})
# Every symbol the objects leave undefined -> PsyQ implementation / owner+offset / PS1 address (pc_syms.ld)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pc_syms.ld
  COMMAND ${Python3_EXECUTABLE} ${PC_GEN} syms ${GAME_DIR} ${CMAKE_CURRENT_BINARY_DIR}/pc_syms.ld $<TARGET_OBJECTS:d1obj>
    --lib=$<TARGET_FILE:psycross> --lib=${LIBC_SO} --lib=${SDL2_SO} ${PC_GEN_PE}
  DEPENDS ${PC_GEN} $<TARGET_OBJECTS:d1obj> psycross COMMAND_EXPAND_LISTS VERBATIM)
add_custom_target(d1syms DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pc_syms.ld)
add_dependencies(d1syms d1obj)

add_executable(driver1 $<TARGET_OBJECTS:d1obj>)
add_dependencies(driver1 d1syms)
set_target_properties(driver1 PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pc_syms.ld)
target_link_libraries(driver1 PRIVATE psycross)
# Non-PIE exe at 4 MB: the game's own globals (primitives in .bss/.data that get linked into OTs) need addresses
# below 16 MB; 0x10000-0x1FFFFF is the PS1 RAM map (pc_main.c). pc_syms.ld is read as an implicit linker script.
# VSync is wrapped: the vblank is where pending CD/MDEC work completes (pc_main.c, pc_cd.c).
if(WIN32)
  # Windows fills the low address space with its own mappings before main() runs, so PS1 RAM can't be mapped
  # there later: it is part of the exe image instead. Image base 0x10000, a 2 MB uninitialised first section
  # (.psxram, port/pc/pc_main.c) up to 0x200000, the program above it. The first 0x400 bytes stay the PE headers
  # (the loader reads them whenever a DLL loads); that part of the PS1 image has a shadow copy (pc_gendata.py).
  # The linker script is the toolchain's own default with .psxram put first.
  execute_process(COMMAND ${CMAKE_C_COMPILER} -Wl,--verbose OUTPUT_VARIABLE LD_VERBOSE ERROR_QUIET)
  string(REGEX REPLACE ".*\n=+\n(.*)\n=+\n.*" "\\1" LD_DEFAULT "${LD_VERBOSE}")
  string(REGEX REPLACE "\n  \\.text  __image_base__[^\n]*:\n"
    "\n  .psxram __image_base__ + 0x1000 (NOLOAD) : { *(.psxram) }\n  .text BLOCK(__section_alignment__) :\n"
    LD_PSX "${LD_DEFAULT}")
  if(LD_PSX STREQUAL LD_DEFAULT)
    message(FATAL_ERROR "could not place .psxram in the default linker script (ld --verbose changed?)")
  endif()
  # -fdata-sections (d1obj, below) puts each object in its own .data$NAME/.rdata$NAME/.bss$NAME: keep them in
  # input (= definition) order instead of sorted by name, so neighbours stay neighbours as on Linux
  string(REPLACE "*(SORT(.data$*))" "*(.data$*)" LD_PSX "${LD_PSX}")
  string(REPLACE "*(SORT(.rdata$*))" "*(.rdata$*)" LD_PSX "${LD_PSX}")
  string(REGEX REPLACE "\n    \\*\\(\\.bss\\)\n" "\n    *(.bss .bss$*)\n" LD_PSX "${LD_PSX}")
  # ld takes neither an implicit script next to -T nor an INCLUDE of one here ("appears multiple times"), so
  # pc_syms.ld is appended to the script: pc_link.x
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/pc_pe.x "${LD_PSX}
")
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pc_link.x
    COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/pc_pe.x ${CMAKE_CURRENT_BINARY_DIR}/pc_syms.ld
      > ${CMAKE_CURRENT_BINARY_DIR}/pc_link.x
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pc_pe.x ${CMAKE_CURRENT_BINARY_DIR}/pc_syms.ld VERBATIM)
  add_custom_target(d1linkx DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pc_link.x)
  add_dependencies(d1linkx d1syms)
  add_dependencies(driver1 d1linkx)
  set_target_properties(driver1 PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pc_link.x)
  target_link_options(driver1 PRIVATE -Wl,--image-base=0x10000 -Wl,--disable-dynamicbase -Wl,--wrap=VSync
    -Wl,-T,${CMAKE_CURRENT_BINARY_DIR}/pc_link.x)
  # pc_trace.c's symbol lookup: the exe's symbol table as a text file next to it (PE keeps no symbol sizes)
  add_custom_command(TARGET driver1 POST_BUILD
    COMMAND ${Python3_EXECUTABLE} ${PC_GEN} symfile $<TARGET_FILE:driver1> $<TARGET_FILE_DIR:driver1>/driver1.sym --pe
    VERBATIM)
else()
  target_link_options(driver1 PRIVATE -no-pie -Wl,-Ttext-segment=0x400000 -Wl,--wrap=VSync
    ${CMAKE_CURRENT_BINARY_DIR}/pc_syms.ld)
endif()
