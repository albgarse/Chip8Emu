#include "Memory.h"

Memory::Memory() {
    // copy charset to memory
    for (int i=0; i<0xF*80; i++) {
        memory[CHARSET_BASE + i] = static_cast<std::byte>(charset[i]);
    }
}

std::byte Memory::read_byte(const MEM_ADDR& addr) const {
    if (addr >= MIN_ADDR && addr <= MAX_ADDR) {
        return memory[addr];
    } else {
        throw std::invalid_argument( "Chip8 memory overflow" );
    }
}

void Memory::write_byte(const MEM_ADDR& addr, const std::byte& data) {
    if (addr >= MIN_ADDR && addr <= MAX_ADDR) {
        memory[addr] = data;
    } else {
        throw std::invalid_argument( "Chip8 memory overflow" );
    }    
}
    
MEM_WORD Memory::read_word(const MEM_ADDR& addr) const {
    if (addr >= MIN_ADDR && addr <= MAX_ADDR) {
        MEM_WORD w {};
        w.byte.b1 = memory[addr];
        w.byte.b2 = memory[addr+1];
        return w;
    } else {
        throw std::invalid_argument( "Chip8 memory overflow" );
    }    
}
    
void Memory::write_word(const MEM_ADDR& addr, const MEM_WORD& data) {
    if (addr >= MIN_ADDR && addr <= MAX_ADDR) {
        memory[addr] = data.byte.b1;
        memory[addr+1] = data.byte.b2;
    } else {
        throw std::invalid_argument( "Chip8 memory overflow" );
    }    
}
