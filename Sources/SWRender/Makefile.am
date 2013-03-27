lib_LTLIBRARIES = libclan30SWRender.la

libclan30SWRender_la_SOURCES = precomp.cpp
if DISABLE_SSE2
libclan30SWRender_la_SOURCES += \
X11/Stub/setup_swrender.cpp \
X11/Stub/swr_program_object.cpp \
X11/Stub/swr_graphic_context.cpp \
X11/Stub/swr_target.cpp
else
libclan30SWRender_la_SOURCES += \
swr_input_device_provider.cpp \
swr_pixel_buffer_provider.cpp \
swr_transfer_buffer_provider.cpp \
swr_vertex_array_buffer_provider.cpp \
swr_primitives_array_provider.cpp \
swr_uniform_buffer_provider.cpp \
swr_graphic_context_provider.cpp \
Canvas/pixel_canvas.cpp \
Canvas/Renderers/pixel_triangle_renderer.cpp \
Canvas/Renderers/pixel_bicubic_renderer.cpp \
Canvas/Renderers/pixel_fill_renderer.cpp \
Canvas/Renderers/pixel_line_renderer.cpp \
Canvas/Pipeline/pixel_pipeline.cpp \
Canvas/Pipeline/pixel_thread_context.cpp \
Canvas/Pipeline/pixel_command.cpp \
Canvas/Commands/pixel_command_set_framebuffer.cpp \
Canvas/Commands/pixel_command_bicubic.cpp \
Canvas/Commands/pixel_command_sprite.cpp \
Canvas/Commands/pixel_command_pixels.cpp \
Canvas/Commands/pixel_command_set_cliprect.cpp \
Canvas/Commands/pixel_command_line.cpp \
Canvas/Commands/pixel_command_triangle.cpp \
Canvas/Commands/pixel_command_set_sampler.cpp \
Canvas/Commands/pixel_command_set_blendfunc.cpp \
Canvas/Commands/pixel_command_clear.cpp \
swr_target_provider.cpp \
swr_element_array_buffer_provider.cpp \
swr_program_object.cpp \
swr_shader_object_provider.cpp \
swr_target.cpp \
swr_graphic_context.cpp \
setup_swrender.cpp \
swr_occlusion_query_provider.cpp \
swr_render_buffer_provider.cpp \
software_program_standard.cpp \
swr_frame_buffer_provider.cpp \
swr_display_window_provider.cpp \
swr_program_object_provider.cpp \
swr_texture_provider.cpp

endif

libclan30SWRender_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_SWRender)

libclan30SWRender_la_CXXFLAGS=$(SWRender_CXXFLAGS) $(extra_CFLAGS_SWRender)

# EOF #

