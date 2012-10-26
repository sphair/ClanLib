lib_LTLIBRARIES = libclan30GL.la

libclan30GL_la_SOURCES = \
	opengl_window_description_impl.h \
	opengl_graphic_context_provider.h \
	opengl_target_provider.h \
	opengl_occlusion_query_provider.h \
	precomp.h \
	opengl_texture_provider.h \
	opengl_shader_object_provider.h \
	opengl_program_object_provider.h \
	opengl_target_provider.cpp \
	precomp.cpp \
	opengl_graphic_context_provider.cpp \
	opengl_occlusion_query_provider.cpp \
	opengl_texture_provider.cpp \
	setup_gl.cpp \
	opengl_window_description.cpp \
	opengl.cpp \
	opengl_target.cpp \
	opengl_standard_programs.cpp \
	opengl_graphic_context.cpp \
	opengl_shader_object_provider.cpp \
	opengl_program_object_provider.cpp \
	opengl_program_uniform_block_provider.cpp \
	opengl_frame_buffer_provider.cpp \
	opengl_render_buffer_provider.cpp \
	opengl_frame_buffer_provider.h \
	opengl_element_array_buffer_provider.cpp \
	opengl_element_array_buffer_provider.h \
	opengl_pixel_buffer_provider.cpp \
	opengl_pixel_buffer_provider.h \
	opengl_buffer_object_provider.h \
	opengl_texture_buffer_provider.h \
	opengl_transform_feedback_buffer_provider.h \
	opengl_uniform_buffer_provider.h \
	opengl_buffer_object_provider.cpp \
	opengl_vertex_array_buffer_provider.cpp \
	opengl_vertex_array_buffer_provider.h \
	opengl_render_buffer_provider.h
if WIN32
libclan30GL_la_SOURCES += \
	WGL/opengl_creation_helper.cpp \
	WGL/opengl_creation_helper.h \
	WGL/opengl_window_provider_wgl.cpp \
	WGL/opengl_window_provider_wgl.h
else
libclan30GL_la_SOURCES += \
	GLX/opengl_window_provider_glx.cpp \
	GLX/opengl_window_provider_glx.h
endif
libclan30GL_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanGL)

libclan30GL_la_CXXFLAGS=$(clanGL_CXXFLAGS) $(extra_CFLAGS_clanGL)

# EOF #
