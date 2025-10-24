lib_LTLIBRARIES = libclan22Display.la

if WIN32
else
libclan22Display_la_LIBADD = -lpng -ljpeg
endif

libclan22Display_la_SOURCES = \
	2D/color.cpp \
	2D/color_hsv.cpp \
	2D/color_hsl.cpp \
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
	2D/render_batch2d.cpp \
	2D/render_batch2d.h \
	2D/render_batch3d.cpp \
	2D/render_batch3d.h \
	2D/render_batch_sprite.h \
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
	Image/icon_set.cpp \
	Image/pixel_buffer.cpp \
	Image/pixel_buffer_help.cpp \
	Image/pixel_buffer_impl.cpp \
	Image/pixel_buffer_impl.h \
	Image/pixel_format.cpp \
	Image/image_import_description.cpp \
	Image/image_import_description_impl.h \
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
	Font/font_description.cpp \
	Font/font_description_impl.h \
	Font/font_freetype.cpp \
	Font/font_metrics.cpp \
	Font/font_metrics_impl.cpp \
	Font/font_metrics_impl.h \
	Font/font_provider_freetype.cpp \
	Font/font_provider_freetype.h \
	Font/font_provider_sprite.cpp \
	Font/font_provider_sprite.h \
	Font/font_provider_system.cpp \
	Font/font_provider_system.h \
	Font/font_provider_vector.cpp \
	Font/font_provider_vector.h \
	Font/font_sprite.cpp \
	Font/font_system.cpp \
	Font/font_vector.cpp \
	Font/glyph_contour.cpp \
	Font/glyph_contour.h \
	Font/glyph_outline.cpp \
	Font/glyph_outline.h \
	Font/glyph_cache.cpp \
	Font/glyph_cache.h \
	Font/FontEngine/font_engine.h \
	Font/FontEngine/font_engine_freetype.cpp \
	Font/FontEngine/font_engine_freetype.h \
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
	Render/shared_gc_data_impl.cpp \
	Render/shared_gc_data_impl.h \
	Render/vertex_array_buffer.cpp \
	Render/primitives_array_builder.cpp \
	Render/shared_gc_data.cpp \
	Render/shared_gc_data.h \
	Window/display_window.cpp \
	Window/display_window_description.cpp \
	Window/display_window_description_impl.h \
	Window/display_window_impl.h \
	Window/input_context.cpp \
	Window/input_context_impl.cpp \
	Window/input_context_impl.h \
	Window/input_device.cpp \
	Window/input_device_impl.h \
	Window/input_event.cpp \
	Window/input_state.cpp \
	Window/cursor.cpp \
	display.cpp \
	display_target.cpp \
	screen_info.cpp \
	precomp.cpp \
	precomp.h \
	setup_display.cpp \
	X11/x11_window.cpp \
	X11/x11_window.h \
	X11/clipboard_x11.cpp \
	X11/clipboard_x11.h \
	X11/display_message_queue_x11.cpp \
	X11/display_message_queue_x11.h \
	X11/cursor_provider_x11.cpp \
	X11/cursor_provider_x11.h \
	X11/input_device_provider_x11mouse.cpp \
	X11/input_device_provider_x11mouse.h \
	X11/input_device_provider_x11keyboard.cpp \
	X11/input_device_provider_x11keyboard.h
if LINUX_JOYSTICK
libclan22Display_la_SOURCES += \
	X11/input_device_provider_linuxjoystick.cpp \
	X11/input_device_provider_linuxjoystick.h
endif
libclan22Display_la_SOURCES += \
	X11/font_config.cpp \
	X11/font_config.h


libclan22Display_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanDisplay)

libclan22Display_la_CXXFLAGS=$(clanDisplay_CXXFLAGS) $(extra_CFLAGS_clanDisplay)
# EOF #

