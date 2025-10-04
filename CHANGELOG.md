# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org).

---

## [Unreleased]

## [0.3.0] - 2025-10-04

### Added

- Add MBC1 support with ROM and RAM banking.
- Add `CartridgeLoader` to handle ROM data loading and separate file-reading logic from `Cartridge` class.

### Changed

- Improve MMU memory mapping with read-only and unmapped regions.
- Refactor MMU ROM and ERAM mapping logic to use MBC interface.

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

[Unreleased]: https://github.com/sebdevnull/boyboy/compare/v0.3.0...develop
[0.3.0]: https://github.com/sebdevnull/boyboy/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/sebdevnull/boyboy/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/sebdevnull/boyboy/releases/tag/v0.1.0
