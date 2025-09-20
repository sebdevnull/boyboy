/**
 * @file registers.h
 * @brief Ppu I/O registers for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

namespace boyboy::ppu::registers {

// LCD Control (LCDC) Register (R/W) - 0xFF40
struct LCDC {
    static constexpr uint8_t LCDAndPPUEnable = 0b10000000;     // Bit 7 - LCD and PPU enable
    static constexpr uint8_t WindowTileMap = 0b01000000;       // Bit 6 - Window Tile Map Area
    static constexpr uint8_t WindowEnable = 0b00100000;        // Bit 5 - Window Display Enable
    static constexpr uint8_t BGAndWindowTileData = 0b00010000; // Bit 4 - BG & Window Tile Data Area
    static constexpr uint8_t BGTileMapArea = 0b00001000;       // Bit 3 - BG Tile Map Area
    static constexpr uint8_t OBJSize = 0b00000100;             // Bit 2 - OBJ (Sprite) Size
    static constexpr uint8_t OBJEnable = 0b00000010;           // Bit 1 - OBJ (Sprite) Enable
    static constexpr uint8_t BGAndWindowEnable = 0b00000001;   // Bit 0 - BG & Window Display Enable

    static constexpr uint16_t BGTileMapArea0 = 0x9800;       // BGTileMapArea = 0
    static constexpr uint16_t BGTileMapArea1 = 0x9C00;       // BGTileMapArea = 1
    static constexpr uint16_t WindowTileMapArea0 = 0x9800;   // WindowTileMap = 0
    static constexpr uint16_t WindowTileMapArea1 = 0x9C00;   // WindowTileMap = 1
    static constexpr uint16_t BGAndWindowTileData0 = 0x9000; // BGAndWindowTileData = 0
    static constexpr uint16_t BGAndWindowTileData1 = 0x8000; // BGAndWindowTileData = 1
};

// LCD Status (STAT) Register (Mixed) - 0xFF41
struct STAT {
    static constexpr uint8_t LYCInt = 0b01000000;         // Bit 6 - (RW) LYC=LY Interrupt
    static constexpr uint8_t Mode2OAMInt = 0b00100000;    // Bit 5 - (RW) Mode 2 OAM Interrupt
    static constexpr uint8_t Mode1VBlankInt = 0b00010000; // Bit 4 - (RW) Mode 1 V-Blank Interrupt
    static constexpr uint8_t Mode0HBlankInt = 0b00001000; // Bit 3 - (RW) Mode 0 H-Blank Interrupt
    static constexpr uint8_t LYCEqualsLY = 0b00000100;    // Bit 2 - (RO) LYC=LY Flag
    static constexpr uint8_t PPUModeFlag1 = 0b00000010;   // Bit 1 - (RO) Mode Flag bit 1
    static constexpr uint8_t PPUModeFlag0 = 0b00000001;   // Bit 0 - (RO) Mode Flag bit 0

    static constexpr uint8_t PPUModeMask = PPUModeFlag1 | PPUModeFlag0;

    enum class PPUMode : uint8_t { HBlank = 0b00, VBlank = 0b01, OAMRead = 0b10, VRAMRead = 0b11 };
};

} // namespace boyboy::ppu::registers