# BoyBoy Detailed Roadmap

## Milestones

### Current milestone

- `v0.2.0` - PPU functional
  - Complete PPU implementation
  - Background, window, and sprite rendering
  - LCD control and status handling
  - VRAM and OAM management
  - DMA integration
  - Joypad input handling
  - SDL2/glad rendering backend
  - Demo playable with Dr. Mario

### Next milestone

- TBD - PPU stabilization and timing. More ROMs supported. MBC cartridges. Performance improvements. APU implementation. CLI/GUI enhancements.

### Completed milestones

- `v0.1.0`: Full CPU instruction set, interrupt handling, timers, passes Blargg's CPU test ROMs.
- `v0.0.x`: Initial CPU and MMU implementation, partial instruction set, basic ROM loading.

### Development Milestones

- `v0.6-ppu-functional`: PPU implementation complete, including background, window, and sprite rendering. SDL2/glad rendering backend. DMA integration. Joypad input handling. Demo playable with Dr. Mario.
- `v0.5-cpu-validated`: CPU core implemented, including complete instruction set, interrupt handling, and timers. Validated with Blargg's CPU test ROMs.
- `v0.4-instructions-complete`: Full CPU instruction set implemented, including 8-bit, 16-bit, CB-prefixed, and special instructions.
- `v0.3-instructions-unprefixed-complete`: Unprefixed 8-bit, 16-bit, and special instructions implemented.
- `v0.2-rom-loading`: Basic ROM loading and header parsing implemented.
- `v0.1-instructions-unprefixed-partial`: Partial implementation of unprefixed instructions. MMU with basic read/write support. Basic project structure and build system set up.

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
  - [ ] Battery-backed RAM support
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
  - [ ] Scaling & resizing ⏳
  - [ ] Fullscreen mode
- [ ] Frame rate control

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
  - [ ] Start/Pause/Reset
  - [ ] Debug options (log level, test mode)
- [ ] GUI interface ⏳
  - [x] Basic window ✅
  - [x] Display rendering ✅
  - [x] Input handling ✅
  - [ ] Basic controls (load, start, pause, reset)
- [ ] Configuration file support
- [ ] Save states
- [ ] Debugging tools ⏳
  - [ ] CPU/Memory viewer ⏳
  - [ ] Breakpoints
  - [ ] Step execution ⏳
  - [ ] Performance profiling ⏳
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
