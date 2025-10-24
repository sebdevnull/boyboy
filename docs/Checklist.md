# BoyBoy feature implementation checklist

Implementation status of every major component and feature in **BoyBoy**.

**Status legend:**

- ✅ Fully implemented
- ⏳ Partial implementation / In progress
- Blank: Not started / Not implemented

> For planned upcoming features, see [README.md](/README.md#upcoming-features).  
> For detailed progress and milestones, see [CHANGELOG.md](/CHANGELOG.md).

## CPU Core & Memory

- [x] CPU instruction set ✅
  - [x] 8-bit instructions ✅
  - [x] 16-bit instructions ✅
  - [x] Special instructions ✅
  - [x] CB-prefixed instructions ✅
- [x] Interrupt handling ✅
- [x] Timers ✅
- [x] Memory management unit (MMU) ✅
  - [x] Read/write memory ✅
  - [x] Memory banking ✅
  - [x] DMA integration ✅
- [x] Direct Memory Access (DMA) ✅

## ROM & Cartridge

- [ ] ROM loading ⏳
  - [x] Header parsing ✅
  - [x] Checksum validation ✅
  - [x] ROM banks mapping ✅
  - [x] ROM banks switching ✅
  - [ ] Boot ROM support
- [ ] Cartridge support ⏳
  - [x] ROM Only ✅
  - [x] MBC1 ✅
  - [ ] MBC2
  - [ ] MBC3
  - [ ] MBC5
  - [ ] Other MBC types
  - [x] Battery-backed RAM support ✅
  - [ ] RTC support

## I/O & Peripherals

- [ ] I/O registers ⏳
  - [x] Joypad ✅
  - [ ] Serial ⏳
  - [x] Timer ✅
  - [x] Interrupts ✅
  - [ ] Audio (APU)
  - [x] Graphics (PPU) ✅
- [x] Input handling ✅

## Graphics & Display

- [x] Graphics processing unit (PPU) ✅
  - [x] Background rendering ✅
  - [x] Sprite rendering ✅
  - [x] LCD control/status ✅
  - [x] Window & scrolling ✅
  - [ ] Color/palette handling (CGB support)
- [x] VRAM and OAM management ✅
- [x] LCD timing & scanline handling ✅
- [x] V-Blank and H-Blank handling ✅
- [x] Frame rendering ✅
- [x] OpenGL rendering backend ✅
- [ ] Window management ⏳
  - [x] Scaling ✅
  - [ ] Resizing
  - [ ] Fullscreen mode
- [ ] Frame rate control ⏳
  - [ ] Frame limiting ⏳
  - [ ] Frame skipping
  - [x] Frame profiling ✅

## Audio & Sound

- [ ] Audio processing unit (APU)
  - [ ] Square wave channels
  - [ ] Wave channel
  - [ ] Noise channel
  - [ ] Volume envelopes
  - [ ] Frequency sweep
  - [ ] Sound mixing/output
- [ ] Audio output (using an audio library)
  - [ ] Buffering & latency management
  - [ ] Volume control

## Extras / Utilities

- [ ] CLI interface ⏳
  - [x] Load ROM ✅
  - [x] Load configuration file ✅
  - [x] Subcommands ✅
  - [ ] Debug options (log level, test mode) ⏳
  - [ ] Interactive CLI
    - [ ] Emulator control (start, pause, reset)
    - [ ] Debug (step, break, etc.)
- [ ] GUI interface ⏳
  - [x] Basic window ✅
  - [x] Display rendering ✅
  - [x] Input handling ✅
  - [ ] Basic controls (load, start, pause, reset)
- [x] Configuration file support ✅
- [ ] Save files ⏳
  - [x] Battery-backed RAM saves ✅
  - [ ] Emulator state saves
- [ ] Debugging tools ⏳
  - [ ] CPU/Memory viewer ⏳
  - [ ] Breakpoints
  - [ ] Step execution ⏳
  - [x] Performance profiling ✅
- [ ] Logging ⏳
  - [x] CPU instructions ✅
  - [ ] Interrupts ⏳
  - [ ] Memory access
  - [ ] Input events ⏳
  - [ ] PPU events ⏳
  - [ ] APU events
  - [x] Log levels (trace, debug, info, warning, error, critical) ✅
  - [ ] Modular logging ⏳
- [ ] OS compatibility ⏳
  - [x] Linux ✅
  - [ ] Windows
  - [ ] macOS

## Testing & Validation

- [x] [Blargg’s CPU test ROMs](../README.md#test-roms) ✅
- [x] CPU instruction set tests ✅
- [x] Interrupt handling tests ✅
- [x] Timer tests ✅
- [ ] MMU tests ⏳
- [ ] DMA tests
- [ ] ROM loading tests ⏳
- [ ] Cartridge tests ⏳
- [ ] I/O registers and Input tests ⏳
  - [ ] Serial ⏳
  - [x] Timer ✅
  - [x] Joypad ✅
- [ ] PPU tests ⏳
  - [ ] Background rendering ⏳
  - [ ] Window rendering
  - [ ] Sprite rendering
- [ ] APU tests
- [ ] Integration tests ⏳
- [ ] Performance tests
