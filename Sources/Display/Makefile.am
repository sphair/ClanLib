lib_LTLIBRARIES = libclan30Display.la

if WIN32
else
libclan30Display_la_LIBADD =
endif
libclan30Display_la_SOURCES = \
	display.cpp \
	Window/input_context_impl.cpp \
	Window/input_context.cpp \
	Window/display_window.cpp \
	Window/cursor.cpp \
	Window/cursor_description.cpp \
	Window/input_event.cpp \
	Window/input_device.cpp \
	Window/display_window_description.cpp \
	Render/depth_stencil_state_description.cpp \
	Render/frame_buffer.cpp \
	Render/texture_3d.cpp \
	Render/program_object.cpp \
	Render/shared_gc_data.cpp \
	Render/graphic_screen.cpp \
	Render/texture.cpp \
	Render/blend_state_description.cpp \
	Render/shader_object.cpp \
	Render/program_attribute.cpp \
	Render/graphic_context_state.cpp \
	Render/texture_cube_array.cpp \
	Render/graphic_context_impl.cpp \
	Render/vertex_array_buffer.cpp \
	Render/depth_stencil_state.cpp \
	Render/texture_1d_array.cpp \
	Render/primitives_array.cpp \
	Render/rasterizer_state.cpp \
	Render/program_uniform.cpp \
	Render/texture_2d.cpp \
	Render/rasterizer_state_description.cpp \
	Render/graphic_context.cpp \
	Render/texture_impl.cpp \
	Render/transfer_buffer.cpp \
	Render/storage_buffer.cpp \
	Render/transfer_texture.cpp \
	Render/shared_gc_data_impl.cpp \
	Render/blend_state.cpp \
	Render/element_array_buffer.cpp \
	Render/render_buffer.cpp \
	Render/occlusion_query.cpp \
	Render/texture_cube.cpp \
	Render/texture_1d.cpp \
	Render/uniform_buffer.cpp \
	Render/texture_2d_array.cpp \
	display_target.cpp \
	2D/rounded_rect_impl.cpp \
	2D/texture_group.cpp \
	2D/color_hsv.cpp \
	2D/sprite_description.cpp \
	2D/image_description.cpp \
	2D/render_batch_triangle.cpp \
	2D/render_batch_buffer.cpp \
	2D/canvas.cpp \
	2D/render_batch_point.cpp \
	2D/texture_group_impl.cpp \
	2D/span_layout_impl.cpp \
	2D/color_hsl.cpp \
	2D/render_batch_line.cpp \
	2D/render_batch_line_texture.cpp \
	2D/subtexture.cpp \
	2D/sprite_impl.cpp \
	2D/sprite.cpp \
	2D/canvas_impl.cpp \
	2D/image.cpp \
	2D/color.cpp \
	2D/rounded_rect.cpp \
	2D/span_layout.cpp \
	setup_display.cpp \
	Collision/outline_provider_bitmap.cpp \
	Collision/collision_outline.cpp \
	Collision/contour.cpp \
	Collision/outline_provider_bitmap_generic.cpp \
	Collision/outline_provider_file.cpp \
	Collision/outline_provider_file_generic.cpp \
	Collision/outline_math.cpp \
	Collision/collision_outline_generic.cpp \
	Collision/resourcedata_collisionoutline.cpp \
	precomp.cpp \
	Font/font_vector.cpp \
	Font/font_manager.cpp \
	2D/path.cpp \
	2D/path_impl.cpp \
	Font/font_provider_vector.cpp \
	Font/font_provider_sprite.cpp \
	Font/font.cpp \
	Font/font_system.cpp \
	Font/font_metrics.cpp \
	Font/font_manager_impl.cpp \
	Font/font_sprite.cpp \
	2D/path_group.cpp \
	2D/path_group_impl.cpp \
	Font/font_description.cpp \
	Font/font_metrics_impl.cpp \
	Font/font_provider_system.cpp \
	Font/glyph_cache.cpp \
	screen_info.cpp \
	ImageProviders/png_output_description.cpp \
	ImageProviders/provider_factory.cpp \
	ImageProviders/targa_provider.cpp \
	ImageProviders/TargaLoader/targa_loader.cpp \
	ImageProviders/PNGLoader/png_loader.cpp \
	ImageProviders/dds_provider.cpp \
	ImageProviders/jpeg_provider.cpp \
	ImageProviders/JPEGLoader/jpeg_huffman_decoder.cpp \
	ImageProviders/JPEGLoader/jpeg_rgb_decoder.cpp \
	ImageProviders/JPEGLoader/jpeg_loader.cpp \
	ImageProviders/JPEGLoader/jpeg_file_reader.cpp \
	ImageProviders/JPEGLoader/jpeg_bit_reader.cpp \
	ImageProviders/JPEGLoader/jpeg_mcu_decoder.cpp \
	ImageProviders/png_provider.cpp \
	ImageProviders/provider_type.cpp \
	ImageProviders/JPEGWriter/jpge.cpp \
	Image/perlin_noise.cpp \
	Image/cpu_pixel_buffer_provider.cpp \
	Image/pixel_buffer_set.cpp \
	Image/pixel_buffer_impl.cpp \
	Image/pixel_buffer_help.cpp \
	Image/image_import_description.cpp \
	Image/icon_set.cpp \
	Image/pixel_buffer.cpp \
	Image/pixel_converter.cpp
if WIN32
libclan30Display_la_SOURCES += \
	Font/FontEngine/font_engine_win32.cpp \
	Win32/win32_window.cpp \
	Win32/input_device_provider_win32mouse.cpp \
	Win32/cursor_provider_win32.cpp \
	Win32/hid.cpp \
	Win32/display_message_queue_win32.cpp \
	Win32/screen_info_provider_win32.cpp \
	Win32/input_device_provider_win32tablet.cpp \
	Win32/input_device_provider_win32keyboard.cpp \
	Win32/dwm_functions.cpp \
	Win32/input_device_provider_win32hid.cpp
else
libclan30Display_la_SOURCES += \
	Font/X11/font_provider_freetype.cpp \
	Font/X11/font_engine_freetype.cpp \
	X11/cursor_provider_x11.cpp \
	X11/input_device_provider_x11keyboard.cpp \
	X11/x11_window.cpp \
	X11/display_message_queue_x11.cpp \
	X11/input_device_provider_x11mouse.cpp \
	X11/clipboard_x11.cpp

if LINUX_JOYSTICK
libclan30Display_la_SOURCES += \
	X11/input_device_provider_linuxjoystick.cpp
endif
libclan30Display_la_SOURCES += \
	X11/font_config.cpp
endif

libclan30Display_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanDisplay)

libclan30Display_la_CXXFLAGS=$(clanDisplay_CXXFLAGS) $(extra_CFLAGS_clanDisplay)
# EOF #

