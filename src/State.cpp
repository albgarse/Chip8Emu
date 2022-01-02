#include "State.h"
#include <iostream>

void State::set_Vx(const uint8_t& reg, const uint8_t& value) {
    if (reg >= MIN_REG && reg <= MAX_REG) {
        Vx[reg] = value;
    } else {
        throw std::invalid_argument( "Chip8 invalid register" );
    }
}

uint8_t State::get_Vx(const uint8_t& reg) const {
    if (reg >= MIN_REG && reg <= MAX_REG) {
        return Vx[reg];
    } else {
        throw std::invalid_argument( "Chip8 invalid register" );
    }
}

void State::set_IR(const MEM_ADDR& addr) {
    if (addr >= MIN_ADDR && addr <= MAX_ADDR) {
        IR = addr;
    } else {
        throw std::invalid_argument( "Chip8 memory overflow" );
    }
}

MEM_ADDR State::get_IR() const {
    return IR;
}

uint8_t State::get_DR() const {
    return DR;
}

void State::set_DR(const uint8_t& value){
    DR = value;
}

uint8_t State::get_SR() const {
    return SR;
}

void State::set_SR(const uint8_t& value){
    SR = value;
}

MEM_ADDR State::get_PC() const {
    return PC;
}

void State::set_PC(const MEM_ADDR& addr) {
    if (addr >= MIN_ADDR && addr <= MAX_ADDR) {
        PC = addr;
    } else {
        throw std::invalid_argument( "Chip8 memory overflow" );
    }    
}

MEM_ADDR State::get_SP() const {
    return SP;
}

void State::set_SP(const MEM_ADDR& value) {
    if (value >= MIN_REG && value <= MAX_REG) {
        SP = value;
    } else {
        throw std::invalid_argument( "Chip8 invalid register" );
    }
}
