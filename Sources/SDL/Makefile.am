lib_LTLIBRARIES = libclanSDL.la

libclanSDL_la_SOURCES = \
display_window_sdl.cpp	\
display_window_sdl.h	\
graphic_context_sdl.cpp \
graphic_context_sdl.h	\
input_device_sdlkeyboard.cpp \
input_device_sdlkeyboard.h   \
input_device_sdlmouse.cpp    \
input_device_sdlmouse.h      \
input_device_sdljoystick.cpp \
input_device_sdljoystick.h   \
pixel_buffer_sdl_frame.cpp   \
pixel_buffer_sdl_frame.h     \
sdl_target.cpp  \
sdl_target.h    \
setupsdl.cpp    \
surface_sdl.cpp \
surface_sdl.h

libclanSDL_la_CXXFLAGS = $(SDL_CFLAGS)
libclanSDL_la_LIBADD   = $(SDL_LIBS)

libclanSDL_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
