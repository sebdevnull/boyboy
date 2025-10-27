# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org).

---

## [Unreleased]

### Changed

- Improve **core components composition** with dependency injection:
  - Make `Emulator` responsible for components composition
  - Add `Io` components registration and improve Io read/write operations
  - Remove `Ppu` memory callbacks and add `Mmu` weak dependency
  - Add `init` methods and improve init/reset logic in general

## [0.6.0] - 2025-10-24

### Added

- **Core** improvements:
  - CPU halt bug emulation
  - MMU VRAM and OAM lock on PPU access
- **Files Module** with general file I/O handling and shared file paths and utilities
  - Text files read and write: `read_text()` and `write_text()`
  - Binary files read and write: `read_binary()` and `write_binary()`
  - Stream handling: `input_stream()` and `output_stream`
  - Atomic operations: `atomic_write()`
  - Shared utilities and validation: `paths.h`, `errors.h` and `utils.h`
- **SRAM save and load** functionality for battery-backed RAM MBC chips
  - `SaveManager` for save and load handling
  - Load save file on start and save to file on periodic intervals and shutdown
- **Config** options for save files:
  - `saves.autosave` for enabling/disabling autosave
  - `saves.save_interval` for configuring autosave interval
- **CLI** options for save files:
  - `--save-path` for custom saving path
  - `--autosave` for enabling/disabling autosave
  - `--save-interval` for setting autosave interval
- Add script `scripts/run_boyboy.sh` for running debug build with ASan suppressions for known external issues and `config/asan_suppress.txt` for said suppressions

### Changed

- Core `Timer` emulation more accurate and close to the real circuit
- `config_utils` and `CartridgeLoader` now use `files` module for files operations
- `CartridgeLoader` now returns a `unique_ptr` to `Cartridge`
- Rename `ERAM` to `SRAM`
- CLI and configuration improvements

### Fixed

- PPU missing LY on reset
- Interrupt handler cycles not being factored
- Mmu Not Usable (0xFEA0) region lock alongside OAM

---

## [0.5.0] - 2025-10-16

### Added

- **Configuration system** based on TOML, with support for limits, default values, validation, and normalization.
  - `Config` struct and config keys for emulator, video, and debug settings.
  - `IConfigLoader` interface and `TomlConfigLoader` implementation.
  - `ConfigValidator` for config validation and normalization.
  - High-level API for loading and saving config files (`load_config`, `save_config`).
- **Command execution system**.
  - `App` executor class, `ICommand` interface and `CommandContext` for information passing, `CommandRegistry` for command registration.
  - Concrete commands: `RunCommand` for emulator running, `InfoCommand` for ROM information, `ConfigCommand` for viewing and editing configuration.
- **Command-Line Interface** with CLI11.
  - CLI application `CLIApp`, abstract adapter interface `ICLIAdapter`, and concrete implementation `CLI11Adapter`.
  - Run command with config, speed, scale, vsync and log level options.
  - Info command to display ROM metadata information.
  - Config with key-value get and set, and full configuration list and reset.
  - Version and build information.
- Log level configuration and conversion helpers.
- Display accessors for scale and vsync.
- Emulator configuration application method.
- Frame limiting based on configured speed.
- Version header generation with build metadata.
- Unit tests for configuration logic.

### Changed

- Main application entry point modified to use `CLIApp`
- Emulator and Display now apply configuration at startup.
- Logging system supports configurable log levels.
- Reorganize codebase moving core components under `boyboy/core/` directory.
- Improve helper scripts `format.py` and `generate_opcodes.py`.

---

## [0.4.0] - 2025-10-07

### Added

- Add profiling module to analyze performance bottlenecks and measure frame times.
  - Profiler based on regular and scoped timers: `Profiler`, `Timer`, `ScopedTimer`.
  - Hot profiler for critical and high-frequency code sections: `HotProfiler`.
  - Frame profiler to measure frame times and FPS: `FrameProfiler`.
  - Profiling API with optional macros: `profiler_utils.h`.
  - Conditional compilation with `ENABLE_PROFILING` CMake option.

### Changed

- Refactor `Emulator` and more specifically the emulation loop
  - Frame-based emulation loop
  - Move `Display::poll_events` to start of each frame instead of calling every step
  - Replace FPS/IPS/CPS counter in `emulator.cpp` for `FrameProfiler`
- Replace `Mmu` loop-based region finding for O(1) lookup table and other optimizations

### Chores

- Update `.clang-format` formatting rules for better readability
- Add `scripts/format.py` to check and apply clang-format to the codebase

---

## [0.3.0] - 2025-10-04

### Added

- Add MBC1 support with ROM and RAM banking.
- Add `CartridgeLoader` to handle ROM data loading and separate file-reading logic from `Cartridge` class.

### Changed

- Improve MMU memory mapping with read-only and unmapped regions.
- Refactor MMU ROM and SRAM mapping logic to use MBC interface.

### Fixed

- Enable LCD on ROM startup (some games expect it).

### Chores

- Add tests for MBC1 functionality.
- Add `TestedROMs.md` with compatibility status for games and test ROMs.
- Add `CHANGELOG.md`.
- Rename `ROADMAP.md` to `Checklist.md` and move tentative planned features to `README.md`.
- Archive CPU instructions checklist in `docs/archive/`.

---

## [0.2.0] - 2025-09-23

### Added

- Complete PPU implementation
  - Background, window, and sprite rendering.
  - LCD control and status handling.
  - VRAM and OAM management.
- DMA integration.
- Joypad input handling.
- CPU tracing for debugging.

### Changed

- Rendering backend switched to SDL2/glad.
- Components orchestration in the main emulator loop.
- Start values for all CPU registers on boot (DMG mode).
- Improved logging across the codebase.

### Chores

- Add tests for PPU rendering and joypad input.

---

## [0.1.0] - 2025-09-18

### Added

- Initial project setup.
- Full CPU instruction set.
- Interrupt handling.
- Basic MMU implementation with read/write support.
- Basic ROM loading and header parsing.
- I/O interface and timer and serial (partial) components.
- Basic emulator loop and CLI.
- Basic logging with spdlog and error handling.
- Blargg's CPU test ROMs passing.

### Chores

- Set up CMake build system with presets.
- Unit and integration tests for CPU instructions, interrupts, MMU, ROM loading, and I/O registers.

[Unreleased]: https://github.com/sebdevnull/boyboy/compare/v0.6.0...HEAD
[0.6.0]: https://github.com/sebdevnull/boyboy/compare/v0.5.0...v0.6.0
[0.5.0]: https://github.com/sebdevnull/boyboy/compare/v0.4.0...v0.5.0
[0.4.0]: https://github.com/sebdevnull/boyboy/compare/v0.3.0...v0.4.0
[0.3.0]: https://github.com/sebdevnull/boyboy/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/sebdevnull/boyboy/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/sebdevnull/boyboy/releases/tag/v0.1.0
