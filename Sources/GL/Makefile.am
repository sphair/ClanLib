lib_LTLIBRARIES = libclan30GL.la

libclan30GL_la_SOURCES = \
	opengl.cpp \
	opengl_graphic_context.cpp \
	opengl_target.cpp \
	opengl_target_provider.cpp \
	opengl_window_description.cpp \
	precomp.cpp \
	setup_gl.cpp \
	GL3/gl3_buffer_object_provider.cpp \
	GL3/gl3_element_array_buffer_provider.cpp \
	GL3/gl3_frame_buffer_provider.cpp \
	GL3/gl3_graphic_context_provider.cpp \
	GL3/gl3_occlusion_query_provider.cpp \
	GL3/gl3_pixel_buffer_provider.cpp \
	GL3/gl3_primitives_array_provider.cpp \
	GL3/gl3_program_object_provider.cpp \
	GL3/gl3_render_buffer_provider.cpp \
	GL3/gl3_shader_object_provider.cpp \
	GL3/gl3_standard_programs.cpp \
	GL3/gl3_storage_buffer_provider.cpp \
	GL3/gl3_texture_provider.cpp \
	GL3/gl3_transfer_buffer_provider.cpp \
	GL3/gl3_uniform_buffer_provider.cpp \
	GL3/gl3_vertex_array_buffer_provider.cpp \
	GL1/gl1_graphic_context_provider.cpp \
	GL1/gl1_primitives_array_provider.cpp \
	GL1/gl1_program_object_provider.cpp \
	GL1/gl1_render_buffer_provider.cpp \
	GL1/gl1_texture_provider.cpp \
	GL1/gl1_transfer_buffer_provider.cpp \
	GL1/gl1_uniform_buffer_provider.cpp \
	GL1/gl1_vertex_array_buffer_provider.cpp \
	GL1/opengl1.cpp

if WIN32
libclan30GL_la_SOURCES += \
	GL1/WGL/gl1_creation_helper.cpp \
	GL1/WGL/gl1_window_provider_wgl.cpp \
	GL3/WGL/gl3_creation_helper.cpp \
	GL3/WGL/gl3_window_provider_wgl.cpp
else
libclan30GL_la_SOURCES += \
	GL3/GLX/gl3_window_provider_glx.cpp \
	GL1/GLX/gl1_window_provider_glx.cpp
endif
libclan30GL_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanGL)

libclan30GL_la_CXXFLAGS=$(clanGL_CXXFLAGS) $(extra_CFLAGS_clanGL)

# EOF #
