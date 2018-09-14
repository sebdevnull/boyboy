//
// Created by zepheon on 5/09/18.
//

#pragma once

#include <string>
#include <vector>



enum CartridgeType {
    ROM_ONLY = 0x00,
    MBC1,
    MBC1_RAM,
    MBC1_RAM_BATTERY,
    MBC2 = 0x05,
    MBC2_BATTERY,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY,
    MMM01 = 0x0B,
    MMM01_RAM,
    MMM01_RAM_BATTERY,
    MBC3_TIMER_BATTERY = 0x0F,
    MBC3_TIMER_RAM_BATTERY,
    MBC3,
    MBC3_RAM,
    MBC3_RAM_BATTERY,
    MBC4 = 0X15,
    MBC4_RAM,
    MBC4_RAM_BATTERY,
    MBC5 = 0X19,
    MBC5_RAM,
    MBC5_RAM_BATTERY,
    MBC5_RUMBLE,
    MBC5_RUMBLE_RAM,
    MBC5_RUMBLE_RAM_BATTERY,
    POCKET_CAMERA = 0xFC,
    BANDAI_TAMA5,
    HUC3,
    HUC1_RAM_BATTERY
};

class Cartridge {
public:

    const uint16_t TITLE_POS = 0x134;

    typedef struct {
        std::string title;
        uint8_t cgb_flag;
        uint8_t sgb_flag;
        CartridgeType cartridge_type;
        uint8_t rom_size;
        uint8_t ram_size;
        uint8_t header_checksum;
        uint16_t checksum;

        std::string CartridgeTypeToStr(CartridgeType type);
        void print();
    } Header;

    Header m_header;
    std::vector<uint8_t> m_rom;


    void load(std::string path);

    void parseHeader();
};
