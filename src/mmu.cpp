/**
 * @file mmu.cpp
 * @brief Memory Management Unit (MMU) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "mmu.h"

#include <cstdint>
#include <span>

namespace boyboy::mmu {

uint8_t* Mmu::get_pointer(uint16_t address) const
{
    for (const auto& region : region_table_) {
        if (address >= region.start && address <= region.end) {
            auto index = static_cast<size_t>(address - region.start);
            return &region.data[index];
        }
    }

    // Unusable or invalid memory
    return nullptr;
}

uint8_t Mmu::read_byte(uint16_t address) const
{
    auto* pointer = get_pointer(address);
    auto pointer_val = *pointer;
    return pointer_val;
    // return *get_pointer(address);
}

uint16_t Mmu::read_word(uint16_t address) const
{
    return (*get_pointer(address) << 8) + *get_pointer(address + 1);
}

void Mmu::write_byte(uint16_t address, uint8_t value)
{
    *get_pointer(address) = value;
}

void Mmu::write_word(uint16_t address, uint16_t value)
{
    *get_pointer(address) = static_cast<uint8_t>(value >> 8);
    *get_pointer(address + 1) = static_cast<uint8_t>(value & 0xFF);
}

void Mmu::copy(uint16_t dst_address, std::span<uint8_t> src)
{
    for (size_t i = 0; i < src.size(); i++) {
        write_byte(dst_address + i, src[i]);
    }
}

} // namespace boyboy::mmu