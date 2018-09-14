//
// Created by zepheon on 5/09/18.
//

#include <fstream>
#include <cartridge.h>
#include <iostream>

#include "cartridge.h"

void Cartridge::load(std::string path) {

    std::ifstream rom(path, std::ios::in | std::ios::binary);

    m_rom = std::vector<uint8_t>((std::istreambuf_iterator<char>(rom)), std::istreambuf_iterator<char>());

//    rom.unsetf(std::ios::skipws);

//    std::streampos file_size;
//
//    rom.seekg(0, std::ios::end);
//    file_size = rom.tellg();
//    rom.seekg(0, std::ios::beg);

//    m_rom.reserve(static_cast<unsigned long>(file_size));

//    m_rom.insert(m_rom.begin(),
//    std::istream_iterator)


}

void Cartridge::parseHeader() {

    m_header.title = "";

    uint16_t pos = TITLE_POS;
    char c = 0;
    while ((c = m_rom.at(pos)) != 0 && pos++ <= 0x143) {
        m_header.title += c;
    }

    m_header.cgb_flag = m_rom.at(0x143);
    m_header.sgb_flag = m_rom.at(0x146);
    m_header.cartridge_type = static_cast<CartridgeType>(m_rom.at(0x147));
    m_header.rom_size = m_rom.at(0x148);
    m_header.ram_size = m_rom.at(0x149);
    m_header.header_checksum = m_rom.at(0x14D);
    m_header.checksum = ((m_rom.at(0x14E) << 8) + m_rom.at(0x14F));
}

std::string Cartridge::Header::CartridgeTypeToStr(CartridgeType type) {
    switch (type) {
        case ROM_ONLY: return "ROM_ONLY";
        case MBC1: return "MBC1";
        case MBC1_RAM: return "MBC1_RAM";
        case MBC1_RAM_BATTERY: return "MBC1_RAM_BATTERY";
        case MBC2: return "MBC2";
        case MBC2_BATTERY: return "MBC2_BATTERY";
        case ROM_RAM: return "ROM_RAM";
        case ROM_RAM_BATTERY: return "ROM_RAM_BATTERY";
        case MMM01: return "MMM01";
        case MMM01_RAM: return "MMM01_RAM";
        case MMM01_RAM_BATTERY: return "MMM01_RAM_BATTERY";
        case MBC3_TIMER_BATTERY: return "MBC3_TIMER_BATTERY";
        case MBC3_TIMER_RAM_BATTERY: return "MBC3_TIMER_RAM_BATTERY";
        case MBC3: return "MBC3";
        case MBC3_RAM: return "MBC3_RAM";
        case MBC3_RAM_BATTERY: return "MBC3_RAM_BATTERY";
        case MBC4: return "MBC4";
        case MBC4_RAM: return "MBC4_RAM";
        case MBC4_RAM_BATTERY: return "MBC4_RAM_BATTERY";
        case MBC5: return "MBC5";
        case MBC5_RAM: return "MBC5_RAM";
        case MBC5_RAM_BATTERY: return "MBC5_RAM_BATTERY";
        case MBC5_RUMBLE: return "MBC5_RUMBLE";
        case MBC5_RUMBLE_RAM: return "MBC5_RUMBLE_RAM";
        case MBC5_RUMBLE_RAM_BATTERY: return "MBC5_RUMBLE_RAM_BATTERY";
        case POCKET_CAMERA: return "POCKET_CAMERA";
        case BANDAI_TAMA5: return "BANDAI_TAMA5";
        case HUC3: return "HUC3";
        case HUC1_RAM_BATTERY: return "HUC1_RAM_BATTERY";
        default:
            return "UNKNOWN";
    }
}

void Cartridge::Header::print() {
    std::string output = "Title: " + this->title + "\n" +
            "CGB Flag: " + std::to_string(this->cgb_flag) + "\n" +
            "SGB Flag: " + std::to_string(this->sgb_flag) + "\n" +
            "Cartridge Type: " + this->CartridgeTypeToStr(this->cartridge_type) + "\n" +
            "ROM Size: " + std::to_string(this->rom_size) + "\n" +
            "RAM Size: " + std::to_string(this->ram_size) + "\n" +
            "Header Checksum: " + std::to_string(this->header_checksum) + "\n" +
            "Global Checksum: " + std::to_string(this->checksum);

    std::cout << output << std::endl;
}
