lib_LTLIBRARIES = libclan23SWRender.la

libclan23SWRender_la_SOURCES = precomp.cpp

if DISABLE_SSE2
libclan23SWRender_la_SOURCES += \
X11/Stub/setup_swrender.cpp \
X11/Stub/swr_program_object.cpp \
X11/Stub/swr_graphic_context.cpp \
X11/Stub/swr_target.cpp
else
libclan23SWRender_la_SOURCES += \
swr_pixel_buffer_provider.h \
swr_element_array_buffer_provider.h \
swr_input_device_provider.h \
swr_shader_object_provider.h \
swr_vertex_array_buffer_provider.h \
swr_target_provider.h \
precomp.h \
swr_render_buffer_provider.h \
swr_program_object_provider.h \
swr_display_window_provider.h \
software_program_standard.h \
Canvas/pixel_canvas.h \
Canvas/Renderers/pixel_fill_renderer.h \
Canvas/Renderers/pixel_bicubic_renderer.h \
Canvas/Renderers/pixel_triangle_renderer.h \
Canvas/Renderers/pixel_line_renderer.h \
Canvas/Pipeline/pixel_pipeline.h \
Canvas/Commands/pixel_command_set_cliprect.h \
Canvas/Commands/pixel_command_set_sampler.h \
Canvas/Commands/pixel_command_clear.h \
Canvas/Commands/pixel_command_bicubic.h \
Canvas/Commands/pixel_command_triangle.h \
Canvas/Commands/pixel_command_set_framebuffer.h \
Canvas/Commands/pixel_command_line.h \
Canvas/Commands/pixel_command_set_blendfunc.h \
Canvas/Commands/pixel_command_sprite.h \
Canvas/Commands/pixel_command_pixels.h \
swr_texture_provider.h \
vertex_attribute_fetcher.h \
swr_frame_buffer_provider.h \
swr_occlusion_query_provider.h \
swr_graphic_context_provider.h \
software_program_standard.cpp \
swr_vertex_array_buffer_provider.cpp \
swr_shader_object_provider.cpp \
swr_program_object_provider.cpp \
setup_swrender.cpp \
swr_display_window_provider.cpp \
swr_graphic_context_provider.cpp \
swr_target_provider.cpp \
swr_render_buffer_provider.cpp \
swr_texture_provider.cpp \
swr_program_object.cpp \
swr_occlusion_query_provider.cpp \
swr_graphic_context.cpp \
swr_frame_buffer_provider.cpp \
swr_element_array_buffer_provider.cpp \
swr_input_device_provider.cpp \
swr_pixel_buffer_provider.cpp \
Canvas/Renderers/pixel_fill_renderer.cpp \
Canvas/Renderers/pixel_bicubic_renderer.cpp \
Canvas/Renderers/pixel_line_renderer.cpp \
Canvas/Renderers/pixel_triangle_renderer.cpp \
Canvas/Pipeline/pixel_thread_context.cpp \
Canvas/Pipeline/pixel_command.cpp \
Canvas/Pipeline/pixel_pipeline.cpp \
Canvas/pixel_canvas.cpp \
Canvas/Commands/pixel_command_set_blendfunc.cpp \
Canvas/Commands/pixel_command_clear.cpp \
Canvas/Commands/pixel_command_set_cliprect.cpp \
Canvas/Commands/pixel_command_bicubic.cpp \
Canvas/Commands/pixel_command_set_sampler.cpp \
Canvas/Commands/pixel_command_set_framebuffer.cpp \
Canvas/Commands/pixel_command_sprite.cpp \
Canvas/Commands/pixel_command_line.cpp \
Canvas/Commands/pixel_command_pixels.cpp \
Canvas/Commands/pixel_command_triangle.cpp \
swr_target.cpp
endif

libclan23SWRender_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_SWRender)

libclan23SWRender_la_CXXFLAGS=$(SWRender_CXXFLAGS) $(extra_CFLAGS_SWRender)

# EOF #

