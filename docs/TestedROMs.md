# Tested ROMs status

This document lists the ROMs that have been tested with the BoyBoy emulator, along with their current status.  
The goal is to ensure compatibility with a wide range of games and test ROMs to validate the accuracy of the emulator.  
It includes both commercial games and well-known test ROMs used in the emulator development community.

> See [Third-Party Licenses](LICENSE-THIRD-PARTY.md) for detailed attribution, licenses, and legal notices.

## Games

The emulator currently supports ROM-Only and MBC1 cartridges. Battery-backed RAM is not yet implemented, so games relying on save functionality might be playable but without saving progress.

| Game Title | Cartridge Type | Notes | Status |
|------------|----------------|--------|:-------:|
| Alleyway | ROM ONLY | Game starts but controllers do not respond | ❌ |
| Dr. Mario | ROM ONLY | Fully playable | ✅ |
| Tetris | ROM ONLY | Fully playable | ✅ |
| Baseball | MBC1 | Game starts but controllers do not respond | ❌ |
| Castelvania 2 | MBC1 | Game starts with blank screen | ❌ |
| Contra | MBC1 | Game starts with blank screen | ❌ |
| Donkey Kong | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Donkey Kong Land | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Donkey Kong Land 2 | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Donkey Kong Land III | MBC1+RAM+BATTERY | Fully playable | ✅ |
| DuckTales | MBC1 | Fully playable | ✅ |
| Final Fantasy Legend II | MBC1+RAM+BATTERY | Game starts with blank screen | ❌ |
| Final Fantasy Legend III | MBC1+RAM+BATTERY | Fully playable | ✅ |
| James Bond 007 | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Jungle Strike | MBC1 | Fully playable | ✅ |
| Kirby's Dream Land | MBC1 | Fully playable | ✅ |
| Kirby's Dream Land 2 | MBC1+RAM+BATTERY | Game starts with blank screen | ❌ |
| Mario's Picross | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Mega Man V | MBC1 | Fully playable | ✅ |
| Metroid II | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Mole Mania | MBC1+RAM+BATTERY | Fully playable | ✅ |
| NBA Jam | MBC1 | Fully playable | ✅ |
| NBA Jam TE | MBC1 | Fully playable | ✅ |
| Pacman | MBC1 | Fully playable | ✅ |
| Paperboy | MBC1 | Fully playable | ✅ |
| Super Mario Land | MBC1 | Fully playable | ✅ |
| Super Mario Land 2: 6 Golden Coins  | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Super Mario Land 3: Wario Land | MBC1+RAM+BATTERY | Fully playable | ✅ |
| The Legend of Zelda: Link's Awakening | MBC1+RAM+BATTERY | Fully playable | ✅ |
| Tumble Pop | MBC1 | Game starts, screen goes blank after intro | ❌ |

>Only ROMs that have been legally obtained by the user should be used with this emulator.  
No copyrighted commercial Nintendo ROMs are distributed with this project.

## Test ROMs

> Every test ROM is tested in all CPU tick modes: instruction, M-cycle and T-cycle.

### Blargg's Test ROMs

#### CPU Test ROMs

> Source: [Blargg's Test ROMs](/README.md#test-roms)

| ROM Name | Description | Notes | Instr | M-cycle | T-cycle |
|----------|-------------|-------|:-----:|:-------:|:-------:|
| 01-special.gb | Tests special CPU instructions | Passes all tests | ✅ | ✅ | ✅ |
| 02-interrupts.gb | Tests interrupt handling | Hangs on "HALT bug" test | ❌ | ❌ | ❌ |
| 03-op sp,hl.gb | Tests stack pointer and HL instructions | Passes all tests | ✅ | ✅ | ✅ |
| 04-op r,imm.gb | Tests immediate load instructions | Passes all tests | ✅ | ✅ | ✅ |
| 05-op rp.gb | Tests register pair instructions | Passes all tests | ✅ | ✅ | ✅ |
| 06-ld r,r.gb | Tests register to register load instructions | Passes all tests | ✅ | ✅ | ✅ |
| 07-jr,jp,call,ret,rst.gb | Tests control flow instructions | Passes all tests | ✅ | ✅ | ✅ |
| 08-misc instrs.gb | Tests miscellaneous instructions | Passes all tests | ✅ | ✅ | ✅ |
| 09-op r,r.gb | Tests register to register arithmetic and logic instructions | Passes all tests | ✅ | ✅ | ✅ |
| 10-bit ops.gb | Tests bit manipulation instructions | Passes all tests | ✅ | ✅ | ✅ |
| 11-op a,(hl).gb | Tests accumulator and (HL) instructions | Passes all tests | ✅ | ✅ | ✅ |
| cpu_instrs.gb | Comprehensive CPU instruction test | Fails on test 2 (interrupts) | ❌ | ❌ | ❌ |

### Mooneye Test Suite

> Source: [Gekkio's Mooneye Test Suite](/README.md#test-roms)

#### Acceptance tests

#### General

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| div_timing.gb | | ✅ | ❌ | ❌ |
| ei_sequence.gb | | ❌ | ✅ | ✅ |
| ei_timing.gb | Pass | ✅ | ✅ | ✅ |
| halt_ime0_ei.gb | Pass | ✅ | ✅ | ✅ |
| halt_ime0_nointr_timing.gb |  | ❌ | ❌ | ❌ |
| halt_ime1_timing.gb | Pass | ✅ | ✅ | ✅ |
| if_ie_registers.gb |  | ❌ | ❌ | ❌ |
| intr_timing.gb | | ✅ | ❌ | ❌ |
| oam_dma_restart.gb | | ❌ | ❌ | ❌ |
| oam_dma_start.gb | | ❌ | ❌ | ❌ |
| oam_dma_timing.gb | | ❌ | ❌ | ❌ |
| rapid_di_ei.gb | | ❌ | ✅ | ✅ |
| reti_intr_timing.gb | Pass | ✅ | ✅ | ✅ |
| reti_timing.gb |  | ❌ | ❌ | ❌ |

#### Bits

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| mem_oam.gb | Pass | ✅ | ✅ | ✅ |
| reg_f.gb | Pass | ✅ | ✅ | ✅ |

#### Instr

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| daa.gb | Pass | ✅ | ✅ | ✅ |

#### Interrupts

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| ie_push.gb | R1: not cancelled | ❌ | ❌ | ❌ |

#### OAM DMA

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| basic.gb | Pass | ✅ | ✅ | ✅ |
| reg_read.gb | Fail: r1 | ❌ | ❌ | ❌ |

#### PPU

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| intr_2_0_timing.gb | | ❌ | ❌ | ❌ |
| intr_2_mode0_timing.gb | | ✅ | ❌ | ❌ |
| intr_2_mode0_timing_sprites.gb | | ❌ | ❌ | ❌ |
| intr_2_mode3_timing.gb | | ✅ | ❌ | ❌ |
| intr_2_oam_ok_timing.gb | | ✅ | ❌ | ❌ |
| stat_irq_blocking.gb | | ❌ | ❌ | ❌ |
| stat_lyc_onoff.gb | | ❌ | ❌ | ❌ |

#### Timer

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| div_write.gb | Pass | ✅ | ✅ | ✅ |
| rapid_toggle.gb | | ✅ | ❌ | ❌ |
| tim00_div_trigger.gb | | ✅ | ❌ | ❌ |
| tim00.gb | | ✅ | ❌ | ❌ |
| tim01_div_trigger.gb | | ✅ | ❌ | ❌ |
| tim01.gb | | ✅ | ❌ | ❌ |
| tim10_div_trigger.gb | | ✅ | ❌ | ❌ |
| tim10.gb | | ✅ | ❌ | ❌ |
| tim11_div_trigger.gb | | ✅ | ❌ | ❌ |
| tim11.gb | | ✅ | ❌ | ❌ |
| tima_reload.gb | | ❌ | ❌ | ❌ |
| tima_write_reloading.gb | | ✅ | ❌ | ❌ |
| tma_write_reloading.gb | | ✅ | ❌ | ❌ |

## Other Test ROMs

### dmg-acid2

> Source: [dmg-acid2 GitHub repository](/README.md#test-roms)

ROM Name: dmg-acid2.gb  
Status: ✅ Renders correctly.
