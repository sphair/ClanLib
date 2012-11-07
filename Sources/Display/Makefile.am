lib_LTLIBRARIES = libclan30Display.la

if WIN32
else
libclan30Display_la_LIBADD = -lpng -ljpeg
endif
libclan30Display_la_SOURCES = \
	Font/font_vector.cpp \
	Font/glyph_contour.cpp \
	Font/glyph_cache.cpp \
	Font/font.cpp \
	Font/font_sprite.cpp \
	Font/glyph_outline.cpp \
	Font/font_provider_vector.cpp \
	Font/X11/font_engine_freetype.cpp \
	Font/font_description.cpp \
	Font/font_system.cpp \
	Font/X11/font_provider_freetype.cpp \
	Font/font_provider_system.cpp \
	Font/font_provider_sprite.cpp \
	Font/font_metrics.cpp \
	Font/font_metrics_impl.cpp \
	screen_info.cpp \
	display_target.cpp \
	display.cpp \
	Render/graphic_context_impl.cpp \
	Render/program_object.cpp \
	Render/render_buffer.cpp \
	Render/shared_gc_data_impl.cpp \
	Render/program_attribute.cpp \
	Render/occlusion_query.cpp \
	Render/program_uniform.cpp \
	Render/uniform_buffer.cpp \
	Render/texture.cpp \
	Render/transfer_texture.cpp \
	Render/primitives_array.cpp \
	Render/vertex_array_buffer.cpp \
	Render/graphic_context.cpp \
	Render/graphic_screen.cpp \
	Render/frame_buffer.cpp \
	Render/element_array_buffer.cpp \
	Render/shared_gc_data.cpp \
	Render/shader_object.cpp \
	2D/canvas_impl.cpp \
	2D/canvas.cpp \
	2D/texture_group_impl.cpp \
	2D/render_batch_line.cpp \
	2D/render_batch_point.cpp \
	2D/sprite_description.cpp \
	2D/sprite_impl.cpp \
	2D/color_hsv.cpp \
	2D/color_hsl.cpp \
	2D/rounded_rect_impl.cpp \
	2D/image.cpp \
	2D/span_layout_impl.cpp \
	2D/span_layout.cpp \
	2D/render_batch_triangle.cpp \
	2D/color.cpp \
	2D/sprite.cpp \
	2D/subtexture.cpp \
	2D/rounded_rect.cpp \
	2D/texture_group.cpp \
	2D/collidable_sprite.cpp \
	ImageProviders/png_provider.cpp \
	ImageProviders/targa_provider.cpp \
	ImageProviders/provider_type.cpp \
	ImageProviders/provider_factory.cpp \
	ImageProviders/JPEGLoader/jpeg_mcu_decoder.cpp \
	ImageProviders/JPEGLoader/jpeg_file_reader.cpp \
	ImageProviders/JPEGLoader/jpeg_huffman_decoder.cpp \
	ImageProviders/JPEGLoader/jpeg_bit_reader.cpp \
	ImageProviders/JPEGLoader/jpeg_loader.cpp \
	ImageProviders/JPEGLoader/jpeg_rgb_decoder.cpp \
	ImageProviders/jpeg_provider.cpp \
	precomp.cpp \
	Image/image_import_description.cpp \
	Image/pixel_buffer_impl.cpp \
	Image/icon_set.cpp \
	Image/pixel_buffer.cpp \
	Image/pixel_buffer_help.cpp \
	Image/perlin_noise.cpp \
	Image/cpu_pixel_buffer_provider.cpp \
	Image/cpu_pixel_buffer_provider.h \
	Window/input_context_impl.cpp \
	Window/display_window_description.cpp \
	Window/input_context.cpp \
	Window/input_device.cpp \
	Window/cursor.cpp \
	Window/input_event.cpp \
	Window/display_window.cpp \
	Collision/collision_outline.cpp \
	Collision/resourcedata_collisionoutline.cpp \
	Collision/outline_provider_file_generic.cpp \
	Collision/outline_provider_bitmap.cpp \
	Collision/outline_provider_file.cpp \
	Collision/collision_outline_generic.cpp \
	Collision/outline_provider_bitmap_generic.cpp \
	Collision/outline_math.cpp \
	precomp.h \
	Font/font_metrics_impl.h \
	Font/font_description_impl.h \
	Font/glyph_outline.h \
	Font/font_provider_vector.h \
	Font/glyph_cache.h \
	Font/X11/font_provider_freetype.h \
	Font/FontEngine/font_engine.h \
	Font/X11/font_engine_freetype.h \
	Font/font_provider_sprite.h \
	Font/font_provider_system.h \
	Font/glyph_contour.h \
	Render/primitives_array_impl.h \
	Render/shared_gc_data_impl.h \
	Render/graphic_context_impl.h \
	2D/sprite_description_impl.h \
	2D/render_batch_triangle.h \
	2D/texture_group_impl.h \
	2D/rounded_rect_impl.h \
	2D/span_layout_impl.h \
	2D/sprite_impl.h \
	ImageProviders/JPEGLoader/jpeg_mcu_decoder.h \
	ImageProviders/JPEGLoader/jpeg_define_huffman_table.h \
	ImageProviders/JPEGLoader/jpeg_rgb_decoder.h \
	ImageProviders/JPEGLoader/jpeg_define_quantization_table.h \
	ImageProviders/JPEGLoader/jpeg_component_dcts.h \
	ImageProviders/JPEGLoader/jpeg_file_reader.h \
	ImageProviders/JPEGLoader/jpeg_start_of_scan.h \
	ImageProviders/JPEGLoader/jpeg_huffman_decoder.h \
	ImageProviders/JPEGLoader/jpeg_loader.h \
	ImageProviders/JPEGLoader/jpeg_bit_reader.h \
	ImageProviders/JPEGLoader/jpeg_markers.h \
	ImageProviders/JPEGLoader/jpeg_start_of_frame.h \
	screen_info_provider.h \
	Image/image_import_description_impl.h \
	Image/pixel_buffer_impl.h \
	Window/display_window_impl.h \
	Window/input_device_impl.h \
	Window/input_context_impl.h \
	Window/display_window_description_impl.h \
	Collision/outline_provider_file_generic.h \
	Collision/resourcedata_collisionoutline.h \
	Collision/outline_provider_bitmap_generic.h \
	Collision/collision_outline_generic.h

if WIN32
libclan30Display_la_SOURCES += \
	Font/FontEngine/font_engine_win32.cpp \
	Font/FontEngine/font_engine_win32.h
endif
libclan30Display_la_SOURCES += \
	setup_display.cpp
if WIN32
libclan30Display_la_SOURCES += \
	Win32/display_message_queue_win32.cpp \
	Win32/cursor_provider_win32.cpp \
	Win32/input_device_provider_directinput.cpp \
	Win32/input_device_provider_win32keyboard.cpp \
	Win32/screen_info_provider_win32.cpp \
	Win32/win32_window.cpp \
	Win32/input_device_provider_win32mouse.cpp \
	Win32/input_device_provider_win32tablet.cpp \
	Win32/input_device_provider_win32mouse.h \
	Win32/screen_info_provider_win32.h \
	Win32/input_device_provider_win32tablet.h \
	Win32/input_device_provider_win32keyboard.h \
	Win32/display_message_queue_win32.h \
	Win32/cursor_provider_win32.h \
	Win32/input_device_provider_directinput.h \
	Win32/win32_window.h
else
libclan30Display_la_SOURCES += \
	X11/clipboard_x11.h \
	X11/display_message_queue_x11.h \
	X11/input_device_provider_x11mouse.h \
	X11/input_device_provider_x11keyboard.h \
	X11/x11_window.h \
	X11/cursor_provider_x11.h \
	X11/cursor_provider_x11.cpp \
	X11/clipboard_x11.cpp \
	X11/x11_window.cpp \
	X11/input_device_provider_x11mouse.cpp \
	X11/input_device_provider_x11keyboard.cpp \
	X11/display_message_queue_x11.cpp

if LINUX_JOYSTICK
libclan30Display_la_SOURCES += \
	X11/input_device_provider_linuxjoystick.cpp \
	X11/input_device_provider_linuxjoystick.h
endif
libclan30Display_la_SOURCES += \
	X11/font_config.cpp \
	X11/font_config.h
endif

libclan30Display_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanDisplay)

libclan30Display_la_CXXFLAGS=$(clanDisplay_CXXFLAGS) $(extra_CFLAGS_clanDisplay)
# EOF #

