/* Chip8Emu
 * Internal interpreter state
 */
 
#include "constants.h"

class State {

private:
    uint8_t Vx[0xF] {};     // Vx registers (V0 to VF)
    MEM_ADDR IR {};         // I register (mem addresses)
    uint8_t DR {};          // the delay register
    uint8_t SR {};          // the sound register
    MEM_ADDR PC {};         // PC register (Program counter)
    uint8_t SP {};          // SP register (Stack Pointer)
    
public:
    void set_Vx(const uint8_t& reg, const uint8_t& value);
    uint8_t get_Vx(const uint8_t& reg) const;
    void set_IR(const MEM_ADDR& addr);
    MEM_ADDR get_IR() const;
    uint8_t get_DR() const;
    void set_DR(const uint8_t& value);
    uint8_t get_SR() const;
    void set_SR(const uint8_t& value);
    MEM_ADDR get_PC() const;
    void set_PC(const MEM_ADDR& addr);
    MEM_ADDR get_SP() const;
    void set_SP(const MEM_ADDR& value);
};