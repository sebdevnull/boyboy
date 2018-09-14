cmake_minimum_required(VERSION 3.9)
project(boyboy)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")

file(GLOB SOURCES
        "src/*.cpp")

file(GLOB INCLUDES
        "include/*.h")


add_executable(boyboy ${SOURCES} ${INCLUDES})