# 🎮 BoyBoy

**boyboy** is a work-in-progress **Game Boy emulator** written in C++.

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

## 🧪 Test ROMs

This project includes several ROMs for emulator testing and verification:

- **blargg’s Game Boy test ROMs** – stored under `tests/roms/blargg/`, widely used for CPU, PPU, and other hardware verification  
  - [GitHub repository](https://github.com/retrio/gb-test-roms)  

- **GameBoyLife ROM** – stored under `tests/roms/gameboylife/` by NovaSquirrel (zlib license)  
  - [GitHub repository](https://github.com/NovaSquirrel/GameBoyLife)
  - [Itch.io page](https://novasquirrel.itch.io/conways-life-for-game-boy)

- **Custom ROMs** – stored under `tests/roms/custom/`  
  - `invalid.gb` – created to test invalid or malformed data handling  

See [LICENSE-THIRD-PARTY.md](LICENSE-THIRD-PARTY.md) for detailed attribution, licenses, and legal notices.

## ⚖️ Legal notice

- **BoyBoy** is an independent project and is **not affiliated with or endorsed by Nintendo**.  
- The Game Boy name, logo, and related trademarks are property of **Nintendo Co., Ltd.**  
- See [LICENSE-THIRD-PARTY.md](LICENSE-THIRD-PARTY.md) for third-party attributions and disclaimers.  
