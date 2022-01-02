src = $(wildcard *.cpp)
obj = $(src:.c=.o)

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
CFLAG = -Wall -g -std=c++17
CCP = g++

chip8: $(obj)
	$(CCP) $(CFLAG) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o chip8