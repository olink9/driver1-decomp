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
set(ENV{PKG_CONFIG_PATH} "/usr/lib/i386-linux-gnu/pkgconfig")
set(ENV{PKG_CONFIG_LIBDIR} "/usr/lib/i386-linux-gnu/pkgconfig:/usr/share/pkgconfig")

set(PSYCROSS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/psycross)
set(GAME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(OPENAL REQUIRED openal)

file(GLOB_RECURSE PSYCROSS_SRCS ${PSYCROSS_DIR}/src/*.c ${PSYCROSS_DIR}/src/*.cpp)
add_library(psycross STATIC ${PSYCROSS_SRCS})
target_include_directories(psycross PUBLIC ${PSYCROSS_DIR}/include ${PSYCROSS_DIR}/include/psx ${SDL2_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIRS})
target_link_directories(psycross PUBLIC ${SDL2_LIBRARY_DIRS} ${OPENAL_LIBRARY_DIRS})
target_link_libraries(psycross PUBLIC ${SDL2_LIBRARIES} ${OPENAL_LIBRARIES} dl m)

# game: main exe TUs + all overlays as plain code (NFMV.EXE's own main() is folded in later, see PORT.md)
file(GLOB GAME_SRCS ${GAME_DIR}/src/*.c
  ${GAME_DIR}/src/frontend/*.c ${GAME_DIR}/src/mcard/*.c ${GAME_DIR}/src/cred/*.c ${GAME_DIR}/src/iview/*.c)
file(GLOB PC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/pc/*.c)
add_executable(driver1 ${GAME_SRCS} ${PC_SRCS})
target_include_directories(driver1 PRIVATE ${GAME_DIR}/include ${GAME_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/pc)
target_compile_options(driver1 PRIVATE -include ${CMAKE_CURRENT_SOURCE_DIR}/pc/pc_compat.h
  $<$<COMPILE_LANGUAGE:C>:-std=gnu11> -fgnu89-inline -funsigned-char -fno-strict-aliasing -fwrapv -fcommon -w)
target_link_libraries(driver1 PRIVATE psycross)
