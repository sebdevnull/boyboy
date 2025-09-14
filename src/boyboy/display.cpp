/**
 * @file display.cpp
 * @brief Display handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/display.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <span>

namespace boyboy::display {

// helper functions
namespace {

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

bool set_window_center(GLFWwindow* window)
{
    if (window == nullptr) {
        return false;
    }

    int sx = 0;
    int sy = 0;
    int px = 0;
    int py = 0;
    int mx = 0;
    int my = 0;
    int monitor_count = 0;
    int best_area = 0;
    int final_x = 0;
    int final_y = 0;

    glfwGetWindowSize(window, &sx, &sy);
    glfwGetWindowPos(window, &px, &py);

    // Iterate through all monitors
    GLFWmonitor** m = glfwGetMonitors(&monitor_count);
    if (m == nullptr) {
        return false;
    }

    std::span<GLFWmonitor*> monitors(m, monitor_count);

    for (int j = 0; j < monitor_count; ++j) {

        glfwGetMonitorPos(monitors[j], &mx, &my);
        const GLFWvidmode* mode = glfwGetVideoMode(monitors[j]);
        if (mode == nullptr) {
            continue;
        }

        // Get intersection of two rectangles - screen and window
        int min_x = std::max(mx, px);
        int min_y = std::max(my, py);

        int max_x = std::min(mx + mode->width, px + sx);
        int max_y = std::min(my + mode->height, py + sy);

        // Calculate area of the intersection
        int area = std::max(max_x - min_x, 0) * std::max(max_y - min_y, 0);

        // If its bigger than actual (window covers more space on this monitor)
        if (area > best_area) {
            // Calculate proper position in this monitor
            final_x = mx + (mode->width - sx) / 2;
            final_y = my + (mode->height - sy) / 2;

            best_area = area;
        }
    }

    // We found something
    if (best_area != 0) {
        glfwSetWindowPos(window, final_x, final_y);
    }

    // Something is wrong - current window has NOT any intersection with any
    // monitors. Move it to the default one.
    else {
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        if (primary != nullptr) {
            const GLFWvidmode* desktop = glfwGetVideoMode(primary);

            if (desktop != nullptr) {
                glfwSetWindowPos(window, (desktop->width - sx) / 2, (desktop->height - sy) / 2);
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }

    return true;
}

} // namespace

void Display::init()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(Width, Height, "BoyBoy", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    set_window_center(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << "\n";
    }

    //    auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glViewport(0, 0, Width, Height);

    while (glfwWindowShouldClose(window) == 0) {
        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        process_input(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

} // namespace boyboy::display