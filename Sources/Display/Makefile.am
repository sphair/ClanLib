lib_LTLIBRARIES = libclanDisplay.la

# FIXME: This needs to be @LIBS@ or @X11LIBS@ or something like that
if WIN32
else
# libclanDisplay_la_LIBADD = -L/usr/X11R6/lib/ -lX11 -lXmu -lGL -lGLU -lpng -ljpeg -lXxf86vm @X_LIBS@
libclanDisplay_la_LIBADD = -lpng -ljpeg
endif

libclanDisplay_la_SOURCES = \
	2D/color.cpp \
	2D/draw.cpp \
	2D/image.cpp \
	2D/rounded_rect.cpp \
	2D/rounded_rect_impl.cpp \
	2D/rounded_rect_impl.h \
	2D/sprite.cpp \
	2D/sprite_description.cpp \
	2D/sprite_description_impl.h \
	2D/sprite_impl.cpp \
	2D/sprite_impl.h \
	2D/subtexture.cpp \
	2D/texture_group.cpp \
	2D/texture_group_impl.cpp \
	2D/texture_group_impl.h \
	Font/vector_font.cpp \
	Font/font_provider_vector.cpp \
	Font/font_provider_vector.h \
	2D/sprite_render_batch.cpp \
	2D/sprite_render_batch_impl.h \
	2D/span_layout.cpp \
	2D/span_layout_impl.cpp \
	2D/span_layout_impl.h \
	Collision/outline_provider_bitmap_generic.h \
	Collision/outline_provider_file_generic.h \
	Collision/collision_outline_generic.h \
	Collision/outline_provider_bitmap_generic.cpp \
	Collision/outline_provider_file_generic.cpp \
	Collision/outline_provider_bitmap.cpp \
	Collision/collision_outline_generic.cpp \
	Collision/outline_math.cpp \
	Collision/outline_provider_file.cpp \
	Collision/collision_outline.cpp \
	Collision/resourcedata_collisionoutline.cpp \
	Collision/resourcedata_collisionoutline.h \
	Image/pixel_buffer.cpp \
	Image/pixel_buffer_impl.cpp \
	Image/pixel_buffer_impl.h \
	Image/pixel_format.cpp \
	ImageProviders/targa_provider_impl.h \
	ImageProviders/png_provider_impl.h \
	ImageProviders/jpeg_provider_impl.h \
	ImageProviders/pcx_provider_impl.h \
	ImageProviders/jpeg_compressor.cpp \
	ImageProviders/png_provider.cpp \
	ImageProviders/png_provider_impl.cpp \
	ImageProviders/jpeg_provider.cpp \
	ImageProviders/jpeg_decompressor.cpp \
	ImageProviders/jpeg_provider_impl.cpp \
	ImageProviders/provider_factory.cpp \
	ImageProviders/pcx_provider.cpp \
	ImageProviders/pcx_provider_impl.cpp \
	ImageProviders/targa_provider.cpp \
	ImageProviders/targa_provider_impl.cpp \
	ImageProviders/provider_type.cpp \
	Font/font.cpp \
	Font/font_texture.cpp \
	Font/font_provider_texture.cpp \
	Font/font_provider_texture.h \
	Font/font_freetype.cpp \
	Font/font_provider_freetype.cpp \
	Font/font_provider_freetype.h \
	Font/font_description.cpp \
	Font/font_description_impl.h \
	Font/font_metrics.cpp \
	Font/font_metrics_impl.cpp \
	Font/font_metrics_impl.h \
	Font/font_cache_native.cpp \
	Font/font_cache_native.h \
	Font/font_native.cpp \
	Font/freetype_font.cpp \
	Font/freetype_font.h \
	Font/freetype_font_provider.cpp \
	Font/freetype_font_provider.h \
	Font/glyph_contour.cpp \
	Font/glyph_contour.h \
	Font/glyph_outline.cpp \
	Font/glyph_outline.h \
	Render/blend_mode.cpp \
	Render/buffer_control.cpp \
	Render/element_array_buffer.cpp \
	Render/frame_buffer.cpp \
	Render/graphic_context.cpp \
	Render/graphic_context_impl.cpp \
	Render/graphic_context_impl.h \
	Render/occlusion_query.cpp \
	Render/pen.cpp \
	Render/polygon_rasterizer.cpp \
	Render/primitives_array.cpp \
	Render/primitives_array_impl.h \
	Render/program_attribute.cpp \
	Render/program_object.cpp \
	Render/program_uniform.cpp \
	Render/render_buffer.cpp \
	Render/shader_object.cpp \
	Render/texture.cpp \
	Render/vertex_array_buffer.cpp \
	Render/primitives_array_builder.cpp \
	Render/shared_gc_data.cpp \
	Render/shared_gc_data.h \
	Window/display_message_queue.cpp \
	Window/display_window.cpp \
	Window/display_window_description.cpp \
	Window/display_window_description_impl.h \
	Window/display_window_impl.h \
	Window/display_window_message.cpp \
	Window/input_context.cpp \
	Window/input_context_impl.cpp \
	Window/input_context_impl.h \
	Window/input_device.cpp \
	Window/input_device_impl.h \
	Window/input_event.cpp \
	Window/input_state.cpp \
	Window/timer.cpp \
	Window/cursor.cpp \
	Window/timer_impl.cpp \
	Window/timer_impl.h \
	Window/display_window_mode.cpp \
	display.cpp \
	display_target.cpp \
	precomp.cpp \
	precomp.h \
	setup_display.cpp \
	X11/x11_window.cpp \
	X11/x11_window.h \
	X11/display_message_queue_x11.cpp \
	X11/display_message_queue_x11.h \
	X11/cursor_provider_x11.cpp \
	X11/cursor_provider_x11.h \
	X11/timer_provider_x11.cpp \
	X11/timer_provider_x11.h \
	X11/input_device_x11keyboard.h \
	X11/input_device_provider_x11mouse.cpp \
	X11/input_device_provider_x11mouse.h \
	X11/input_device_provider_x11keyboard.cpp \
	X11/input_device_provider_x11keyboard.h
if LINUX_JOYSTICK
libclanDisplay_la_SOURCES += \
	X11/input_device_provider_linuxjoystick.cpp \
	X11/input_device_provider_linuxjoystick.h
endif
libclanDisplay_la_SOURCES += \
	X11/font_config.cpp \
	X11/font_config.h \
	X11/input_device_x11keyboard.cpp

# if WIN32
# libclanDisplay_la_SOURCES += \
# Win32/dc_buffer.cpp \
# Win32/dc_buffer_generic.cpp \
# Win32/dc_buffer_generic.h \
# Win32/display_window_win32.cpp \
# Win32/display_window_win32.h \
# Win32/input_device_directinput.cpp \
# Win32/input_device_directinput.h \
# Win32/input_device_win32keyboard.cpp \
# Win32/input_device_win32keyboard.h \
# Win32/input_device_win32mouse.cpp \
# Win32/input_device_win32mouse.h
# endif

libclanDisplay_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
