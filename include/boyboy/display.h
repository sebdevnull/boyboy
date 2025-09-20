/**
 * @file display.h
 * @brief Display handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>

#include "boyboy/ppu/ppu.h"

// Forward declarations for SDL and OpenGL types
class SDL_Window;
using SDL_GLContext = void*;
using GLuint = unsigned int;

namespace boyboy::display {

static constexpr int DefaultScale = 4;

class Display {
public:
    Display(int scale = DefaultScale) : scale_(scale) {}

    bool init(const std::string& title = "BoyBoy");
    void shutdown();
    void poll_events(bool& running);
    void render_frame(const ppu::FrameBuffer& framebuffer);

private:
    int width_ = ppu::LCDWidth;
    int height_ = ppu::LCDHeight;
    int scale_;

    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
    GLuint texture_{}, VAO_{}, VBO_{}, EBO_{};
    GLuint shader_program_{};

    void init_opengl();
};

} // namespace boyboy::display