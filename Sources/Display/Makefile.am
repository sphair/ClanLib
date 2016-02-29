lib_LTLIBRARIES = libclan40Display.la

if WIN32
else
libclan40Display_la_LIBADD =
endif
libclan40Display_la_SOURCES = \
Render/storage_buffer.cpp \
Render/graphic_context.cpp \
Render/shared_gc_data.cpp \
Render/transfer_buffer.cpp \
Render/rasterizer_state.cpp \
Render/transfer_texture.cpp \
Render/depth_stencil_state_description.cpp \
Render/render_buffer.cpp \
Render/texture_impl.cpp \
Render/blend_state.cpp \
Render/program_object.cpp \
Render/texture_1d_array.cpp \
Render/texture_2d_array.cpp \
Render/texture_1d.cpp \
Render/shader_object.cpp \
Render/rasterizer_state_description.cpp \
Render/vertex_array_buffer.cpp \
Render/graphic_context_state.cpp \
Render/graphic_screen.cpp \
Render/frame_buffer.cpp \
Render/texture_2d.cpp \
Render/graphic_context_impl.cpp \
Render/texture_cube_array.cpp \
Render/primitives_array.cpp \
Render/texture_cube.cpp \
Render/uniform_buffer.cpp \
Render/depth_stencil_state.cpp \
Render/element_array_buffer.cpp \
Render/texture.cpp \
Render/blend_state_description.cpp \
Render/texture_3d.cpp \
Render/occlusion_query.cpp \
Render/shared_gc_data_impl.cpp \
screen_info.cpp \
display_target.cpp \
System/run_loop.cpp \
System/timer.cpp \
System/detect_hang.cpp \
2D/render_batch_line.cpp \
2D/render_batch_line_texture.cpp \
2D/sprite.cpp \
2D/render_batch_triangle.cpp \
2D/render_batch_path.cpp \
2D/texture_group.cpp \
2D/sprite_impl.cpp \
2D/color.cpp \
2D/image.cpp \
2D/path.cpp \
2D/canvas_batcher.cpp \
2D/canvas_impl.cpp \
2D/texture_group_impl.cpp \
2D/color_hsv.cpp \
2D/span_layout.cpp \
2D/render_batch_point.cpp \
2D/subtexture.cpp \
2D/span_layout_impl.cpp \
2D/render_batch_buffer.cpp \
2D/canvas.cpp \
2D/path_renderer.cpp \
2D/path_fill_renderer.cpp \
2D/path_stroke_renderer.cpp \
2D/color_hsl.cpp \
setup_display.cpp \
Image/icon_set.cpp \
Image/perlin_noise.cpp \
Image/image_import_description.cpp \
Image/pixel_buffer.cpp \
Image/pixel_buffer_help.cpp \
Image/pixel_buffer_set.cpp \
Image/pixel_converter.cpp \
Image/cpu_pixel_buffer_provider.cpp \
Image/pixel_buffer_impl.cpp \
Resources/file_display_cache.cpp \
Resources/display_cache.cpp \
precomp.cpp \
Font/font.cpp \
Font/font_family.cpp \
Font/glyph_cache.cpp \
Font/path_cache.cpp \
Font/font_description.cpp \
Font/font_metrics_impl.cpp \
Font/font_metrics.cpp \
Font/font_impl.cpp \
Font/font_family_impl.cpp \
Font/FontDraw/font_draw_flat.cpp \
Font/FontDraw/font_draw_path.cpp \
Font/FontDraw/font_draw_scaled.cpp \
Font/FontDraw/font_draw_subpixel.cpp \
ShaderEffect/shader_effect_description.cpp \
ShaderEffect/shader_effect.cpp \
Window/input_event.cpp \
Window/cursor.cpp \
Window/display_window.cpp \
Window/display_window_description.cpp \
Window/cursor_description.cpp \
Window/input_device.cpp \
Window/keys.cpp \
ImageProviders/dds_provider.cpp \
ImageProviders/JPEGLoader/jpeg_huffman_decoder.cpp \
ImageProviders/JPEGLoader/jpeg_mcu_decoder.cpp \
ImageProviders/JPEGLoader/jpeg_loader.cpp \
ImageProviders/JPEGLoader/jpeg_rgb_decoder.cpp \
ImageProviders/JPEGLoader/jpeg_bit_reader.cpp \
ImageProviders/JPEGLoader/jpeg_file_reader.cpp \
ImageProviders/JPEGWriter/jpge.cpp \
ImageProviders/jpeg_provider.cpp \
ImageProviders/PNGLoader/png_loader.cpp \
ImageProviders/PNGWriter/png_writer.cpp \
ImageProviders/png_output_description.cpp \
ImageProviders/png_provider.cpp \
ImageProviders/targa_provider.cpp \
ImageProviders/TargaLoader/targa_loader.cpp \
ImageProviders/provider_factory.cpp \
ImageProviders/provider_type.cpp

if WIN32
libclan40Display_la_SOURCES += \
Font/FontEngine/font_engine_win32.cpp \
Platform/Win32/win32_window.cpp \
Platform/Win32/screen_info_provider_win32.cpp \
Platform/Win32/display_message_queue_win32.cpp \
Platform/Win32/input_device_provider_win32hid.cpp \
Platform/Win32/hid.cpp \
Platform/Win32/input_device_provider_win32mouse.cpp \
Platform/Win32/cursor_provider_win32.cpp \
Platform/Win32/input_device_provider_win32keyboard.cpp \
Platform/Win32/dwm_functions.cpp
else
libclan40Display_la_SOURCES += \
Font/FontEngine/font_engine_freetype.cpp \
Platform/X11/display_message_queue_x11.cpp \
Platform/X11/input_device_provider_x11keyboard.cpp \
Platform/X11/clipboard_x11.cpp \
Platform/X11/x11_atoms.cpp \
Platform/X11/x11_window.cpp \
Platform/X11/input_device_provider_x11mouse.cpp \
Platform/X11/cursor_provider_x11.cpp

if LINUX_JOYSTICK
libclan40Display_la_SOURCES += \
Platform/X11/input_device_provider_linuxjoystick.cpp
endif
libclan40Display_la_SOURCES += \
Platform/X11/font_config.cpp
endif

libclan40Display_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanDisplay)

libclan40Display_la_CXXFLAGS=$(clanDisplay_CXXFLAGS) $(extra_CFLAGS_clanDisplay)
# EOF #

