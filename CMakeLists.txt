cmake_minimum_required(VERSION 3.16)
project(tlozooa C CXX)

include(CheckIPOSupported)
include(FetchContent)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif()
option(GBRECOMP_SHRINK "Aggressive size shrink (dead-strip + symbol strip)" ON)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3)
    endif()
    if(GBRECOMP_SHRINK)
        add_compile_options(-ffunction-sections -fdata-sections)
        add_link_options(-Wl,--gc-sections)
        if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_link_options(-s)
        endif()
    endif()
endif()

# ---------------------------------------------------------------------------
# gb-recompiled runtime
# ---------------------------------------------------------------------------
if(NOT TARGET gbrt)
    set(GBRT_REF "main" CACHE STRING "git ref of GB-Recomp/gb-recompiled to use")
    FetchContent_Declare(gb_recompiled
        GIT_REPOSITORY https://github.com/GB-Recomp/gb-recompiled.git
        GIT_TAG ${GBRT_REF}
        GIT_SHALLOW TRUE
    )
    FetchContent_GetProperties(gb_recompiled)
    if(NOT gb_recompiled_POPULATED)
        FetchContent_Populate(gb_recompiled)
    endif()
    add_subdirectory(${gb_recompiled_SOURCE_DIR}/runtime ${CMAKE_BINARY_DIR}/_gbrt_build)
endif()

# ---------------------------------------------------------------------------
# Cart sources
# ---------------------------------------------------------------------------
file(GLOB TLOZOOA_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tlozooa.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/tlozooa_main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/tlozooa_rom.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/tlozooa_dispatch_chunk_*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/tlozooa_funcs_*.c"
)

set(GBRECOMP_GENERATED_OPT_LEVEL "1" CACHE STRING "Optimization level for generated ROM source files")
set_source_files_properties(${TLOZOOA_SOURCES} PROPERTIES COMPILE_OPTIONS "-O${GBRECOMP_GENERATED_OPT_LEVEL}")

# ---------------------------------------------------------------------------
# tlozooa_cart: static library so umbrella compilations can link us in.
# ---------------------------------------------------------------------------
add_library(tlozooa_cart STATIC ${TLOZOOA_SOURCES})
target_include_directories(tlozooa_cart PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tlozooa_cart PUBLIC gbrt)

# ---------------------------------------------------------------------------
# Standalone launcher executable.
# ---------------------------------------------------------------------------
if(CMAKE_PROJECT_NAME STREQUAL "tlozooa")
    add_executable(tlozooa_launcher launcher_main.cpp)
    target_link_libraries(tlozooa_launcher PRIVATE tlozooa_cart gbrt)
    set_target_properties(tlozooa_launcher PROPERTIES OUTPUT_NAME tlozooa)
endif()
