/**
 * @file registers.h
 * @brief CPU registers for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */
#pragma once

#include <cstdint>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

namespace boyboy::cpu {

// TODO: avoid anonymous structs
struct Registers {
    union {
        struct {
            uint8_t a;
            uint8_t f;
        };
        uint16_t af;
    };

    struct {
        union {
            struct {
                uint8_t b;
                uint8_t c;
            };
            uint16_t bc;
        };
    };

    struct {
        union {
            struct {
                uint8_t d;
                uint8_t e;
            };
            uint16_t de;
        };
    };

    struct {
        union {
            struct {
                uint8_t h;
                uint8_t l;
            };
            uint16_t hl;
        };
    };

    uint16_t sp;
    uint16_t pc;
};

} // namespace boyboy::cpu

#ifdef __clang__
#pragma clang diagnostic pop
#endif