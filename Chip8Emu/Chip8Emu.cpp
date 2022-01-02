#include "Chip8Emu.h"

void Chip8Emu::load_program(const std::vector<DSK_BYTE>& program) {
    MEM_ADDR mem_pos = PROGRAM_ENTRY_POINT;
    for (DSK_BYTE b : program) {
        mem.write_byte(mem_pos++, static_cast<std::byte>(b));
    }
}

std::vector<bool> Chip8Emu::get_screen() const {
    return fb.get_frame_buffer();
}


void Chip8Emu::press_key(const uint8_t& key) {
    io.press_key(key);
}


void Chip8Emu::release_key(const uint8_t& key) {
    io.release_key(key);
}

void Chip8Emu::step(int64_t timer) {

    uint64_t ellapsed_sound_time = timer - sound_timer;
    uint64_t ellapsed_cpu_time = timer - cpu_timer;
    
    // decrease DR and SR at a 60hz rate
    if (ellapsed_sound_time > HZ60)  {
        sound_timer = timer;
        
        if (st.get_DR()>0) {
            st.set_DR(st.get_DR()-1);
        }
        
        if (st.get_SR()>0) {
            st.set_SR(st.get_SR()-1);
        }
    }
    
    if (ellapsed_cpu_time > CPU_rate) {
        cpu_timer = timer;
        
        fetch();
        decode();
        execute();
    }
}

void Chip8Emu::init() {
    st.set_PC(PROGRAM_ENTRY_POINT);
}


void Chip8Emu::fetch() {
    MEM_ADDR pc_addr = st.get_PC();
    cur_instruction = mem.read_word(pc_addr);
    st.set_PC(pc_addr + WORD_SIZE); // point to next instruction
}

void Chip8Emu::decode() {
    // instructions are stored most-significant-byte first
    std::byte b_temp = cur_instruction.byte.b1;
    cur_instruction.byte.b1 = cur_instruction.byte.b2;
    cur_instruction.byte.b2 = b_temp;
    
    // extract fields (wich we use depends on the instruction)
    nnn = cur_instruction.word & 0x0FFF; // remove MSB byte to get 12 bits address
    x = static_cast<uint8_t>(cur_instruction.nibble.n3);
    y = static_cast<uint8_t>(cur_instruction.nibble.n2);
    kk = static_cast<uint8_t>(cur_instruction.byte.b1);
    in = static_cast<uint8_t>(cur_instruction.nibble.n4);
    fl = static_cast<uint8_t>(cur_instruction.nibble.n1);
}

void Chip8Emu::execute() {
    if (in == 0 && kk != 0xE0 && kk != 0xEE) {
        // 0nnn - SYS addr
        // Not implemented.
        
    } else if (in == 0 && kk == 0xE0) {
        // 00E0 - CLS
        fb.clear_screen();
        
    } else if (in == 0 && kk == 0xEE) {
        // 00EE - RET
        if (st.get_SP()>=0) {
            st.set_PC(mem.read_word(st.get_SP()).word);
            st.set_SP(st.get_SP() - WORD_SIZE);
        } else {
            throw std::invalid_argument( "Chip8 stack overflow" );
        }
        
    } else if (in == 1) {
        // 1nnn - JP addr
        st.set_PC(nnn);

    } else if (in == 2) {
        // 2nnn - CALL addr
        if (st.get_SP() < 0xF) {
            st.set_SP(st.get_SP() + WORD_SIZE);
            MEM_WORD w; 
            w.word = st.get_PC();
            mem.write_word(st.get_SP(), w);
            st.set_PC(nnn);
        } else {
            throw std::invalid_argument( "Chip8 stack overflow" );
        }
        
    } else if (in == 3) {
        // 3xkk - SE Vx, byte
        if (st.get_Vx(x) == kk) {
            st.set_PC(st.get_PC() + WORD_SIZE);
        }
        
    } else if (in == 4) {
        // 4xkk - SNE Vx, byte
        if (st.get_Vx(x) != kk) {
            st.set_PC(st.get_PC() + WORD_SIZE);
        }

    } else if (in == 5 && fl == 0) {
        // 5xy0 - SE Vx, Vy
        if (st.get_Vx(x) == st.get_Vx(y)) {
            st.set_PC(st.get_PC() + WORD_SIZE);
        }
        
    } else if (in == 6) {
        // 6xkk - LD Vx, byte
        st.set_Vx(x, kk);
        
    } else if (in == 7) {
        // 7xkk - ADD Vx, byte
        st.set_Vx(x, st.get_Vx(x) + kk);
        
    } else if (in == 8 && fl == 0) {
        // 8xy0 - LD Vx, Vy
        st.set_Vx(x, st.get_Vx(y));
        
    } else if (in == 8 && fl == 1) {
        // 8xy1 - OR Vx, Vy
        st.set_Vx(x, st.get_Vx(x) | st.get_Vx(y));
        
    } else if (in == 8 && fl == 2) {
        // 8xy2 - AND Vx, Vy
        st.set_Vx(x, st.get_Vx(x) & st.get_Vx(y));

    } else if (in == 8 && fl == 3) {
        // 8xy3 - XOR Vx, Vy
        st.set_Vx(x, st.get_Vx(x) ^ st.get_Vx(y));

    } else if (in == 8 && fl == 4) {
        // 8xy4 - ADD Vx, Vy
        uint16_t result = st.get_Vx(x) + st.get_Vx(y);
        st.set_Vx(x, result);
        st.set_Vx(0xF, result > 255? 1 : 0);
        
    } else if (in == 8 && fl == 5) {
        // 8xy5 - SUB Vx, Vy
        st.set_Vx(0xF, st.get_Vx(x) > st.get_Vx(y)? 1 : 0);
        uint8_t result = st.get_Vx(x) - st.get_Vx(y);
        st.set_Vx(x, result);
        
    } else if (in == 8 && fl == 6) {
        // 8xy6 - SHR Vx {, Vy}
        st.set_Vx(0xF, st.get_Vx(x) & 0b00000001);
        st.set_Vx(x, st.get_Vx(x) >> 1); // or divide by 2
        
    } else if (in == 8 && fl == 7) {
        // 8xy7 - SUBN Vx, Vy
        st.set_Vx(0xF, st.get_Vx(y) > st.get_Vx(x)? 1 : 0);
        uint8_t result = st.get_Vx(y) - st.get_Vx(x);
        st.set_Vx(x, result);
        
    } else if (in == 8 && fl == 0xE) {
        // 8xyE - SHL Vx {, Vy}
        st.set_Vx(0xF, st.get_Vx(x) & 0b10000000);
        st.set_Vx(x, st.get_Vx(x) << 1); // or mult. by 2
        
    } else if (in == 9 && fl == 0) {
        // 9xy0 - SNE Vx, Vy
        if (st.get_Vx(x) != st.get_Vx(y)) {
            st.set_PC(st.get_PC() + WORD_SIZE);
        }
    
    } else if (in == 0xA) {
        // Annn - LD I, addr
        st.set_IR(nnn);
        
    } else if (in == 0xB) {
        // Bnnn - JP V0, addr
        st.set_PC(st.get_PC() + st.get_Vx(0));
        
    } else if (in == 0xC) {
        // Cxkk - RND Vx, byte
        srand(time(0));
        st.set_Vx(x, (rand() % 255) & kk);

    } else if (in == 0xD) {
        // Dxyn - DRW Vx, Vy, nibble
        st.set_Vx(0xF,0);
        MEM_ADDR base = st.get_IR();
        int n=fl;
        // draw 8xN sprite
        bool col;
        Coordinate c;
        int yy = st.get_Vx(y);
        for (int i=0; i<n; i++) {
            MEM_ADDR addr = base + i;
            uint8_t sprline = static_cast<uint8_t>(mem.read_byte(addr));
            int xx = st.get_Vx(x);
            for (int j=0x80; j>=0x01; j=j/0x2) {
                if ((sprline & j) == j) {
                    c.x = xx; c.y = yy;
                    col = fb.flip_pixel(c);
                    if (col)  {
                        st.set_Vx(0xF,1);
                    }
                }
                xx++;
            }
            yy++;
        }
        
    } else if (in == 0xE && kk == 0x9E) {
        // Ex9E - SKP Vx
        if (io.is_pressed(st.get_Vx(x))) {
            st.set_PC(st.get_PC() + WORD_SIZE);
        }
        
    } else if (in == 0xE && kk == 0xA1) {
        // ExA1 - SKNP Vx
        if (!io.is_pressed(st.get_Vx(x))) {
            st.set_PC(st.get_PC() + WORD_SIZE);
        }
        
    } else if (in == 0xF && kk == 0x07) {
        // Fx07 - LD Vx, DT
        st.set_Vx(x, st.get_DR());

    } else if (in == 0xF && kk == 0x0A) {
        // Fx0A - LD Vx, K
        uint8_t key;
        while((key = io.is_any_key_pressed()) != NONE) {
            st.set_Vx(x, key);
        }

    } else if (in == 0xF && kk == 0x15) {
        // Fx15 - LD DT, Vx
        st.set_DR(st.get_Vx(x));

    } else if (in == 0xF && kk == 0x18) {
        // Fx18 - LD ST, Vx
        st.set_SR(st.get_Vx(x));
        
    } else if (in == 0xF && kk == 0x1E) {
        // Fx1E - ADD I, Vx
        st.set_IR(st.get_IR() + st.get_Vx(x));
        
    } else if (in == 0xF && kk == 0x29) {
        // Fx29 - LD F, Vx
        st.set_IR(CHARSET_BASE + (st.get_Vx(x) * 5));
        
    } else if (in == 0xF && kk == 0x33) {
        // Fx33 - LD B, Vx
        uint8_t n = st.get_Vx(x);
        uint8_t cent = static_cast<uint8_t>(n/100);
        n = n - (cent*100);
        uint8_t dec = static_cast<uint8_t>(n/10);
        uint8_t units = n - (dec*10);
        mem.write_byte(st.get_IR(), static_cast<std::byte>( cent ));
        mem.write_byte(st.get_IR() + 1, static_cast<std::byte>( dec ));
        mem.write_byte(st.get_IR() + 2, static_cast<std::byte>( units ));
    
    } else if (in == 0xF && kk == 0x55) {
        // Fx55 - LD [I], Vx
        for (int i=0; i<=x; i++) {
            mem.write_byte(st.get_IR() + i, static_cast<std::byte>( st.get_Vx(i) ));
        }
        
    } else if (in == 0xF && kk == 0x65) {
        // Fx65 - LD Vx, [I]
        for (int i=0; i<=x; i++) {
            st.set_Vx(i, static_cast<uint8_t>(mem.read_byte(st.get_IR() + i)));
        }
    } else {
        throw std::invalid_argument( "Chip8 invalid opcode " );
    }
    
}