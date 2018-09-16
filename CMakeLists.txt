cmake_minimum_required(VERSION 3.9)
project(boyboy)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")

file(GLOB SOURCES
        "src/*.cpp")

file(GLOB INCLUDES
        "include/*.h")

#file(GLOB GLFW
#        "/usr/local/include/GLFW/*.h")
#file(GLOB GLEW
#        "/usr/include/GL/*.h")

find_package (glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


add_executable(boyboy ${SOURCES} ${INCLUDES} ${GLFW} ${GLEW})
target_include_directories(boyboy PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(boyboy
        glfw
        ${OPENGL_gl_LIBRARY})
target_include_directories(boyboy PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(boyboy ${GLEW_LIBRARIES})