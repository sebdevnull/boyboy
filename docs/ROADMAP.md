# BoyBoy Detailed Roadmap

## Milestones

### 🎯 Current milestone

- `v0.5-cpu-validated`: CPU core implemented, including complete instruction set, interrupt handling, and timers. Validated with Blargg's CPU test ROMs.

### 🔜 Next milestone

- `v0.6-ppu-partial`: Basic PPU (graphics) implementation with background rendering and LCD control.

### ✅ Completed milestones

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
- [ ] Memory management unit (MMU) ⏳
  - [x] Read/write memory ✅
  - [ ] Memory banking ⏳
  - [ ] DMA integration
- [ ] Direct Memory Access (DMA)

## ROM & Cartridge

- [ ] ROM loading ⏳
  - [x] Header parsing ✅
  - [x] Checksum validation ✅
  - [x] ROM banks mapping ✅
  - [ ] ROM banks switching
  - [ ] Boot ROM support
- [ ] Cartridge support ⏳
  - [ ] MBC
  - [ ] Save RAM support
  - [ ] RTC support

## I/O & Peripherals

- [ ] I/O registers ⏳
  - [ ] Joypad
  - [ ] Serial ⏳
  - [x] Timer ✅
  - [x] Interrupts ✅
  - [ ] Audio (APU)
  - [ ] Graphics (PPU)
- [ ] Input handling

## Graphics & Display

- [ ] Graphics processing unit (PPU)
  - [ ] Background rendering
  - [ ] Sprite rendering
  - [ ] LCD control/status
  - [ ] Window & scrolling
  - [ ] Color/palette handling (CGB support)
- [ ] VRAM and OAM management
- [ ] LCD timing & scanline handling
- [ ] V-Blank and H-Blank handling
- [ ] Frame rendering
- [ ] OpenGL rendering backend
- [ ] Window management ⏳
  - [ ] Scaling & resizing
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
  - [ ] Basic window ⏳
  - [ ] Display rendering
  - [ ] Input handling
  - [ ] Basic controls (load, start, pause, reset)
- [ ] Configuration file support
- [ ] Save states
- [ ] Debugging tools ⏳
  - [ ] CPU/Memory viewer ⏳
  - [ ] Breakpoints
  - [ ] Step execution ⏳
  - [ ] Performance profiling
- [ ] Logging ⏳
  - [x] CPU instructions ✅
  - [ ] Interrupts
  - [ ] Memory access
  - [ ] Input events ⏳
  - [ ] PPU events
  - [ ] APU events
  - [x] Log levels (trace, debug, info, warning, error, critical) ✅
  - [ ] Modular logging ⏳
- [ ] OS compatibility ⏳
  - [x] Linux ✅
  - [ ] Windows
  - [ ] macOS

## Testing & Validation

- [x] [Blargg’s CPU test ROMs](../README.md#🧪-test-roms) ✅
- [x] CPU instruction set tests ✅
- [x] Interrupt handling tests ✅
- [x] Timer tests ✅
- [ ] MMU tests ⏳
- [ ] DMA tests
- [ ] ROM loading tests ⏳
- [ ] Cartridge tests ⏳
- [ ] I/O registers and Input tests ⏳
- [ ] PPU tests
- [ ] APU tests
- [ ] Integration tests ⏳
- [ ] Performance tests
