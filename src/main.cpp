#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <iterator>
#include "Util.h"
#include "Chip8Emu.h"

int main( int argc, char** argv )
{

    if (argc != 2) {
        std::cerr << "You must provide the filename for the Chip8 ROM.";
        exit(-1);
    }
    
    // keyboard definitions
    std::map<sf::Keyboard::Key, uint8_t> keymapping;
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num0, 0x0));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num1, 0x1));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num2, 0x2));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num3, 0x3));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num4, 0x4));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num5, 0x5));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num6, 0x6));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num7, 0x7));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num8, 0x8));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::Num9, 0x9));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::A, 0xA));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::B, 0xB));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::C, 0xC));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::D, 0xD));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::E, 0xE));
    keymapping.insert(std::pair<sf::Keyboard::Key, uint8_t>(sf::Keyboard::F, 0xF));

    // Prepare sound
    bool was_playing {false};
	sf::Int16 raw[SAMPLES];

	const double TWO_PI = 6.28318;
	const double increment = 440./44100;
	double x = 0;
	for (unsigned i = 0; i < SAMPLES; i++) {
		raw[i] = AMPLITUDE * sin(x*TWO_PI);
		x += increment;
	}
	
	sf::SoundBuffer Buffer;
	if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		return 1;
	}

	sf::Sound Sound;
	Sound.setBuffer(Buffer);
    Sound.setLoop(true);

    // start emulation
    Chip8Emu emu; // the emulator
    Util util;
    
    // load the program into emulator memory
    std::string filename = static_cast<std::string>(argv[1]);
    std::vector<DSK_BYTE> program = util.read_file(filename);
    emu.load_program(program);
    
    sf::RenderWindow window(sf::VideoMode(win_x_size, win_y_size), "Chip8 emulator");
    sf::Clock clock;
    uint64_t timer {};
    
    // calculate pixel size
    float px_x_size = int(win_x_size/frame_width);
    float px_y_size = int(win_y_size/frame_height);
    sf::RectangleShape shape(sf::Vector2(px_x_size, px_y_size));
    shape.setFillColor(sf::Color::Green);
    
    emu.init();
    clock.restart();
    
    while (window.isOpen()) {
        
        emu.step(clock.getElapsedTime().asMilliseconds()); // one step
        
        // sound
        if (emu.is_speaker_active() && !was_playing) {
            Sound.play();
            was_playing = true;
        } else if (!emu.is_speaker_active() && was_playing) {
            Sound.stop();
            was_playing = false;
        }
        
        // events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // check for keys pressed
            std::map<sf::Keyboard::Key, uint8_t>::iterator itr; 
            for (itr =  keymapping.begin(); itr !=  keymapping.end(); ++itr) {
                if (sf::Keyboard::isKeyPressed(itr->first)) {
                    emu.press_key(itr->second);
                } else {
                    emu.release_key(itr->second);
                }
            }
        }

        // a quick and dirty drawing temporization
        if (clock.getElapsedTime().asMilliseconds() - timer > HZ60) {
        
            window.clear();
            
            // draw screen
            std::vector<bool> screen = emu.get_screen();
            for (uint16_t i=0; i<frame_height; i++) {
                for (uint16_t j=0; j<frame_width; j++) {
                    if (screen[i*frame_width+j]) {
                        shape.setPosition(j*px_x_size, i*px_y_size);
                        window.draw(shape);
                    }
                }
            }
            window.display();
            timer = clock.getElapsedTime().asMilliseconds();
        }
        
    }

    return 0;    
}
