/**
 * @file display.cpp
 * @brief Display handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// clang-format off
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <SDL2/SDL.h>
// clang-format on

#include "boyboy/core/display/display.h"

#include <array>
#include <iostream>

#include "boyboy/common/log/logging.h"
#include "boyboy/core/io/buttons.h"
#include "boyboy/core/ppu/ppu.h"
#include "boyboy/core/profiling/profiler_utils.h"

namespace boyboy::core::display {

using namespace boyboy::common;

bool Display::init(const std::string& title)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    // RAII guard to ensure SDL_Quit is called on error
    struct SDLGuard { // NOLINT
        ~SDLGuard()
        {
            if (should_quit) {
                SDL_Quit();
            }
        };
        bool should_quit = true;
    } sdl_guard;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_ * scale_,
        height_ * scale_,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (window_ == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return false;
    }

    gl_context_ = SDL_GL_CreateContext(window_);
    if (gl_context_ == nullptr) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Enable VSync
    SDL_GL_SetSwapInterval(vsync_ ? 1 : 0);

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) { // NOLINT
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    init_opengl();

    auto gl_version_to_string = [](GLenum name) -> std::string {
        const auto* str = glGetString(name);
        return str != nullptr ? reinterpret_cast<const char*>(str) : "Unknown"; // NOLINT
    };

    log::info("Display initialized: {}x{} @ {}x scale", width_, height_, scale_);
    log::info("OpenGL Version: {}", gl_version_to_string(GL_VERSION));
    log::info("OpenGL Renderer: {}", gl_version_to_string(GL_RENDERER));

    sdl_guard.should_quit = false;
    return true;
}

void Display::shutdown()
{
    log::info("Shutting down display...");

    // Cleanup OpenGL resources
    glDeleteTextures(1, &texture_);
    glDeleteProgram(shader_program_);
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);

    // Destroy window and OpenGL context
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);

    SDL_Quit();

    log::debug("Display shut down");
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Display::poll_events(bool& running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                handle_key_event(event, true);
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_Event quit_event;
                    quit_event.type = SDL_QUIT;
                    SDL_PushEvent(&quit_event);
                }
                break;
            case SDL_KEYUP:
                handle_key_event(event, false);
                break;
            default:
                break;
        }
    }
}

void Display::handle_key_event(const SDL_Event& event, bool pressed)
{
    if (!button_cb_) {
        log::warn("Button event ignored, no callback set");
        return;
    }

    switch (event.key.keysym.sym) {
        case SDLK_z:
            button_cb_(io::Button::A, pressed);
            break;
        case SDLK_x:
            button_cb_(io::Button::B, pressed);
            break;
        case SDLK_RETURN:
            button_cb_(io::Button::Start, pressed);
            break;
        case SDLK_BACKSPACE:
            button_cb_(io::Button::Select, pressed);
            break;
        case SDLK_UP:
            button_cb_(io::Button::Up, pressed);
            break;
        case SDLK_DOWN:
            button_cb_(io::Button::Down, pressed);
            break;
        case SDLK_LEFT:
            button_cb_(io::Button::Left, pressed);
            break;
        case SDLK_RIGHT:
            button_cb_(io::Button::Right, pressed);
            break;
        default:
            break;
    }
}

void Display::render_frame(const ppu::FrameBuffer& framebuffer)
{
    BB_PROFILE_SCOPE(profiling::FrameTimer::Render);

    glTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer.data()
    );

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);
    glBindVertexArray(VAO_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    SDL_GL_SwapWindow(window_);
}

void Display::init_opengl()
{
    // Create texture
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Quad vertices
    // clang-format off
    std::array<float, 16> vertices = {
        // positions  // tex coords
        -1.F,  1.F, 0.F, 0.F,
        -1.F, -1.F, 0.F, 1.F,
         1.F, -1.F, 1.F, 1.F,
         1.F,  1.F, 1.F, 0.F
    };
    // clang-format on
    std::array<unsigned int, 6> indices = {0, 1, 2, 0, 2, 3};

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * indices.size(),
        indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float)) // NOLINT
    );
    glEnableVertexAttribArray(1);

    // Shader sources
    const char* vertex_shader_src = R"(
            #version 330 core
            layout (location=0) in vec2 aPos;
            layout (location=1) in vec2 aTex;
            out vec2 TexCoord;
            void main() {
                gl_Position = vec4(aPos,0,1);
                TexCoord = aTex;
            }
        )";

    const char* frag_shader_src = R"(
            #version 330 core
            in vec2 TexCoord;
            out vec4 FragColor;
            uniform sampler2D screenTexture;
            void main() {
                FragColor = texture(screenTexture, TexCoord);
            }
        )";

    // Compile shaders
    auto compile_shader = [](GLenum type, const char* src) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            std::array<char, 512> info_log{};
            glGetShaderInfoLog(shader, 512, nullptr, info_log.data());
            std::cerr << "Shader Compilation Error: " << info_log.data() << "\n";
        }
        return shader;
    };

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint frag_shader = compile_shader(GL_FRAGMENT_SHADER, frag_shader_src);
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader);
    glAttachShader(shader_program_, frag_shader);
    glLinkProgram(shader_program_);
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    glUseProgram(shader_program_);
    glUniform1i(glGetUniformLocation(shader_program_, "screenTexture"), 0);
}

} // namespace boyboy::core::display
