# 🎮 BoyBoy

**boyboy** is a work-in-progress **Game Boy emulator** written in C++.

## 🚧 Project status

Currently **paused**. Development may resume in the future, but for now no active work is being done.

## ✨ Vision

The long-term goal of **BoyBoy** was to:

- Emulate the original Game Boy hardware (CPU, memory, graphics, sound)  
- Run commercial and homebrew ROMs  
- Provide a simple interface for debugging and testing  

## 📦 Dependencies

To build **boyboy**, the following libraries are required:

- [GLFW](https://www.glfw.org/) – For windowing and input  
- [GLEW](http://glew.sourceforge.net/) – For OpenGL extension loading  
- OpenGL – For rendering graphics  

On Linux (Debian/Ubuntu), you can install them with:

```bash
sudo apt update
sudo apt install libglfw3-dev libglew-dev libglu1-mesa-dev mesa-common-dev
```

Make sure you have **CMake 3.27+** and a C++23 compiler.

## 🛠 Build instructions

**Using CMake presets (recommended):**

1. Create and configure a build:

    ```bash
    cmake --preset debug   # or 'release'
    ```

2. Build the project:

    ```bash
    cmake --build --preset debug
    ```

3. Run tests (if any):

    ```bash
    ctest --preset debug
    ```

## 📂 Repository

This repository is mainly for reference and archival.
