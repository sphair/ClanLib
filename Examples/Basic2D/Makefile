EXAMPLE_BIN=basic2d
OBJF = basic2d.o
LIBS=clanApp clanDisplay clanCore clanGL

include ../Makefile.conf

basic2d_sdl :
	$(CXX) -DUSE_SDL $(CXXFLAGS) basic2d.cpp -o $@ `pkg-config --libs $(patsubst %,%-1.0,clanApp clanDisplay clanCore clanSDL)`

# EOF #
