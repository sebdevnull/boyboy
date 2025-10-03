# Tested ROMs status

This document lists the ROMs that have been tested with the BoyBoy emulator, along with their current status. The goal is to ensure compatibility with a wide range of games and test ROMs to validate the accuracy of the emulator.  
It includes both commercial games and well-known test ROMs used in the emulator development community.

> See [LICENSE-THIRD-PARTY.md](LICENSE-THIRD-PARTY.md) for detailed attribution, licenses, and legal notices.

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
| Donkey Kong | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Donkey Kong Land | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Donkey Kong Land 2 | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Donkey Kong Land III | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| DuckTales | MBC1 | Fully playable | ✅ |
| Final Fantasy Legend II | MBC1+RAM+BATTERY | Game starts with blank screen | ❌ |
| Final Fantasy Legend III | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| James Bond 007 | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Jungle Strike | MBC1 | Fully playable | ✅ |
| Kirby's Dream Land | MBC1 | Fully playable | ✅ |
| Kirby's Dream Land 2 | MBC1+RAM+BATTERY | Game starts with blank screen | ❌ |
| Mario's Picross | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Mega Man V | MBC1 | Fully playable | ✅ |
| Metroid II | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Mole Mania | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| NBA Jam | MBC1 | Fully playable | ✅ |
| NBA Jam TE | MBC1 | Fully playable | ✅ |
| Pacman | MBC1 | Fully playable | ✅ |
| Paperboy | MBC1 | Fully playable | ✅ |
| Super Mario Land | MBC1 | Fully playable | ✅ |
| Super Mario Land 2: 6 Golden Coins  | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Super Mario Land 3: Wario Land | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| The Legend of Zelda: Link's Awakening | MBC1+RAM+BATTERY | Game playable, battery-backed RAM not implemented | ⏳ |
| Tumble Pop | MBC1 | Game starts, screen goes blank after intro | ❌ |

>Only ROMs that have been legally obtained by the user should be used with this emulator.  
No copyrighted commercial Nintendo ROMs are distributed with this project.

## Test ROMs

### CPU Test ROMs

#### Blargg's CPU Test ROMs

> Source: [Blargg's Test ROMs](/README.md#test-roms)

| ROM Name | Description | Notes | Status |
|----------|-------------|-------|:------:|
| 01-special.gb | Tests special CPU instructions | Passes all tests | ✅ |
| 02-interrupts.gb | Tests interrupt handling | Hangs on "HALT bug" test | ❌ |
| 03-op sp,hl.gb | Tests stack pointer and HL instructions | Passes all tests | ✅ |
| 04-op r,imm.gb | Tests immediate load instructions | Passes all tests | ✅ |
| 05-op rp.gb | Tests register pair instructions | Passes all tests | ✅ |
| 06-ld r,r.gb | Tests register to register load instructions | Passes all tests | ✅ |
| 07-jr,jp,call,ret,rst.gb | Tests control flow instructions | Passes all tests | ✅ |
| 08-misc instrs.gb | Tests miscellaneous instructions | Passes all tests | ✅ |
| 09-op r,r.gb | Tests register to register arithmetic and logic instructions | Passes all tests | ✅ |
| 10-bit ops.gb | Tests bit manipulation instructions | Passes all tests | ✅ |
| 11-op a,(hl).gb | Tests accumulator and (HL) instructions | Passes all tests | ✅ |
| cpu_instrs.gb | Comprehensive CPU instruction test | Fails on test 2 (interrupts) | ❌ |

### PPU Test ROMs

#### dmg-acid2

> Source: [dmg-acid2 GitHub repository](/README.md#test-roms)

ROM Name: dmg-acid2.gb  
Status: ✅ Renders correctly.
