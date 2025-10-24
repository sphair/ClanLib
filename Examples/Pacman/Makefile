EXAMPLE_BIN=pacman

OBJF=\
	sources/application.o \
	sources/fontblowup.o \
	sources/gameobject_ghost.o \
	sources/gameobject_moving.o \
	sources/gameobject_pacman.o \
	sources/map.o \
	sources/world.o

LIBS=clanCore clanDisplay clanSound clanApp clanGL clanMikMod

include ../Makefile.conf

%.dat : %.scr
	@echo "Building datafile $<"
	@./pacman -datafile $< $@

# EOF #