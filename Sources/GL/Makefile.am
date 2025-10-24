lib_LTLIBRARIES = libclanGL.la

libclanGL_la_SOURCES = \
graphic_context_opengl.cpp \
graphic_context_opengl.h \
gl_state_checker.cpp \
pixel_buffer_opengl_texture.cpp \
pixel_buffer_opengl_texture.h \
opengl.cpp \
opengl_target.cpp \
opengl_target.h \
opengl_window.cpp \
opengl_window_description.cpp \
opengl_window_description_generic.h \
setupgl.cpp \
opengl_surface.cpp \
surface_target_opengl.cpp \
surface_target_opengl.h \
canvas_opengl.cpp \
canvas_opengl.h \
opengl_state.cpp \
opengl_state_generic.cpp \
opengl_state_generic.h \
opengl_state_data.cpp \
viewpoint.cpp \
light_source.cpp \
shader_object.cpp \
program_object.cpp \
program_attribute.cpp \
program_uniform.cpp \
texture.cpp

if WIN32
libclanGL_la_SOURCES += \
WGL/display_window_opengl.h \
WGL/display_window_opengl.cpp
else
libclanGL_la_SOURCES += \
GLX/display_window_opengl.h \
GLX/display_window_opengl.cpp
if LINUX_JOYSTICK
libclanGL_la_SOURCES += \
GLX/input_device_linuxjoystick.cpp \
GLX/input_device_linuxjoystick.h
endif
if LINUX_INPUT
libclanGL_la_SOURCES += \
GLX/input_device_linuxevent.cpp \
GLX/input_device_linuxevent.h
endif
libclanGL_la_SOURCES += \
GLX/input_device_linuxusbmouse.cpp \
GLX/input_device_linuxusbmouse.h \
GLX/display_window_opengl.h \
GLX/input_device_x11mouse.cpp \
GLX/input_device_x11mouse.h \
GLX/input_device_x11keyboard.cpp \
GLX/input_device_x11keyboard.h \
GLX/input_device_xinput.cpp \
GLX/input_device_xinput.h
libclanGL_la_LIBADD = -lXxf86vm -lXi -lGLU -lGL 
endif

EXTRA_DIST = \
 WGL/displaycard_opengl_win32.cpp \
 WGL/displaycard_opengl_win32.h   \
 WGL/setupgl_win32.cpp

libclanGL_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
