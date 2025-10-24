all: provider_test_gl provider_test_sdl

provider_test_gl : provider_test.cpp provider_test.h
	$(CXX) -Wall -O2 $< -o $@ `pkg-config --cflags --libs clanApp-0.8 clanCore-0.8 clanDisplay-0.8 clanGL-0.8`

provider_test_sdl : provider_test.cpp provider_test.h
	$(CXX)  $< -o $@ -DUSE_SDL `pkg-config --cflags --libs clanApp-0.8 clanCore-0.8 clanDisplay-0.8 clanSDL-0.8`

# EOF #
