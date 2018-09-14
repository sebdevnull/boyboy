#include <iostream>
#include <cartridge.h>

#include "cpu.h"

int main() {

    Cartridge cart = Cartridge();

    cart.load("../roms/tetris.gb");
    cart.parseHeader();

    cart.m_header.print();

    return 0;
}