/* Chip8Emu
 * FrameBuffer
 */
 
#include "constants.h"
#include <vector>

class FrameBuffer {

private:
    std::vector<bool> frame_buffer = std::vector<bool>(frame_height*frame_width);
    
public:
    void set_pixel(const Coordinate& c);
    void clear_pixel(const Coordinate& c);
    bool flip_pixel(const Coordinate& c);
    bool is_set(const Coordinate& c) const;
    void clear_screen();
    std::vector<bool> get_frame_buffer() const;
};