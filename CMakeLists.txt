cmake_minimum_required(VERSION 3.27)

project(boyboy
    VERSION 0.1.0
    DESCRIPTION "BoyBoy: A Game Boy emulator written in C++"
    LANGUAGES CXX)

# --- Set standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- System dependencies ---
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# --- Output directory for executables ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)

# --- Output directories for libraries ---
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# ---- Targets ----
set(BOYBOY_LIB ${PROJECT_NAME}_lib)
set(BOYBOY_APP ${PROJECT_NAME})

# --- Library target ---
add_library(${BOYBOY_LIB} STATIC)
target_sources(${BOYBOY_LIB} PRIVATE
    src/cartridge.cpp
    src/cpu.cpp
    src/display.cpp
    src/mmu.cpp
)

# --- Include directories ---
target_include_directories(${BOYBOY_LIB} PUBLIC
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include)

# Ensure artifact name is clean
set_target_properties(${BOYBOY_LIB} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    POSITION_INDEPENDENT_CODE ON
)

# --- Executable ---
add_executable(${BOYBOY_APP} src/main.cpp)
target_compile_options(${BOYBOY_APP} PRIVATE
    -Wall -Wextra -Wpedantic
)

# --- Link libraries ---
target_link_libraries(${BOYBOY_APP} PRIVATE ${BOYBOY_LIB}
    glfw
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES})

# ---- Sanitizer option ----
option(ENABLE_SANITIZERS "Enable address/UB sanitizers" ON)

if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Sanitizers enabled")

    target_compile_options(${BOYBOY_LIB} PRIVATE -fsanitize=address,undefined)
    target_link_options(${BOYBOY_LIB} PRIVATE -fsanitize=address,undefined)
endif()

# --- Tests ---
enable_testing()
include(CTest)
if(BUILD_TESTING)
    set(INSTALL_GTEST OFF CACHE BOOL "Disable gtest install" FORCE)
    add_subdirectory(tests)
endif()
