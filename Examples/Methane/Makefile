EXAMPLE_BIN=methane
OBJF = sources/game.o sources/baddie.o sources/methane.o sources/target.o sources/maps.o sources/gfxoff.o sources/mapdata.o sources/objlist.o sources/doc.o sources/bitdraw.o sources/global.o sources/suck.o sources/power.o sources/goodie.o sources/bititem.o sources/player.o sources/weapon.o sources/bitgroup.o sources/boss.o sources/sound.o sources/gasobj.o sources/misc.o
LIBS=clanApp clanDisplay clanCore clanGL
#LIBS += clanSound clanMikMod
include ../Makefile.conf

#CXXFLAGS += -DENABLE_SOUND

gdi :
	$(CXX) -DUSE_GDI $(CXXFLAGS) sources/game.cpp sources/baddie.cpp sources/methane.cpp sources/target.cpp sources/maps.cpp sources/gfxoff.cpp sources/mapdata.cpp sources/objlist.cpp sources/doc.cpp sources/bitdraw.cpp sources/global.cpp sources/suck.cpp sources/power.cpp sources/goodie.cpp sources/bititem.cpp sources/player.cpp sources/weapon.cpp sources/bitgroup.cpp sources/boss.cpp sources/sound.cpp sources/gasobj.cpp sources/misc.cpp -o methane `pkg-config --libs $(patsubst %,%-2.0, clanApp clanDisplay clanCore clanGDI)`

sdl :
	$(CXX) -DUSE_SDL $(CXXFLAGS) sources/game.cpp sources/baddie.cpp sources/methane.cpp sources/target.cpp sources/maps.cpp sources/gfxoff.cpp sources/mapdata.cpp sources/objlist.cpp sources/doc.cpp sources/bitdraw.cpp sources/global.cpp sources/suck.cpp sources/power.cpp sources/goodie.cpp sources/bititem.cpp sources/player.cpp sources/weapon.cpp sources/bitgroup.cpp sources/boss.cpp sources/sound.cpp sources/gasobj.cpp sources/misc.cpp -o methane `pkg-config --libs $(patsubst %,%-2.0, clanApp clanDisplay clanCore clanSDL)`

# EOF #

