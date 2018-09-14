//
// Created by zepheon on 5/09/18.
//

#pragma once

#include <cstdint>

struct registers {
//        struct {
    union {
        struct {
            uint8_t a;
            uint8_t f;
        };
        uint16_t af;
    };
//        };

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