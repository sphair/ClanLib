lib_LTLIBRARIES = libclan30LegacyGL.la

libclan30LegacyGL_la_SOURCES = \
	setup_gl1.cpp \
	gl1_transfer_buffer_provider.cpp \
	gl1_uniform_buffer_provider.cpp \
	gl1_target.cpp \
	gl1_program_object_provider.cpp \
	gl1_vertex_array_buffer_provider.cpp \
	gl1_texture_provider.cpp \
	gl1_primitives_array_provider.cpp \
	gl1_target_provider.cpp \
	graphic_context_gl1.cpp \
	opengl1.cpp \
	precomp.cpp \
	gl1_graphic_context_provider.cpp \
	gl1_render_buffer_provider.cpp 

if WIN32
libclan30LegacyGL_la_SOURCES += \
	WGL/gl1_creation_helper.cpp \
	WGL/gl1_window_provider_wgl.cpp
else
libclan30LegacyGL_la_SOURCES += \
	GLX/gl1_window_provider_glx.cpp
endif
libclan30LegacyGL_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanLegacyGL)

libclan30LegacyGL_la_CXXFLAGS=$(clanLegacyGL_CXXFLAGS) $(extra_CFLAGS_clanLegacyGL)

# EOF #
