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
union SDL_Event;

// Forward declaration for Button enum
namespace boyboy::io {
enum class Button : uint8_t;
}

namespace boyboy::display {

static constexpr int DefaultScale = 4;

class Display {
public:
    using ButtonCallback = std::function<void(io::Button button, bool pressed)>;

    Display(int scale = DefaultScale) : scale_(scale) {}

    // Display operations
    bool init(const std::string& title = "BoyBoy");
    void shutdown();
    void poll_events(bool& running);
    void render_frame(const ppu::FrameBuffer& framebuffer);

    // Button event callback
    void set_button_cb(ButtonCallback cb) { button_cb_ = std::move(cb); }

private:
    int width_ = ppu::LCDWidth;
    int height_ = ppu::LCDHeight;
    int scale_;

    ButtonCallback button_cb_;

    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
    GLuint texture_{}, VAO_{}, VBO_{}, EBO_{};
    GLuint shader_program_{};

    void handle_key_event(const SDL_Event& event, bool pressed);

    void init_opengl();
};

} // namespace boyboy::display