/**
 * @file mmu_constants.h
 * @brief Constants for the Memory Management Unit (MMU) of the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace boyboy::core::mmu {

// --- MMU memory map constants ---
static constexpr uint32_t MemoryMapSize = (1 << 16); // 16-bit address space (64KB)

static constexpr uint16_t ROMBank0Start = 0x0000;
static constexpr uint16_t ROMBank0End = 0x3FFF;
static constexpr uint16_t ROMBank1Start = 0x4000;
static constexpr uint16_t ROMBank1End = 0x7FFF;
static constexpr uint16_t ROMStart = ROMBank0Start;
static constexpr uint16_t ROMEnd = ROMBank1End;

static constexpr uint16_t VRAMStart = 0x8000;
static constexpr uint16_t VRAMEnd = 0x9FFF;

static constexpr uint16_t ERAMStart = 0xA000;
static constexpr uint16_t ERAMEnd = 0xBFFF;

static constexpr uint16_t WRAM0Start = 0xC000;
static constexpr uint16_t WRAM0End = 0xCFFF;
static constexpr uint16_t WRAM1Start = 0xD000;
static constexpr uint16_t WRAM1End = 0xDFFF;
static constexpr uint16_t WRAMStart = WRAM0Start;
static constexpr uint16_t WRAMEnd = WRAM1End;

static constexpr uint16_t ECHOStart = 0xE000;
static constexpr uint16_t ECHOEnd = 0xFDFF;

static constexpr uint16_t OAMStart = 0xFE00;
static constexpr uint16_t OAMEnd = 0xFE9F;

static constexpr uint16_t NotUsableStart = 0xFEA0;
static constexpr uint16_t NotUsableEnd = 0xFEFF;

static constexpr uint16_t IOStart = 0xFF00;
static constexpr uint16_t IOEnd = 0xFF7F;

static constexpr uint16_t HRAMStart = 0xFF80;
static constexpr uint16_t HRAMEnd = 0xFFFE;

static constexpr uint16_t IEAddr = 0xFFFF;

// --- MMU memory bank sizes ---
static constexpr size_t ROMBank0Size = ROMBank0End - ROMBank0Start + 1;
static constexpr size_t ROMBank1Size = ROMBank1End - ROMBank1Start + 1;
static constexpr size_t ROMBankSize = ROMBank0Size + ROMBank1Size;
static constexpr size_t VRAMSize = VRAMEnd - VRAMStart + 1;
static constexpr size_t ERAMSize = ERAMEnd - ERAMStart + 1;
static constexpr size_t WRAM0Size = WRAM0End - WRAM0Start + 1;
static constexpr size_t WRAM1Size = WRAM1End - WRAM1Start + 1;
static constexpr size_t WRAMSize = WRAM0Size + WRAM1Size;
static constexpr size_t OAMSize = OAMEnd - OAMStart + 1;
static constexpr size_t NotUsableSize = NotUsableEnd - NotUsableStart + 1;
static constexpr size_t IOSize = IOEnd - IOStart + 1;
static constexpr size_t HRAMSize = HRAMEnd - HRAMStart + 1;

// --- DMA constants ---
static constexpr uint16_t DMATransferSize = 160;                   // 160 bytes for DMA transfer
static constexpr uint16_t DMATransferCycles = DMATransferSize * 4; // 4 cycles per byte

// --- Other MMU constants ---
static constexpr uint8_t OpenBusValue = 0xFF; // Default value for open bus reads

} // namespace boyboy::core::mmu