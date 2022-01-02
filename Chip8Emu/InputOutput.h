/* Chip8Emu
 * Input (keyboard) and output (sound)
 */
 
#include <vector>
#include <cstdint>
#include "constants.h"

class InputOutput {

private:
    // keys 0 to F
    std::vector<bool> keys = std::vector<bool>(0xF);

public:
    //void update_keyboard_state(const std::vector<bool>& keystate);
    void press_key(const uint8_t& key);
    void release_key(const uint8_t& key);
    bool is_pressed(const uint8_t& key) const;
    uint8_t is_any_key_pressed() const; // return the first pressed key. NONE otherwise.
};