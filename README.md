# 🎮 BoyBoy

[![C++23](https://img.shields.io/badge/C++-23-blue.svg)](https://isocpp.org/)
[![Linux](https://img.shields.io/badge/Linux-supported-brightgreen.svg)](https://www.linux.org/)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)

**BoyBoy** is a work-in-progress **Game Boy emulator** written in C++23, designed to be accurate, maintainable, and easy for developers to explore and extend.  
It implements a complete CPU core, memory management, and a functional PPU for graphics rendering.  
BoyBoy provides a hands-on platform for learning about emulator development and low-level programming.

## Gameplay

| ![Zelda gameplay](docs/images/zelda.gif) | ![Super Mario Land 2 gameplay](docs/images/super_mario_land2.gif) | ![Kirby's Dream Land gameplay](docs/images/kirbys_dream_land.gif) |
|:---------------------:|:---------------------:|:---------------------:|
| *The Legend of Zelda: Link's Awakening* | *Super Mario Land 2: 6 Golden Coins* | *Kirby's Dream Land* |

| ![Tetris gameplay](docs/images/tetris.gif) | ![Dr. Mario gameplay](docs/images/dr_mario.gif) | ![Donkey Kong gameplay](docs/images/donkey_kong.gif) |
|:---------------------:|:---------------------:|:---------------------:|
| *Tetris*     | *Dr. Mario*  | *Donkey Kong* |

> For full ROM compatibility status, check [Tested ROMs](docs/TestedROMs.md#games).

## Project goals

**BoyBoy** aims to be an accurate, maintainable, and developer-friendly Game Boy emulator.

The project is designed to:

- Faithfully emulate original Game Boy hardware: CPU, memory, I/O, graphics, and audio
- Run commercial and homebrew ROMs  
- Provide a developer-friendly debugging and testing environment
- Serve as a hands-on learning platform for emulator development and low-level programming for the community

## Project status

**BoyBoy** is currently in active development.  

**Current version:** [`v0.2.0`](CHANGELOG.md#020---2025-09-23)  

**Development stage:** Ongoing improvements and feature implementation.

**Progress overview:**

| Component             | Status        |
|-----------------------|---------------|
| CPU core              | ✅ Implemented |
| Memory & DMA          | ✅ Implemented |
| ROM & Cartridge       | ⏳ Partial     |
| PPU (graphics)        | ✅ Implemented |
| APU (audio)           | ❌ Not started |
| I/O & Peripherals     | ⏳ Partial     |
| CLI/GUI & Extras      | ⏳ Partial     |
| Testing & Validation  | ⏳ Partial     |

> For a detailed feature checklist, see [Feature Checklist](/docs/Checklist.md).  
> For detailed progress and changes, see [CHANGELOG](/CHANGELOG.md).

## Upcoming features

The following features are planned for future releases, in tentative order:

- **Core performance optimizations**: smoother emulation and rendering
- **Complete cartridge support**: MBC2, MBC3, MBC5, battery-backed RAM, RTC
- **Save states**: save and restore emulator state
- **Boot ROM support**: authentic startup sequence
- **Debugging enhancements**: breakpoints, step execution, memory and state inspection
- **CLI improvements**: more options and features
- **Audio (APU) implementation**: full sound emulation
- **GUI frontend**: controls,  window management, visualization and debugging tools
- **Cross-platform support**: Windows and macOS builds

## Dependencies

To build **BoyBoy**, the following libraries are required:

- [glad](https://github.com/Dav1dde/glad) – For loading OpenGL functions (included pregenerated header-only version in `external/glad/`)
- [SDL2](https://www.libsdl.org/) – For window management, input handling, and audio  
- OpenGL – For rendering graphics  

On Linux (Debian/Ubuntu), you can install them with:

```bash
sudo apt update
sudo apt install libsdl2-dev libglu1-mesa-dev mesa-common-dev
```

Make sure you have **CMake 3.27+** and a C++23 compiler.

## Build instructions

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

## Test ROMs

This project includes several ROMs for emulator testing and verification.

> See [Tested ROMs](docs/TestedROMs.md#test-roms) for test ROMs passing status.  

The included test ROMs are:

- **blargg’s Game Boy test ROMs** – stored under `tests/roms/cpu/blargg/`, widely used for CPU, PPU, and other hardware verification  
  - [GitHub repository](https://github.com/retrio/gb-test-roms)  

- **dmg-acid2** – stored under `tests/roms/ppu/dmg-acid2.gb`, a PPU test ROM by Matt Currie (MIT license)
  - [GitHub repository](https://github.com/mattcurrie/dmg-acid2)

- **GameBoyLife ROM** – stored under `tests/roms/misc/gameboylife/` by NovaSquirrel (zlib license)  
  - [GitHub repository](https://github.com/NovaSquirrel/GameBoyLife)
  - [Itch.io page](https://novasquirrel.itch.io/conways-life-for-game-boy)

- **Custom ROMs** – stored under `tests/roms/misc/custom/`  
  - `invalid.gb` – created to test invalid or malformed data handling  

> See [Third-Party Licenses](LICENSE-THIRD-PARTY.md) for detailed attribution, licenses, and legal notices.

## Legal notice

- **BoyBoy** is an independent project and is **not affiliated with or endorsed by Nintendo**.  
- The Game Boy name, logo, and related trademarks are property of **Nintendo Co., Ltd.**  
- See [Third-Party Licenses](LICENSE-THIRD-PARTY.md) for third-party attributions and disclaimers.  
