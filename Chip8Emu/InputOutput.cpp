#include "InputOutput.h"

void InputOutput::press_key(const uint8_t& key) {
    keys[key] = true;
}

void InputOutput::release_key(const uint8_t& key) {
    keys[key] = false;
}

//void InputOutput::update_keyboard_state(const std::vector<bool>& keystate) {
//    if (keystate.size() == keys.size()) {
//        keys = keystate;
//    }
//}

bool InputOutput::is_pressed(const uint8_t& key) const {
    return keys[key];
}

uint8_t InputOutput::is_any_key_pressed() const {
    for (uint8_t i=0; i<0xF; i++) {
        if (is_pressed(i)) {
            return i;
        }
    }
    return NONE;
}