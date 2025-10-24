EXAMPLE_BIN=basic2d
OBJF = basic2d.o
LIBS=clanApp clanDisplay clanCore clanGL

include ../Makefile.conf

basic2d_gdi :
	$(CXX) -DUSE_GDI $(CXXFLAGS) basic2d.cpp -o $@ `pkg-config --libs $(patsubst %,%-0.9,clanApp clanDisplay clanCore clanGDI)`

# EOF #

