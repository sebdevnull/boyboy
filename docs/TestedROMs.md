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
| Kirby's Dream Land 2 | MBC1+RAM+BATTERY | Fully playable | ✅ |
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

> Source: [Blargg's Test ROMs](/README.md#test-roms)

#### CPU Instrs

| ROM Name | Description | Notes | Instr | M-cycle | T-cycle |
|----------|-------------|-------|:-----:|:-------:|:-------:|
| 01-special.gb | Tests special CPU instructions | Passes all tests | ✅ | ✅ | ✅ |
| 02-interrupts.gb | Tests interrupt handling | Passes all tests | ✅ | ✅ | ✅ |
| 03-op sp,hl.gb | Tests stack pointer and HL instructions | Passes all tests | ✅ | ✅ | ✅ |
| 04-op r,imm.gb | Tests immediate load instructions | Passes all tests | ✅ | ✅ | ✅ |
| 05-op rp.gb | Tests register pair instructions | Passes all tests | ✅ | ✅ | ✅ |
| 06-ld r,r.gb | Tests register to register load instructions | Passes all tests | ✅ | ✅ | ✅ |
| 07-jr,jp,call,ret,rst.gb | Tests control flow instructions | Passes all tests | ✅ | ✅ | ✅ |
| 08-misc instrs.gb | Tests miscellaneous instructions | Passes all tests | ✅ | ✅ | ✅ |
| 09-op r,r.gb | Tests register to register arithmetic and logic instructions | Passes all tests | ✅ | ✅ | ✅ |
| 10-bit ops.gb | Tests bit manipulation instructions | Passes all tests | ✅ | ✅ | ✅ |
| 11-op a,(hl).gb | Tests accumulator and (HL) instructions | Passes all tests | ✅ | ✅ | ✅ |
| cpu_instrs.gb | Comprehensive CPU instruction test | Fails on test 3 (STOP instruction) | ❌ | ❌ | ❌ |

#### DMG Sound

Not implemented yet.

#### Instr Timing

| ROM Name |  Notes | Instr | M-cycle | T-cycle |
|----------|--------|:-----:|:-------:|:-------:|
| instr_timing.gb | Pass | ✅ | ✅ | ✅ |

#### Interrupt Time

| ROM Name |  Notes | Instr | M-cycle | T-cycle |
|----------|--------|:-----:|:-------:|:-------:|
| interrupt_time.gb | | ❌ | ❌ | ❌ |

#### Mem Timing

| ROM Name |  Notes | Instr | M-cycle | T-cycle |
|----------|--------|:-----:|:-------:|:-------:|
| 01-read_timing.gb | | ❌ | ✅ | ✅ |
| 02-write_timing.gb | | ❌ | ✅ | ✅ |
| 03-modify_timing.gb | | ❌ | ❌ | ❌ |

#### Mem Timing 2

| ROM Name |  Notes | Instr | M-cycle | T-cycle |
|----------|--------|:-----:|:-------:|:-------:|
| 01-read_timing.gb | | ❌ | ✅ | ✅ |
| 02-write_timing.gb | | ❌ | ✅ | ✅ |
| 03-modify_timing.gb | | ❌ | ❌ | ❌ |

#### OAM Bug

| ROM Name |  Notes | Instr | M-cycle | T-cycle |
|----------|--------|:-----:|:-------:|:-------:|
| 1-lcd_sync.gb | | ❌ | ❌ | ❌ |
| 2-causes.gb | | ❌ | ❌ | ❌ |
| 3-non_causes.gb | Pass | ✅ | ✅ | ✅ |
| 4-scanline_timing.gb |   | ❌ | ❌ | ❌ |
| 5-timing_bug.gb | | ❌ | ❌ | ❌ |
| 6-timing_no_bug.gb | Pass | ✅ | ✅ | ✅ |
| 7-timing_effect.gb | | ❌ | ❌ | ❌ |
| 8-instr_effect.gb | | ❌ | ❌ | ❌ |

### Mooneye Test Suite

> Source: [Gekkio's Mooneye Test Suite](/README.md#test-roms)

#### Acceptance tests

#### General

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| add_sp_e_timing.gb | | ❌ | ❌ | ❌ |
| boot_div-dmg0.gb | DIV counter is off on start | ❌ | ✅ | ✅ |
| boot_hwio-dmg0.gb | Fails on PPU.STAT | ❌ | ❌ | ❌ |
| boot_regs-dmg0.gb | Pass | ✅ | ✅ | ✅ |
| call_cc_timing.gb | | ❌ | ❌ | ❌ |
| call_cc_timing2.gb | | ❌ | ❌ | ❌ |
| call_timing.gb | | ❌ | ❌ | ❌ |
| call_timing2.gb | | ❌ | ❌ | ❌ |
| di_timing-GS.gb | | ❌ | ✅ | ✅ |
| div_timing.gb | Pass | ✅ | ✅ | ✅ |
| ei_sequence.gb | | ❌ | ✅ | ✅ |
| ei_timing.gb | Pass | ✅ | ✅ | ✅ |
| halt_ime0_ei.gb | Pass | ✅ | ✅ | ✅ |
| halt_ime0_nointr_timing.gb |  | ✅ | ✅ | ❌ |
| halt_ime1_timing.gb | Pass | ✅ | ✅ | ✅ |
| halt_ime1_timing2-GS.gb | | ❌ | ✅ | ✅ |
| if_ie_registers.gb |  | ❌ | ❌ | ❌ |
| intr_timing.gb | Pass | ✅ | ✅ | ✅ |
| jp_cc_timing.gb | | ❌ | ❌ | ❌ |
| jp_timing.gb | | ❌ | ❌ | ❌ |
| ld_hl_sp_e_timing.gb | | ❌ | ❌ | ❌ |
| oam_dma_restart.gb | | ❌ | ❌ | ❌ |
| oam_dma_start.gb | | ❌ | ❌ | ❌ |
| oam_dma_timing.gb | | ❌ | ❌ | ❌ |
| pop_timing.gb | | ❌ | ❌ | ❌ |
| push_timing.gb | | ❌ | ❌ | ❌ |
| rapid_di_ei.gb | | ❌ | ✅ | ✅ |
| ret_cc_timing.gb | | ❌ | ❌ | ❌ |
| reti_intr_timing.gb | Pass | ✅ | ✅ | ✅ |
| reti_timing.gb | | ❌ | ❌ | ❌ |
| ret_timing.gb | | ❌ | ❌ | ❌ |
| rst_timing.gb | | ❌ | ❌ | ❌ |

#### Bits

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| mem_oam.gb | Pass | ✅ | ✅ | ✅ |
| reg_f.gb | Pass | ✅ | ✅ | ✅ |
| unused_hwio-GS.gb | | ❌ | ❌ | ❌ |

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
| sources-GS.gb | Need MBC5 to run | ❌ | ❌ | ❌ |

#### PPU

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| hblank_ly_scx_timing-GS.gb | | ❌ | ❌ | ❌ |
| intr_1_2_timing-GS.gb | | ❌ | ✅ | ✅ |
| intr_2_0_timing.gb | | ❌ | ✅ | ✅ |
| intr_2_mode0_timing.gb | | ✅ | ❌ | ❌ |
| intr_2_mode0_timing_sprites.gb | | ❌ | ❌ | ❌ |
| intr_2_mode3_timing.gb | | ✅ | ❌ | ❌ |
| intr_2_oam_ok_timing.gb | | ✅ | ❌ | ❌ |
| lcdon_timing-GS.gb | | ❌ | ❌ | ❌ |
| lcdon_write_timing-GS.gb | | ❌ | ❌ | ❌ |
| stat_irq_blocking.gb | | ❌ | ❌ | ❌ |
| stat_lyc_onoff.gb | | ❌ | ❌ | ❌ |
| vblank_stat_intr-GS.gb | | ❌ | ❌ | ❌ |

#### Timer

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| div_write.gb | Pass | ✅ | ✅ | ✅ |
| rapid_toggle.gb | | ✅ | ❌ | ❌ |
| tim00_div_trigger.gb | Pass | ✅ | ✅ | ✅ |
| tim00.gb | Pass | ✅ | ✅ | ✅ |
| tim01_div_trigger.gb | Pass | ✅ | ✅ | ✅ |
| tim01.gb | Pass | ✅ | ✅ | ✅ |
| tim10_div_trigger.gb | Pass | ✅ | ✅ | ✅ |
| tim10.gb | Pass | ✅ | ✅ | ✅ |
| tim11_div_trigger.gb | Pass | ✅ | ✅ | ✅ |
| tim11.gb | Pass | ✅ | ✅ | ✅ |
| tima_reload.gb | Pass | ✅ | ✅ | ✅ |
| tima_write_reloading.gb | Pass | ✅ | ✅ | ✅ |
| tma_write_reloading.gb | Pass | ✅ | ✅ | ✅ |

#### Emulator Only

#### MBC1

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| bits_bank1.gb | Pass | ✅ | ✅ | ✅ |
| bits_bank2.gb | Pass | ✅ | ✅ | ✅ |
| bits_mode.gb | Pass | ✅ | ✅ | ✅ |
| bits_ramg.gb | Pass | ✅ | ✅ | ✅ |
| multicart_rom_8Mb.gb | | ❌ | ❌ | ❌ |
| ram_256kb.gb | Fail: Round 2 | ❌ | ❌ | ❌ |
| ram_64kb.gb | Fail: Round 2 | ❌ | ❌ | ❌ |
| rom_16Mb.gb | Seg Fault: Div by 0 | ❌ | ❌ | ❌ |
| rom_1Mb.gb | | ❌ | ❌ | ❌ |
| rom_2Mb.gb | | ❌ | ❌ | ❌ |
| rom_512kb.gb | | ❌ | ❌ | ❌ |
| rom_4Mb.gb | Seg Fault: Div by 0 | ❌ | ❌ | ❌ |
| rom_8Mb.gb | Seg Fault: Div by 0 | ❌ | ❌ | ❌ |

#### MBC2

Not implemented yet.

#### MBC5

Not implemented yet.

## Other Test ROMs

### Double-halt-cancel

> Source: [little-things-gb GitHub repository](https://github.com/nitro2k01/little-things-gb)

| ROM Name | Notes | Instr | M-cycle | T-cycle |
|----------|-------|:-----:|:-------:|:-------:|
| double_halt_cancel.gb | One extra div count in cycle modes | ✅ | ❌ | ❌ |

### dmg-acid2

> Source: [dmg-acid2 GitHub repository](/README.md#test-roms)

ROM Name: dmg-acid2.gb  
Status: ✅ Renders correctly.
