/* Chip8Emu
 * Interpreter
 */
#include <cstddef>
#include <cstdlib>
#include <vector>

#include "constants.h"
#include "State.h"
#include "Memory.h"
#include "FrameBuffer.h"
#include "InputOutput.h"

class Chip8Emu {

private:
    MEM_WORD cur_instruction;
    
    // nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
    // n or nibble - A 4-bit value, the lowest 4 bits of the instruction
    // x - A 4-bit value, the lower 4 bits of the high byte of the instruction
    // y - A 4-bit value, the upper 4 bits of the low byte of the instruction
    // kk or byte - An 8-bit value, the lowest 8 bits of the instruction
    // in - instruction byte.
    // fl - flavor of the instruction (LSB nibble)
    uint16_t nnn;
    uint8_t kk;
    uint8_t x, y, in, fl;
    
    State st;
    Memory mem;
    FrameBuffer fb;
    InputOutput io;
    
    uint64_t sound_timer {};
    uint64_t cpu_timer {};
 
    void fetch();
    void decode();
    void execute();
   
public:
    void init();
    void step(int64_t timer);
    void load_program(const std::vector<DSK_BYTE>& program);
    void press_key(const uint8_t& key);
    void release_key(const uint8_t& key);
    bool is_speaker_active() const;
    std::vector<bool> get_screen() const;
    

};
