//
// Created by zepheon on 5/09/18.
//

#pragma once

#include <cstdint>
#include <cstdlib>

/**
 *
  0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
  4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
  8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
  A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
  C000-CFFF   4KB Work RAM Bank 0 (WRAM)
  D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
  E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
  FE00-FE9F   Sprite Attribute Table (OAM)
  FEA0-FEFF   Not Usable
  FF00-FF7F   I/O Ports
  FF80-FFFE   High RAM (HRAM)
  FFFF        Interrupt Enable Register
 */

class Mmu {

    // memory banks
    uint8_t cart[0x8000];   // cartridge
    uint8_t vram[0x2000];   // video ram
    uint8_t eram[0x2000];   // external ram
    uint8_t wram[0x2000];   // work ram
    uint8_t oam[0x100];     // sprite attribute table
    uint8_t io[0x80];       // i/o ports
    uint8_t hram[0x80];     // high ram
    uint8_t ie = 0;         // interrupt enable register


public:
    uint8_t readByte(uint16_t address);
    uint16_t readWord(uint16_t address);
    void writeByte(uint16_t address, uint8_t value);
    void writeWord(uint16_t address, uint16_t value);

    void copy(uint16_t dst_address, uint8_t *src, size_t len);

private:
    uint8_t *getPointer(uint16_t address);
};