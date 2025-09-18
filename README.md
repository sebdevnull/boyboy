# 🎮 BoyBoy

[![C++23](https://img.shields.io/badge/C++-23-blue.svg)](https://isocpp.org/)
[![Linux](https://img.shields.io/badge/Linux-supported-brightgreen.svg)](https://www.linux.org/)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)

**BoyBoy** is a work-in-progress **Game Boy emulator** written in C++23, focusing on accuracy and learning.

## 📊 Project status

**Development stage:** Early development, not playable yet.

**Progress overview:**

| Component             | Status        |
|-----------------------|---------------|
| CPU core              | ✅ Implemented |
| Memory & DMA          | ⏳ Partial     |
| ROM & Cartridge       | ⏳ Partial     |
| PPU (graphics)        | ❌ Not started |
| APU (audio)           | ❌ Not started |
| I/O & Peripherals     | ⏳ Partial     |
| CLI/GUI & Extras      | ⏳ Partial     |
| Testing & Validation  | ⏳ Partial     |

**Current milestone:** `v0.5-cpu-validated` - CPU core implemented and validated with Blargg's CPU test ROMs.

> For a detailed feature checklist, see [ROADMAP](docs/ROADMAP.md).

## 🎯 Vision

**BoyBoy** aims to be an accurate, maintainable, and developer-friendly Game Boy emulator.

The project is designed to:

- Faithfully emulate original Game Boy hardware: CPU, memory, I/O, graphics, and audio
- Run commercial and homebrew ROMs  
- Provide a developer-friendly debugging and testing environment
- Serve as a hands-on learning platform for emulator development and low-level programming

## 📦 Dependencies

To build **BoyBoy**, the following libraries are required:

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

3. Run tests:

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
