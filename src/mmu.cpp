//
// Created by zepheon on 5/09/18.
//

#include "mmu.h"

uint8_t *Mmu::getPointer(uint16_t address) {

    if (address < 0x8000)
        return cart + address;
    else if (address < 0xA000)
        return vram + (address - 0x8000);
    else if (address < 0xC000)
        return eram + (address - 0xA000);
    else if (address < 0xE000)
        return wram + (address - 0xC000);
    else if (address < 0xFE00)
        return wram + (address - 0xE000);
    else if (address < 0xFEA0)
        return oam + (address - 0xFE00);
    else if (address < 0xFF00)
        return nullptr;
    else if (address < 0xFF80)
        return io + (address - 0xFF00);
    else if (address < 0xFFFF)
        return hram + (address - 0xFF80);
    else if (address == 0xFFFF)
        return &ie;
    else
        return nullptr;

}

uint8_t Mmu::readByte(uint16_t address) {
    auto pointer = getPointer(address);
    auto pointer_val = *pointer;
    return pointer_val;
    //return *getPointer(address);
}

uint16_t Mmu::readWord(uint16_t address) {
    return (*getPointer(address) << 8) + *getPointer(address+1);
}

void Mmu::writeByte(uint16_t address, uint8_t value) {
    *getPointer(address) = value;
}

void Mmu::writeWord(uint16_t address, uint16_t value) {
    *getPointer(address) = static_cast<uint8_t>(value >> 8);
    *getPointer(address+1) = static_cast<uint8_t>(value & 0xFF);
}

void Mmu::copy(uint16_t dst_address, uint8_t *src, size_t len) {
    for (int i = 0; i < len; i++) {
        writeByte(dst_address+i, src[i]);
    }
}
