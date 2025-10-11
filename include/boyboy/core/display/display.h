/**
 * @file display.h
 * @brief Display handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>

#include "boyboy/core/ppu/ppu.h"

// Forward declarations for SDL and OpenGL types
class SDL_Window;
using SDL_GLContext = void*;
using GLuint = unsigned int;
union SDL_Event;

// Forward declaration for Button enum
namespace boyboy::core::io {
enum class Button : uint8_t;
}

namespace boyboy::core::display {

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

    // Accessors
    [[nodiscard]] int width() const { return width_ * scale_; }
    [[nodiscard]] int height() const { return height_ * scale_; }
    [[nodiscard]] int scale() const { return scale_; }
    [[nodiscard]] bool vsync() const { return vsync_; }
    void set_scale(int scale) { scale_ = scale; }
    void set_vsync(bool vsync) { vsync_ = vsync; }

    // Button event callback
    void set_button_cb(ButtonCallback cb) { button_cb_ = std::move(cb); }

private:
    int width_ = ppu::LCDWidth;
    int height_ = ppu::LCDHeight;
    int scale_ = DefaultScale;
    bool vsync_ = true;

    ButtonCallback button_cb_;

    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
    GLuint texture_{}, VAO_{}, VBO_{}, EBO_{};
    GLuint shader_program_{};

    void handle_key_event(const SDL_Event& event, bool pressed);

    void init_opengl();
};

} // namespace boyboy::core::display