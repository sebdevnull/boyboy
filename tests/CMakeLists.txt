# ---- Test target name ----
set(BOYBOY_TESTS "${PROJECT_NAME}_tests")

# ---- Fetch GoogleTest ----
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Disable clang-tidy for gtest
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")

# ---- Test sources ----
set(TEST_SOURCES
    test_main.cpp
    helpers/rom_fixtures.cpp
    helpers/file_fixtures.cpp
    cartridge/test_cartridge.cpp
    cartridge/test_romonly.cpp
    cartridge/test_mbc.cpp
    mmu/test_mmu.cpp
    io/test_serial.cpp
    io/test_timer.cpp
    io/test_joypad.cpp
    ppu/test_ppu.cpp
    cpu/test_cpu.cpp
    cpu/test_state.cpp
    cpu/test_registers.cpp
    cpu/test_instruction_table.cpp
    cpu/test_instruction_stubs.cpp
    cpu/test_interrupts.cpp
    cpu/instructions/alu/inc_dec.cpp
    cpu/instructions/alu/add_sub.cpp
    cpu/instructions/alu/logic.cpp
    cpu/instructions/alu/misc.cpp
    cpu/instructions/load_store/ld_accumulator.cpp
    cpu/instructions/load_store/ld_r_r.cpp
    cpu/instructions/load_store/ld_r_n.cpp
    cpu/instructions/load_store/ld_r_hl.cpp
    cpu/instructions/load_store/ld_hl_r.cpp
    cpu/instructions/load_store/ld_hl_n.cpp
    cpu/instructions/load_store/push_pop.cpp
    cpu/instructions/load_store/ld_misc.cpp
    cpu/instructions/control_flow/jump.cpp
    cpu/instructions/control_flow/call_ret.cpp
    cpu/instructions/bit_ops/rotate_accumulator.cpp
    cpu/instructions/bit_ops/rotate.cpp
    cpu/instructions/bit_ops/shift.cpp
    cpu/instructions/bit_ops/swap.cpp
    cpu/instructions/bit_ops/bit.cpp
    cpu/instructions/bit_ops/res.cpp
    cpu/instructions/bit_ops/set.cpp
    cpu/instructions/misc/misc.cpp
    cpu/instructions/instructions_8bit.cpp
    cpu/instructions/instructions_16bit.cpp
    cpu/test_roms.cpp
    config/test_config.cpp
    files/test_utils.cpp
    files/test_io.cpp
)

# --- Options ---
option(TEST_CPU_STUBS "Enable CPU stubs testing" OFF)
if(TEST_CPU_STUBS)
    message(NOTICE "[INFO] CPU stubs testing enabled")
    add_compile_definitions(TEST_CPU_STUBS)
endif()

# --- Create test executable ---
add_executable(${BOYBOY_TESTS} ${TEST_SOURCES})
target_link_libraries(${BOYBOY_TESTS}
    PRIVATE
    ${BOYBOY_LIB}
    gtest
)

target_include_directories(${BOYBOY_TESTS}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/tests
)

# Output directories for tests
set_target_properties(${BOYBOY_TESTS} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/tests
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/tests/debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/tests/release
)

# Enable C++23
target_compile_features(${BOYBOY_TESTS} PRIVATE cxx_std_23)

# Link sanitizers for Debug builds (optional)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${BOYBOY_TESTS} PRIVATE -fsanitize=address,undefined)
    target_link_options(${BOYBOY_TESTS} PRIVATE -fsanitize=address,undefined)
endif()

# --- Register the test with CTest ---
add_test(NAME BoyBoyTests COMMAND ${BOYBOY_TESTS})

# --- Set working directory for relative paths ---
set_tests_properties(BoyBoyTests PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# --- Enable testing with CTest ---
enable_testing()
include(GoogleTest)
gtest_discover_tests(${BOYBOY_TESTS})
