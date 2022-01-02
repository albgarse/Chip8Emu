/* 
 * Some constants
 */

#include <cstdint>
#include <cstddef>

#pragma once

// a value for NONE (ie. no key pressed)
const uint8_t NONE = -1;

// Frame buffer dimensions
const uint8_t frame_width = 64;
const uint8_t frame_height = 32;

// window size
const uint16_t win_x_size = 800;
const uint16_t win_y_size = 600;

// buffer coord
struct Coordinate {
    uint8_t x,y;
};

// temporization
const uint16_t HZ60 = 1000/60;
const uint16_t CPU_rate = 2;

// Word size in bytes
const uint8_t WORD_SIZE = 2;

// type representing a memory address
typedef uint16_t MEM_ADDR;

// type to bytes read from disk
typedef unsigned char DSK_BYTE;
 
// Max and Min values for Vx and SP registers
const uint8_t MIN_REG = 0x0;
const uint8_t MAX_REG = 0xF;

// charset base
const uint16_t CHARSET_BASE = 0x100;

// program entry point
const uint16_t PROGRAM_ENTRY_POINT = 0x200;

// Max and Min memory address
const MEM_ADDR MIN_ADDR = 0;
const MEM_ADDR MAX_ADDR = 0xFFF;

// type to hold a 16 bits word while allowing single byte and single nibble access. 
union MEM_WORD {
    uint16_t word;  
  
    struct Byte {
        std::byte b1, b2; 
    } byte;
    
    struct Nibble { 
        std::byte n1:4;
        std::byte n2:4;
        std::byte n3:4;
        std::byte n4:4;
    } nibble;  
};

