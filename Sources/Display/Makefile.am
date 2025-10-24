lib_LTLIBRARIES = libclanDisplay.la

# FIXME: This needs to be @LIBS@ or @X11LIBS@ or something like that
if WIN32
else
libclanDisplay_la_LIBADD = -L/usr/X11R6/lib/ -lX11 -lGL -lGLU -lpng -ljpeg -lXxf86vm @X_LIBS@
endif

libclanDisplay_la_SOURCES = \
display_target.cpp \
display_precomp.h \
surface.cpp \
canvas.cpp \
canvas_generic.h \
color.cpp \
display_iostream.cpp \
display_mode.cpp \
display_window.cpp \
sprite_packer.cpp \
surface_generic.cpp \
setupdisplay.cpp \
input_device_generic.h \
sprite.cpp \
graphic_context.cpp \
graphic_context_generic.cpp \
pixel_buffer_memory.h \
display.cpp \
display_window_description.cpp \
font.cpp \
font_target_sprite.cpp \
keyboard.cpp \
joystick.cpp \
mouse.cpp \
sprite_description.cpp \
input_buffer.cpp \
input_buffer_generic.h \
input_button.cpp \
input_button_generic.cpp \
input_device_generic.cpp \
input_context_generic.cpp \
input_event.cpp \
surface_target.cpp \
display_precomp.cpp \
input_device.cpp \
key_binding.cpp \
resourcedata_collisionoutline.cpp \
resourcedata_font.cpp \
resourcedata_text_styler.cpp \
Providers/jpeg_provider.cpp \
Providers/jpeg_provider_generic.cpp \
Providers/jpeg_provider_generic.h \
Providers/pcx_provider.cpp \
Providers/pcx_provider_generic.cpp \
Providers/pcx_provider_generic.h \
Providers/bmp_provider.cpp \
Providers/bmp_provider_generic.cpp \
Providers/bmp_provider_generic.h \
Providers/png_provider.cpp \
Providers/png_provider_generic.h \
Providers/png_provider_generic.cpp \
Providers/provider_factory.cpp \
Providers/provider_type.cpp \
Providers/targa_provider.cpp \
Providers/targa_provider_generic.cpp \
Providers/targa_provider_generic.h \
input_context_generic.h \
sprite_packer_generic.cpp \
sprite_generic.cpp \
pixel_format.cpp \
pixel_buffer.cpp \
pixel_buffer_generic.cpp \
pixel_buffer_generic.h \
pixel_buffer_memory.cpp \
display_window_generic.h \
sprite_generic.h \
font_generic.h \
font_target_sprite.h \
surface_generic.h \
sprite_packer_generic.h \
font_generic.cpp \
sprite_description_generic.h \
surface_opengl.h \
resourcedata_surface.cpp \
resourcedata_sprite.cpp \
resourcedata_sprite_description.cpp \
surface_target.h \
input_context.cpp \
display_window_generic.cpp \
glyph_buffer.cpp \
text_styler.cpp \
frameratecounter.cpp \
frameratecounter_generic.h \
Collision/collision_outline.cpp \
Collision/collision_outline_generic.cpp \
Collision/collision_outline_generic.h \
Collision/outline_math.cpp \
Collision/outline_provider_bitmap.cpp \
Collision/outline_provider_bitmap_generic.cpp \
Collision/outline_provider_bitmap_generic.h \
Collision/outline_provider_file.cpp \
Collision/outline_provider_file_generic.cpp \
Collision/outline_provider_file_generic.h


if WIN32
libclanDisplay_la_SOURCES += \
Win32/dc_buffer.cpp \
Win32/dc_buffer_generic.cpp \
Win32/dc_buffer_generic.h \
Win32/display_window_win32.cpp \
Win32/display_window_win32.h \
Win32/input_device_directinput.cpp \
Win32/input_device_directinput.h \
Win32/input_device_win32keyboard.cpp \
Win32/input_device_win32keyboard.h \
Win32/input_device_win32mouse.cpp \
Win32/input_device_win32mouse.h
endif

libclanDisplay_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
