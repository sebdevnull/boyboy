cmake_minimum_required(VERSION 3.27)

project(boyboy
        VERSION 0.1.0
        DESCRIPTION "BoyBoy: A Game Boy emulator written in C++"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Output directory for executables ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)

# --- Sources and headers ---
file(GLOB SOURCES "src/*.cpp")
file(GLOB INCLUDES "include/*.h")

# --- System dependencies ---
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# --- Executable ---
add_executable(boyboy ${SOURCES} ${INCLUDES})

# --- Include directories ---
target_include_directories(boyboy PUBLIC
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include)

# --- Link libraries ---
target_link_libraries(boyboy
        glfw
        ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES})

# --- Sanitizers for Debug build ---
if(CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_options(boyboy PRIVATE -fsanitize=address,undefined)
        target_link_options(boyboy PRIVATE -fsanitize=address,undefined)
endif()
