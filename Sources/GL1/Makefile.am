lib_LTLIBRARIES = libclan30GL1.la

libclan30GL1_la_SOURCES = \
	setup_gl1.cpp \
	gl1_frame_buffer_provider.cpp \
	gl1_transfer_buffer_provider.cpp \
	gl1_uniform_buffer_provider.cpp \
	texture_unit.cpp \
	material.cpp \
	pbuffer.cpp \
	gl1_target.cpp \
	gl1_program_object_provider.cpp \
	light_source.cpp \
	gl1_vertex_array_buffer_provider.cpp \
	gl1_texture_provider.cpp \
	gl1_primitives_array_provider.cpp \
	gl1_target_provider.cpp \
	graphic_context_gl1.cpp \
	opengl1.cpp \
	precomp.cpp \
	gl1_window_description.cpp \
	gl1_graphic_context_provider.cpp \
	light_model.cpp \
	gl1_render_buffer_provider.cpp 

if WIN32
libclan30GL1_la_SOURCES += \
	WGL/gl1_creation_helper.cpp \
	WGL/pbuffer_impl.cpp \
	WGL/gl1_window_provider_wgl.cpp
else
libclan30GL1_la_SOURCES += \
	GLX/gl1_window_provider_glx.cpp \
	GLX/pbuffer_impl.cpp
endif
libclan30GL1_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanGL1)

libclan30GL1_la_CXXFLAGS=$(clanGL1_CXXFLAGS) $(extra_CFLAGS_clanGL1)

# EOF #
