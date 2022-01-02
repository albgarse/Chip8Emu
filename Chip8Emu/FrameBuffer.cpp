#include "FrameBuffer.h"

void FrameBuffer::set_pixel(const Coordinate& c) {
    uint8_t x = c.x % frame_width;
    uint8_t y = c.y % frame_height;
    frame_buffer[y * frame_width + x] = true;
}

void FrameBuffer::clear_pixel(const Coordinate& c) {
    uint8_t x = c.x % frame_width;
    uint8_t y = c.y % frame_height;
    frame_buffer[y * frame_width + x] = false;
}

bool FrameBuffer::flip_pixel(const Coordinate& c) {
    if (is_set(c)) {
        clear_pixel(c);
        return true;
    } else {
        set_pixel(c);
        return false;
    }
}

bool FrameBuffer::is_set(const Coordinate& c) const {
    return frame_buffer[c.y*frame_width+c.x];
}

void FrameBuffer::clear_screen() {
    for (uint16_t i=0; i<frame_buffer.size(); i++) {
        frame_buffer[i] = false;
    }        
}

std::vector<bool> FrameBuffer::get_frame_buffer() const {
    return frame_buffer;
}