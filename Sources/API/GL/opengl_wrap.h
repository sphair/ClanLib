/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

/// \addtogroup clanGL_System clanGL System
/// \{

#pragma once

#include "opengl.h"
#include "opengl_defines.h"

namespace clan
{

#ifdef WIN32
#define GLFUNC WINAPI
#else
#define GLFUNC
#endif

/// \name OpenGL 2.0 binds
/// \{

/// \brief GLFunctions
class GLFunctions
{

/// \name Typedefs
/// \{

public:
	typedef void (GLFUNC *ptr_glCullFace)(GLenum mode);
	typedef void (GLFUNC *ptr_glFrontFace)(GLenum mode);
	typedef void (GLFUNC *ptr_glHint)(GLenum target, GLenum mode);
	typedef void (GLFUNC *ptr_glLineWidth)(GLfloat width);
	typedef void (GLFUNC *ptr_glPointSize)(GLfloat size);
	typedef void (GLFUNC *ptr_glPolygonMode)(GLenum face, GLenum mode);
	typedef void (GLFUNC *ptr_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
	typedef void (GLFUNC *ptr_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
	typedef void (GLFUNC *ptr_glTexParameteri)(GLenum target, GLenum pname, GLint param);
	typedef void (GLFUNC *ptr_glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
	typedef void (GLFUNC *ptr_glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (GLFUNC *ptr_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (GLFUNC *ptr_glDrawBuffer)(GLenum mode);
	typedef void (GLFUNC *ptr_glClear)(GLbitfield mask);
	typedef void (GLFUNC *ptr_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	typedef void (GLFUNC *ptr_glClearStencil)(GLint s);
	typedef void (GLFUNC *ptr_glClearDepth)(GLdouble depth);
	typedef void (GLFUNC *ptr_glStencilMask)(GLuint mask);
	typedef void (GLFUNC *ptr_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	typedef void (GLFUNC *ptr_glDepthMask)(GLboolean flag);
	typedef void (GLFUNC *ptr_glDisable)(GLenum cap);
	typedef void (GLFUNC *ptr_glEnable)(GLenum cap);
	typedef void (GLFUNC *ptr_glFinish)(void);
	typedef void (GLFUNC *ptr_glFlush)(void);
	typedef void (GLFUNC *ptr_glBlendFunc)(GLenum sfactor, GLenum dfactor);
	typedef void (GLFUNC *ptr_glLogicOp)(GLenum opcode);
	typedef void (GLFUNC *ptr_glStencilFunc)(GLenum func, GLint ref, GLuint mask);
	typedef void (GLFUNC *ptr_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
	typedef void (GLFUNC *ptr_glDepthFunc)(GLenum func);
	typedef void (GLFUNC *ptr_glPixelStoref)(GLenum pname, GLfloat param);
	typedef void (GLFUNC *ptr_glPixelStorei)(GLenum pname, GLint param);
	typedef void (GLFUNC *ptr_glReadBuffer)(GLenum mode);
	typedef void (GLFUNC *ptr_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	typedef void (GLFUNC *ptr_glGetBooleanv)(GLenum pname, GLboolean *params);
	typedef void (GLFUNC *ptr_glGetDoublev)(GLenum pname, GLdouble *params);
	typedef GLenum (GLFUNC *ptr_glGetError)(void);
	typedef void (GLFUNC *ptr_glGetFloatv)(GLenum pname, GLfloat *params);
	typedef void (GLFUNC *ptr_glGetIntegerv)(GLenum pname, GLint *params);
	typedef const GLubyte * (GLFUNC *ptr_glGetString)(GLenum name);
	typedef void (GLFUNC *ptr_glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
	typedef void (GLFUNC *ptr_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
	typedef void (GLFUNC *ptr_glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
	typedef void (GLFUNC *ptr_glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
	typedef GLboolean (GLFUNC *ptr_glIsEnabled)(GLenum cap);
	typedef void (GLFUNC *ptr_glDepthRange)(GLdouble near, GLdouble far);
	typedef void (GLFUNC *ptr_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
	typedef void (GLFUNC *ptr_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	typedef void (GLFUNC *ptr_glGetPointerv)(GLenum pname, GLvoid* *params);
	typedef void (GLFUNC *ptr_glPolygonOffset)(GLfloat factor, GLfloat units);
	typedef void (GLFUNC *ptr_glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
	typedef void (GLFUNC *ptr_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	typedef void (GLFUNC *ptr_glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
	typedef void (GLFUNC *ptr_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (GLFUNC *ptr_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (GLFUNC *ptr_glBindTexture)(GLenum target, GLuint texture);
	typedef void (GLFUNC *ptr_glDeleteTextures)(GLsizei n, const GLuint *textures);
	typedef void (GLFUNC *ptr_glGenTextures)(GLsizei n, GLuint *textures);
	typedef GLboolean (GLFUNC *ptr_glIsTexture)(GLuint texture);
	typedef void (GLFUNC *ptr_glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	typedef void (GLFUNC *ptr_glBlendEquation)(GLenum mode);
	typedef void (GLFUNC *ptr_glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
	typedef void (GLFUNC *ptr_glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (GLFUNC *ptr_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (GLFUNC *ptr_glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glActiveTexture)(GLenum texture);
	typedef void (GLFUNC *ptr_glSampleCoverage)(GLfloat value, GLboolean invert);
	typedef void (GLFUNC *ptr_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (GLFUNC *ptr_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (GLFUNC *ptr_glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (GLFUNC *ptr_glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
	typedef void (GLFUNC *ptr_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
	typedef void (GLFUNC *ptr_glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
	typedef void (GLFUNC *ptr_glGetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);
	typedef void (GLFUNC *ptr_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
	typedef void (GLFUNC *ptr_glMultiDrawArrays)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
	typedef void (GLFUNC *ptr_glMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei drawcount);
	typedef void (GLFUNC *ptr_glPointParameterf)(GLenum pname, GLfloat param);
	typedef void (GLFUNC *ptr_glPointParameterfv)(GLenum pname, const GLfloat *params);
	typedef void (GLFUNC *ptr_glPointParameteri)(GLenum pname, GLint param);
	typedef void (GLFUNC *ptr_glPointParameteriv)(GLenum pname, const GLint *params);
	typedef void (GLFUNC *ptr_glGenQueries)(GLsizei n, GLuint *ids);
	typedef void (GLFUNC *ptr_glDeleteQueries)(GLsizei n, const GLuint *ids);
	typedef GLboolean (GLFUNC *ptr_glIsQuery)(GLuint id);
	typedef void (GLFUNC *ptr_glBeginQuery)(GLenum target, GLuint id);
	typedef void (GLFUNC *ptr_glEndQuery)(GLenum target);
	typedef void (GLFUNC *ptr_glGetQueryiv)(GLenum target, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
	typedef void (GLFUNC *ptr_glBindBuffer)(GLenum target, GLuint buffer);
	typedef void (GLFUNC *ptr_glDeleteBuffers)(GLsizei n, const GLuint *buffers);
	typedef void (GLFUNC *ptr_glGenBuffers)(GLsizei n, GLuint *buffers);
	typedef GLboolean (GLFUNC *ptr_glIsBuffer)(GLuint buffer);
	typedef void (GLFUNC *ptr_glBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	typedef void (GLFUNC *ptr_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
	typedef void (GLFUNC *ptr_glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
	typedef GLvoid* (GLFUNC *ptr_glMapBuffer)(GLenum target, GLenum access);
	typedef GLboolean (GLFUNC *ptr_glUnmapBuffer)(GLenum target);
	typedef void (GLFUNC *ptr_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetBufferPointerv)(GLenum target, GLenum pname, GLvoid* *params);
	typedef void (GLFUNC *ptr_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
	typedef void (GLFUNC *ptr_glDrawBuffers)(GLsizei n, const GLenum *bufs);
	typedef void (GLFUNC *ptr_glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
	typedef void (GLFUNC *ptr_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
	typedef void (GLFUNC *ptr_glStencilMaskSeparate)(GLenum face, GLuint mask);
	typedef void (GLFUNC *ptr_glAttachShader)(GLuint program, GLuint shader);
	typedef void (GLFUNC *ptr_glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
	typedef void (GLFUNC *ptr_glCompileShader)(GLuint shader);
	typedef GLuint (GLFUNC *ptr_glCreateProgram)(void);
	typedef GLuint (GLFUNC *ptr_glCreateShader)(GLenum type);
	typedef void (GLFUNC *ptr_glDeleteProgram)(GLuint program);
	typedef void (GLFUNC *ptr_glDeleteShader)(GLuint shader);
	typedef void (GLFUNC *ptr_glDetachShader)(GLuint program, GLuint shader);
	typedef void (GLFUNC *ptr_glDisableVertexAttribArray)(GLuint index);
	typedef void (GLFUNC *ptr_glEnableVertexAttribArray)(GLuint index);
	typedef void (GLFUNC *ptr_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
	typedef void (GLFUNC *ptr_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
	typedef void (GLFUNC *ptr_glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
	typedef GLint (GLFUNC *ptr_glGetAttribLocation)(GLuint program, const GLchar *name);
	typedef void (GLFUNC *ptr_glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef void (GLFUNC *ptr_glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef void (GLFUNC *ptr_glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
	typedef GLint (GLFUNC *ptr_glGetUniformLocation)(GLuint program, const GLchar *name);
	typedef void (GLFUNC *ptr_glGetUniformfv)(GLuint program, GLint location, GLfloat *params);
	typedef void (GLFUNC *ptr_glGetUniformiv)(GLuint program, GLint location, GLint *params);
	typedef void (GLFUNC *ptr_glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
	typedef void (GLFUNC *ptr_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
	typedef void (GLFUNC *ptr_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid* *pointer);
	typedef GLboolean (GLFUNC *ptr_glIsProgram)(GLuint program);
	typedef GLboolean (GLFUNC *ptr_glIsShader)(GLuint shader);
	typedef void (GLFUNC *ptr_glLinkProgram)(GLuint program);
	typedef void (GLFUNC *ptr_glShaderSource)(GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
	typedef void (GLFUNC *ptr_glUseProgram)(GLuint program);
	typedef void (GLFUNC *ptr_glUniform1f)(GLint location, GLfloat v0);
	typedef void (GLFUNC *ptr_glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
	typedef void (GLFUNC *ptr_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (GLFUNC *ptr_glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (GLFUNC *ptr_glUniform1i)(GLint location, GLint v0);
	typedef void (GLFUNC *ptr_glUniform2i)(GLint location, GLint v0, GLint v1);
	typedef void (GLFUNC *ptr_glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (GLFUNC *ptr_glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (GLFUNC *ptr_glUniform1fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniform2fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniform4fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniform1iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glUniform2iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glUniform3iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glUniform4iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glValidateProgram)(GLuint program);
	typedef void (GLFUNC *ptr_glVertexAttrib1d)(GLuint index, GLdouble x);
	typedef void (GLFUNC *ptr_glVertexAttrib1dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttrib1f)(GLuint index, GLfloat x);
	typedef void (GLFUNC *ptr_glVertexAttrib1fv)(GLuint index, const GLfloat *v);
	typedef void (GLFUNC *ptr_glVertexAttrib1s)(GLuint index, GLshort x);
	typedef void (GLFUNC *ptr_glVertexAttrib1sv)(GLuint index, const GLshort *v);
	typedef void (GLFUNC *ptr_glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
	typedef void (GLFUNC *ptr_glVertexAttrib2dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
	typedef void (GLFUNC *ptr_glVertexAttrib2fv)(GLuint index, const GLfloat *v);
	typedef void (GLFUNC *ptr_glVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
	typedef void (GLFUNC *ptr_glVertexAttrib2sv)(GLuint index, const GLshort *v);
	typedef void (GLFUNC *ptr_glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
	typedef void (GLFUNC *ptr_glVertexAttrib3dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
	typedef void (GLFUNC *ptr_glVertexAttrib3fv)(GLuint index, const GLfloat *v);
	typedef void (GLFUNC *ptr_glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
	typedef void (GLFUNC *ptr_glVertexAttrib3sv)(GLuint index, const GLshort *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4Nbv)(GLuint index, const GLbyte *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4Niv)(GLuint index, const GLint *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4Nsv)(GLuint index, const GLshort *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
	typedef void (GLFUNC *ptr_glVertexAttrib4Nubv)(GLuint index, const GLubyte *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4Nuiv)(GLuint index, const GLuint *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4Nusv)(GLuint index, const GLushort *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4bv)(GLuint index, const GLbyte *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	typedef void (GLFUNC *ptr_glVertexAttrib4dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	typedef void (GLFUNC *ptr_glVertexAttrib4fv)(GLuint index, const GLfloat *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4iv)(GLuint index, const GLint *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
	typedef void (GLFUNC *ptr_glVertexAttrib4sv)(GLuint index, const GLshort *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4ubv)(GLuint index, const GLubyte *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4uiv)(GLuint index, const GLuint *v);
	typedef void (GLFUNC *ptr_glVertexAttrib4usv)(GLuint index, const GLushort *v);
	typedef void (GLFUNC *ptr_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	typedef void (GLFUNC *ptr_glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
	typedef void (GLFUNC *ptr_glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
	typedef void (GLFUNC *ptr_glGetIntegeri_v)(GLenum target, GLuint index, GLint *data);
	typedef void (GLFUNC *ptr_glEnablei)(GLenum target, GLuint index);
	typedef void (GLFUNC *ptr_glDisablei)(GLenum target, GLuint index);
	typedef GLboolean (GLFUNC *ptr_glIsEnabledi)(GLenum target, GLuint index);
	typedef void (GLFUNC *ptr_glBeginTransformFeedback)(GLenum primitiveMode);
	typedef void (GLFUNC *ptr_glEndTransformFeedback)(void);
	typedef void (GLFUNC *ptr_glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
	typedef void (GLFUNC *ptr_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
	typedef void (GLFUNC *ptr_glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar* const *varyings, GLenum bufferMode);
	typedef void (GLFUNC *ptr_glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
	typedef void (GLFUNC *ptr_glClampColor)(GLenum target, GLenum clamp);
	typedef void (GLFUNC *ptr_glBeginConditionalRender)(GLuint id, GLenum mode);
	typedef void (GLFUNC *ptr_glEndConditionalRender)(void);
	typedef void (GLFUNC *ptr_glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	typedef void (GLFUNC *ptr_glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
	typedef void (GLFUNC *ptr_glVertexAttribI1i)(GLuint index, GLint x);
	typedef void (GLFUNC *ptr_glVertexAttribI2i)(GLuint index, GLint x, GLint y);
	typedef void (GLFUNC *ptr_glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
	typedef void (GLFUNC *ptr_glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
	typedef void (GLFUNC *ptr_glVertexAttribI1ui)(GLuint index, GLuint x);
	typedef void (GLFUNC *ptr_glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
	typedef void (GLFUNC *ptr_glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
	typedef void (GLFUNC *ptr_glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
	typedef void (GLFUNC *ptr_glVertexAttribI1iv)(GLuint index, const GLint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI2iv)(GLuint index, const GLint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI3iv)(GLuint index, const GLint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI4iv)(GLuint index, const GLint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI1uiv)(GLuint index, const GLuint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI2uiv)(GLuint index, const GLuint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI3uiv)(GLuint index, const GLuint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI4uiv)(GLuint index, const GLuint *v);
	typedef void (GLFUNC *ptr_glVertexAttribI4bv)(GLuint index, const GLbyte *v);
	typedef void (GLFUNC *ptr_glVertexAttribI4sv)(GLuint index, const GLshort *v);
	typedef void (GLFUNC *ptr_glVertexAttribI4ubv)(GLuint index, const GLubyte *v);
	typedef void (GLFUNC *ptr_glVertexAttribI4usv)(GLuint index, const GLushort *v);
	typedef void (GLFUNC *ptr_glGetUniformuiv)(GLuint program, GLint location, GLuint *params);
	typedef void (GLFUNC *ptr_glBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name);
	typedef GLint (GLFUNC *ptr_glGetFragDataLocation)(GLuint program, const GLchar *name);
	typedef void (GLFUNC *ptr_glUniform1ui)(GLint location, GLuint v0);
	typedef void (GLFUNC *ptr_glUniform2ui)(GLint location, GLuint v0, GLuint v1);
	typedef void (GLFUNC *ptr_glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
	typedef void (GLFUNC *ptr_glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
	typedef void (GLFUNC *ptr_glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glTexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
	typedef void (GLFUNC *ptr_glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
	typedef void (GLFUNC *ptr_glGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
	typedef void (GLFUNC *ptr_glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
	typedef void (GLFUNC *ptr_glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
	typedef void (GLFUNC *ptr_glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
	typedef void (GLFUNC *ptr_glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
	typedef const GLubyte * (GLFUNC *ptr_glGetStringi)(GLenum name, GLuint index);
	typedef void (GLFUNC *ptr_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
	typedef void (GLFUNC *ptr_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount);
	typedef void (GLFUNC *ptr_glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
	typedef void (GLFUNC *ptr_glPrimitiveRestartIndex)(GLuint index);
	typedef void (GLFUNC *ptr_glGetInteger64i_v)(GLenum target, GLuint index, CLint64 *data);
	typedef void (GLFUNC *ptr_glGetBufferParameteri64v)(GLenum target, GLenum pname, CLint64 *params);
	typedef void (GLFUNC *ptr_glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
	typedef void (GLFUNC *ptr_glVertexAttribDivisor)(GLuint index, GLuint divisor);
	typedef void (GLFUNC *ptr_glMinSampleShading)(GLfloat value);
	typedef void (GLFUNC *ptr_glBlendEquationi)(GLuint buf, GLenum mode);
	typedef void (GLFUNC *ptr_glBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
	typedef void (GLFUNC *ptr_glBlendFunci)(GLuint buf, GLenum src, GLenum dst);
	typedef void (GLFUNC *ptr_glBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	typedef GLboolean (GLFUNC *ptr_glIsRenderbuffer)(GLuint renderbuffer);
	typedef void (GLFUNC *ptr_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
	typedef void (GLFUNC *ptr_glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
	typedef void (GLFUNC *ptr_glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
	typedef void (GLFUNC *ptr_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
	typedef GLboolean (GLFUNC *ptr_glIsFramebuffer)(GLuint framebuffer);
	typedef void (GLFUNC *ptr_glBindFramebuffer)(GLenum target, GLuint framebuffer);
	typedef void (GLFUNC *ptr_glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
	typedef void (GLFUNC *ptr_glGenFramebuffers)(GLsizei n, GLuint *framebuffers);
	typedef GLenum (GLFUNC *ptr_glCheckFramebufferStatus)(GLenum target);
	typedef void (GLFUNC *ptr_glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (GLFUNC *ptr_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (GLFUNC *ptr_glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
	typedef void (GLFUNC *ptr_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	typedef void (GLFUNC *ptr_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGenerateMipmap)(GLenum target);
	typedef void (GLFUNC *ptr_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
	typedef void (GLFUNC *ptr_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
	typedef GLvoid* (GLFUNC *ptr_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
	typedef void (GLFUNC *ptr_glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
	typedef void (GLFUNC *ptr_glBindVertexArray)(GLuint array);
	typedef void (GLFUNC *ptr_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
	typedef void (GLFUNC *ptr_glGenVertexArrays)(GLsizei n, GLuint *arrays);
	typedef GLboolean (GLFUNC *ptr_glIsVertexArray)(GLuint array);
	typedef void (GLFUNC *ptr_glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar* const *uniformNames, GLuint *uniformIndices);
	typedef void (GLFUNC *ptr_glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
	typedef GLuint (GLFUNC *ptr_glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
	typedef void (GLFUNC *ptr_glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
	typedef void (GLFUNC *ptr_glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
	typedef void (GLFUNC *ptr_glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
	typedef void (GLFUNC *ptr_glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
	typedef void (GLFUNC *ptr_glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
	typedef void (GLFUNC *ptr_glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount, GLint basevertex);
	typedef void (GLFUNC *ptr_glMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei drawcount, const GLint *basevertex);
	typedef void (GLFUNC *ptr_glProvokingVertex)(GLenum mode);
	typedef CLsync (GLFUNC *ptr_glFenceSync)(GLenum condition, GLbitfield flags);
	typedef GLboolean (GLFUNC *ptr_glIsSync)(CLsync sync);
	typedef void (GLFUNC *ptr_glDeleteSync)(CLsync sync);
	typedef GLenum (GLFUNC *ptr_glClientWaitSync)(CLsync sync, GLbitfield flags, CLuint64 timeout);
	typedef void (GLFUNC *ptr_glWaitSync)(CLsync sync, GLbitfield flags, CLuint64 timeout);
	typedef void (GLFUNC *ptr_glGetInteger64v)(GLenum pname, CLint64 *params);
	typedef void (GLFUNC *ptr_glGetSynciv)(CLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
	typedef void (GLFUNC *ptr_glTexImage2DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
	typedef void (GLFUNC *ptr_glTexImage3DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
	typedef void (GLFUNC *ptr_glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
	typedef void (GLFUNC *ptr_glSampleMaski)(GLuint index, GLbitfield mask);
	typedef void (GLFUNC *ptr_glBlendEquationiARB)(GLuint buf, GLenum mode);
	typedef void (GLFUNC *ptr_glBlendEquationSeparateiARB)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
	typedef void (GLFUNC *ptr_glBlendFunciARB)(GLuint buf, GLenum src, GLenum dst);
	typedef void (GLFUNC *ptr_glBlendFuncSeparateiARB)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	typedef void (GLFUNC *ptr_glMinSampleShadingARB)(GLfloat value);
	typedef void (GLFUNC *ptr_glNamedStringARB)(GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string);
	typedef void (GLFUNC *ptr_glDeleteNamedStringARB)(GLint namelen, const GLchar *name);
	typedef void (GLFUNC *ptr_glCompileShaderIncludeARB)(GLuint shader, GLsizei count, const GLchar* *path, const GLint *length);
	typedef GLboolean (GLFUNC *ptr_glIsNamedStringARB)(GLint namelen, const GLchar *name);
	typedef void (GLFUNC *ptr_glGetNamedStringARB)(GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string);
	typedef void (GLFUNC *ptr_glGetNamedStringivARB)(GLint namelen, const GLchar *name, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
	typedef GLint (GLFUNC *ptr_glGetFragDataIndex)(GLuint program, const GLchar *name);
	typedef void (GLFUNC *ptr_glGenSamplers)(GLsizei count, GLuint *samplers);
	typedef void (GLFUNC *ptr_glDeleteSamplers)(GLsizei count, const GLuint *samplers);
	typedef GLboolean (GLFUNC *ptr_glIsSampler)(GLuint sampler);
	typedef void (GLFUNC *ptr_glBindSampler)(GLuint unit, GLuint sampler);
	typedef void (GLFUNC *ptr_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
	typedef void (GLFUNC *ptr_glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);
	typedef void (GLFUNC *ptr_glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
	typedef void (GLFUNC *ptr_glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);
	typedef void (GLFUNC *ptr_glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param);
	typedef void (GLFUNC *ptr_glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param);
	typedef void (GLFUNC *ptr_glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
	typedef void (GLFUNC *ptr_glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
	typedef void (GLFUNC *ptr_glQueryCounter)(GLuint id, GLenum target);
	typedef void (GLFUNC *ptr_glGetQueryObjecti64v)(GLuint id, GLenum pname, CLint64 *params);
	typedef void (GLFUNC *ptr_glGetQueryObjectui64v)(GLuint id, GLenum pname, CLuint64 *params);
	typedef void (GLFUNC *ptr_glVertexP2ui)(GLenum type, GLuint value);
	typedef void (GLFUNC *ptr_glVertexP2uiv)(GLenum type, const GLuint *value);
	typedef void (GLFUNC *ptr_glVertexP3ui)(GLenum type, GLuint value);
	typedef void (GLFUNC *ptr_glVertexP3uiv)(GLenum type, const GLuint *value);
	typedef void (GLFUNC *ptr_glVertexP4ui)(GLenum type, GLuint value);
	typedef void (GLFUNC *ptr_glVertexP4uiv)(GLenum type, const GLuint *value);
	typedef void (GLFUNC *ptr_glTexCoordP1ui)(GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glTexCoordP1uiv)(GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glTexCoordP2ui)(GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glTexCoordP2uiv)(GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glTexCoordP3ui)(GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glTexCoordP3uiv)(GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glTexCoordP4ui)(GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glTexCoordP4uiv)(GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP1ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP1uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP2ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP2uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP3ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP3uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP4ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glMultiTexCoordP4uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glNormalP3ui)(GLenum type, GLuint coords);
	typedef void (GLFUNC *ptr_glNormalP3uiv)(GLenum type, const GLuint *coords);
	typedef void (GLFUNC *ptr_glColorP3ui)(GLenum type, GLuint color);
	typedef void (GLFUNC *ptr_glColorP3uiv)(GLenum type, const GLuint *color);
	typedef void (GLFUNC *ptr_glColorP4ui)(GLenum type, GLuint color);
	typedef void (GLFUNC *ptr_glColorP4uiv)(GLenum type, const GLuint *color);
	typedef void (GLFUNC *ptr_glSecondaryColorP3ui)(GLenum type, GLuint color);
	typedef void (GLFUNC *ptr_glSecondaryColorP3uiv)(GLenum type, const GLuint *color);
	typedef void (GLFUNC *ptr_glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (GLFUNC *ptr_glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (GLFUNC *ptr_glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (GLFUNC *ptr_glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (GLFUNC *ptr_glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (GLFUNC *ptr_glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (GLFUNC *ptr_glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (GLFUNC *ptr_glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (GLFUNC *ptr_glDrawArraysIndirect)(GLenum mode, const GLvoid *indirect);
	typedef void (GLFUNC *ptr_glDrawElementsIndirect)(GLenum mode, GLenum type, const GLvoid *indirect);
	typedef void (GLFUNC *ptr_glUniform1d)(GLint location, GLdouble x);
	typedef void (GLFUNC *ptr_glUniform2d)(GLint location, GLdouble x, GLdouble y);
	typedef void (GLFUNC *ptr_glUniform3d)(GLint location, GLdouble x, GLdouble y, GLdouble z);
	typedef void (GLFUNC *ptr_glUniform4d)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	typedef void (GLFUNC *ptr_glUniform1dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniform2dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniform3dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniform4dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix2x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix2x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix3x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix3x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix4x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glUniformMatrix4x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glGetUniformdv)(GLuint program, GLint location, GLdouble *params);
	typedef GLint (GLFUNC *ptr_glGetSubroutineUniformLocation)(GLuint program, GLenum shadertype, const GLchar *name);
	typedef GLuint (GLFUNC *ptr_glGetSubroutineIndex)(GLuint program, GLenum shadertype, const GLchar *name);
	typedef void (GLFUNC *ptr_glGetActiveSubroutineUniformiv)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
	typedef void (GLFUNC *ptr_glGetActiveSubroutineUniformName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
	typedef void (GLFUNC *ptr_glGetActiveSubroutineName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
	typedef void (GLFUNC *ptr_glUniformSubroutinesuiv)(GLenum shadertype, GLsizei count, const GLuint *indices);
	typedef void (GLFUNC *ptr_glGetUniformSubroutineuiv)(GLenum shadertype, GLint location, GLuint *params);
	typedef void (GLFUNC *ptr_glGetProgramStageiv)(GLuint program, GLenum shadertype, GLenum pname, GLint *values);
	typedef void (GLFUNC *ptr_glPatchParameteri)(GLenum pname, GLint value);
	typedef void (GLFUNC *ptr_glPatchParameterfv)(GLenum pname, const GLfloat *values);
	typedef void (GLFUNC *ptr_glBindTransformFeedback)(GLenum target, GLuint id);
	typedef void (GLFUNC *ptr_glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
	typedef void (GLFUNC *ptr_glGenTransformFeedbacks)(GLsizei n, GLuint *ids);
	typedef GLboolean (GLFUNC *ptr_glIsTransformFeedback)(GLuint id);
	typedef void (GLFUNC *ptr_glPauseTransformFeedback)(void);
	typedef void (GLFUNC *ptr_glResumeTransformFeedback)(void);
	typedef void (GLFUNC *ptr_glDrawTransformFeedback)(GLenum mode, GLuint id);
	typedef void (GLFUNC *ptr_glDrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
	typedef void (GLFUNC *ptr_glBeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
	typedef void (GLFUNC *ptr_glEndQueryIndexed)(GLenum target, GLuint index);
	typedef void (GLFUNC *ptr_glGetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glReleaseShaderCompiler)(void);
	typedef void (GLFUNC *ptr_glShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const GLvoid *binary, GLsizei length);
	typedef void (GLFUNC *ptr_glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
	typedef void (GLFUNC *ptr_glDepthRangef)(GLfloat n, GLfloat f);
	typedef void (GLFUNC *ptr_glClearDepthf)(GLfloat d);
	typedef void (GLFUNC *ptr_glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
	typedef void (GLFUNC *ptr_glProgramBinary)(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length);
	typedef void (GLFUNC *ptr_glProgramParameteri)(GLuint program, GLenum pname, GLint value);
	typedef void (GLFUNC *ptr_glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
	typedef void (GLFUNC *ptr_glActiveShaderProgram)(GLuint pipeline, GLuint program);
	typedef GLuint (GLFUNC *ptr_glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar* const *strings);
	typedef void (GLFUNC *ptr_glBindProgramPipeline)(GLuint pipeline);
	typedef void (GLFUNC *ptr_glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
	typedef void (GLFUNC *ptr_glGenProgramPipelines)(GLsizei n, GLuint *pipelines);
	typedef GLboolean (GLFUNC *ptr_glIsProgramPipeline)(GLuint pipeline);
	typedef void (GLFUNC *ptr_glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glProgramUniform1i)(GLuint program, GLint location, GLint v0);
	typedef void (GLFUNC *ptr_glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
	typedef void (GLFUNC *ptr_glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniform1d)(GLuint program, GLint location, GLdouble v0);
	typedef void (GLFUNC *ptr_glProgramUniform1dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
	typedef void (GLFUNC *ptr_glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
	typedef void (GLFUNC *ptr_glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
	typedef void (GLFUNC *ptr_glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniform2d)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
	typedef void (GLFUNC *ptr_glProgramUniform2dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
	typedef void (GLFUNC *ptr_glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (GLFUNC *ptr_glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (GLFUNC *ptr_glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniform3d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
	typedef void (GLFUNC *ptr_glProgramUniform3dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
	typedef void (GLFUNC *ptr_glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (GLFUNC *ptr_glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (GLFUNC *ptr_glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (GLFUNC *ptr_glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniform4d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
	typedef void (GLFUNC *ptr_glProgramUniform4dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
	typedef void (GLFUNC *ptr_glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix2x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix3x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix2x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix4x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix3x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glProgramUniformMatrix4x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (GLFUNC *ptr_glValidateProgramPipeline)(GLuint pipeline);
	typedef void (GLFUNC *ptr_glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef void (GLFUNC *ptr_glVertexAttribL1d)(GLuint index, GLdouble x);
	typedef void (GLFUNC *ptr_glVertexAttribL2d)(GLuint index, GLdouble x, GLdouble y);
	typedef void (GLFUNC *ptr_glVertexAttribL3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
	typedef void (GLFUNC *ptr_glVertexAttribL4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	typedef void (GLFUNC *ptr_glVertexAttribL1dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttribL2dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttribL3dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttribL4dv)(GLuint index, const GLdouble *v);
	typedef void (GLFUNC *ptr_glVertexAttribLPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	typedef void (GLFUNC *ptr_glGetVertexAttribLdv)(GLuint index, GLenum pname, GLdouble *params);
	typedef void (GLFUNC *ptr_glViewportArrayv)(GLuint first, GLsizei count, const GLfloat *v);
	typedef void (GLFUNC *ptr_glViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	typedef void (GLFUNC *ptr_glViewportIndexedfv)(GLuint index, const GLfloat *v);
	typedef void (GLFUNC *ptr_glScissorArrayv)(GLuint first, GLsizei count, const GLint *v);
	typedef void (GLFUNC *ptr_glScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glScissorIndexedv)(GLuint index, const GLint *v);
	typedef void (GLFUNC *ptr_glDepthRangeArrayv)(GLuint first, GLsizei count, const GLdouble *v);
	typedef void (GLFUNC *ptr_glDepthRangeIndexed)(GLuint index, GLdouble n, GLdouble f);
	typedef void (GLFUNC *ptr_glGetFloati_v)(GLenum target, GLuint index, GLfloat *data);
	typedef void (GLFUNC *ptr_glGetDoublei_v)(GLenum target, GLuint index, GLdouble *data);
	typedef CLsync (GLFUNC *ptr_glCreateSyncFromCLeventARB)(struct _cl_context * context, struct _cl_event * event, GLbitfield flags);
	typedef void (GLFUNC *ptr_glDebugMessageControlARB)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
	typedef void (GLFUNC *ptr_glDebugMessageInsertARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
	typedef void (GLFUNC *ptr_glDebugMessageCallbackARB)(CLDEBUGPROCARB callback, const GLvoid *userParam);
	typedef GLuint (GLFUNC *ptr_glGetDebugMessageLogARB)(GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
	typedef GLenum (GLFUNC *ptr_glGetGraphicsResetStatusARB)(void);
	typedef void (GLFUNC *ptr_glGetnMapdvARB)(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v);
	typedef void (GLFUNC *ptr_glGetnMapfvARB)(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v);
	typedef void (GLFUNC *ptr_glGetnMapivARB)(GLenum target, GLenum query, GLsizei bufSize, GLint *v);
	typedef void (GLFUNC *ptr_glGetnPixelMapfvARB)(GLenum map, GLsizei bufSize, GLfloat *values);
	typedef void (GLFUNC *ptr_glGetnPixelMapuivARB)(GLenum map, GLsizei bufSize, GLuint *values);
	typedef void (GLFUNC *ptr_glGetnPixelMapusvARB)(GLenum map, GLsizei bufSize, GLushort *values);
	typedef void (GLFUNC *ptr_glGetnPolygonStippleARB)(GLsizei bufSize, GLubyte *pattern);
	typedef void (GLFUNC *ptr_glGetnColorTableARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *table);
	typedef void (GLFUNC *ptr_glGetnConvolutionFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *image);
	typedef void (GLFUNC *ptr_glGetnSeparableFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid *row, GLsizei columnBufSize, GLvoid *column, GLvoid *span);
	typedef void (GLFUNC *ptr_glGetnHistogramARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
	typedef void (GLFUNC *ptr_glGetnMinmaxARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
	typedef void (GLFUNC *ptr_glGetnTexImageARB)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid *img);
	typedef void (GLFUNC *ptr_glReadnPixelsARB)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid *data);
	typedef void (GLFUNC *ptr_glGetnCompressedTexImageARB)(GLenum target, GLint lod, GLsizei bufSize, GLvoid *img);
	typedef void (GLFUNC *ptr_glGetnUniformfvARB)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
	typedef void (GLFUNC *ptr_glGetnUniformivARB)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
	typedef void (GLFUNC *ptr_glGetnUniformuivARB)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
	typedef void (GLFUNC *ptr_glGetnUniformdvARB)(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
	typedef void (GLFUNC *ptr_glDrawArraysInstancedBaseInstance)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
	typedef void (GLFUNC *ptr_glDrawElementsInstancedBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
	typedef void (GLFUNC *ptr_glDrawElementsInstancedBaseVertexBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
	typedef void (GLFUNC *ptr_glDrawTransformFeedbackInstanced)(GLenum mode, GLuint id, GLsizei instancecount);
	typedef void (GLFUNC *ptr_glDrawTransformFeedbackStreamInstanced)(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
	typedef void (GLFUNC *ptr_glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
	typedef void (GLFUNC *ptr_glGetActiveAtomicCounterBufferiv)(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
	typedef void (GLFUNC *ptr_glMemoryBarrier)(GLbitfield barriers);
	typedef void (GLFUNC *ptr_glTexStorage1D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
	typedef void (GLFUNC *ptr_glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
	typedef void (GLFUNC *ptr_glTextureStorage1DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
	typedef void (GLFUNC *ptr_glTextureStorage2DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glTextureStorage3DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
	typedef void (GLFUNC *ptr_glDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
	typedef void (GLFUNC *ptr_glDebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
	typedef void (GLFUNC *ptr_glDebugMessageCallback)(CLDEBUGPROC callback, const void *userParam);
	typedef GLuint (GLFUNC *ptr_glGetDebugMessageLog)(GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
	typedef void (GLFUNC *ptr_glPushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
	typedef void (GLFUNC *ptr_glPopDebugGroup)(void);
	typedef void (GLFUNC *ptr_glObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
	typedef void (GLFUNC *ptr_glGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
	typedef void (GLFUNC *ptr_glObjectPtrLabel)(const void *ptr, GLsizei length, const GLchar *label);
	typedef void (GLFUNC *ptr_glGetObjectPtrLabel)(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
	typedef void (GLFUNC *ptr_glClearBufferData)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
	typedef void (GLFUNC *ptr_glClearBufferSubData)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
	typedef void (GLFUNC *ptr_glClearNamedBufferDataEXT)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
	typedef void (GLFUNC *ptr_glClearNamedBufferSubDataEXT)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, GLsizeiptr offset, GLsizeiptr size, const void *data);
	typedef void (GLFUNC *ptr_glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
	typedef void (GLFUNC *ptr_glDispatchComputeIndirect)(GLintptr indirect);
	typedef void (GLFUNC *ptr_glCopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
	typedef void (GLFUNC *ptr_glFramebufferParameteri)(GLenum target, GLenum pname, GLint param);
	typedef void (GLFUNC *ptr_glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glNamedFramebufferParameteriEXT)(GLuint framebuffer, GLenum pname, GLint param);
	typedef void (GLFUNC *ptr_glGetNamedFramebufferParameterivEXT)(GLuint framebuffer, GLenum pname, GLint *params);
	typedef void (GLFUNC *ptr_glGetInternalformati64v)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, CLint64 *params);
	typedef void (GLFUNC *ptr_glInvalidateTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
	typedef void (GLFUNC *ptr_glInvalidateTexImage)(GLuint texture, GLint level);
	typedef void (GLFUNC *ptr_glInvalidateBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr length);
	typedef void (GLFUNC *ptr_glInvalidateBufferData)(GLuint buffer);
	typedef void (GLFUNC *ptr_glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
	typedef void (GLFUNC *ptr_glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (GLFUNC *ptr_glMultiDrawArraysIndirect)(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
	typedef void (GLFUNC *ptr_glMultiDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
	typedef void (GLFUNC *ptr_glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
	typedef GLuint (GLFUNC *ptr_glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);
	typedef void (GLFUNC *ptr_glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
	typedef void (GLFUNC *ptr_glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
	typedef GLint (GLFUNC *ptr_glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);
	typedef GLint (GLFUNC *ptr_glGetProgramResourceLocationIndex)(GLuint program, GLenum programInterface, const GLchar *name);
	typedef void (GLFUNC *ptr_glShaderStorageBlockBinding)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
	typedef void (GLFUNC *ptr_glTexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
	typedef void (GLFUNC *ptr_glTextureBufferRangeEXT)(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
	typedef void (GLFUNC *ptr_glTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
	typedef void (GLFUNC *ptr_glTexStorage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
	typedef void (GLFUNC *ptr_glTextureStorage2DMultisampleEXT)(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
	typedef void (GLFUNC *ptr_glTextureStorage3DMultisampleEXT)(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
	typedef void (GLFUNC *ptr_glTextureView)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
	typedef void (GLFUNC *ptr_glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
	typedef void (GLFUNC *ptr_glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
	typedef void (GLFUNC *ptr_glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (GLFUNC *ptr_glVertexAttribLFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (GLFUNC *ptr_glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);
	typedef void (GLFUNC *ptr_glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
	typedef void (GLFUNC *ptr_glVertexArrayBindVertexBufferEXT)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
	typedef void (GLFUNC *ptr_glVertexArrayVertexAttribFormatEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
	typedef void (GLFUNC *ptr_glVertexArrayVertexAttribIFormatEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (GLFUNC *ptr_glVertexArrayVertexAttribLFormatEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (GLFUNC *ptr_glVertexArrayVertexAttribBindingEXT)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
	typedef void (GLFUNC *ptr_glVertexArrayVertexBindingDivisorEXT)(GLuint vaobj, GLuint bindingindex, GLuint divisor);

	// For Legacy OpenGL (For GL1 target)
	typedef GLvoid (GLFUNC *ptr_glClientActiveTextureARB)(GLenum);
#ifdef WIN32
    DECLARE_HANDLE(HPBUFFERARB);
	typedef HPBUFFERARB (GLFUNC *ptr_wglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
	typedef HDC (GLFUNC *ptr_wglGetPbufferDCARB)(HPBUFFERARB hPbuffer);
	typedef int (GLFUNC *ptr_wglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
	typedef BOOL (GLFUNC *ptr_wglDestroyPbufferARB)(HPBUFFERARB hPbuffer);
	typedef BOOL (GLFUNC *ptr_wglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
#endif


/// \}

/// \name Functions
/// \{

public:
	ptr_glCullFace cullFace;
	ptr_glFrontFace frontFace;
	ptr_glHint hint;
	ptr_glLineWidth lineWidth;
	ptr_glPointSize pointSize;
	ptr_glPolygonMode polygonMode;
	ptr_glScissor scissor;
	ptr_glTexParameterf texParameterf;
	ptr_glTexParameterfv texParameterfv;
	ptr_glTexParameteri texParameteri;
	ptr_glTexParameteriv texParameteriv;
	ptr_glTexImage1D texImage1D;
	ptr_glTexImage2D texImage2D;
	ptr_glDrawBuffer drawBuffer;
	ptr_glClear clear;
	ptr_glClearColor clearColor;
	ptr_glClearStencil clearStencil;
	ptr_glClearDepth clearDepth;
	ptr_glStencilMask stencilMask;
	ptr_glColorMask colorMask;
	ptr_glDepthMask depthMask;
	ptr_glDisable disable;
	ptr_glEnable enable;
	ptr_glFinish finish;
	ptr_glFlush flush;
	ptr_glBlendFunc blendFunc;
	ptr_glLogicOp logicOp;
	ptr_glStencilFunc stencilFunc;
	ptr_glStencilOp stencilOp;
	ptr_glDepthFunc depthFunc;
	ptr_glPixelStoref pixelStoref;
	ptr_glPixelStorei pixelStorei;
	ptr_glReadBuffer readBuffer;
	ptr_glReadPixels readPixels;
	ptr_glGetBooleanv getBooleanv;
	ptr_glGetDoublev getDoublev;
	ptr_glGetError getError;
	ptr_glGetFloatv getFloatv;
	ptr_glGetIntegerv getIntegerv;
	ptr_glGetString getString;
	ptr_glGetTexImage getTexImage;
	ptr_glGetTexParameterfv getTexParameterfv;
	ptr_glGetTexParameteriv getTexParameteriv;
	ptr_glGetTexLevelParameterfv getTexLevelParameterfv;
	ptr_glGetTexLevelParameteriv getTexLevelParameteriv;
	ptr_glIsEnabled isEnabled;
	ptr_glDepthRange depthRange;
	ptr_glViewport viewport;
	ptr_glDrawArrays drawArrays;
	ptr_glDrawElements drawElements;
	ptr_glGetPointerv getPointerv;
	ptr_glPolygonOffset polygonOffset;
	ptr_glCopyTexImage1D copyTexImage1D;
	ptr_glCopyTexImage2D copyTexImage2D;
	ptr_glCopyTexSubImage1D copyTexSubImage1D;
	ptr_glCopyTexSubImage2D copyTexSubImage2D;
	ptr_glTexSubImage1D texSubImage1D;
	ptr_glTexSubImage2D texSubImage2D;
	ptr_glBindTexture bindTexture;
	ptr_glDeleteTextures deleteTextures;
	ptr_glGenTextures genTextures;
	ptr_glIsTexture isTexture;
	ptr_glBlendColor blendColor;
	ptr_glBlendEquation blendEquation;
	ptr_glDrawRangeElements drawRangeElements;
	ptr_glTexImage3D texImage3D;
	ptr_glTexSubImage3D texSubImage3D;
	ptr_glCopyTexSubImage3D copyTexSubImage3D;
	ptr_glActiveTexture activeTexture;
	ptr_glSampleCoverage sampleCoverage;
	ptr_glCompressedTexImage3D compressedTexImage3D;
	ptr_glCompressedTexImage2D compressedTexImage2D;
	ptr_glCompressedTexImage1D compressedTexImage1D;
	ptr_glCompressedTexSubImage3D compressedTexSubImage3D;
	ptr_glCompressedTexSubImage2D compressedTexSubImage2D;
	ptr_glCompressedTexSubImage1D compressedTexSubImage1D;
	ptr_glGetCompressedTexImage getCompressedTexImage;
	ptr_glBlendFuncSeparate blendFuncSeparate;
	ptr_glMultiDrawArrays multiDrawArrays;
	ptr_glMultiDrawElements multiDrawElements;
	ptr_glPointParameterf pointParameterf;
	ptr_glPointParameterfv pointParameterfv;
	ptr_glPointParameteri pointParameteri;
	ptr_glPointParameteriv pointParameteriv;
	ptr_glGenQueries genQueries;
	ptr_glDeleteQueries deleteQueries;
	ptr_glIsQuery isQuery;
	ptr_glBeginQuery beginQuery;
	ptr_glEndQuery endQuery;
	ptr_glGetQueryiv getQueryiv;
	ptr_glGetQueryObjectiv getQueryObjectiv;
	ptr_glGetQueryObjectuiv getQueryObjectuiv;
	ptr_glBindBuffer bindBuffer;
	ptr_glDeleteBuffers deleteBuffers;
	ptr_glGenBuffers genBuffers;
	ptr_glIsBuffer isBuffer;
	ptr_glBufferData bufferData;
	ptr_glBufferSubData bufferSubData;
	ptr_glGetBufferSubData getBufferSubData;
	ptr_glMapBuffer mapBuffer;
	ptr_glUnmapBuffer unmapBuffer;
	ptr_glGetBufferParameteriv getBufferParameteriv;
	ptr_glGetBufferPointerv getBufferPointerv;
	ptr_glBlendEquationSeparate blendEquationSeparate;
	ptr_glDrawBuffers drawBuffers;
	ptr_glStencilOpSeparate stencilOpSeparate;
	ptr_glStencilFuncSeparate stencilFuncSeparate;
	ptr_glStencilMaskSeparate stencilMaskSeparate;
	ptr_glAttachShader attachShader;
	ptr_glBindAttribLocation bindAttribLocation;
	ptr_glCompileShader compileShader;
	ptr_glCreateProgram createProgram;
	ptr_glCreateShader createShader;
	ptr_glDeleteProgram deleteProgram;
	ptr_glDeleteShader deleteShader;
	ptr_glDetachShader detachShader;
	ptr_glDisableVertexAttribArray disableVertexAttribArray;
	ptr_glEnableVertexAttribArray enableVertexAttribArray;
	ptr_glGetActiveAttrib getActiveAttrib;
	ptr_glGetActiveUniform getActiveUniform;
	ptr_glGetAttachedShaders getAttachedShaders;
	ptr_glGetAttribLocation getAttribLocation;
	ptr_glGetProgramiv getProgramiv;
	ptr_glGetProgramInfoLog getProgramInfoLog;
	ptr_glGetShaderiv getShaderiv;
	ptr_glGetShaderInfoLog getShaderInfoLog;
	ptr_glGetShaderSource getShaderSource;
	ptr_glGetUniformLocation getUniformLocation;
	ptr_glGetUniformfv getUniformfv;
	ptr_glGetUniformiv getUniformiv;
	ptr_glGetVertexAttribdv getVertexAttribdv;
	ptr_glGetVertexAttribfv getVertexAttribfv;
	ptr_glGetVertexAttribiv getVertexAttribiv;
	ptr_glGetVertexAttribPointerv getVertexAttribPointerv;
	ptr_glIsProgram isProgram;
	ptr_glIsShader isShader;
	ptr_glLinkProgram linkProgram;
	ptr_glShaderSource shaderSource;
	ptr_glUseProgram useProgram;
	ptr_glUniform1f uniform1f;
	ptr_glUniform2f uniform2f;
	ptr_glUniform3f uniform3f;
	ptr_glUniform4f uniform4f;
	ptr_glUniform1i uniform1i;
	ptr_glUniform2i uniform2i;
	ptr_glUniform3i uniform3i;
	ptr_glUniform4i uniform4i;
	ptr_glUniform1fv uniform1fv;
	ptr_glUniform2fv uniform2fv;
	ptr_glUniform3fv uniform3fv;
	ptr_glUniform4fv uniform4fv;
	ptr_glUniform1iv uniform1iv;
	ptr_glUniform2iv uniform2iv;
	ptr_glUniform3iv uniform3iv;
	ptr_glUniform4iv uniform4iv;
	ptr_glUniformMatrix2fv uniformMatrix2fv;
	ptr_glUniformMatrix3fv uniformMatrix3fv;
	ptr_glUniformMatrix4fv uniformMatrix4fv;
	ptr_glValidateProgram validateProgram;
	ptr_glVertexAttrib1d vertexAttrib1d;
	ptr_glVertexAttrib1dv vertexAttrib1dv;
	ptr_glVertexAttrib1f vertexAttrib1f;
	ptr_glVertexAttrib1fv vertexAttrib1fv;
	ptr_glVertexAttrib1s vertexAttrib1s;
	ptr_glVertexAttrib1sv vertexAttrib1sv;
	ptr_glVertexAttrib2d vertexAttrib2d;
	ptr_glVertexAttrib2dv vertexAttrib2dv;
	ptr_glVertexAttrib2f vertexAttrib2f;
	ptr_glVertexAttrib2fv vertexAttrib2fv;
	ptr_glVertexAttrib2s vertexAttrib2s;
	ptr_glVertexAttrib2sv vertexAttrib2sv;
	ptr_glVertexAttrib3d vertexAttrib3d;
	ptr_glVertexAttrib3dv vertexAttrib3dv;
	ptr_glVertexAttrib3f vertexAttrib3f;
	ptr_glVertexAttrib3fv vertexAttrib3fv;
	ptr_glVertexAttrib3s vertexAttrib3s;
	ptr_glVertexAttrib3sv vertexAttrib3sv;
	ptr_glVertexAttrib4Nbv vertexAttrib4Nbv;
	ptr_glVertexAttrib4Niv vertexAttrib4Niv;
	ptr_glVertexAttrib4Nsv vertexAttrib4Nsv;
	ptr_glVertexAttrib4Nub vertexAttrib4Nub;
	ptr_glVertexAttrib4Nubv vertexAttrib4Nubv;
	ptr_glVertexAttrib4Nuiv vertexAttrib4Nuiv;
	ptr_glVertexAttrib4Nusv vertexAttrib4Nusv;
	ptr_glVertexAttrib4bv vertexAttrib4bv;
	ptr_glVertexAttrib4d vertexAttrib4d;
	ptr_glVertexAttrib4dv vertexAttrib4dv;
	ptr_glVertexAttrib4f vertexAttrib4f;
	ptr_glVertexAttrib4fv vertexAttrib4fv;
	ptr_glVertexAttrib4iv vertexAttrib4iv;
	ptr_glVertexAttrib4s vertexAttrib4s;
	ptr_glVertexAttrib4sv vertexAttrib4sv;
	ptr_glVertexAttrib4ubv vertexAttrib4ubv;
	ptr_glVertexAttrib4uiv vertexAttrib4uiv;
	ptr_glVertexAttrib4usv vertexAttrib4usv;
	ptr_glVertexAttribPointer vertexAttribPointer;
	ptr_glUniformMatrix2x3fv uniformMatrix2x3fv;
	ptr_glUniformMatrix3x2fv uniformMatrix3x2fv;
	ptr_glUniformMatrix2x4fv uniformMatrix2x4fv;
	ptr_glUniformMatrix4x2fv uniformMatrix4x2fv;
	ptr_glUniformMatrix3x4fv uniformMatrix3x4fv;
	ptr_glUniformMatrix4x3fv uniformMatrix4x3fv;
	ptr_glColorMaski colorMaski;
	ptr_glGetBooleani_v getBooleani_v;
	ptr_glGetIntegeri_v getIntegeri_v;
	ptr_glEnablei enablei;
	ptr_glDisablei disablei;
	ptr_glIsEnabledi isEnabledi;
	ptr_glBeginTransformFeedback beginTransformFeedback;
	ptr_glEndTransformFeedback endTransformFeedback;
	ptr_glBindBufferRange bindBufferRange;
	ptr_glBindBufferBase bindBufferBase;
	ptr_glTransformFeedbackVaryings transformFeedbackVaryings;
	ptr_glGetTransformFeedbackVarying getTransformFeedbackVarying;
	ptr_glClampColor clampColor;
	ptr_glBeginConditionalRender beginConditionalRender;
	ptr_glEndConditionalRender endConditionalRender;
	ptr_glVertexAttribIPointer vertexAttribIPointer;
	ptr_glGetVertexAttribIiv getVertexAttribIiv;
	ptr_glGetVertexAttribIuiv getVertexAttribIuiv;
	ptr_glVertexAttribI1i vertexAttribI1i;
	ptr_glVertexAttribI2i vertexAttribI2i;
	ptr_glVertexAttribI3i vertexAttribI3i;
	ptr_glVertexAttribI4i vertexAttribI4i;
	ptr_glVertexAttribI1ui vertexAttribI1ui;
	ptr_glVertexAttribI2ui vertexAttribI2ui;
	ptr_glVertexAttribI3ui vertexAttribI3ui;
	ptr_glVertexAttribI4ui vertexAttribI4ui;
	ptr_glVertexAttribI1iv vertexAttribI1iv;
	ptr_glVertexAttribI2iv vertexAttribI2iv;
	ptr_glVertexAttribI3iv vertexAttribI3iv;
	ptr_glVertexAttribI4iv vertexAttribI4iv;
	ptr_glVertexAttribI1uiv vertexAttribI1uiv;
	ptr_glVertexAttribI2uiv vertexAttribI2uiv;
	ptr_glVertexAttribI3uiv vertexAttribI3uiv;
	ptr_glVertexAttribI4uiv vertexAttribI4uiv;
	ptr_glVertexAttribI4bv vertexAttribI4bv;
	ptr_glVertexAttribI4sv vertexAttribI4sv;
	ptr_glVertexAttribI4ubv vertexAttribI4ubv;
	ptr_glVertexAttribI4usv vertexAttribI4usv;
	ptr_glGetUniformuiv getUniformuiv;
	ptr_glBindFragDataLocation bindFragDataLocation;
	ptr_glGetFragDataLocation getFragDataLocation;
	ptr_glUniform1ui uniform1ui;
	ptr_glUniform2ui uniform2ui;
	ptr_glUniform3ui uniform3ui;
	ptr_glUniform4ui uniform4ui;
	ptr_glUniform1uiv uniform1uiv;
	ptr_glUniform2uiv uniform2uiv;
	ptr_glUniform3uiv uniform3uiv;
	ptr_glUniform4uiv uniform4uiv;
	ptr_glTexParameterIiv texParameterIiv;
	ptr_glTexParameterIuiv texParameterIuiv;
	ptr_glGetTexParameterIiv getTexParameterIiv;
	ptr_glGetTexParameterIuiv getTexParameterIuiv;
	ptr_glClearBufferiv clearBufferiv;
	ptr_glClearBufferuiv clearBufferuiv;
	ptr_glClearBufferfv clearBufferfv;
	ptr_glClearBufferfi clearBufferfi;
	ptr_glGetStringi getStringi;
	ptr_glDrawArraysInstanced drawArraysInstanced;
	ptr_glDrawElementsInstanced drawElementsInstanced;
	ptr_glTexBuffer texBuffer;
	ptr_glPrimitiveRestartIndex primitiveRestartIndex;
	ptr_glGetInteger64i_v getInteger64i_v;
	ptr_glGetBufferParameteri64v getBufferParameteri64v;
	ptr_glFramebufferTexture framebufferTexture;
	ptr_glVertexAttribDivisor vertexAttribDivisor;
	ptr_glMinSampleShading minSampleShading;
	ptr_glBlendEquationi blendEquationi;
	ptr_glBlendEquationSeparatei blendEquationSeparatei;
	ptr_glBlendFunci blendFunci;
	ptr_glBlendFuncSeparatei blendFuncSeparatei;
	ptr_glIsRenderbuffer isRenderbuffer;
	ptr_glBindRenderbuffer bindRenderbuffer;
	ptr_glDeleteRenderbuffers deleteRenderbuffers;
	ptr_glGenRenderbuffers genRenderbuffers;
	ptr_glRenderbufferStorage renderbufferStorage;
	ptr_glGetRenderbufferParameteriv getRenderbufferParameteriv;
	ptr_glIsFramebuffer isFramebuffer;
	ptr_glBindFramebuffer bindFramebuffer;
	ptr_glDeleteFramebuffers deleteFramebuffers;
	ptr_glGenFramebuffers genFramebuffers;
	ptr_glCheckFramebufferStatus checkFramebufferStatus;
	ptr_glFramebufferTexture1D framebufferTexture1D;
	ptr_glFramebufferTexture2D framebufferTexture2D;
	ptr_glFramebufferTexture3D framebufferTexture3D;
	ptr_glFramebufferRenderbuffer framebufferRenderbuffer;
	ptr_glGetFramebufferAttachmentParameteriv getFramebufferAttachmentParameteriv;
	ptr_glGenerateMipmap generateMipmap;
	ptr_glBlitFramebuffer blitFramebuffer;
	ptr_glRenderbufferStorageMultisample renderbufferStorageMultisample;
	ptr_glFramebufferTextureLayer framebufferTextureLayer;
	ptr_glMapBufferRange mapBufferRange;
	ptr_glFlushMappedBufferRange flushMappedBufferRange;
	ptr_glBindVertexArray bindVertexArray;
	ptr_glDeleteVertexArrays deleteVertexArrays;
	ptr_glGenVertexArrays genVertexArrays;
	ptr_glIsVertexArray isVertexArray;
	ptr_glGetUniformIndices getUniformIndices;
	ptr_glGetActiveUniformsiv getActiveUniformsiv;
	ptr_glGetActiveUniformName getActiveUniformName;
	ptr_glGetUniformBlockIndex getUniformBlockIndex;
	ptr_glGetActiveUniformBlockiv getActiveUniformBlockiv;
	ptr_glGetActiveUniformBlockName getActiveUniformBlockName;
	ptr_glUniformBlockBinding uniformBlockBinding;
	ptr_glCopyBufferSubData copyBufferSubData;
	ptr_glDrawElementsBaseVertex drawElementsBaseVertex;
	ptr_glDrawRangeElementsBaseVertex drawRangeElementsBaseVertex;
	ptr_glDrawElementsInstancedBaseVertex drawElementsInstancedBaseVertex;
	ptr_glMultiDrawElementsBaseVertex multiDrawElementsBaseVertex;
	ptr_glProvokingVertex provokingVertex;
	ptr_glFenceSync fenceSync;
	ptr_glIsSync isSync;
	ptr_glDeleteSync deleteSync;
	ptr_glClientWaitSync clientWaitSync;
	ptr_glWaitSync waitSync;
	ptr_glGetInteger64v getInteger64v;
	ptr_glGetSynciv getSynciv;
	ptr_glTexImage2DMultisample texImage2DMultisample;
	ptr_glTexImage3DMultisample texImage3DMultisample;
	ptr_glGetMultisamplefv getMultisamplefv;
	ptr_glSampleMaski sampleMaski;
	ptr_glBlendEquationiARB blendEquationiARB;
	ptr_glBlendEquationSeparateiARB blendEquationSeparateiARB;
	ptr_glBlendFunciARB blendFunciARB;
	ptr_glBlendFuncSeparateiARB blendFuncSeparateiARB;
	ptr_glMinSampleShadingARB minSampleShadingARB;
	ptr_glNamedStringARB namedStringARB;
	ptr_glDeleteNamedStringARB deleteNamedStringARB;
	ptr_glCompileShaderIncludeARB compileShaderIncludeARB;
	ptr_glIsNamedStringARB isNamedStringARB;
	ptr_glGetNamedStringARB getNamedStringARB;
	ptr_glGetNamedStringivARB getNamedStringivARB;
	ptr_glBindFragDataLocationIndexed bindFragDataLocationIndexed;
	ptr_glGetFragDataIndex getFragDataIndex;
	ptr_glGenSamplers genSamplers;
	ptr_glDeleteSamplers deleteSamplers;
	ptr_glIsSampler isSampler;
	ptr_glBindSampler bindSampler;
	ptr_glSamplerParameteri samplerParameteri;
	ptr_glSamplerParameteriv samplerParameteriv;
	ptr_glSamplerParameterf samplerParameterf;
	ptr_glSamplerParameterfv samplerParameterfv;
	ptr_glSamplerParameterIiv samplerParameterIiv;
	ptr_glSamplerParameterIuiv samplerParameterIuiv;
	ptr_glGetSamplerParameteriv getSamplerParameteriv;
	ptr_glGetSamplerParameterIiv getSamplerParameterIiv;
	ptr_glGetSamplerParameterfv getSamplerParameterfv;
	ptr_glGetSamplerParameterIuiv getSamplerParameterIuiv;
	ptr_glQueryCounter queryCounter;
	ptr_glGetQueryObjecti64v getQueryObjecti64v;
	ptr_glGetQueryObjectui64v getQueryObjectui64v;
	ptr_glVertexP2ui vertexP2ui;
	ptr_glVertexP2uiv vertexP2uiv;
	ptr_glVertexP3ui vertexP3ui;
	ptr_glVertexP3uiv vertexP3uiv;
	ptr_glVertexP4ui vertexP4ui;
	ptr_glVertexP4uiv vertexP4uiv;
	ptr_glTexCoordP1ui texCoordP1ui;
	ptr_glTexCoordP1uiv texCoordP1uiv;
	ptr_glTexCoordP2ui texCoordP2ui;
	ptr_glTexCoordP2uiv texCoordP2uiv;
	ptr_glTexCoordP3ui texCoordP3ui;
	ptr_glTexCoordP3uiv texCoordP3uiv;
	ptr_glTexCoordP4ui texCoordP4ui;
	ptr_glTexCoordP4uiv texCoordP4uiv;
	ptr_glMultiTexCoordP1ui multiTexCoordP1ui;
	ptr_glMultiTexCoordP1uiv multiTexCoordP1uiv;
	ptr_glMultiTexCoordP2ui multiTexCoordP2ui;
	ptr_glMultiTexCoordP2uiv multiTexCoordP2uiv;
	ptr_glMultiTexCoordP3ui multiTexCoordP3ui;
	ptr_glMultiTexCoordP3uiv multiTexCoordP3uiv;
	ptr_glMultiTexCoordP4ui multiTexCoordP4ui;
	ptr_glMultiTexCoordP4uiv multiTexCoordP4uiv;
	ptr_glNormalP3ui normalP3ui;
	ptr_glNormalP3uiv normalP3uiv;
	ptr_glColorP3ui colorP3ui;
	ptr_glColorP3uiv colorP3uiv;
	ptr_glColorP4ui colorP4ui;
	ptr_glColorP4uiv colorP4uiv;
	ptr_glSecondaryColorP3ui secondaryColorP3ui;
	ptr_glSecondaryColorP3uiv secondaryColorP3uiv;
	ptr_glVertexAttribP1ui vertexAttribP1ui;
	ptr_glVertexAttribP1uiv vertexAttribP1uiv;
	ptr_glVertexAttribP2ui vertexAttribP2ui;
	ptr_glVertexAttribP2uiv vertexAttribP2uiv;
	ptr_glVertexAttribP3ui vertexAttribP3ui;
	ptr_glVertexAttribP3uiv vertexAttribP3uiv;
	ptr_glVertexAttribP4ui vertexAttribP4ui;
	ptr_glVertexAttribP4uiv vertexAttribP4uiv;
	ptr_glDrawArraysIndirect drawArraysIndirect;
	ptr_glDrawElementsIndirect drawElementsIndirect;
	ptr_glUniform1d uniform1d;
	ptr_glUniform2d uniform2d;
	ptr_glUniform3d uniform3d;
	ptr_glUniform4d uniform4d;
	ptr_glUniform1dv uniform1dv;
	ptr_glUniform2dv uniform2dv;
	ptr_glUniform3dv uniform3dv;
	ptr_glUniform4dv uniform4dv;
	ptr_glUniformMatrix2dv uniformMatrix2dv;
	ptr_glUniformMatrix3dv uniformMatrix3dv;
	ptr_glUniformMatrix4dv uniformMatrix4dv;
	ptr_glUniformMatrix2x3dv uniformMatrix2x3dv;
	ptr_glUniformMatrix2x4dv uniformMatrix2x4dv;
	ptr_glUniformMatrix3x2dv uniformMatrix3x2dv;
	ptr_glUniformMatrix3x4dv uniformMatrix3x4dv;
	ptr_glUniformMatrix4x2dv uniformMatrix4x2dv;
	ptr_glUniformMatrix4x3dv uniformMatrix4x3dv;
	ptr_glGetUniformdv getUniformdv;
	ptr_glGetSubroutineUniformLocation getSubroutineUniformLocation;
	ptr_glGetSubroutineIndex getSubroutineIndex;
	ptr_glGetActiveSubroutineUniformiv getActiveSubroutineUniformiv;
	ptr_glGetActiveSubroutineUniformName getActiveSubroutineUniformName;
	ptr_glGetActiveSubroutineName getActiveSubroutineName;
	ptr_glUniformSubroutinesuiv uniformSubroutinesuiv;
	ptr_glGetUniformSubroutineuiv getUniformSubroutineuiv;
	ptr_glGetProgramStageiv getProgramStageiv;
	ptr_glPatchParameteri patchParameteri;
	ptr_glPatchParameterfv patchParameterfv;
	ptr_glBindTransformFeedback bindTransformFeedback;
	ptr_glDeleteTransformFeedbacks deleteTransformFeedbacks;
	ptr_glGenTransformFeedbacks genTransformFeedbacks;
	ptr_glIsTransformFeedback isTransformFeedback;
	ptr_glPauseTransformFeedback pauseTransformFeedback;
	ptr_glResumeTransformFeedback resumeTransformFeedback;
	ptr_glDrawTransformFeedback drawTransformFeedback;
	ptr_glDrawTransformFeedbackStream drawTransformFeedbackStream;
	ptr_glBeginQueryIndexed beginQueryIndexed;
	ptr_glEndQueryIndexed endQueryIndexed;
	ptr_glGetQueryIndexediv getQueryIndexediv;
	ptr_glReleaseShaderCompiler releaseShaderCompiler;
	ptr_glShaderBinary shaderBinary;
	ptr_glGetShaderPrecisionFormat getShaderPrecisionFormat;
	ptr_glDepthRangef depthRangef;
	ptr_glClearDepthf clearDepthf;
	ptr_glGetProgramBinary getProgramBinary;
	ptr_glProgramBinary programBinary;
	ptr_glProgramParameteri programParameteri;
	ptr_glUseProgramStages useProgramStages;
	ptr_glActiveShaderProgram activeShaderProgram;
	ptr_glCreateShaderProgramv createShaderProgramv;
	ptr_glBindProgramPipeline bindProgramPipeline;
	ptr_glDeleteProgramPipelines deleteProgramPipelines;
	ptr_glGenProgramPipelines genProgramPipelines;
	ptr_glIsProgramPipeline isProgramPipeline;
	ptr_glGetProgramPipelineiv getProgramPipelineiv;
	ptr_glProgramUniform1i programUniform1i;
	ptr_glProgramUniform1iv programUniform1iv;
	ptr_glProgramUniform1f programUniform1f;
	ptr_glProgramUniform1fv programUniform1fv;
	ptr_glProgramUniform1d programUniform1d;
	ptr_glProgramUniform1dv programUniform1dv;
	ptr_glProgramUniform1ui programUniform1ui;
	ptr_glProgramUniform1uiv programUniform1uiv;
	ptr_glProgramUniform2i programUniform2i;
	ptr_glProgramUniform2iv programUniform2iv;
	ptr_glProgramUniform2f programUniform2f;
	ptr_glProgramUniform2fv programUniform2fv;
	ptr_glProgramUniform2d programUniform2d;
	ptr_glProgramUniform2dv programUniform2dv;
	ptr_glProgramUniform2ui programUniform2ui;
	ptr_glProgramUniform2uiv programUniform2uiv;
	ptr_glProgramUniform3i programUniform3i;
	ptr_glProgramUniform3iv programUniform3iv;
	ptr_glProgramUniform3f programUniform3f;
	ptr_glProgramUniform3fv programUniform3fv;
	ptr_glProgramUniform3d programUniform3d;
	ptr_glProgramUniform3dv programUniform3dv;
	ptr_glProgramUniform3ui programUniform3ui;
	ptr_glProgramUniform3uiv programUniform3uiv;
	ptr_glProgramUniform4i programUniform4i;
	ptr_glProgramUniform4iv programUniform4iv;
	ptr_glProgramUniform4f programUniform4f;
	ptr_glProgramUniform4fv programUniform4fv;
	ptr_glProgramUniform4d programUniform4d;
	ptr_glProgramUniform4dv programUniform4dv;
	ptr_glProgramUniform4ui programUniform4ui;
	ptr_glProgramUniform4uiv programUniform4uiv;
	ptr_glProgramUniformMatrix2fv programUniformMatrix2fv;
	ptr_glProgramUniformMatrix3fv programUniformMatrix3fv;
	ptr_glProgramUniformMatrix4fv programUniformMatrix4fv;
	ptr_glProgramUniformMatrix2dv programUniformMatrix2dv;
	ptr_glProgramUniformMatrix3dv programUniformMatrix3dv;
	ptr_glProgramUniformMatrix4dv programUniformMatrix4dv;
	ptr_glProgramUniformMatrix2x3fv programUniformMatrix2x3fv;
	ptr_glProgramUniformMatrix3x2fv programUniformMatrix3x2fv;
	ptr_glProgramUniformMatrix2x4fv programUniformMatrix2x4fv;
	ptr_glProgramUniformMatrix4x2fv programUniformMatrix4x2fv;
	ptr_glProgramUniformMatrix3x4fv programUniformMatrix3x4fv;
	ptr_glProgramUniformMatrix4x3fv programUniformMatrix4x3fv;
	ptr_glProgramUniformMatrix2x3dv programUniformMatrix2x3dv;
	ptr_glProgramUniformMatrix3x2dv programUniformMatrix3x2dv;
	ptr_glProgramUniformMatrix2x4dv programUniformMatrix2x4dv;
	ptr_glProgramUniformMatrix4x2dv programUniformMatrix4x2dv;
	ptr_glProgramUniformMatrix3x4dv programUniformMatrix3x4dv;
	ptr_glProgramUniformMatrix4x3dv programUniformMatrix4x3dv;
	ptr_glValidateProgramPipeline validateProgramPipeline;
	ptr_glGetProgramPipelineInfoLog getProgramPipelineInfoLog;
	ptr_glVertexAttribL1d vertexAttribL1d;
	ptr_glVertexAttribL2d vertexAttribL2d;
	ptr_glVertexAttribL3d vertexAttribL3d;
	ptr_glVertexAttribL4d vertexAttribL4d;
	ptr_glVertexAttribL1dv vertexAttribL1dv;
	ptr_glVertexAttribL2dv vertexAttribL2dv;
	ptr_glVertexAttribL3dv vertexAttribL3dv;
	ptr_glVertexAttribL4dv vertexAttribL4dv;
	ptr_glVertexAttribLPointer vertexAttribLPointer;
	ptr_glGetVertexAttribLdv getVertexAttribLdv;
	ptr_glViewportArrayv viewportArrayv;
	ptr_glViewportIndexedf viewportIndexedf;
	ptr_glViewportIndexedfv viewportIndexedfv;
	ptr_glScissorArrayv scissorArrayv;
	ptr_glScissorIndexed scissorIndexed;
	ptr_glScissorIndexedv scissorIndexedv;
	ptr_glDepthRangeArrayv depthRangeArrayv;
	ptr_glDepthRangeIndexed depthRangeIndexed;
	ptr_glGetFloati_v getFloati_v;
	ptr_glGetDoublei_v getDoublei_v;
	ptr_glCreateSyncFromCLeventARB createSyncFromCLeventARB;
	ptr_glDebugMessageControlARB debugMessageControlARB;
	ptr_glDebugMessageInsertARB debugMessageInsertARB;
	ptr_glDebugMessageCallbackARB debugMessageCallbackARB;
	ptr_glGetDebugMessageLogARB getDebugMessageLogARB;
	ptr_glGetGraphicsResetStatusARB getGraphicsResetStatusARB;
	ptr_glGetnMapdvARB getnMapdvARB;
	ptr_glGetnMapfvARB getnMapfvARB;
	ptr_glGetnMapivARB getnMapivARB;
	ptr_glGetnPixelMapfvARB getnPixelMapfvARB;
	ptr_glGetnPixelMapuivARB getnPixelMapuivARB;
	ptr_glGetnPixelMapusvARB getnPixelMapusvARB;
	ptr_glGetnPolygonStippleARB getnPolygonStippleARB;
	ptr_glGetnColorTableARB getnColorTableARB;
	ptr_glGetnConvolutionFilterARB getnConvolutionFilterARB;
	ptr_glGetnSeparableFilterARB getnSeparableFilterARB;
	ptr_glGetnHistogramARB getnHistogramARB;
	ptr_glGetnMinmaxARB getnMinmaxARB;
	ptr_glGetnTexImageARB getnTexImageARB;
	ptr_glReadnPixelsARB readnPixelsARB;
	ptr_glGetnCompressedTexImageARB getnCompressedTexImageARB;
	ptr_glGetnUniformfvARB getnUniformfvARB;
	ptr_glGetnUniformivARB getnUniformivARB;
	ptr_glGetnUniformuivARB getnUniformuivARB;
	ptr_glGetnUniformdvARB getnUniformdvARB;
	ptr_glDrawArraysInstancedBaseInstance drawArraysInstancedBaseInstance;
	ptr_glDrawElementsInstancedBaseInstance drawElementsInstancedBaseInstance;
	ptr_glDrawElementsInstancedBaseVertexBaseInstance drawElementsInstancedBaseVertexBaseInstance;
	ptr_glDrawTransformFeedbackInstanced drawTransformFeedbackInstanced;
	ptr_glDrawTransformFeedbackStreamInstanced drawTransformFeedbackStreamInstanced;
	ptr_glGetInternalformativ getInternalformativ;
	ptr_glGetActiveAtomicCounterBufferiv getActiveAtomicCounterBufferiv;
	ptr_glBindImageTexture bindImageTexture;
	ptr_glMemoryBarrier memoryBarrier;
	ptr_glTexStorage1D texStorage1D;
	ptr_glTexStorage2D texStorage2D;
	ptr_glTexStorage3D texStorage3D;
	ptr_glTextureStorage1DEXT textureStorage1DEXT;
	ptr_glTextureStorage2DEXT textureStorage2DEXT;
	ptr_glTextureStorage3DEXT textureStorage3DEXT;
	ptr_glDebugMessageControl debugMessageControl;
	ptr_glDebugMessageInsert debugMessageInsert;
	ptr_glDebugMessageCallback debugMessageCallback;
	ptr_glGetDebugMessageLog getDebugMessageLog;
	ptr_glPushDebugGroup pushDebugGroup;
	ptr_glPopDebugGroup popDebugGroup;
	ptr_glObjectLabel objectLabel;
	ptr_glGetObjectLabel getObjectLabel;
	ptr_glObjectPtrLabel objectPtrLabel;
	ptr_glGetObjectPtrLabel getObjectPtrLabel;
	ptr_glClearBufferData clearBufferData;
	ptr_glClearBufferSubData clearBufferSubData;
	ptr_glClearNamedBufferDataEXT clearNamedBufferDataEXT;
	ptr_glClearNamedBufferSubDataEXT clearNamedBufferSubDataEXT;
	ptr_glDispatchCompute dispatchCompute;
	ptr_glDispatchComputeIndirect dispatchComputeIndirect;
	ptr_glCopyImageSubData copyImageSubData;
	ptr_glFramebufferParameteri framebufferParameteri;
	ptr_glGetFramebufferParameteriv getFramebufferParameteriv;
	ptr_glNamedFramebufferParameteriEXT namedFramebufferParameteriEXT;
	ptr_glGetNamedFramebufferParameterivEXT getNamedFramebufferParameterivEXT;
	ptr_glGetInternalformati64v getInternalformati64v;
	ptr_glInvalidateTexSubImage invalidateTexSubImage;
	ptr_glInvalidateTexImage invalidateTexImage;
	ptr_glInvalidateBufferSubData invalidateBufferSubData;
	ptr_glInvalidateBufferData invalidateBufferData;
	ptr_glInvalidateFramebuffer invalidateFramebuffer;
	ptr_glInvalidateSubFramebuffer invalidateSubFramebuffer;
	ptr_glMultiDrawArraysIndirect multiDrawArraysIndirect;
	ptr_glMultiDrawElementsIndirect multiDrawElementsIndirect;
	ptr_glGetProgramInterfaceiv getProgramInterfaceiv;
	ptr_glGetProgramResourceIndex getProgramResourceIndex;
	ptr_glGetProgramResourceName getProgramResourceName;
	ptr_glGetProgramResourceiv getProgramResourceiv;
	ptr_glGetProgramResourceLocation getProgramResourceLocation;
	ptr_glGetProgramResourceLocationIndex getProgramResourceLocationIndex;
	ptr_glShaderStorageBlockBinding shaderStorageBlockBinding;
	ptr_glTexBufferRange texBufferRange;
	ptr_glTextureBufferRangeEXT textureBufferRangeEXT;
	ptr_glTexStorage2DMultisample texStorage2DMultisample;
	ptr_glTexStorage3DMultisample texStorage3DMultisample;
	ptr_glTextureStorage2DMultisampleEXT textureStorage2DMultisampleEXT;
	ptr_glTextureStorage3DMultisampleEXT textureStorage3DMultisampleEXT;
	ptr_glTextureView textureView;
	ptr_glBindVertexBuffer bindVertexBuffer;
	ptr_glVertexAttribFormat vertexAttribFormat;
	ptr_glVertexAttribIFormat vertexAttribIFormat;
	ptr_glVertexAttribLFormat vertexAttribLFormat;
	ptr_glVertexAttribBinding vertexAttribBinding;
	ptr_glVertexBindingDivisor vertexBindingDivisor;
	ptr_glVertexArrayBindVertexBufferEXT vertexArrayBindVertexBufferEXT;
	ptr_glVertexArrayVertexAttribFormatEXT vertexArrayVertexAttribFormatEXT;
	ptr_glVertexArrayVertexAttribIFormatEXT vertexArrayVertexAttribIFormatEXT;
	ptr_glVertexArrayVertexAttribLFormatEXT vertexArrayVertexAttribLFormatEXT;
	ptr_glVertexArrayVertexAttribBindingEXT vertexArrayVertexAttribBindingEXT;
	ptr_glVertexArrayVertexBindingDivisorEXT vertexArrayVertexBindingDivisorEXT;

	// For Legacy OpenGL (For GL1 target)
	ptr_glClientActiveTextureARB clientActiveTexture;
#ifdef WIN32
	ptr_wglCreatePbufferARB wglCreatePbufferARB;
	ptr_wglGetPbufferDCARB wglGetPbufferDCARB;
	ptr_wglReleasePbufferDCARB wglReleasePbufferDCARB;
	ptr_wglDestroyPbufferARB wglDestroyPbufferARB;
	ptr_wglQueryPbufferARB wglQueryPbufferARB;
#endif


/// \}
};

#define glCullFace OpenGL::functions->cullFace
#define glFrontFace OpenGL::functions->frontFace
#define glHint OpenGL::functions->hint
#define glLineWidth OpenGL::functions->lineWidth
#define glPointSize OpenGL::functions->pointSize
#define glPolygonMode OpenGL::functions->polygonMode
#define glScissor OpenGL::functions->scissor
#define glTexParameterf OpenGL::functions->texParameterf
#define glTexParameterfv OpenGL::functions->texParameterfv
#define glTexParameteri OpenGL::functions->texParameteri
#define glTexParameteriv OpenGL::functions->texParameteriv
#define glTexImage1D OpenGL::functions->texImage1D
#define glTexImage2D OpenGL::functions->texImage2D
#define glDrawBuffer OpenGL::functions->drawBuffer
#define glClear OpenGL::functions->clear
#define glClearColor OpenGL::functions->clearColor
#define glClearStencil OpenGL::functions->clearStencil
#define glClearDepth OpenGL::functions->clearDepth
#define glStencilMask OpenGL::functions->stencilMask
#define glColorMask OpenGL::functions->colorMask
#define glDepthMask OpenGL::functions->depthMask
#define glDisable OpenGL::functions->disable
#define glEnable OpenGL::functions->enable
#define glFinish OpenGL::functions->finish
#define glFlush OpenGL::functions->flush
#define glBlendFunc OpenGL::functions->blendFunc
#define glLogicOp OpenGL::functions->logicOp
#define glStencilFunc OpenGL::functions->stencilFunc
#define glStencilOp OpenGL::functions->stencilOp
#define glDepthFunc OpenGL::functions->depthFunc
#define glPixelStoref OpenGL::functions->pixelStoref
#define glPixelStorei OpenGL::functions->pixelStorei
#define glReadBuffer OpenGL::functions->readBuffer
#define glReadPixels OpenGL::functions->readPixels
#define glGetBooleanv OpenGL::functions->getBooleanv
#define glGetDoublev OpenGL::functions->getDoublev
#define glGetError OpenGL::functions->getError
#define glGetFloatv OpenGL::functions->getFloatv
#define glGetIntegerv OpenGL::functions->getIntegerv
#define glGetString OpenGL::functions->getString
#define glGetTexImage OpenGL::functions->getTexImage
#define glGetTexParameterfv OpenGL::functions->getTexParameterfv
#define glGetTexParameteriv OpenGL::functions->getTexParameteriv
#define glGetTexLevelParameterfv OpenGL::functions->getTexLevelParameterfv
#define glGetTexLevelParameteriv OpenGL::functions->getTexLevelParameteriv
#define glIsEnabled OpenGL::functions->isEnabled
#define glDepthRange OpenGL::functions->depthRange
#define glViewport OpenGL::functions->viewport
#define glDrawArrays OpenGL::functions->drawArrays
#define glDrawElements OpenGL::functions->drawElements
#define glGetPointerv OpenGL::functions->getPointerv
#define glPolygonOffset OpenGL::functions->polygonOffset
#define glCopyTexImage1D OpenGL::functions->copyTexImage1D
#define glCopyTexImage2D OpenGL::functions->copyTexImage2D
#define glCopyTexSubImage1D OpenGL::functions->copyTexSubImage1D
#define glCopyTexSubImage2D OpenGL::functions->copyTexSubImage2D
#define glTexSubImage1D OpenGL::functions->texSubImage1D
#define glTexSubImage2D OpenGL::functions->texSubImage2D
#define glBindTexture OpenGL::functions->bindTexture
#define glDeleteTextures OpenGL::functions->deleteTextures
#define glGenTextures OpenGL::functions->genTextures
#define glIsTexture OpenGL::functions->isTexture
#define glBlendColor OpenGL::functions->blendColor
#define glBlendEquation OpenGL::functions->blendEquation
#define glDrawRangeElements OpenGL::functions->drawRangeElements
#define glTexImage3D OpenGL::functions->texImage3D
#define glTexSubImage3D OpenGL::functions->texSubImage3D
#define glCopyTexSubImage3D OpenGL::functions->copyTexSubImage3D
#define glActiveTexture OpenGL::functions->activeTexture
#define glSampleCoverage OpenGL::functions->sampleCoverage
#define glCompressedTexImage3D OpenGL::functions->compressedTexImage3D
#define glCompressedTexImage2D OpenGL::functions->compressedTexImage2D
#define glCompressedTexImage1D OpenGL::functions->compressedTexImage1D
#define glCompressedTexSubImage3D OpenGL::functions->compressedTexSubImage3D
#define glCompressedTexSubImage2D OpenGL::functions->compressedTexSubImage2D
#define glCompressedTexSubImage1D OpenGL::functions->compressedTexSubImage1D
#define glGetCompressedTexImage OpenGL::functions->getCompressedTexImage
#define glBlendFuncSeparate OpenGL::functions->blendFuncSeparate
#define glMultiDrawArrays OpenGL::functions->multiDrawArrays
#define glMultiDrawElements OpenGL::functions->multiDrawElements
#define glPointParameterf OpenGL::functions->pointParameterf
#define glPointParameterfv OpenGL::functions->pointParameterfv
#define glPointParameteri OpenGL::functions->pointParameteri
#define glPointParameteriv OpenGL::functions->pointParameteriv
#define glGenQueries OpenGL::functions->genQueries
#define glDeleteQueries OpenGL::functions->deleteQueries
#define glIsQuery OpenGL::functions->isQuery
#define glBeginQuery OpenGL::functions->beginQuery
#define glEndQuery OpenGL::functions->endQuery
#define glGetQueryiv OpenGL::functions->getQueryiv
#define glGetQueryObjectiv OpenGL::functions->getQueryObjectiv
#define glGetQueryObjectuiv OpenGL::functions->getQueryObjectuiv
#define glBindBuffer OpenGL::functions->bindBuffer
#define glDeleteBuffers OpenGL::functions->deleteBuffers
#define glGenBuffers OpenGL::functions->genBuffers
#define glIsBuffer OpenGL::functions->isBuffer
#define glBufferData OpenGL::functions->bufferData
#define glBufferSubData OpenGL::functions->bufferSubData
#define glGetBufferSubData OpenGL::functions->getBufferSubData
#define glMapBuffer OpenGL::functions->mapBuffer
#define glUnmapBuffer OpenGL::functions->unmapBuffer
#define glGetBufferParameteriv OpenGL::functions->getBufferParameteriv
#define glGetBufferPointerv OpenGL::functions->getBufferPointerv
#define glBlendEquationSeparate OpenGL::functions->blendEquationSeparate
#define glDrawBuffers OpenGL::functions->drawBuffers
#define glStencilOpSeparate OpenGL::functions->stencilOpSeparate
#define glStencilFuncSeparate OpenGL::functions->stencilFuncSeparate
#define glStencilMaskSeparate OpenGL::functions->stencilMaskSeparate
#define glAttachShader OpenGL::functions->attachShader
#define glBindAttribLocation OpenGL::functions->bindAttribLocation
#define glCompileShader OpenGL::functions->compileShader
#define glCreateProgram OpenGL::functions->createProgram
#define glCreateShader OpenGL::functions->createShader
#define glDeleteProgram OpenGL::functions->deleteProgram
#define glDeleteShader OpenGL::functions->deleteShader
#define glDetachShader OpenGL::functions->detachShader
#define glDisableVertexAttribArray OpenGL::functions->disableVertexAttribArray
#define glEnableVertexAttribArray OpenGL::functions->enableVertexAttribArray
#define glGetActiveAttrib OpenGL::functions->getActiveAttrib
#define glGetActiveUniform OpenGL::functions->getActiveUniform
#define glGetAttachedShaders OpenGL::functions->getAttachedShaders
#define glGetAttribLocation OpenGL::functions->getAttribLocation
#define glGetProgramiv OpenGL::functions->getProgramiv
#define glGetProgramInfoLog OpenGL::functions->getProgramInfoLog
#define glGetShaderiv OpenGL::functions->getShaderiv
#define glGetShaderInfoLog OpenGL::functions->getShaderInfoLog
#define glGetShaderSource OpenGL::functions->getShaderSource
#define glGetUniformLocation OpenGL::functions->getUniformLocation
#define glGetUniformfv OpenGL::functions->getUniformfv
#define glGetUniformiv OpenGL::functions->getUniformiv
#define glGetVertexAttribdv OpenGL::functions->getVertexAttribdv
#define glGetVertexAttribfv OpenGL::functions->getVertexAttribfv
#define glGetVertexAttribiv OpenGL::functions->getVertexAttribiv
#define glGetVertexAttribPointerv OpenGL::functions->getVertexAttribPointerv
#define glIsProgram OpenGL::functions->isProgram
#define glIsShader OpenGL::functions->isShader
#define glLinkProgram OpenGL::functions->linkProgram
#define glShaderSource OpenGL::functions->shaderSource
#define glUseProgram OpenGL::functions->useProgram
#define glUniform1f OpenGL::functions->uniform1f
#define glUniform2f OpenGL::functions->uniform2f
#define glUniform3f OpenGL::functions->uniform3f
#define glUniform4f OpenGL::functions->uniform4f
#define glUniform1i OpenGL::functions->uniform1i
#define glUniform2i OpenGL::functions->uniform2i
#define glUniform3i OpenGL::functions->uniform3i
#define glUniform4i OpenGL::functions->uniform4i
#define glUniform1fv OpenGL::functions->uniform1fv
#define glUniform2fv OpenGL::functions->uniform2fv
#define glUniform3fv OpenGL::functions->uniform3fv
#define glUniform4fv OpenGL::functions->uniform4fv
#define glUniform1iv OpenGL::functions->uniform1iv
#define glUniform2iv OpenGL::functions->uniform2iv
#define glUniform3iv OpenGL::functions->uniform3iv
#define glUniform4iv OpenGL::functions->uniform4iv
#define glUniformMatrix2fv OpenGL::functions->uniformMatrix2fv
#define glUniformMatrix3fv OpenGL::functions->uniformMatrix3fv
#define glUniformMatrix4fv OpenGL::functions->uniformMatrix4fv
#define glValidateProgram OpenGL::functions->validateProgram
#define glVertexAttrib1d OpenGL::functions->vertexAttrib1d
#define glVertexAttrib1dv OpenGL::functions->vertexAttrib1dv
#define glVertexAttrib1f OpenGL::functions->vertexAttrib1f
#define glVertexAttrib1fv OpenGL::functions->vertexAttrib1fv
#define glVertexAttrib1s OpenGL::functions->vertexAttrib1s
#define glVertexAttrib1sv OpenGL::functions->vertexAttrib1sv
#define glVertexAttrib2d OpenGL::functions->vertexAttrib2d
#define glVertexAttrib2dv OpenGL::functions->vertexAttrib2dv
#define glVertexAttrib2f OpenGL::functions->vertexAttrib2f
#define glVertexAttrib2fv OpenGL::functions->vertexAttrib2fv
#define glVertexAttrib2s OpenGL::functions->vertexAttrib2s
#define glVertexAttrib2sv OpenGL::functions->vertexAttrib2sv
#define glVertexAttrib3d OpenGL::functions->vertexAttrib3d
#define glVertexAttrib3dv OpenGL::functions->vertexAttrib3dv
#define glVertexAttrib3f OpenGL::functions->vertexAttrib3f
#define glVertexAttrib3fv OpenGL::functions->vertexAttrib3fv
#define glVertexAttrib3s OpenGL::functions->vertexAttrib3s
#define glVertexAttrib3sv OpenGL::functions->vertexAttrib3sv
#define glVertexAttrib4Nbv OpenGL::functions->vertexAttrib4Nbv
#define glVertexAttrib4Niv OpenGL::functions->vertexAttrib4Niv
#define glVertexAttrib4Nsv OpenGL::functions->vertexAttrib4Nsv
#define glVertexAttrib4Nub OpenGL::functions->vertexAttrib4Nub
#define glVertexAttrib4Nubv OpenGL::functions->vertexAttrib4Nubv
#define glVertexAttrib4Nuiv OpenGL::functions->vertexAttrib4Nuiv
#define glVertexAttrib4Nusv OpenGL::functions->vertexAttrib4Nusv
#define glVertexAttrib4bv OpenGL::functions->vertexAttrib4bv
#define glVertexAttrib4d OpenGL::functions->vertexAttrib4d
#define glVertexAttrib4dv OpenGL::functions->vertexAttrib4dv
#define glVertexAttrib4f OpenGL::functions->vertexAttrib4f
#define glVertexAttrib4fv OpenGL::functions->vertexAttrib4fv
#define glVertexAttrib4iv OpenGL::functions->vertexAttrib4iv
#define glVertexAttrib4s OpenGL::functions->vertexAttrib4s
#define glVertexAttrib4sv OpenGL::functions->vertexAttrib4sv
#define glVertexAttrib4ubv OpenGL::functions->vertexAttrib4ubv
#define glVertexAttrib4uiv OpenGL::functions->vertexAttrib4uiv
#define glVertexAttrib4usv OpenGL::functions->vertexAttrib4usv
#define glVertexAttribPointer OpenGL::functions->vertexAttribPointer
#define glUniformMatrix2x3fv OpenGL::functions->uniformMatrix2x3fv
#define glUniformMatrix3x2fv OpenGL::functions->uniformMatrix3x2fv
#define glUniformMatrix2x4fv OpenGL::functions->uniformMatrix2x4fv
#define glUniformMatrix4x2fv OpenGL::functions->uniformMatrix4x2fv
#define glUniformMatrix3x4fv OpenGL::functions->uniformMatrix3x4fv
#define glUniformMatrix4x3fv OpenGL::functions->uniformMatrix4x3fv
#define glColorMaski OpenGL::functions->colorMaski
#define glGetBooleani_v OpenGL::functions->getBooleani_v
#define glGetIntegeri_v OpenGL::functions->getIntegeri_v
#define glEnablei OpenGL::functions->enablei
#define glDisablei OpenGL::functions->disablei
#define glIsEnabledi OpenGL::functions->isEnabledi
#define glBeginTransformFeedback OpenGL::functions->beginTransformFeedback
#define glEndTransformFeedback OpenGL::functions->endTransformFeedback
#define glBindBufferRange OpenGL::functions->bindBufferRange
#define glBindBufferBase OpenGL::functions->bindBufferBase
#define glTransformFeedbackVaryings OpenGL::functions->transformFeedbackVaryings
#define glGetTransformFeedbackVarying OpenGL::functions->getTransformFeedbackVarying
#define glClampColor OpenGL::functions->clampColor
#define glBeginConditionalRender OpenGL::functions->beginConditionalRender
#define glEndConditionalRender OpenGL::functions->endConditionalRender
#define glVertexAttribIPointer OpenGL::functions->vertexAttribIPointer
#define glGetVertexAttribIiv OpenGL::functions->getVertexAttribIiv
#define glGetVertexAttribIuiv OpenGL::functions->getVertexAttribIuiv
#define glVertexAttribI1i OpenGL::functions->vertexAttribI1i
#define glVertexAttribI2i OpenGL::functions->vertexAttribI2i
#define glVertexAttribI3i OpenGL::functions->vertexAttribI3i
#define glVertexAttribI4i OpenGL::functions->vertexAttribI4i
#define glVertexAttribI1ui OpenGL::functions->vertexAttribI1ui
#define glVertexAttribI2ui OpenGL::functions->vertexAttribI2ui
#define glVertexAttribI3ui OpenGL::functions->vertexAttribI3ui
#define glVertexAttribI4ui OpenGL::functions->vertexAttribI4ui
#define glVertexAttribI1iv OpenGL::functions->vertexAttribI1iv
#define glVertexAttribI2iv OpenGL::functions->vertexAttribI2iv
#define glVertexAttribI3iv OpenGL::functions->vertexAttribI3iv
#define glVertexAttribI4iv OpenGL::functions->vertexAttribI4iv
#define glVertexAttribI1uiv OpenGL::functions->vertexAttribI1uiv
#define glVertexAttribI2uiv OpenGL::functions->vertexAttribI2uiv
#define glVertexAttribI3uiv OpenGL::functions->vertexAttribI3uiv
#define glVertexAttribI4uiv OpenGL::functions->vertexAttribI4uiv
#define glVertexAttribI4bv OpenGL::functions->vertexAttribI4bv
#define glVertexAttribI4sv OpenGL::functions->vertexAttribI4sv
#define glVertexAttribI4ubv OpenGL::functions->vertexAttribI4ubv
#define glVertexAttribI4usv OpenGL::functions->vertexAttribI4usv
#define glGetUniformuiv OpenGL::functions->getUniformuiv
#define glBindFragDataLocation OpenGL::functions->bindFragDataLocation
#define glGetFragDataLocation OpenGL::functions->getFragDataLocation
#define glUniform1ui OpenGL::functions->uniform1ui
#define glUniform2ui OpenGL::functions->uniform2ui
#define glUniform3ui OpenGL::functions->uniform3ui
#define glUniform4ui OpenGL::functions->uniform4ui
#define glUniform1uiv OpenGL::functions->uniform1uiv
#define glUniform2uiv OpenGL::functions->uniform2uiv
#define glUniform3uiv OpenGL::functions->uniform3uiv
#define glUniform4uiv OpenGL::functions->uniform4uiv
#define glTexParameterIiv OpenGL::functions->texParameterIiv
#define glTexParameterIuiv OpenGL::functions->texParameterIuiv
#define glGetTexParameterIiv OpenGL::functions->getTexParameterIiv
#define glGetTexParameterIuiv OpenGL::functions->getTexParameterIuiv
#define glClearBufferiv OpenGL::functions->clearBufferiv
#define glClearBufferuiv OpenGL::functions->clearBufferuiv
#define glClearBufferfv OpenGL::functions->clearBufferfv
#define glClearBufferfi OpenGL::functions->clearBufferfi
#define glGetStringi OpenGL::functions->getStringi
#define glDrawArraysInstanced OpenGL::functions->drawArraysInstanced
#define glDrawElementsInstanced OpenGL::functions->drawElementsInstanced
#define glTexBuffer OpenGL::functions->texBuffer
#define glPrimitiveRestartIndex OpenGL::functions->primitiveRestartIndex
#define glGetInteger64i_v OpenGL::functions->getInteger64i_v
#define glGetBufferParameteri64v OpenGL::functions->getBufferParameteri64v
#define glFramebufferTexture OpenGL::functions->framebufferTexture
#define glVertexAttribDivisor OpenGL::functions->vertexAttribDivisor
#define glMinSampleShading OpenGL::functions->minSampleShading
#define glBlendEquationi OpenGL::functions->blendEquationi
#define glBlendEquationSeparatei OpenGL::functions->blendEquationSeparatei
#define glBlendFunci OpenGL::functions->blendFunci
#define glBlendFuncSeparatei OpenGL::functions->blendFuncSeparatei
#define glIsRenderbuffer OpenGL::functions->isRenderbuffer
#define glBindRenderbuffer OpenGL::functions->bindRenderbuffer
#define glDeleteRenderbuffers OpenGL::functions->deleteRenderbuffers
#define glGenRenderbuffers OpenGL::functions->genRenderbuffers
#define glRenderbufferStorage OpenGL::functions->renderbufferStorage
#define glGetRenderbufferParameteriv OpenGL::functions->getRenderbufferParameteriv
#define glIsFramebuffer OpenGL::functions->isFramebuffer
#define glBindFramebuffer OpenGL::functions->bindFramebuffer
#define glDeleteFramebuffers OpenGL::functions->deleteFramebuffers
#define glGenFramebuffers OpenGL::functions->genFramebuffers
#define glCheckFramebufferStatus OpenGL::functions->checkFramebufferStatus
#define glFramebufferTexture1D OpenGL::functions->framebufferTexture1D
#define glFramebufferTexture2D OpenGL::functions->framebufferTexture2D
#define glFramebufferTexture3D OpenGL::functions->framebufferTexture3D
#define glFramebufferRenderbuffer OpenGL::functions->framebufferRenderbuffer
#define glGetFramebufferAttachmentParameteriv OpenGL::functions->getFramebufferAttachmentParameteriv
#define glGenerateMipmap OpenGL::functions->generateMipmap
#define glBlitFramebuffer OpenGL::functions->blitFramebuffer
#define glRenderbufferStorageMultisample OpenGL::functions->renderbufferStorageMultisample
#define glFramebufferTextureLayer OpenGL::functions->framebufferTextureLayer
#define glMapBufferRange OpenGL::functions->mapBufferRange
#define glFlushMappedBufferRange OpenGL::functions->flushMappedBufferRange
#define glBindVertexArray OpenGL::functions->bindVertexArray
#define glDeleteVertexArrays OpenGL::functions->deleteVertexArrays
#define glGenVertexArrays OpenGL::functions->genVertexArrays
#define glIsVertexArray OpenGL::functions->isVertexArray
#define glGetUniformIndices OpenGL::functions->getUniformIndices
#define glGetActiveUniformsiv OpenGL::functions->getActiveUniformsiv
#define glGetActiveUniformName OpenGL::functions->getActiveUniformName
#define glGetUniformBlockIndex OpenGL::functions->getUniformBlockIndex
#define glGetActiveUniformBlockiv OpenGL::functions->getActiveUniformBlockiv
#define glGetActiveUniformBlockName OpenGL::functions->getActiveUniformBlockName
#define glUniformBlockBinding OpenGL::functions->uniformBlockBinding
#define glCopyBufferSubData OpenGL::functions->copyBufferSubData
#define glDrawElementsBaseVertex OpenGL::functions->drawElementsBaseVertex
#define glDrawRangeElementsBaseVertex OpenGL::functions->drawRangeElementsBaseVertex
#define glDrawElementsInstancedBaseVertex OpenGL::functions->drawElementsInstancedBaseVertex
#define glMultiDrawElementsBaseVertex OpenGL::functions->multiDrawElementsBaseVertex
#define glProvokingVertex OpenGL::functions->provokingVertex
#define glFenceSync OpenGL::functions->fenceSync
#define glIsSync OpenGL::functions->isSync
#define glDeleteSync OpenGL::functions->deleteSync
#define glClientWaitSync OpenGL::functions->clientWaitSync
#define glWaitSync OpenGL::functions->waitSync
#define glGetInteger64v OpenGL::functions->getInteger64v
#define glGetSynciv OpenGL::functions->getSynciv
#define glTexImage2DMultisample OpenGL::functions->texImage2DMultisample
#define glTexImage3DMultisample OpenGL::functions->texImage3DMultisample
#define glGetMultisamplefv OpenGL::functions->getMultisamplefv
#define glSampleMaski OpenGL::functions->sampleMaski
#define glBlendEquationiARB OpenGL::functions->blendEquationiARB
#define glBlendEquationSeparateiARB OpenGL::functions->blendEquationSeparateiARB
#define glBlendFunciARB OpenGL::functions->blendFunciARB
#define glBlendFuncSeparateiARB OpenGL::functions->blendFuncSeparateiARB
#define glMinSampleShadingARB OpenGL::functions->minSampleShadingARB
#define glNamedStringARB OpenGL::functions->namedStringARB
#define glDeleteNamedStringARB OpenGL::functions->deleteNamedStringARB
#define glCompileShaderIncludeARB OpenGL::functions->compileShaderIncludeARB
#define glIsNamedStringARB OpenGL::functions->isNamedStringARB
#define glGetNamedStringARB OpenGL::functions->getNamedStringARB
#define glGetNamedStringivARB OpenGL::functions->getNamedStringivARB
#define glBindFragDataLocationIndexed OpenGL::functions->bindFragDataLocationIndexed
#define glGetFragDataIndex OpenGL::functions->getFragDataIndex
#define glGenSamplers OpenGL::functions->genSamplers
#define glDeleteSamplers OpenGL::functions->deleteSamplers
#define glIsSampler OpenGL::functions->isSampler
#define glBindSampler OpenGL::functions->bindSampler
#define glSamplerParameteri OpenGL::functions->samplerParameteri
#define glSamplerParameteriv OpenGL::functions->samplerParameteriv
#define glSamplerParameterf OpenGL::functions->samplerParameterf
#define glSamplerParameterfv OpenGL::functions->samplerParameterfv
#define glSamplerParameterIiv OpenGL::functions->samplerParameterIiv
#define glSamplerParameterIuiv OpenGL::functions->samplerParameterIuiv
#define glGetSamplerParameteriv OpenGL::functions->getSamplerParameteriv
#define glGetSamplerParameterIiv OpenGL::functions->getSamplerParameterIiv
#define glGetSamplerParameterfv OpenGL::functions->getSamplerParameterfv
#define glGetSamplerParameterIuiv OpenGL::functions->getSamplerParameterIuiv
#define glQueryCounter OpenGL::functions->queryCounter
#define glGetQueryObjecti64v OpenGL::functions->getQueryObjecti64v
#define glGetQueryObjectui64v OpenGL::functions->getQueryObjectui64v
#define glVertexP2ui OpenGL::functions->vertexP2ui
#define glVertexP2uiv OpenGL::functions->vertexP2uiv
#define glVertexP3ui OpenGL::functions->vertexP3ui
#define glVertexP3uiv OpenGL::functions->vertexP3uiv
#define glVertexP4ui OpenGL::functions->vertexP4ui
#define glVertexP4uiv OpenGL::functions->vertexP4uiv
#define glTexCoordP1ui OpenGL::functions->texCoordP1ui
#define glTexCoordP1uiv OpenGL::functions->texCoordP1uiv
#define glTexCoordP2ui OpenGL::functions->texCoordP2ui
#define glTexCoordP2uiv OpenGL::functions->texCoordP2uiv
#define glTexCoordP3ui OpenGL::functions->texCoordP3ui
#define glTexCoordP3uiv OpenGL::functions->texCoordP3uiv
#define glTexCoordP4ui OpenGL::functions->texCoordP4ui
#define glTexCoordP4uiv OpenGL::functions->texCoordP4uiv
#define glMultiTexCoordP1ui OpenGL::functions->multiTexCoordP1ui
#define glMultiTexCoordP1uiv OpenGL::functions->multiTexCoordP1uiv
#define glMultiTexCoordP2ui OpenGL::functions->multiTexCoordP2ui
#define glMultiTexCoordP2uiv OpenGL::functions->multiTexCoordP2uiv
#define glMultiTexCoordP3ui OpenGL::functions->multiTexCoordP3ui
#define glMultiTexCoordP3uiv OpenGL::functions->multiTexCoordP3uiv
#define glMultiTexCoordP4ui OpenGL::functions->multiTexCoordP4ui
#define glMultiTexCoordP4uiv OpenGL::functions->multiTexCoordP4uiv
#define glNormalP3ui OpenGL::functions->normalP3ui
#define glNormalP3uiv OpenGL::functions->normalP3uiv
#define glColorP3ui OpenGL::functions->colorP3ui
#define glColorP3uiv OpenGL::functions->colorP3uiv
#define glColorP4ui OpenGL::functions->colorP4ui
#define glColorP4uiv OpenGL::functions->colorP4uiv
#define glSecondaryColorP3ui OpenGL::functions->secondaryColorP3ui
#define glSecondaryColorP3uiv OpenGL::functions->secondaryColorP3uiv
#define glVertexAttribP1ui OpenGL::functions->vertexAttribP1ui
#define glVertexAttribP1uiv OpenGL::functions->vertexAttribP1uiv
#define glVertexAttribP2ui OpenGL::functions->vertexAttribP2ui
#define glVertexAttribP2uiv OpenGL::functions->vertexAttribP2uiv
#define glVertexAttribP3ui OpenGL::functions->vertexAttribP3ui
#define glVertexAttribP3uiv OpenGL::functions->vertexAttribP3uiv
#define glVertexAttribP4ui OpenGL::functions->vertexAttribP4ui
#define glVertexAttribP4uiv OpenGL::functions->vertexAttribP4uiv
#define glDrawArraysIndirect OpenGL::functions->drawArraysIndirect
#define glDrawElementsIndirect OpenGL::functions->drawElementsIndirect
#define glUniform1d OpenGL::functions->uniform1d
#define glUniform2d OpenGL::functions->uniform2d
#define glUniform3d OpenGL::functions->uniform3d
#define glUniform4d OpenGL::functions->uniform4d
#define glUniform1dv OpenGL::functions->uniform1dv
#define glUniform2dv OpenGL::functions->uniform2dv
#define glUniform3dv OpenGL::functions->uniform3dv
#define glUniform4dv OpenGL::functions->uniform4dv
#define glUniformMatrix2dv OpenGL::functions->uniformMatrix2dv
#define glUniformMatrix3dv OpenGL::functions->uniformMatrix3dv
#define glUniformMatrix4dv OpenGL::functions->uniformMatrix4dv
#define glUniformMatrix2x3dv OpenGL::functions->uniformMatrix2x3dv
#define glUniformMatrix2x4dv OpenGL::functions->uniformMatrix2x4dv
#define glUniformMatrix3x2dv OpenGL::functions->uniformMatrix3x2dv
#define glUniformMatrix3x4dv OpenGL::functions->uniformMatrix3x4dv
#define glUniformMatrix4x2dv OpenGL::functions->uniformMatrix4x2dv
#define glUniformMatrix4x3dv OpenGL::functions->uniformMatrix4x3dv
#define glGetUniformdv OpenGL::functions->getUniformdv
#define glGetSubroutineUniformLocation OpenGL::functions->getSubroutineUniformLocation
#define glGetSubroutineIndex OpenGL::functions->getSubroutineIndex
#define glGetActiveSubroutineUniformiv OpenGL::functions->getActiveSubroutineUniformiv
#define glGetActiveSubroutineUniformName OpenGL::functions->getActiveSubroutineUniformName
#define glGetActiveSubroutineName OpenGL::functions->getActiveSubroutineName
#define glUniformSubroutinesuiv OpenGL::functions->uniformSubroutinesuiv
#define glGetUniformSubroutineuiv OpenGL::functions->getUniformSubroutineuiv
#define glGetProgramStageiv OpenGL::functions->getProgramStageiv
#define glPatchParameteri OpenGL::functions->patchParameteri
#define glPatchParameterfv OpenGL::functions->patchParameterfv
#define glBindTransformFeedback OpenGL::functions->bindTransformFeedback
#define glDeleteTransformFeedbacks OpenGL::functions->deleteTransformFeedbacks
#define glGenTransformFeedbacks OpenGL::functions->genTransformFeedbacks
#define glIsTransformFeedback OpenGL::functions->isTransformFeedback
#define glPauseTransformFeedback OpenGL::functions->pauseTransformFeedback
#define glResumeTransformFeedback OpenGL::functions->resumeTransformFeedback
#define glDrawTransformFeedback OpenGL::functions->drawTransformFeedback
#define glDrawTransformFeedbackStream OpenGL::functions->drawTransformFeedbackStream
#define glBeginQueryIndexed OpenGL::functions->beginQueryIndexed
#define glEndQueryIndexed OpenGL::functions->endQueryIndexed
#define glGetQueryIndexediv OpenGL::functions->getQueryIndexediv
#define glReleaseShaderCompiler OpenGL::functions->releaseShaderCompiler
#define glShaderBinary OpenGL::functions->shaderBinary
#define glGetShaderPrecisionFormat OpenGL::functions->getShaderPrecisionFormat
#define glDepthRangef OpenGL::functions->depthRangef
#define glClearDepthf OpenGL::functions->clearDepthf
#define glGetProgramBinary OpenGL::functions->getProgramBinary
#define glProgramBinary OpenGL::functions->programBinary
#define glProgramParameteri OpenGL::functions->programParameteri
#define glUseProgramStages OpenGL::functions->useProgramStages
#define glActiveShaderProgram OpenGL::functions->activeShaderProgram
#define glCreateShaderProgramv OpenGL::functions->createShaderProgramv
#define glBindProgramPipeline OpenGL::functions->bindProgramPipeline
#define glDeleteProgramPipelines OpenGL::functions->deleteProgramPipelines
#define glGenProgramPipelines OpenGL::functions->genProgramPipelines
#define glIsProgramPipeline OpenGL::functions->isProgramPipeline
#define glGetProgramPipelineiv OpenGL::functions->getProgramPipelineiv
#define glProgramUniform1i OpenGL::functions->programUniform1i
#define glProgramUniform1iv OpenGL::functions->programUniform1iv
#define glProgramUniform1f OpenGL::functions->programUniform1f
#define glProgramUniform1fv OpenGL::functions->programUniform1fv
#define glProgramUniform1d OpenGL::functions->programUniform1d
#define glProgramUniform1dv OpenGL::functions->programUniform1dv
#define glProgramUniform1ui OpenGL::functions->programUniform1ui
#define glProgramUniform1uiv OpenGL::functions->programUniform1uiv
#define glProgramUniform2i OpenGL::functions->programUniform2i
#define glProgramUniform2iv OpenGL::functions->programUniform2iv
#define glProgramUniform2f OpenGL::functions->programUniform2f
#define glProgramUniform2fv OpenGL::functions->programUniform2fv
#define glProgramUniform2d OpenGL::functions->programUniform2d
#define glProgramUniform2dv OpenGL::functions->programUniform2dv
#define glProgramUniform2ui OpenGL::functions->programUniform2ui
#define glProgramUniform2uiv OpenGL::functions->programUniform2uiv
#define glProgramUniform3i OpenGL::functions->programUniform3i
#define glProgramUniform3iv OpenGL::functions->programUniform3iv
#define glProgramUniform3f OpenGL::functions->programUniform3f
#define glProgramUniform3fv OpenGL::functions->programUniform3fv
#define glProgramUniform3d OpenGL::functions->programUniform3d
#define glProgramUniform3dv OpenGL::functions->programUniform3dv
#define glProgramUniform3ui OpenGL::functions->programUniform3ui
#define glProgramUniform3uiv OpenGL::functions->programUniform3uiv
#define glProgramUniform4i OpenGL::functions->programUniform4i
#define glProgramUniform4iv OpenGL::functions->programUniform4iv
#define glProgramUniform4f OpenGL::functions->programUniform4f
#define glProgramUniform4fv OpenGL::functions->programUniform4fv
#define glProgramUniform4d OpenGL::functions->programUniform4d
#define glProgramUniform4dv OpenGL::functions->programUniform4dv
#define glProgramUniform4ui OpenGL::functions->programUniform4ui
#define glProgramUniform4uiv OpenGL::functions->programUniform4uiv
#define glProgramUniformMatrix2fv OpenGL::functions->programUniformMatrix2fv
#define glProgramUniformMatrix3fv OpenGL::functions->programUniformMatrix3fv
#define glProgramUniformMatrix4fv OpenGL::functions->programUniformMatrix4fv
#define glProgramUniformMatrix2dv OpenGL::functions->programUniformMatrix2dv
#define glProgramUniformMatrix3dv OpenGL::functions->programUniformMatrix3dv
#define glProgramUniformMatrix4dv OpenGL::functions->programUniformMatrix4dv
#define glProgramUniformMatrix2x3fv OpenGL::functions->programUniformMatrix2x3fv
#define glProgramUniformMatrix3x2fv OpenGL::functions->programUniformMatrix3x2fv
#define glProgramUniformMatrix2x4fv OpenGL::functions->programUniformMatrix2x4fv
#define glProgramUniformMatrix4x2fv OpenGL::functions->programUniformMatrix4x2fv
#define glProgramUniformMatrix3x4fv OpenGL::functions->programUniformMatrix3x4fv
#define glProgramUniformMatrix4x3fv OpenGL::functions->programUniformMatrix4x3fv
#define glProgramUniformMatrix2x3dv OpenGL::functions->programUniformMatrix2x3dv
#define glProgramUniformMatrix3x2dv OpenGL::functions->programUniformMatrix3x2dv
#define glProgramUniformMatrix2x4dv OpenGL::functions->programUniformMatrix2x4dv
#define glProgramUniformMatrix4x2dv OpenGL::functions->programUniformMatrix4x2dv
#define glProgramUniformMatrix3x4dv OpenGL::functions->programUniformMatrix3x4dv
#define glProgramUniformMatrix4x3dv OpenGL::functions->programUniformMatrix4x3dv
#define glValidateProgramPipeline OpenGL::functions->validateProgramPipeline
#define glGetProgramPipelineInfoLog OpenGL::functions->getProgramPipelineInfoLog
#define glVertexAttribL1d OpenGL::functions->vertexAttribL1d
#define glVertexAttribL2d OpenGL::functions->vertexAttribL2d
#define glVertexAttribL3d OpenGL::functions->vertexAttribL3d
#define glVertexAttribL4d OpenGL::functions->vertexAttribL4d
#define glVertexAttribL1dv OpenGL::functions->vertexAttribL1dv
#define glVertexAttribL2dv OpenGL::functions->vertexAttribL2dv
#define glVertexAttribL3dv OpenGL::functions->vertexAttribL3dv
#define glVertexAttribL4dv OpenGL::functions->vertexAttribL4dv
#define glVertexAttribLPointer OpenGL::functions->vertexAttribLPointer
#define glGetVertexAttribLdv OpenGL::functions->getVertexAttribLdv
#define glViewportArrayv OpenGL::functions->viewportArrayv
#define glViewportIndexedf OpenGL::functions->viewportIndexedf
#define glViewportIndexedfv OpenGL::functions->viewportIndexedfv
#define glScissorArrayv OpenGL::functions->scissorArrayv
#define glScissorIndexed OpenGL::functions->scissorIndexed
#define glScissorIndexedv OpenGL::functions->scissorIndexedv
#define glDepthRangeArrayv OpenGL::functions->depthRangeArrayv
#define glDepthRangeIndexed OpenGL::functions->depthRangeIndexed
#define glGetFloati_v OpenGL::functions->getFloati_v
#define glGetDoublei_v OpenGL::functions->getDoublei_v
#define glCreateSyncFromCLeventARB OpenGL::functions->createSyncFromCLeventARB
#define glDebugMessageControlARB OpenGL::functions->debugMessageControlARB
#define glDebugMessageInsertARB OpenGL::functions->debugMessageInsertARB
#define glDebugMessageCallbackARB OpenGL::functions->debugMessageCallbackARB
#define glGetDebugMessageLogARB OpenGL::functions->getDebugMessageLogARB
#define glGetGraphicsResetStatusARB OpenGL::functions->getGraphicsResetStatusARB
#define glGetnMapdvARB OpenGL::functions->getnMapdvARB
#define glGetnMapfvARB OpenGL::functions->getnMapfvARB
#define glGetnMapivARB OpenGL::functions->getnMapivARB
#define glGetnPixelMapfvARB OpenGL::functions->getnPixelMapfvARB
#define glGetnPixelMapuivARB OpenGL::functions->getnPixelMapuivARB
#define glGetnPixelMapusvARB OpenGL::functions->getnPixelMapusvARB
#define glGetnPolygonStippleARB OpenGL::functions->getnPolygonStippleARB
#define glGetnColorTableARB OpenGL::functions->getnColorTableARB
#define glGetnConvolutionFilterARB OpenGL::functions->getnConvolutionFilterARB
#define glGetnSeparableFilterARB OpenGL::functions->getnSeparableFilterARB
#define glGetnHistogramARB OpenGL::functions->getnHistogramARB
#define glGetnMinmaxARB OpenGL::functions->getnMinmaxARB
#define glGetnTexImageARB OpenGL::functions->getnTexImageARB
#define glReadnPixelsARB OpenGL::functions->readnPixelsARB
#define glGetnCompressedTexImageARB OpenGL::functions->getnCompressedTexImageARB
#define glGetnUniformfvARB OpenGL::functions->getnUniformfvARB
#define glGetnUniformivARB OpenGL::functions->getnUniformivARB
#define glGetnUniformuivARB OpenGL::functions->getnUniformuivARB
#define glGetnUniformdvARB OpenGL::functions->getnUniformdvARB
#define glDrawArraysInstancedBaseInstance OpenGL::functions->drawArraysInstancedBaseInstance
#define glDrawElementsInstancedBaseInstance OpenGL::functions->drawElementsInstancedBaseInstance
#define glDrawElementsInstancedBaseVertexBaseInstance OpenGL::functions->drawElementsInstancedBaseVertexBaseInstance
#define glDrawTransformFeedbackInstanced OpenGL::functions->drawTransformFeedbackInstanced
#define glDrawTransformFeedbackStreamInstanced OpenGL::functions->drawTransformFeedbackStreamInstanced
#define glGetInternalformativ OpenGL::functions->getInternalformativ
#define glGetActiveAtomicCounterBufferiv OpenGL::functions->getActiveAtomicCounterBufferiv
#define glBindImageTexture OpenGL::functions->bindImageTexture
#define glMemoryBarrier OpenGL::functions->memoryBarrier
#define glTexStorage1D OpenGL::functions->texStorage1D
#define glTexStorage2D OpenGL::functions->texStorage2D
#define glTexStorage3D OpenGL::functions->texStorage3D
#define glTextureStorage1DEXT OpenGL::functions->textureStorage1DEXT
#define glTextureStorage2DEXT OpenGL::functions->textureStorage2DEXT
#define glTextureStorage3DEXT OpenGL::functions->textureStorage3DEXT
#define glDebugMessageControl OpenGL::functions->debugMessageControl
#define glDebugMessageInsert OpenGL::functions->debugMessageInsert
#define glDebugMessageCallback OpenGL::functions->debugMessageCallback
#define glGetDebugMessageLog OpenGL::functions->getDebugMessageLog
#define glPushDebugGroup OpenGL::functions->pushDebugGroup
#define glPopDebugGroup OpenGL::functions->popDebugGroup
#define glObjectLabel OpenGL::functions->objectLabel
#define glGetObjectLabel OpenGL::functions->getObjectLabel
#define glObjectPtrLabel OpenGL::functions->objectPtrLabel
#define glGetObjectPtrLabel OpenGL::functions->getObjectPtrLabel
#define glClearBufferData OpenGL::functions->clearBufferData
#define glClearBufferSubData OpenGL::functions->clearBufferSubData
#define glClearNamedBufferDataEXT OpenGL::functions->clearNamedBufferDataEXT
#define glClearNamedBufferSubDataEXT OpenGL::functions->clearNamedBufferSubDataEXT
#define glDispatchCompute OpenGL::functions->dispatchCompute
#define glDispatchComputeIndirect OpenGL::functions->dispatchComputeIndirect
#define glCopyImageSubData OpenGL::functions->copyImageSubData
#define glFramebufferParameteri OpenGL::functions->framebufferParameteri
#define glGetFramebufferParameteriv OpenGL::functions->getFramebufferParameteriv
#define glNamedFramebufferParameteriEXT OpenGL::functions->namedFramebufferParameteriEXT
#define glGetNamedFramebufferParameterivEXT OpenGL::functions->getNamedFramebufferParameterivEXT
#define glGetInternalformati64v OpenGL::functions->getInternalformati64v
#define glInvalidateTexSubImage OpenGL::functions->invalidateTexSubImage
#define glInvalidateTexImage OpenGL::functions->invalidateTexImage
#define glInvalidateBufferSubData OpenGL::functions->invalidateBufferSubData
#define glInvalidateBufferData OpenGL::functions->invalidateBufferData
#define glInvalidateFramebuffer OpenGL::functions->invalidateFramebuffer
#define glInvalidateSubFramebuffer OpenGL::functions->invalidateSubFramebuffer
#define glMultiDrawArraysIndirect OpenGL::functions->multiDrawArraysIndirect
#define glMultiDrawElementsIndirect OpenGL::functions->multiDrawElementsIndirect
#define glGetProgramInterfaceiv OpenGL::functions->getProgramInterfaceiv
#define glGetProgramResourceIndex OpenGL::functions->getProgramResourceIndex
#define glGetProgramResourceName OpenGL::functions->getProgramResourceName
#define glGetProgramResourceiv OpenGL::functions->getProgramResourceiv
#define glGetProgramResourceLocation OpenGL::functions->getProgramResourceLocation
#define glGetProgramResourceLocationIndex OpenGL::functions->getProgramResourceLocationIndex
#define glShaderStorageBlockBinding OpenGL::functions->shaderStorageBlockBinding
#define glTexBufferRange OpenGL::functions->texBufferRange
#define glTextureBufferRangeEXT OpenGL::functions->textureBufferRangeEXT
#define glTexStorage2DMultisample OpenGL::functions->texStorage2DMultisample
#define glTexStorage3DMultisample OpenGL::functions->texStorage3DMultisample
#define glTextureStorage2DMultisampleEXT OpenGL::functions->textureStorage2DMultisampleEXT
#define glTextureStorage3DMultisampleEXT OpenGL::functions->textureStorage3DMultisampleEXT
#define glTextureView OpenGL::functions->textureView
#define glBindVertexBuffer OpenGL::functions->bindVertexBuffer
#define glVertexAttribFormat OpenGL::functions->vertexAttribFormat
#define glVertexAttribIFormat OpenGL::functions->vertexAttribIFormat
#define glVertexAttribLFormat OpenGL::functions->vertexAttribLFormat
#define glVertexAttribBinding OpenGL::functions->vertexAttribBinding
#define glVertexBindingDivisor OpenGL::functions->vertexBindingDivisor
#define glVertexArrayBindVertexBufferEXT OpenGL::functions->vertexArrayBindVertexBufferEXT
#define glVertexArrayVertexAttribFormatEXT OpenGL::functions->vertexArrayVertexAttribFormatEXT
#define glVertexArrayVertexAttribIFormatEXT OpenGL::functions->vertexArrayVertexAttribIFormatEXT
#define glVertexArrayVertexAttribLFormatEXT OpenGL::functions->vertexArrayVertexAttribLFormatEXT
#define glVertexArrayVertexAttribBindingEXT OpenGL::functions->vertexArrayVertexAttribBindingEXT
#define glVertexArrayVertexBindingDivisorEXT OpenGL::functions->vertexArrayVertexBindingDivisorEXT

// For Legacy OpenGL (For GL1 target)
#define glClientActiveTexture OpenGL::functions->clientActiveTexture
#ifdef WIN32
#define glWglCreatePbufferARB OpenGL::functions->wglCreatePbufferARB
#define glWglGetPbufferDCARB OpenGL::functions->wglGetPbufferDCARB
#define glWglReleasePbufferDCARB OpenGL::functions->wglReleasePbufferDCARB
#define glWglDestroyPbufferARB OpenGL::functions->wglDestroyPbufferARB
#define glWglQueryPbufferARB OpenGL::functions->wglQueryPbufferARB
#endif
/// \}

}

/// \}

