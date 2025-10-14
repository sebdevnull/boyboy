cmake_minimum_required(VERSION 3.27)

project(boyboy
    VERSION 0.4.0
    DESCRIPTION "BoyBoy: A Game Boy emulator written in C++"
    LANGUAGES CXX)

# --- Enable clang-tidy optionally ---
option(ENABLE_CLANG_TIDY "Enable clang-tidy analysis" OFF)
if(ENABLE_CLANG_TIDY)
    message(NOTICE "[INFO] clang-tidy enabled")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy-19;-p=${CMAKE_BINARY_DIR}")
endif()

# --- Set standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Fetch spdlog ---
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(spdlog)

# Disable clang-tidy for spdlog targets
set_target_properties(spdlog PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(spdlog_header_only PROPERTIES CXX_CLANG_TIDY "")

# --- Fetch toml++ ---
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

# --- Fetch CLI11 ---
FetchContent_Declare(
    cli11_proj
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(cli11_proj)

# --- System dependencies ---
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# --- Output directory for executables ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)

# --- Output directories for libraries ---
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib/release)

# ---- Targets ----
set(BOYBOY_LIB ${PROJECT_NAME}_lib)
set(BOYBOY_APP ${PROJECT_NAME})

# --- Options ---
option(CPU_STUBS "Enable CPU stubs" OFF)
if(CPU_STUBS)
    message(NOTICE "[INFO] CPU stubs enabled")
    add_compile_definitions(CPU_STUBS)
endif()

option(BOYBOY_DEBUG "Enable debug mode logs" OFF)
if(BOYBOY_DEBUG)
    message(NOTICE "[INFO] Debugging logs enabled")
    add_compile_definitions(BOYBOY_DEBUG)
endif()

option(LOG_LEVEL "Set log level (TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL, OFF)" "INFO")
if(LOG_LEVEL)
    message(NOTICE "[INFO] Log level set to ${LOG_LEVEL}")
    add_compile_definitions(LOG_LEVEL="${LOG_LEVEL}")
endif()

option(DISASSEMBLY_LOG "Enable instruction disassembly logs" OFF)
if(DISASSEMBLY_LOG)
    message(NOTICE "[INFO] Instruction disassembly logs enabled")
    add_compile_definitions(DISASSEMBLY_LOG)
endif()

option(ENABLE_PROFILING "Enable profiling" OFF)
if(ENABLE_PROFILING)
    message(NOTICE "[INFO] Profiling enabled")
    add_compile_definitions(ENABLE_PROFILING)
endif()

# --- Library target ---
add_library(${BOYBOY_LIB} STATIC)
target_sources(${BOYBOY_LIB} PRIVATE
    src/boyboy/common/utils.cpp
    src/boyboy/common/config/config_utils.cpp
    src/boyboy/common/config/config_validator.cpp
    src/boyboy/common/config/toml_config_loader.cpp
    src/boyboy/core/cpu/cpu.cpp
    src/boyboy/core/cpu/instructions.cpp
    src/boyboy/core/cpu/instructions_table.cpp
    src/boyboy/core/cpu/interrupt_handler.cpp
    src/boyboy/core/mmu/mmu.cpp
    src/boyboy/core/io/io.cpp
    src/boyboy/core/io/serial.cpp
    src/boyboy/core/io/timer.cpp
    src/boyboy/core/io/joypad.cpp
    src/boyboy/core/ppu/ppu.cpp
    src/boyboy/core/cartridge/cartridge.cpp
    src/boyboy/core/cartridge/cartridge_loader.cpp
    src/boyboy/core/cartridge/mbc.cpp
    src/boyboy/core/display/display.cpp
    src/boyboy/core/emulator/emulator.cpp
)

# --- Include directories ---
target_include_directories(${BOYBOY_LIB}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_BINARY_DIR}/generated
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/boyboy
)

# --- Link dependencies ---
target_link_libraries(${BOYBOY_LIB} PUBLIC
    spdlog::spdlog_header_only
    tomlplusplus::tomlplusplus
    SDL2::SDL2
    OpenGL::GL
)

# Ensure artifact name is clean
set_target_properties(${BOYBOY_LIB} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    POSITION_INDEPENDENT_CODE ON
)

# --- Executable ---
add_executable(${BOYBOY_APP}
    src/boyboy/main.cpp
    src/boyboy/frontend/cli/cli_app.cpp
    src/boyboy/frontend/cli/adapters/cli11_adapter.cpp
    src/boyboy/app/app.cpp
    src/boyboy/app/commands/run_command.cpp
    src/boyboy/app/commands/info_command.cpp
)
target_compile_options(${BOYBOY_APP} PRIVATE
    -Wall -Wextra -Wpedantic
)

# --- Link BoyBoy lib
target_link_libraries(${BOYBOY_APP} PRIVATE
    ${BOYBOY_LIB}
    CLI11::CLI11
)

# ---- Sanitizer option ----
option(ENABLE_SANITIZERS "Enable address/UB sanitizers" ON)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Sanitizers enabled")
    target_compile_options(${BOYBOY_LIB} PRIVATE -fsanitize=address,undefined)
    target_link_options(${BOYBOY_LIB} PRIVATE -fsanitize=address,undefined)
endif()

# ---- Build metadata ----
# Build type (Debug/Release/etc.)
if(CMAKE_BUILD_TYPE)
    set(BOYBOY_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
else()
    set(BOYBOY_BUILD_TYPE "Unknown")
endif()

# Compiler info
set(BOYBOY_COMPILER_ID "${CMAKE_CXX_COMPILER_ID}")
set(BOYBOY_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")

# Try to get current Git commit hash
find_package(Git QUIET)
if(Git_FOUND)
    # Short commit hash
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE BOYBOY_GIT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Detect dirty working tree
    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff --quiet
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_DIFF_RESULT
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff --cached --quiet
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_DIFF_CACHED_RESULT
    )

    if(NOT GIT_DIFF_RESULT EQUAL 0 OR NOT GIT_DIFF_CACHED_RESULT EQUAL 0)
        set(BOYBOY_GIT_COMMIT "${BOYBOY_GIT_COMMIT}-dirty")
    endif()

    # Current branch
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE BOYBOY_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Closest tag (if any)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE BOYBOY_GIT_TAG
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(BOYBOY_GIT_COMMIT "unknown")
    set(BOYBOY_GIT_BRANCH "unknown")
    set(BOYBOY_GIT_TAG "unknown")
endif()

# System info
set(BOYBOY_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}")
set(BOYBOY_SYSTEM_PROCESSOR "${CMAKE_SYSTEM_PROCESSOR}")

# CMake info
set(BOYBOY_CMAKE_VERSION "${CMAKE_VERSION}")

# Build timestamp
string(TIMESTAMP BOYBOY_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")

# Configure version header
configure_file(
    ${CMAKE_SOURCE_DIR}/include/boyboy/version.h.in
    ${CMAKE_BINARY_DIR}/generated/boyboy/version.h
    @ONLY
)

# --- Tests ---
enable_testing()
include(CTest)
if(BUILD_TESTING)
    message(NOTICE "[INFO] Tests enabled")
    set(INSTALL_GTEST OFF CACHE BOOL "Disable gtest install" FORCE)
    add_subdirectory(tests)
endif()
