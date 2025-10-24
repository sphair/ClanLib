lib_LTLIBRARIES = libclan21GL.la

libclan21GL_la_SOURCES = \
	opengl_window_description_impl.h \
	opengl_graphic_context_provider.h \
	opengl_target_provider.h \
	opengl_occlusion_query_provider.h \
	precomp.h \
	opengl_texture_provider.h \
	opengl_shader_object_provider.h \
	opengl_window_provider.h \
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
	opengl_graphic_context.cpp \
	opengl_shader_object_provider.cpp \
	opengl_program_object_provider.cpp \
	opengl_frame_buffer_provider.cpp \
	opengl_render_buffer_provider.cpp \
	opengl_frame_buffer_provider.h \
	opengl_element_array_buffer_provider.cpp \
	opengl_element_array_buffer_provider.h \
	opengl_vertex_array_buffer_provider.cpp \
	opengl_vertex_array_buffer_provider.h \
	opengl_render_buffer_provider.h

if WIN32
	libclan21GL_la_SOURCES += \
	WGL/opengl_creation_helper.cpp \
	WGL/opengl_window_provider_wgl.cpp \
	WGL/opengl_creation_helper.h \
	WGL/opengl_window_provider_wgl.h
else
libclan21GL_la_SOURCES += \
	GLX/opengl_window_provider_glx.cpp \
	GLX/opengl_window_provider_glx.h

#libclan21GL_la_LIBADD = -lXi
endif

# EXTRA_DIST = \
# WGL/displaycard_opengl_win32.cpp \
# WGL/displaycard_opengl_win32.h   \
# WGL/setupgl_win32.cpp

libclan21GL_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
