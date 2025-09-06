# ---- Test target name ----
set(BOYBOY_TESTS "${PROJECT_NAME}_tests")

# ---- Fetch GoogleTest ----
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
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

# --- Collect all test source files ---
file(GLOB TEST_SOURCES "*.cpp")

# --- Create test executable ---
add_executable(${BOYBOY_TESTS} ${TEST_SOURCES})
target_link_libraries(${BOYBOY_TESTS}
    PRIVATE
    ${BOYBOY_LIB}
    gtest_main
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

# --- Enable testing with CTest ---
enable_testing()
include(GoogleTest)
gtest_discover_tests(${BOYBOY_TESTS})
