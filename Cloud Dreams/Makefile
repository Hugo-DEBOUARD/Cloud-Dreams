CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

irrlicht-physique: main.o events.o
	g++ $^ -o $@ -lIrrlicht

clean:
	@rm -f irrlicht-physique *.o
