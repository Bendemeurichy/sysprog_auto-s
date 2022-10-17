cmake_minimum_required(VERSION 3.0)
project(SPG LANGUAGES C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Uncomment to see compiler commands
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Detect if compiling on raspberry PI.
if ( EXISTS /opt/vc/include/bcm_host.h )
    message(STATUS "Note: Compiling on Raspberry Pi")
    add_definitions( -DRPI -DIS_RPI )
    set(RPI True)
else()
    message(STATUS "Note: Compiling on NON Raspberry Pi")
endif()

if ( RPI )
    # Find OpenGL (and other libraries) on Raspberry Pi
    list(APPEND CMAKE_PREFIX_PATH "/usr/lib/arm-linux-gnueabihf/")
    #list(APPEND CMAKE_PREFIX_PATH "/opt/vc/lib/")
endif()

if (NOT CMAKE_COMPILER_IS_GNUCC)
    message(FATAL_ERROR "You need to build with GCC")
endif()

# according to https://gcc.gnu.org/projects/cxx-status.html we need at least 8 for full C++17 support
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
   message(FATAL_ERROR "You need at least gcc 8  (found gcc version ${CMAKE_CXX_COMPILER_VERSION})")
endif()


set(OpenGL_GL_PREFERENCE GLVND)
set(SYSTEM_TYPE x64)
set(SDL2_FOLDER_LOCATION ${CMAKE_SOURCE_DIR}/libraries/SDL2/x86_64-w64-mingw32/  )
set(GLMC_FOLDER_LOCATION ${CMAKE_SOURCE_DIR}/libraries/glmc)
set(GLM_FOLDER_LOCATION ${CMAKE_SOURCE_DIR}/libraries/glm)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# Note: consider -Werror instead of -Wall to make all warnings fatal
#set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -g -std=gnu11 -Wall -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unknown-pragmas -fms-extensions -fdiagnostics-color -O0")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -g -std=gnu11 -Wall -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unknown-pragmas -fms-extensions -fdiagnostics-color -Og")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unknown-pragmas -fms-extensions -fdiagnostics-color -pthread -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unknown-pragmas -fms-extensions -fdiagnostics-color -pthread -Og")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")

# architecture settings:
#   Specific for RPi3:            -march=armv8-a -mcpu=cortex-a53 -mfpu=neon-vfpv4
#   Specific for RPi4:            -march=armv8-a+crc+simd -mcpu=cortex-a72 -mfpu=vfp
#   Generic for all Pi's :        -march=armv6 -mcpu=arm1176jzf-s        (but doesn't seem to be correct)
#   Automatic specific for host:  -march=native -mcpu=native -mfpu=auto  (doesn't work for older gcc om ARM)
if ( RPI )
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -march=native -mcpu=native -mfpu=auto")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mcpu=native -mfpu=auto")
else()
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -march=native")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif( RPI )

# Enable the address sanitizer on Mac and Linux.
# Turn this off by setting the `NO_ASAN` environment variable to `1`
#   (cmake -DNO_ASAN=1 .)
# Cache the environment variable
if (NOT "$ENV{NO_ASAN}" STREQUAL "")
    if ("$ENV{NO_ASAN}" STREQUAL "1")
        set(NO_ASAN "$ENV{NO_ASAN}" CACHE INTERNAL "Copied from environment variable")
    elseif ("$ENV{NO_ASAN}" STREQUAL "0")
        unset (NO_ASAN CACHE)
    endif()
endif()
if(NOT WIN32 AND NOT NO_ASAN)
    message("ASAN enabled: To disable, run: NO_ASAN=1 cmake .")
    if(EXISTS "/etc/arch-release")
        message(STATUS "Note: Compiling with dynamic asan address sanitization and leak checking (\"Arch, as a rule, doesn't include static libs\")")
        set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS}      -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS}    -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    else()
        message(STATUS "Note: Compiling with asan address sanitization and leak checking")
        set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS}      -fsanitize=address -static-libasan -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS}    -fsanitize=address -static-libasan -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -static-libasan -fno-omit-frame-pointer")
    endif()
else()
    message("ASAN disabled: To enable, run: NO_ASAN=0 cmake .")
endif()


#
# Build lists with certain groups of source files.
# We will use these lists later.
#

file(GLOB es_SRC
        "game/es/*.h"
        "game/es/*.c"
        )

file(GLOB es_system_SRC
        "game/es/system/*.h"
        "game/es/system/*.c"
        )

file(GLOB graphics_SRC
        "game/graphics/*.h"
        "game/graphics/*.c"
        )

file(GLOB level_SRC
        "game/level/*.h"
        "game/level/*.c"
        )

file(GLOB util_C_SRC
        "util/util.h"
        "util/util.c"
        "util/sleep.h"
        "util/sleep.c"
        "util/glm_helper.h"
        "util/glm_helper.cpp"
        )

file(GLOB glmc_lib_SRC
        "libraries/glmc/include/*.h"
        "libraries/glmc/src/*.c"
        )

file(GLOB glm_lib_SRC
        "libraries/glm/glm/*.hpp"
        )

file(GLOB carbrain_SRC
        "game/carbrain/*.h"
        "game/carbrain/*.c"
        )

file(GLOB carbrain_c_SRC
        "game/carbrain/part1/*.h"
        "game/carbrain/part1/*.c"
        )

file(GLOB carbrain_cpp_SRC
        "game/carbrain/part2/*.h"
        "game/carbrain/part2/*.cpp"
        )

file(GLOB tests_part2_SRC
        "tests/part2/*.h"
        "tests/part2/*.cpp"
        )

file(GLOB tests_part1_SRC
        "tests/part1/*.h"
        "tests/part1/*.c"
        )

#
# Define which files are used to create which executable
#


set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${level_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${es_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${es_system_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${util_C_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${graphics_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${glmc_lib_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${carbrain_SRC})

set(GAME_PART1_TEST_FILES ${GAME_PART1_TEST_FILES} ${tests_part1_SRC})

set(GAME_PART2_SRC_FILES ${GAME_PART2_SRC_FILES} ${carbrain_cpp_SRC})
set(GAME_PART2_SRC_FILES ${GAME_PART2_SRC_FILES} ${glm_lib_SRC})

set(GAME_PART2_TEST_FILES ${GAME_PART2_TEST_FILES} ${tests_part2_SRC})


add_executable(spg_part1 spg_game.c ${GAME_PART1_SRC_FILES} ${carbrain_c_SRC})
add_executable(spg_part1_edit spg_edit.c ${GAME_PART1_SRC_FILES} ${carbrain_c_SRC})
add_executable(spg_part1_tests ${GAME_PART1_SRC_FILES} ${carbrain_c_SRC} ${GAME_PART1_TEST_FILES})

add_executable(spg_part2 spg_game.c ${GAME_PART2_SRC_FILES} ${GAME_PART1_SRC_FILES})
add_executable(spg_part2_tests spg_part2_test_main.cpp ${GAME_PART1_SRC_FILES} ${GAME_PART2_SRC_FILES} ${GAME_PART2_TEST_FILES})


add_executable(glmc_demo glmc_demo_main.c ${glmc_lib_SRC})
add_executable(glm_demo glm_demo_main.cpp util/glm_helper.cpp ${glm_lib_SRC})

target_compile_options(spg_part2_tests PRIVATE -DONLY_IN_PART2_TESTS)


#
# Now find the required dependencies (libraries) and use them when linking the executables
#
if(WIN32)
    message( STATUS "Windows uses included SDL2 libs")
    # This is needed because the default SDL2 lib hides stdout or stderr
    set(SDL2_INCLUDE_DIR ${SDL2_FOLDER_LOCATION}/include/SDL2)
    set(SDL2_LIBRARIES ;${SDL2_FOLDER_LOCATION}/lib/libSDL2main.a;${SDL2_FOLDER_LOCATION}/lib/libSDL2.dll.a)

    set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_FOLDER_LOCATION}/include)
    set(SDL2_IMAGE_LIBRARIES ;${SDL2_FOLDER_LOCATION}/lib/libSDL2_image.dll.a)

    add_definitions( -DSDL_MAIN_HANDLED )
else()
    if ( RPI )
        set(SDL2_DIR ${SDL2_DIR} "/usr/lib/arm-linux-gnueabihf/cmake/SDL2/")
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/lib/arm-linux-gnueabihf/cmake/SDL2/")
    endif( RPI )
    message( STATUS "*nix SDL2 image check")
    message( STATUS "CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
    find_package(SDL2 REQUIRED)
    if(NOT SDL2_FOUND)
        if(EXISTS "/usr/bin/apt-get")
            message( FATAL ERROR "SDL2 not found! Please run: apt-get install libsdl2-dev libsdl2-image-dev")
        else()
        message( FATAL ERROR "SDL2 not found!")
        endif()
    endif(NOT SDL2_FOUND)

find_package(SDL2_image REQUIRED)

if(NOT SDL2IMAGE_FOUND)
        if(EXISTS "/usr/bin/apt-get")
            message( FATAL ERROR "SDL2 image not found! Please run: apt-get install libsdl2-image-dev")
        else()
    message( FATAL ERROR "SDL2 image not found!")
        endif()
endif(NOT SDL2IMAGE_FOUND)
endif(WIN32)

set(GLMC_INCLUDE_DIR ${GLMC_FOLDER_LOCATION}/include)
if (NOT EXISTS ${GLMC_INCLUDE_DIR}/glmc.h )
    message( FATAL_ERROR "GLMC header not found at ${GLMC_INCLUDE_DIR}/glmc.h!")
endif()

set(GLMC_SOURCE_DIR ${GLMC_FOLDER_LOCATION}/src)
if (NOT EXISTS ${GLMC_SOURCE_DIR}/glmc_model.c )
    message( FATAL_ERROR "GLMC source not found at ${GLMC_SOURCE_DIR}/glmc_model.c!")
endif()

set(GLM_INCLUDE_DIR ${GLM_FOLDER_LOCATION})
if (NOT EXISTS ${GLM_INCLUDE_DIR}/glm/vec3.hpp )
    message( FATAL_ERROR "GLM vec3 header not found at ${GLM_INCLUDE_DIR}/glm/vec3.hpp!")
endif()

include_directories(${GLMC_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})

if ( NOT TARGET SDL2::SDL2 )
    message( STATUS "Using SDL2_LIBRARIES=${SDL2_LIBRARIES}")
    if(NOT WIN32)
        string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)  # this is needed on ubuntu 16.04  (due to a broken local SDL cmake file)
    endif()
    target_link_libraries (spg_part1 ${SDL2_LIBRARIES})
    target_link_libraries (spg_part1_edit ${SDL2_LIBRARIES})
    target_link_libraries (spg_part1_tests ${SDL2_LIBRARIES})
    target_link_libraries (spg_part2 ${SDL2_LIBRARIES})
    target_link_libraries (spg_part2_tests ${SDL2_LIBRARIES})
else()
    message( STATUS "TARGET SDL2::SDL2 found, SDL2_LIBRARIES=${SDL2_LIBRARIES}")
    target_link_libraries (spg_part1 SDL2::SDL2)
    target_link_libraries (spg_part1_edit SDL2::SDL2)
    target_link_libraries (spg_part1_tests SDL2::SDL2)
    target_link_libraries (spg_part2 SDL2::SDL2)
    target_link_libraries (spg_part2_tests SDL2::SDL2)
endif()

if ( NOT TARGET SDL2::SDL2_image )
    message( STATUS "Using SDL2_IMAGE_LIBRARIES=${SDL2_IMAGE_LIBRARIES}")
    target_link_libraries (spg_part1 ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries (spg_part1_edit ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries (spg_part1_tests ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries (spg_part2 ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries (spg_part2_tests ${SDL2_IMAGE_LIBRARIES})
else()
    message( STATUS "TARGET SDL2::SDL2_image found, SDL2_IMAGE_LIBRARIES=${SDL2_IMAGE_LIBRARIES}")
    target_link_libraries (spg_part1 SDL2::SDL2_image)
    target_link_libraries (spg_part1_edit SDL2::SDL2_image)
    target_link_libraries (spg_part1_tests SDL2::SDL2_image)
    target_link_libraries (spg_part2 SDL2::SDL2_image)
    target_link_libraries (spg_part2_tests SDL2::SDL2_image)
endif()

if ( TARGET SDL2::SDL2main )
    message( STATUS "TARGET SDL2::SDL2main found")
    target_link_libraries (spg_part1 SDL2::SDL2main)
    target_link_libraries (spg_part1_edit SDL2::SDL2main)
    target_link_libraries (spg_part1_tests SDL2::SDL2main)
    target_link_libraries (spg_part2 SDL2::SDL2main)
    target_link_libraries (spg_part2_tests SDL2::SDL2main)
else()
    message( STATUS "No TARGET SDL2::SDL2main")
endif()

target_link_libraries (glmc_demo)
target_link_libraries (glm_demo)

if( NOT WIN32 )
    target_link_libraries (spg_part1 m)
    target_link_libraries (spg_part1_edit m)
    target_link_libraries (spg_part1_tests m)
    target_link_libraries (spg_part2 m stdc++fs)
    target_link_libraries (glmc_demo m)
    target_link_libraries (glm_demo m)
    target_link_libraries (spg_part2_tests m stdc++fs)
endif()

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
target_link_libraries(spg_part2_tests ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
