EXAMPLE_BIN=basic2d
OBJF = basic2d.o
LIBS=clanApp clanDisplay clanCore clanGL

include ../Makefile.conf

gdi :
	$(CXX) -DUSE_GDI $(CXXFLAGS) basic2d.cpp -o basic2d `pkg-config --libs $(patsubst %,%-2.0, clanApp clanDisplay clanCore clanGDI)`

sdl :
	$(CXX) -DUSE_SDL $(CXXFLAGS) basic2d.cpp -o basic2d `pkg-config --libs $(patsubst %,%-2.0, clanApp clanDisplay clanCore clanSDL)`

# EOF #

