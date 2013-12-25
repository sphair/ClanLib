EXAMPLE_BIN=object3d
SOURCES = $(wildcard ./Sources/*.cpp)
OBJF = $(patsubst %.cpp,%.o,$(SOURCES))

LIBS=clanApp clanDisplay clanCore clanGL
EXTRA_LIBS=-lassimp

CXXFLAGS += -I./ -I Sources

include ../../Makefile.conf

# EOF #

