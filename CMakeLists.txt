cmake_minimum_required(VERSION 3.25)
project(progV8)

macro(progp_detect_architecture)
    message("PROGP_ARCH: ${CMAKE_SYSTEM_NAME}")
    message("CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")

    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
        set(PROGP_ARCH "darwin_arm64")
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND ${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
        set(PROGP_ARCH "linux_x86_64")

        SET (CMAKE_C_COMPILER   "/usr/bin/clang")
        SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")

        SET (CMAKE_AR      "/usr/bin/llvm-ar")
        SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
        SET (CMAKE_NM      "/usr/bin/llvm-nm")
        SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
        SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
    else()
        set(PROGP_ARCH "unknown")
    endif()
endmacro()

macro(progp_find_v8)
    find_library (
            V8_LIB
            NAMES v8 libv8                      # what to look for
            HINTS ${PROGP_LIB_DIR}              # where to look
            NO_DEFAULT_PATH                     # do not search system default paths
            REQUIRED
    )

    message("V8_LIB=${V8_LIB}")

    set(V8_COMPILE_OPTIONS -DV8_COMPRESS_POINTERS -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_ENABLE_SANDBOX)
endmacro()

macro(progp_set_vars)
    set(CMAKE_CXX_STANDARD 17)

    progp_detect_architecture()

    FILE(GLOB PROGP_SOURCES_H                "${CMAKE_SOURCE_DIR}/*.h")
    FILE(GLOB PROGP_SOURCES_CPP              "${CMAKE_SOURCE_DIR}/*.cpp")

    set(PROGP_LIB_DIR                       "${CMAKE_SOURCE_DIR}/libs/${PROGP_ARCH}")
    set(PROGP_INCLUDE_DIR                   "${CMAKE_SOURCE_DIR}/include")
    set(PROGP_BIN_DIR                       "${CMAKE_SOURCE_DIR}/bin/${PROGP_ARCH}")

    #set(CMAKE_LIBRARY_OUTPUT_DIRECTORY      ${PROGP_LIB_DIR})
    #set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY      ${PROGP_LIB_DIR})
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY      ${PROGP_BIN_DIR})

    set(PROGP_COMPILE_OPTIONS               -Wall -fno-rtti -fPIC -std=c++17)
    set(PROGP_INCLUDE_SEARCH                ${PROGP_INCLUDE_DIR} "${PROGP_INCLUDE_DIR}/v8")
endmacro()

macro(progp_init)
    progp_set_vars()
    progp_find_v8()
endmacro()

macro(progp_printInfos)
    message("PROGP_ARCH: ${PROGP_ARCH}")
    message("PROGP_SOURCES_H: ${PROGP_SOURCES_H}")
    message("PROGP_SOURCES_CPP: ${PROGP_SOURCES_CPP}")
    message("PROGP_LIB_DIR: ${PROGP_LIB_DIR}")
    message("V8_COMPILE_OPTIONS: ${V8_COMPILE_OPTIONS}")
    message("PROGP_INCLUDE_SEARCH: ${PROGP_INCLUDE_SEARCH}")
endmacro()

macro(progp_buildAsExecutable)

    add_executable(progpV8)
    #
    target_sources(progpV8 PRIVATE ${PROGP_SOURCES_CPP})
    target_link_libraries(progpV8 PUBLIC pthread -lv8 -L${PROGP_LIB_DIR})
    target_compile_options(progpV8 PUBLIC ${PROGP_COMPILE_OPTIONS} ${V8_COMPILE_OPTIONS} -DPROGP_STANDALONE)
    target_include_directories(progpV8 PRIVATE ${PROGP_INCLUDE_SEARCH})

endmacro()

progp_init()
#progp_printInfos()
progp_buildAsExecutable()

# To use Clang in Ubuntu:
# sudo update-alternatives --config c++
# Once done select Clang




