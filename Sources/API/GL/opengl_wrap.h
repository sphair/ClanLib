/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
*/

/// \addtogroup clanGL_System clanGL System
/// \{

#pragma once

#include "opengl.h"
#include "opengl_defines.h"

#ifdef WIN32
#define CL_GLFUNC WINAPI
#else
#define CL_GLFUNC
#endif

/// \name OpenGL 2.0 binds
/// \{

/// \brief CL_GLFunctions
///
/// \xmlonly !group=GL/System! !header=gl.h! \endxmlonly
class CL_GLFunctions
{

/// \name Typedefs
/// \{

public:
	typedef void (CL_GLFUNC *ptr_glCullFace)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glFrontFace)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glHint)(CLenum target, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glLineWidth)(CLfloat width);
	typedef void (CL_GLFUNC *ptr_glPointSize)(CLfloat size);
	typedef void (CL_GLFUNC *ptr_glPolygonMode)(CLenum face, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glScissor)(CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glTexParameterf)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexParameterfv)(CLenum target, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexParameteri)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glTexParameteriv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexImage1D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLint border, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexImage2D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLsizei height, CLint border, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glDrawBuffer)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glClear)(CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glClearColor)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glClearStencil)(CLint s);
	typedef void (CL_GLFUNC *ptr_glClearDepth)(CLclampd depth);
	typedef void (CL_GLFUNC *ptr_glStencilMask)(CLuint mask);
	typedef void (CL_GLFUNC *ptr_glColorMask)(CLboolean red, CLboolean green, CLboolean blue, CLboolean alpha);
	typedef void (CL_GLFUNC *ptr_glDepthMask)(CLboolean flag);
	typedef void (CL_GLFUNC *ptr_glDisable)(CLenum cap);
	typedef void (CL_GLFUNC *ptr_glEnable)(CLenum cap);
	typedef void (CL_GLFUNC *ptr_glFinish)(void);
	typedef void (CL_GLFUNC *ptr_glFlush)(void);
	typedef void (CL_GLFUNC *ptr_glBlendFunc)(CLenum sfactor, CLenum dfactor);
	typedef void (CL_GLFUNC *ptr_glLogicOp)(CLenum opcode);
	typedef void (CL_GLFUNC *ptr_glStencilFunc)(CLenum func, CLint ref, CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilOp)(CLenum fail, CLenum zfail, CLenum zpass);
	typedef void (CL_GLFUNC *ptr_glDepthFunc)(CLenum func);
	typedef void (CL_GLFUNC *ptr_glPixelStoref)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glPixelStorei)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glReadBuffer)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glReadPixels)(CLint x, CLint y, CLsizei width, CLsizei height, CLenum format, CLenum type, CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glGetBooleanv)(CLenum pname, CLboolean *params);
	typedef void (CL_GLFUNC *ptr_glGetDoublev)(CLenum pname, CLdouble *params);
	typedef CLenum (CL_GLFUNC *ptr_glGetError)(void);
	typedef void (CL_GLFUNC *ptr_glGetFloatv)(CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetIntegerv)(CLenum pname, CLint *params);
	typedef const CLubyte * (CL_GLFUNC *ptr_glGetString)(CLenum name);
	typedef void (CL_GLFUNC *ptr_glGetTexImage)(CLenum target, CLint level, CLenum format, CLenum type, CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterfv)(CLenum target, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameteriv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameterfv)(CLenum target, CLint level, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameteriv)(CLenum target, CLint level, CLenum pname, CLint *params);
	typedef CLboolean (CL_GLFUNC *ptr_glIsEnabled)(CLenum cap);
	typedef void (CL_GLFUNC *ptr_glDepthRange)(CLclampd near, CLclampd far);
	typedef void (CL_GLFUNC *ptr_glViewport)(CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glDrawArrays)(CLenum mode, CLint first, CLsizei count);
	typedef void (CL_GLFUNC *ptr_glDrawElements)(CLenum mode, CLsizei count, CLenum type, const CLvoid *indices);
	typedef void (CL_GLFUNC *ptr_glGetPointerv)(CLenum pname, CLvoid* *params);
	typedef void (CL_GLFUNC *ptr_glPolygonOffset)(CLfloat factor, CLfloat units);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage1D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage2D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLsizei height, CLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLsizei width, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glBindTexture)(CLenum target, CLuint texture);
	typedef void (CL_GLFUNC *ptr_glDeleteTextures)(CLsizei n, const CLuint *textures);
	typedef void (CL_GLFUNC *ptr_glGenTextures)(CLsizei n, CLuint *textures);
	typedef CLboolean (CL_GLFUNC *ptr_glIsTexture)(CLuint texture);
	typedef void (CL_GLFUNC *ptr_glBlendColor)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glBlendEquation)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glDrawRangeElements)(CLenum mode, CLuint start, CLuint end, CLsizei count, CLenum type, const CLvoid *indices);
	typedef void (CL_GLFUNC *ptr_glTexImage3D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLsizei height, CLsizei depth, CLint border, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage3D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLsizei width, CLsizei height, CLsizei depth, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage3D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glActiveTexture)(CLenum texture);
	typedef void (CL_GLFUNC *ptr_glSampleCoverage)(CLclampf value, CLboolean invert);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage3D)(CLenum target, CLint level, CLenum internalformat, CLsizei width, CLsizei height, CLsizei depth, CLint border, CLsizei imageSize, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage2D)(CLenum target, CLint level, CLenum internalformat, CLsizei width, CLsizei height, CLint border, CLsizei imageSize, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage1D)(CLenum target, CLint level, CLenum internalformat, CLsizei width, CLint border, CLsizei imageSize, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage3D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLsizei width, CLsizei height, CLsizei depth, CLenum format, CLsizei imageSize, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLsizei width, CLsizei height, CLenum format, CLsizei imageSize, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLsizei width, CLenum format, CLsizei imageSize, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glGetCompressedTexImage)(CLenum target, CLint level, CLvoid *img);
	typedef void (CL_GLFUNC *ptr_glBlendFuncSeparate)(CLenum sfactorRGB, CLenum dfactorRGB, CLenum sfactorAlpha, CLenum dfactorAlpha);
	typedef void (CL_GLFUNC *ptr_glMultiDrawArrays)(CLenum mode, CLint *first, CLsizei *count, CLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glMultiDrawElements)(CLenum mode, const CLsizei *count, CLenum type, const CLvoid* *indices, CLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glPointParameterf)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glPointParameterfv)(CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glPointParameteri)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glPointParameteriv)(CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glGenQueries)(CLsizei n, CLuint *ids);
	typedef void (CL_GLFUNC *ptr_glDeleteQueries)(CLsizei n, const CLuint *ids);
	typedef CLboolean (CL_GLFUNC *ptr_glIsQuery)(CLuint id);
	typedef void (CL_GLFUNC *ptr_glBeginQuery)(CLenum target, CLuint id);
	typedef void (CL_GLFUNC *ptr_glEndQuery)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetQueryiv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectiv)(CLuint id, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectuiv)(CLuint id, CLenum pname, CLuint *params);
	typedef void (CL_GLFUNC *ptr_glBindBuffer)(CLenum target, CLuint buffer);
	typedef void (CL_GLFUNC *ptr_glDeleteBuffers)(CLsizei n, const CLuint *buffers);
	typedef void (CL_GLFUNC *ptr_glGenBuffers)(CLsizei n, CLuint *buffers);
	typedef CLboolean (CL_GLFUNC *ptr_glIsBuffer)(CLuint buffer);
	typedef void (CL_GLFUNC *ptr_glBufferData)(CLenum target, CLsizeiptr size, const CLvoid *data, CLenum usage);
	typedef void (CL_GLFUNC *ptr_glBufferSubData)(CLenum target, CLintptr offset, CLsizeiptr size, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glGetBufferSubData)(CLenum target, CLintptr offset, CLsizeiptr size, CLvoid *data);
	typedef CLvoid* (CL_GLFUNC *ptr_glMapBuffer)(CLenum target, CLenum access);
	typedef CLboolean (CL_GLFUNC *ptr_glUnmapBuffer)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetBufferParameteriv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetBufferPointerv)(CLenum target, CLenum pname, CLvoid* *params);
	typedef void (CL_GLFUNC *ptr_glBlendEquationSeparate)(CLenum modeRGB, CLenum modeAlpha);
	typedef void (CL_GLFUNC *ptr_glDrawBuffers)(CLsizei n, const CLenum *bufs);
	typedef void (CL_GLFUNC *ptr_glStencilOpSeparate)(CLenum face, CLenum sfail, CLenum dpfail, CLenum dppass);
	typedef void (CL_GLFUNC *ptr_glStencilFuncSeparate)(CLenum frontfunc, CLenum backfunc, CLint ref, CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilMaskSeparate)(CLenum face, CLuint mask);
	typedef void (CL_GLFUNC *ptr_glAttachShader)(CLuint program, CLuint shader);
	typedef void (CL_GLFUNC *ptr_glBindAttribLocation)(CLuint program, CLuint index, const CLchar *name);
	typedef void (CL_GLFUNC *ptr_glCompileShader)(CLuint shader);
	typedef CLuint (CL_GLFUNC *ptr_glCreateProgram)(void);
	typedef CLuint (CL_GLFUNC *ptr_glCreateShader)(CLenum type);
	typedef void (CL_GLFUNC *ptr_glDeleteProgram)(CLuint program);
	typedef void (CL_GLFUNC *ptr_glDeleteShader)(CLuint shader);
	typedef void (CL_GLFUNC *ptr_glDetachShader)(CLuint program, CLuint shader);
	typedef void (CL_GLFUNC *ptr_glDisableVertexAttribArray)(CLuint index);
	typedef void (CL_GLFUNC *ptr_glEnableVertexAttribArray)(CLuint index);
	typedef void (CL_GLFUNC *ptr_glGetActiveAttrib)(CLuint program, CLuint index, CLsizei bufSize, CLsizei *length, CLint *size, CLenum *type, CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniform)(CLuint program, CLuint index, CLsizei bufSize, CLsizei *length, CLint *size, CLenum *type, CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetAttachedShaders)(CLuint program, CLsizei maxCount, CLsizei *count, CLuint *obj);
	typedef CLint (CL_GLFUNC *ptr_glGetAttribLocation)(CLuint program, const CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetProgramiv)(CLuint program, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetProgramInfoLog)(CLuint program, CLsizei bufSize, CLsizei *length, CLchar *infoLog);
	typedef void (CL_GLFUNC *ptr_glGetShaderiv)(CLuint shader, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetShaderInfoLog)(CLuint shader, CLsizei bufSize, CLsizei *length, CLchar *infoLog);
	typedef void (CL_GLFUNC *ptr_glGetShaderSource)(CLuint shader, CLsizei bufSize, CLsizei *length, CLchar *source);
	typedef CLint (CL_GLFUNC *ptr_glGetUniformLocation)(CLuint program, const CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetUniformfv)(CLuint program, CLint location, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetUniformiv)(CLuint program, CLint location, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribdv)(CLuint index, CLenum pname, CLdouble *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribfv)(CLuint index, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribiv)(CLuint index, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribPointerv)(CLuint index, CLenum pname, CLvoid* *pointer);
	typedef CLboolean (CL_GLFUNC *ptr_glIsProgram)(CLuint program);
	typedef CLboolean (CL_GLFUNC *ptr_glIsShader)(CLuint shader);
	typedef void (CL_GLFUNC *ptr_glLinkProgram)(CLuint program);
	typedef void (CL_GLFUNC *ptr_glShaderSource)(CLuint shader, CLsizei count, const CLchar* *string, const CLint *length);
	typedef void (CL_GLFUNC *ptr_glUseProgram)(CLuint program);
	typedef void (CL_GLFUNC *ptr_glUniform1f)(CLint location, CLfloat v0);
	typedef void (CL_GLFUNC *ptr_glUniform2f)(CLint location, CLfloat v0, CLfloat v1);
	typedef void (CL_GLFUNC *ptr_glUniform3f)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2);
	typedef void (CL_GLFUNC *ptr_glUniform4f)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2, CLfloat v3);
	typedef void (CL_GLFUNC *ptr_glUniform1i)(CLint location, CLint v0);
	typedef void (CL_GLFUNC *ptr_glUniform2i)(CLint location, CLint v0, CLint v1);
	typedef void (CL_GLFUNC *ptr_glUniform3i)(CLint location, CLint v0, CLint v1, CLint v2);
	typedef void (CL_GLFUNC *ptr_glUniform4i)(CLint location, CLint v0, CLint v1, CLint v2, CLint v3);
	typedef void (CL_GLFUNC *ptr_glUniform1fv)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform2fv)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform3fv)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform4fv)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform1iv)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform2iv)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform3iv)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform4iv)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glValidateProgram)(CLuint program);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1d)(CLuint index, CLdouble x);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1dv)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1f)(CLuint index, CLfloat x);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1fv)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1s)(CLuint index, CLshort x);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1sv)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2d)(CLuint index, CLdouble x, CLdouble y);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2dv)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2f)(CLuint index, CLfloat x, CLfloat y);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2fv)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2s)(CLuint index, CLshort x, CLshort y);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2sv)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3d)(CLuint index, CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3dv)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3f)(CLuint index, CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3fv)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3s)(CLuint index, CLshort x, CLshort y, CLshort z);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3sv)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nbv)(CLuint index, const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Niv)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nsv)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nub)(CLuint index, CLubyte x, CLubyte y, CLubyte z, CLubyte w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nubv)(CLuint index, const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nuiv)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nusv)(CLuint index, const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4bv)(CLuint index, const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4d)(CLuint index, CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4dv)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4f)(CLuint index, CLfloat x, CLfloat y, CLfloat z, CLfloat w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4fv)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4iv)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4s)(CLuint index, CLshort x, CLshort y, CLshort z, CLshort w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4sv)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4ubv)(CLuint index, const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4uiv)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4usv)(CLuint index, const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribPointer)(CLuint index, CLint size, CLenum type, CLboolean normalized, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x3fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x2fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x4fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x2fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x4fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x3fv)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glColorMaski)(CLuint index, CLboolean r, CLboolean g, CLboolean b, CLboolean a);
	typedef void (CL_GLFUNC *ptr_glGetBooleani_v)(CLenum target, CLuint index, CLboolean *data);
	typedef void (CL_GLFUNC *ptr_glGetIntegeri_v)(CLenum target, CLuint index, CLint *data);
	typedef void (CL_GLFUNC *ptr_glEnablei)(CLenum target, CLuint index);
	typedef void (CL_GLFUNC *ptr_glDisablei)(CLenum target, CLuint index);
	typedef CLboolean (CL_GLFUNC *ptr_glIsEnabledi)(CLenum target, CLuint index);
	typedef void (CL_GLFUNC *ptr_glBeginTransformFeedback)(CLenum primitiveMode);
	typedef void (CL_GLFUNC *ptr_glEndTransformFeedback)(void);
	typedef void (CL_GLFUNC *ptr_glBindBufferRange)(CLenum target, CLuint index, CLuint buffer, CLintptr offset, CLsizeiptr size);
	typedef void (CL_GLFUNC *ptr_glBindBufferBase)(CLenum target, CLuint index, CLuint buffer);
	typedef void (CL_GLFUNC *ptr_glTransformFeedbackVaryings)(CLuint program, CLsizei count, const CLchar* *varyings, CLenum bufferMode);
	typedef void (CL_GLFUNC *ptr_glGetTransformFeedbackVarying)(CLuint program, CLuint index, CLsizei bufSize, CLsizei *length, CLsizei *size, CLenum *type, CLchar *name);
	typedef void (CL_GLFUNC *ptr_glClampColor)(CLenum target, CLenum clamp);
	typedef void (CL_GLFUNC *ptr_glBeginConditionalRender)(CLuint id, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glEndConditionalRender)(void);
	typedef void (CL_GLFUNC *ptr_glVertexAttribIPointer)(CLuint index, CLint size, CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribIiv)(CLuint index, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribIuiv)(CLuint index, CLenum pname, CLuint *params);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1i)(CLuint index, CLint x);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2i)(CLuint index, CLint x, CLint y);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3i)(CLuint index, CLint x, CLint y, CLint z);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4i)(CLuint index, CLint x, CLint y, CLint z, CLint w);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1ui)(CLuint index, CLuint x);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2ui)(CLuint index, CLuint x, CLuint y);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3ui)(CLuint index, CLuint x, CLuint y, CLuint z);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4ui)(CLuint index, CLuint x, CLuint y, CLuint z, CLuint w);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1iv)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2iv)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3iv)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4iv)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1uiv)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2uiv)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3uiv)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4uiv)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4bv)(CLuint index, const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4sv)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4ubv)(CLuint index, const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4usv)(CLuint index, const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glGetUniformuiv)(CLuint program, CLint location, CLuint *params);
	typedef void (CL_GLFUNC *ptr_glBindFragDataLocation)(CLuint program, CLuint color, const CLchar *name);
	typedef CLint (CL_GLFUNC *ptr_glGetFragDataLocation)(CLuint program, const CLchar *name);
	typedef void (CL_GLFUNC *ptr_glUniform1ui)(CLint location, CLuint v0);
	typedef void (CL_GLFUNC *ptr_glUniform2ui)(CLint location, CLuint v0, CLuint v1);
	typedef void (CL_GLFUNC *ptr_glUniform3ui)(CLint location, CLuint v0, CLuint v1, CLuint v2);
	typedef void (CL_GLFUNC *ptr_glUniform4ui)(CLint location, CLuint v0, CLuint v1, CLuint v2, CLuint v3);
	typedef void (CL_GLFUNC *ptr_glUniform1uiv)(CLint location, CLsizei count, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glUniform2uiv)(CLint location, CLsizei count, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glUniform3uiv)(CLint location, CLsizei count, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glUniform4uiv)(CLint location, CLsizei count, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glTexParameterIiv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexParameterIuiv)(CLenum target, CLenum pname, const CLuint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterIiv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterIuiv)(CLenum target, CLenum pname, CLuint *params);
	typedef void (CL_GLFUNC *ptr_glClearBufferiv)(CLenum buffer, CLint drawbuffer, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glClearBufferuiv)(CLenum buffer, CLint drawbuffer, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glClearBufferfv)(CLenum buffer, CLint drawbuffer, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glClearBufferfi)(CLenum buffer, CLint drawbuffer, CLfloat depth, CLint stencil);
	typedef const CLubyte * (CL_GLFUNC *ptr_glGetStringi)(CLenum name, CLuint index);
	typedef void (CL_GLFUNC *ptr_glDrawArraysInstanced)(CLenum mode, CLint first, CLsizei count, CLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glDrawElementsInstanced)(CLenum mode, CLsizei count, CLenum type, const CLvoid *indices, CLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glTexBuffer)(CLenum target, CLenum internalformat, CLuint buffer);
	typedef void (CL_GLFUNC *ptr_glPrimitiveRestartIndex)(CLuint index);
	typedef void (CL_GLFUNC *ptr_glGetInteger64i_v)(CLenum target, CLuint index, CLint64 *data);
	typedef void (CL_GLFUNC *ptr_glGetBufferParameteri64v)(CLenum target, CLenum pname, CLint64 *params);
	typedef void (CL_GLFUNC *ptr_glProgramParameteri)(CLuint program, CLenum pname, CLint value);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture)(CLenum target, CLenum attachment, CLuint texture, CLint level);
	typedef CLboolean (CL_GLFUNC *ptr_glIsRenderbuffer)(CLuint renderbuffer);
	typedef void (CL_GLFUNC *ptr_glBindRenderbuffer)(CLenum target, CLuint renderbuffer);
	typedef void (CL_GLFUNC *ptr_glDeleteRenderbuffers)(CLsizei n, const CLuint *renderbuffers);
	typedef void (CL_GLFUNC *ptr_glGenRenderbuffers)(CLsizei n, CLuint *renderbuffers);
	typedef void (CL_GLFUNC *ptr_glRenderbufferStorage)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glGetRenderbufferParameteriv)(CLenum target, CLenum pname, CLint *params);
	typedef CLboolean (CL_GLFUNC *ptr_glIsFramebuffer)(CLuint framebuffer);
	typedef void (CL_GLFUNC *ptr_glBindFramebuffer)(CLenum target, CLuint framebuffer);
	typedef void (CL_GLFUNC *ptr_glDeleteFramebuffers)(CLsizei n, const CLuint *framebuffers);
	typedef void (CL_GLFUNC *ptr_glGenFramebuffers)(CLsizei n, CLuint *framebuffers);
	typedef CLenum (CL_GLFUNC *ptr_glCheckFramebufferStatus)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture1D)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture2D)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture3D)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level, CLint zoffset);
	typedef void (CL_GLFUNC *ptr_glFramebufferRenderbuffer)(CLenum target, CLenum attachment, CLenum renderbuffertarget, CLuint renderbuffer);
	typedef void (CL_GLFUNC *ptr_glGetFramebufferAttachmentParameteriv)(CLenum target, CLenum attachment, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGenerateMipmap)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glBlitFramebuffer)(CLint srcX0, CLint srcY0, CLint srcX1, CLint srcY1, CLint dstX0, CLint dstY0, CLint dstX1, CLint dstY1, CLbitfield mask, CLenum filter);
	typedef void (CL_GLFUNC *ptr_glRenderbufferStorageMultisample)(CLenum target, CLsizei samples, CLenum internalformat, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glFramebufferTextureLayer)(CLenum target, CLenum attachment, CLuint texture, CLint level, CLint layer);
	typedef CLvoid* (CL_GLFUNC *ptr_glMapBufferRange)(CLenum target, CLintptr offset, CLsizeiptr length, CLbitfield access);
	typedef void (CL_GLFUNC *ptr_glFlushMappedBufferRange)(CLenum target, CLintptr offset, CLsizeiptr length);
	typedef void (CL_GLFUNC *ptr_glBindVertexArray)(CLuint array);
	typedef void (CL_GLFUNC *ptr_glDeleteVertexArrays)(CLsizei n, const CLuint *arrays);
	typedef void (CL_GLFUNC *ptr_glGenVertexArrays)(CLsizei n, CLuint *arrays);
	typedef CLboolean (CL_GLFUNC *ptr_glIsVertexArray)(CLuint array);
	typedef void (CL_GLFUNC *ptr_glGetUniformIndices)(CLuint program, CLsizei uniformCount, const CLchar* *uniformNames, CLuint *uniformIndices);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformsiv)(CLuint program, CLsizei uniformCount, const CLuint *uniformIndices, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformName)(CLuint program, CLuint uniformIndex, CLsizei bufSize, CLsizei *length, CLchar *uniformName);
	typedef CLuint (CL_GLFUNC *ptr_glGetUniformBlockIndex)(CLuint program, const CLchar *uniformBlockName);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformBlockiv)(CLuint program, CLuint uniformBlockIndex, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformBlockName)(CLuint program, CLuint uniformBlockIndex, CLsizei bufSize, CLsizei *length, CLchar *uniformBlockName);
	typedef void (CL_GLFUNC *ptr_glUniformBlockBinding)(CLuint program, CLuint uniformBlockIndex, CLuint uniformBlockBinding);
	typedef void (CL_GLFUNC *ptr_glCopyBufferSubData)(CLenum readTarget, CLenum writeTarget, CLintptr readOffset, CLintptr writeOffset, CLsizeiptr size);
	typedef void (CL_GLFUNC *ptr_glDrawElementsBaseVertex)(CLenum mode, CLsizei count, CLenum type, const CLvoid *indices, CLint basevertex);
	typedef void (CL_GLFUNC *ptr_glDrawRangeElementsBaseVertex)(CLenum mode, CLuint start, CLuint end, CLsizei count, CLenum type, const CLvoid *indices, CLint basevertex);
	typedef void (CL_GLFUNC *ptr_glDrawElementsInstancedBaseVertex)(CLenum mode, CLsizei count, CLenum type, const CLvoid *indices, CLsizei primcount, CLint basevertex);
	typedef void (CL_GLFUNC *ptr_glMultiDrawElementsBaseVertex)(CLenum mode, const CLsizei *count, CLenum type, const CLvoid* *indices, CLsizei primcount, const CLint *basevertex);
	typedef void (CL_GLFUNC *ptr_glProvokingVertex)(CLenum mode);
	typedef CLsync (CL_GLFUNC *ptr_glFenceSync)(CLenum condition, CLbitfield flags);
	typedef CLboolean (CL_GLFUNC *ptr_glIsSync)(CLsync sync);
	typedef void (CL_GLFUNC *ptr_glDeleteSync)(CLsync sync);
	typedef CLenum (CL_GLFUNC *ptr_glClientWaitSync)(CLsync sync, CLbitfield flags, CLuint64 timeout);
	typedef void (CL_GLFUNC *ptr_glWaitSync)(CLsync sync, CLbitfield flags, CLuint64 timeout);
	typedef void (CL_GLFUNC *ptr_glGetInteger64v)(CLenum pname, CLint64 *params);
	typedef void (CL_GLFUNC *ptr_glGetSynciv)(CLsync sync, CLenum pname, CLsizei bufSize, CLsizei *length, CLint *values);
	typedef void (CL_GLFUNC *ptr_glTexImage2DMultisample)(CLenum target, CLsizei samples, CLint internalformat, CLsizei width, CLsizei height, CLboolean fixedsamplelocations);
	typedef void (CL_GLFUNC *ptr_glTexImage3DMultisample)(CLenum target, CLsizei samples, CLint internalformat, CLsizei width, CLsizei height, CLsizei depth, CLboolean fixedsamplelocations);
	typedef void (CL_GLFUNC *ptr_glGetMultisamplefv)(CLenum pname, CLuint index, CLfloat *val);
	typedef void (CL_GLFUNC *ptr_glSampleMaski)(CLuint index, CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glBlendEquationi)(CLuint buf, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glBlendEquationSeparatei)(CLuint buf, CLenum modeRGB, CLenum modeAlpha);
	typedef void (CL_GLFUNC *ptr_glBlendFunci)(CLuint buf, CLenum src, CLenum dst);
	typedef void (CL_GLFUNC *ptr_glBlendFuncSeparatei)(CLuint buf, CLenum srcRGB, CLenum dstRGB, CLenum srcAlpha, CLenum dstAlpha);
	typedef void (CL_GLFUNC *ptr_glMinSampleShading)(CLclampf value);
	typedef void (CL_GLFUNC *ptr_glBindFragDataLocationIndexed)(CLuint program, CLuint colorNumber, CLuint index, const CLchar *name);
	typedef CLint (CL_GLFUNC *ptr_glGetFragDataIndex)(CLuint program, const CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGenSamplers)(CLsizei count, CLuint *samplers);
	typedef void (CL_GLFUNC *ptr_glDeleteSamplers)(CLsizei count, const CLuint *samplers);
	typedef CLboolean (CL_GLFUNC *ptr_glIsSampler)(CLuint sampler);
	typedef void (CL_GLFUNC *ptr_glBindSampler)(CLenum unit, CLuint sampler);
	typedef void (CL_GLFUNC *ptr_glSamplerParameteri)(CLuint sampler, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameteriv)(CLuint sampler, CLenum pname, const CLint *param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterf)(CLuint sampler, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterfv)(CLuint sampler, CLenum pname, const CLfloat *param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterIiv)(CLuint sampler, CLenum pname, const CLint *param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterIuiv)(CLuint sampler, CLenum pname, const CLuint *param);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameteriv)(CLuint sampler, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameterIiv)(CLuint sampler, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameterfv)(CLuint sampler, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameterIfv)(CLuint sampler, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glQueryCounter)(CLuint id, CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjecti64v)(CLuint id, CLenum pname, CLint64 *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectui64v)(CLuint id, CLenum pname, CLuint64 *params);
	typedef void (CL_GLFUNC *ptr_glVertexP2ui)(CLenum type, CLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexP2uiv)(CLenum type, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexP3ui)(CLenum type, CLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexP3uiv)(CLenum type, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexP4ui)(CLenum type, CLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexP4uiv)(CLenum type, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glTexCoordP1ui)(CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP1uiv)(CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP2ui)(CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP2uiv)(CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP3ui)(CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP3uiv)(CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP4ui)(CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP4uiv)(CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP1ui)(CLenum texture, CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP1uiv)(CLenum texture, CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP2ui)(CLenum texture, CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP2uiv)(CLenum texture, CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP3ui)(CLenum texture, CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP3uiv)(CLenum texture, CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP4ui)(CLenum texture, CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP4uiv)(CLenum texture, CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glNormalP3ui)(CLenum type, CLuint coords);
	typedef void (CL_GLFUNC *ptr_glNormalP3uiv)(CLenum type, const CLuint *coords);
	typedef void (CL_GLFUNC *ptr_glColorP3ui)(CLenum type, CLuint color);
	typedef void (CL_GLFUNC *ptr_glColorP3uiv)(CLenum type, const CLuint *color);
	typedef void (CL_GLFUNC *ptr_glColorP4ui)(CLenum type, CLuint color);
	typedef void (CL_GLFUNC *ptr_glColorP4uiv)(CLenum type, const CLuint *color);
	typedef void (CL_GLFUNC *ptr_glSecondaryColorP3ui)(CLenum type, CLuint color);
	typedef void (CL_GLFUNC *ptr_glSecondaryColorP3uiv)(CLenum type, const CLuint *color);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP1ui)(CLuint index, CLenum type, CLboolean normalized, CLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP1uiv)(CLuint index, CLenum type, CLboolean normalized, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP2ui)(CLuint index, CLenum type, CLboolean normalized, CLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP2uiv)(CLuint index, CLenum type, CLboolean normalized, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP3ui)(CLuint index, CLenum type, CLboolean normalized, CLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP3uiv)(CLuint index, CLenum type, CLboolean normalized, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP4ui)(CLuint index, CLenum type, CLboolean normalized, CLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP4uiv)(CLuint index, CLenum type, CLboolean normalized, const CLuint *value);
	typedef void (CL_GLFUNC *ptr_glDrawArraysIndirect)(CLenum mode, const CLvoid *indirect);
	typedef void (CL_GLFUNC *ptr_glDrawElementsIndirect)(CLenum mode, CLenum type, const CLvoid *indirect);
	typedef void (CL_GLFUNC *ptr_glUniform1d)(CLint location, CLdouble x);
	typedef void (CL_GLFUNC *ptr_glUniform2d)(CLint location, CLdouble x, CLdouble y);
	typedef void (CL_GLFUNC *ptr_glUniform3d)(CLint location, CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glUniform4d)(CLint location, CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GLFUNC *ptr_glUniform1dv)(CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniform2dv)(CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniform3dv)(CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniform4dv)(CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x3dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x4dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x2dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x4dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x2dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x3dv)(CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glGetUniformdv)(CLuint program, CLint location, CLdouble *params);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1dEXT)(CLuint program, CLint location, CLdouble x);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2dEXT)(CLuint program, CLint location, CLdouble x, CLdouble y);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3dEXT)(CLuint program, CLint location, CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4dEXT)(CLuint program, CLint location, CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1dvEXT)(CLuint program, CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2dvEXT)(CLuint program, CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3dvEXT)(CLuint program, CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4dvEXT)(CLuint program, CLint location, CLsizei count, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2x3dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2x4dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3x2dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3x4dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4x2dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4x3dvEXT)(CLuint program, CLint location, CLsizei count, CLboolean transpose, const CLdouble *value);
	typedef CLint (CL_GLFUNC *ptr_glGetSubroutineUniformLocation)(CLuint program, CLenum shadertype, const CLchar *name);
	typedef CLuint (CL_GLFUNC *ptr_glGetSubroutineIndex)(CLuint program, CLenum shadertype, const CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveSubroutineUniformiv)(CLuint program, CLenum shadertype, CLuint index, CLenum pname, CLint *values);
	typedef void (CL_GLFUNC *ptr_glGetActiveSubroutineUniformName)(CLuint program, CLenum shadertype, CLuint index, CLsizei bufsize, CLsizei *length, CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveSubroutineName)(CLuint program, CLenum shadertype, CLuint index, CLsizei bufsize, CLsizei *length, CLchar *name);
	typedef void (CL_GLFUNC *ptr_glUniformSubroutinesuiv)(CLenum shadertype, CLsizei count, const CLuint *indices);
	typedef void (CL_GLFUNC *ptr_glGetUniformSubroutineuiv)(CLenum shadertype, CLint location, CLuint *params);
	typedef void (CL_GLFUNC *ptr_glGetProgramStageiv)(CLuint program, CLenum shadertype, CLenum pname, CLint *values);
	typedef void (CL_GLFUNC *ptr_glPatchParameteri)(CLenum pname, CLint value);
	typedef void (CL_GLFUNC *ptr_glPatchParameterfv)(CLenum pname, const CLfloat *values);
	typedef void (CL_GLFUNC *ptr_glBindTransformFeedback)(CLenum target, CLuint id);
	typedef void (CL_GLFUNC *ptr_glDeleteTransformFeedbacks)(CLsizei n, const CLuint *ids);
	typedef void (CL_GLFUNC *ptr_glGenTransformFeedbacks)(CLsizei n, CLuint *ids);
	typedef CLboolean (CL_GLFUNC *ptr_glIsTransformFeedback)(CLuint id);
	typedef void (CL_GLFUNC *ptr_glPauseTransformFeedback)(void);
	typedef void (CL_GLFUNC *ptr_glResumeTransformFeedback)(void);
	typedef void (CL_GLFUNC *ptr_glDrawTransformFeedback)(CLenum mode, CLuint id);
	typedef void (CL_GLFUNC *ptr_glDrawTransformFeedbackStream)(CLenum mode, CLuint id, CLuint stream);
	typedef void (CL_GLFUNC *ptr_glBeginQueryIndexed)(CLenum target, CLuint index, CLuint id);
	typedef void (CL_GLFUNC *ptr_glEndQueryIndexed)(CLenum target, CLuint index);
	typedef void (CL_GLFUNC *ptr_glGetQueryIndexediv)(CLenum target, CLuint index, CLenum pname, CLint *params);

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
	ptr_glProgramParameteri programParameteri;
	ptr_glFramebufferTexture framebufferTexture;
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
	ptr_glBlendEquationi blendEquationi;
	ptr_glBlendEquationSeparatei blendEquationSeparatei;
	ptr_glBlendFunci blendFunci;
	ptr_glBlendFuncSeparatei blendFuncSeparatei;
	ptr_glMinSampleShading minSampleShading;
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
	ptr_glGetSamplerParameterIfv getSamplerParameterIfv;
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
	ptr_glProgramUniform1dEXT programUniform1dEXT;
	ptr_glProgramUniform2dEXT programUniform2dEXT;
	ptr_glProgramUniform3dEXT programUniform3dEXT;
	ptr_glProgramUniform4dEXT programUniform4dEXT;
	ptr_glProgramUniform1dvEXT programUniform1dvEXT;
	ptr_glProgramUniform2dvEXT programUniform2dvEXT;
	ptr_glProgramUniform3dvEXT programUniform3dvEXT;
	ptr_glProgramUniform4dvEXT programUniform4dvEXT;
	ptr_glProgramUniformMatrix2dvEXT programUniformMatrix2dvEXT;
	ptr_glProgramUniformMatrix3dvEXT programUniformMatrix3dvEXT;
	ptr_glProgramUniformMatrix4dvEXT programUniformMatrix4dvEXT;
	ptr_glProgramUniformMatrix2x3dvEXT programUniformMatrix2x3dvEXT;
	ptr_glProgramUniformMatrix2x4dvEXT programUniformMatrix2x4dvEXT;
	ptr_glProgramUniformMatrix3x2dvEXT programUniformMatrix3x2dvEXT;
	ptr_glProgramUniformMatrix3x4dvEXT programUniformMatrix3x4dvEXT;
	ptr_glProgramUniformMatrix4x2dvEXT programUniformMatrix4x2dvEXT;
	ptr_glProgramUniformMatrix4x3dvEXT programUniformMatrix4x3dvEXT;
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

/// \}
};

#define clCullFace CL_OpenGL::functions->cullFace
#define clFrontFace CL_OpenGL::functions->frontFace
#define clHint CL_OpenGL::functions->int
#define clLineWidth CL_OpenGL::functions->lineWidth
#define clPointSize CL_OpenGL::functions->pointSize
#define clPolygonMode CL_OpenGL::functions->polygonMode
#define clScissor CL_OpenGL::functions->scissor
#define clTexParameterf CL_OpenGL::functions->texParameterf
#define clTexParameterfv CL_OpenGL::functions->texParameterfv
#define clTexParameteri CL_OpenGL::functions->texParameteri
#define clTexParameteriv CL_OpenGL::functions->texParameteriv
#define clTexImage1D CL_OpenGL::functions->texImage1D
#define clTexImage2D CL_OpenGL::functions->texImage2D
#define clDrawBuffer CL_OpenGL::functions->drawBuffer
#define clClear CL_OpenGL::functions->clear
#define clClearColor CL_OpenGL::functions->clearColor
#define clClearStencil CL_OpenGL::functions->clearStencil
#define clClearDepth CL_OpenGL::functions->clearDepth
#define clStencilMask CL_OpenGL::functions->stencilMask
#define clColorMask CL_OpenGL::functions->colorMask
#define clDepthMask CL_OpenGL::functions->depthMask
#define clDisable CL_OpenGL::functions->disable
#define clEnable CL_OpenGL::functions->enable
#define clFinish CL_OpenGL::functions->finish
#define clFlush CL_OpenGL::functions->flush
#define clBlendFunc CL_OpenGL::functions->blendFunc
#define clLogicOp CL_OpenGL::functions->logicOp
#define clStencilFunc CL_OpenGL::functions->stencilFunc
#define clStencilOp CL_OpenGL::functions->stencilOp
#define clDepthFunc CL_OpenGL::functions->depthFunc
#define clPixelStoref CL_OpenGL::functions->pixelStoref
#define clPixelStorei CL_OpenGL::functions->pixelStorei
#define clReadBuffer CL_OpenGL::functions->readBuffer
#define clReadPixels CL_OpenGL::functions->readPixels
#define clGetBooleanv CL_OpenGL::functions->getBooleanv
#define clGetDoublev CL_OpenGL::functions->getDoublev
#define clGetError CL_OpenGL::functions->getError
#define clGetFloatv CL_OpenGL::functions->getFloatv
#define clGetIntegerv CL_OpenGL::functions->getIntegerv
#define clGetString CL_OpenGL::functions->getString
#define clGetTexImage CL_OpenGL::functions->getTexImage
#define clGetTexParameterfv CL_OpenGL::functions->getTexParameterfv
#define clGetTexParameteriv CL_OpenGL::functions->getTexParameteriv
#define clGetTexLevelParameterfv CL_OpenGL::functions->getTexLevelParameterfv
#define clGetTexLevelParameteriv CL_OpenGL::functions->getTexLevelParameteriv
#define clIsEnabled CL_OpenGL::functions->isEnabled
#define clDepthRange CL_OpenGL::functions->depthRange
#define clViewport CL_OpenGL::functions->viewport
#define clDrawArrays CL_OpenGL::functions->drawArrays
#define clDrawElements CL_OpenGL::functions->drawElements
#define clGetPointerv CL_OpenGL::functions->getPointerv
#define clPolygonOffset CL_OpenGL::functions->polygonOffset
#define clCopyTexImage1D CL_OpenGL::functions->copyTexImage1D
#define clCopyTexImage2D CL_OpenGL::functions->copyTexImage2D
#define clCopyTexSubImage1D CL_OpenGL::functions->copyTexSubImage1D
#define clCopyTexSubImage2D CL_OpenGL::functions->copyTexSubImage2D
#define clTexSubImage1D CL_OpenGL::functions->texSubImage1D
#define clTexSubImage2D CL_OpenGL::functions->texSubImage2D
#define clBindTexture CL_OpenGL::functions->bindTexture
#define clDeleteTextures CL_OpenGL::functions->deleteTextures
#define clGenTextures CL_OpenGL::functions->genTextures
#define clIsTexture CL_OpenGL::functions->isTexture
#define clBlendColor CL_OpenGL::functions->blendColor
#define clBlendEquation CL_OpenGL::functions->blendEquation
#define clDrawRangeElements CL_OpenGL::functions->drawRangeElements
#define clTexImage3D CL_OpenGL::functions->texImage3D
#define clTexSubImage3D CL_OpenGL::functions->texSubImage3D
#define clCopyTexSubImage3D CL_OpenGL::functions->copyTexSubImage3D
#define clActiveTexture CL_OpenGL::functions->activeTexture
#define clSampleCoverage CL_OpenGL::functions->sampleCoverage
#define clCompressedTexImage3D CL_OpenGL::functions->compressedTexImage3D
#define clCompressedTexImage2D CL_OpenGL::functions->compressedTexImage2D
#define clCompressedTexImage1D CL_OpenGL::functions->compressedTexImage1D
#define clCompressedTexSubImage3D CL_OpenGL::functions->compressedTexSubImage3D
#define clCompressedTexSubImage2D CL_OpenGL::functions->compressedTexSubImage2D
#define clCompressedTexSubImage1D CL_OpenGL::functions->compressedTexSubImage1D
#define clGetCompressedTexImage CL_OpenGL::functions->getCompressedTexImage
#define clBlendFuncSeparate CL_OpenGL::functions->blendFuncSeparate
#define clMultiDrawArrays CL_OpenGL::functions->multiDrawArrays
#define clMultiDrawElements CL_OpenGL::functions->multiDrawElements
#define clPointParameterf CL_OpenGL::functions->pointParameterf
#define clPointParameterfv CL_OpenGL::functions->pointParameterfv
#define clPointParameteri CL_OpenGL::functions->pointParameteri
#define clPointParameteriv CL_OpenGL::functions->pointParameteriv
#define clGenQueries CL_OpenGL::functions->genQueries
#define clDeleteQueries CL_OpenGL::functions->deleteQueries
#define clIsQuery CL_OpenGL::functions->isQuery
#define clBeginQuery CL_OpenGL::functions->beginQuery
#define clEndQuery CL_OpenGL::functions->endQuery
#define clGetQueryiv CL_OpenGL::functions->getQueryiv
#define clGetQueryObjectiv CL_OpenGL::functions->getQueryObjectiv
#define clGetQueryObjectuiv CL_OpenGL::functions->getQueryObjectuiv
#define clBindBuffer CL_OpenGL::functions->bindBuffer
#define clDeleteBuffers CL_OpenGL::functions->deleteBuffers
#define clGenBuffers CL_OpenGL::functions->genBuffers
#define clIsBuffer CL_OpenGL::functions->isBuffer
#define clBufferData CL_OpenGL::functions->bufferData
#define clBufferSubData CL_OpenGL::functions->bufferSubData
#define clGetBufferSubData CL_OpenGL::functions->getBufferSubData
#define clMapBuffer CL_OpenGL::functions->mapBuffer
#define clUnmapBuffer CL_OpenGL::functions->unmapBuffer
#define clGetBufferParameteriv CL_OpenGL::functions->getBufferParameteriv
#define clGetBufferPointerv CL_OpenGL::functions->getBufferPointerv
#define clBlendEquationSeparate CL_OpenGL::functions->blendEquationSeparate
#define clDrawBuffers CL_OpenGL::functions->drawBuffers
#define clStencilOpSeparate CL_OpenGL::functions->stencilOpSeparate
#define clStencilFuncSeparate CL_OpenGL::functions->stencilFuncSeparate
#define clStencilMaskSeparate CL_OpenGL::functions->stencilMaskSeparate
#define clAttachShader CL_OpenGL::functions->attachShader
#define clBindAttribLocation CL_OpenGL::functions->bindAttribLocation
#define clCompileShader CL_OpenGL::functions->compileShader
#define clCreateProgram CL_OpenGL::functions->createProgram
#define clCreateShader CL_OpenGL::functions->createShader
#define clDeleteProgram CL_OpenGL::functions->deleteProgram
#define clDeleteShader CL_OpenGL::functions->deleteShader
#define clDetachShader CL_OpenGL::functions->detachShader
#define clDisableVertexAttribArray CL_OpenGL::functions->disableVertexAttribArray
#define clEnableVertexAttribArray CL_OpenGL::functions->enableVertexAttribArray
#define clGetActiveAttrib CL_OpenGL::functions->getActiveAttrib
#define clGetActiveUniform CL_OpenGL::functions->getActiveUniform
#define clGetAttachedShaders CL_OpenGL::functions->getAttachedShaders
#define clGetAttribLocation CL_OpenGL::functions->getAttribLocation
#define clGetProgramiv CL_OpenGL::functions->getProgramiv
#define clGetProgramInfoLog CL_OpenGL::functions->getProgramInfoLog
#define clGetShaderiv CL_OpenGL::functions->getShaderiv
#define clGetShaderInfoLog CL_OpenGL::functions->getShaderInfoLog
#define clGetShaderSource CL_OpenGL::functions->getShaderSource
#define clGetUniformLocation CL_OpenGL::functions->getUniformLocation
#define clGetUniformfv CL_OpenGL::functions->getUniformfv
#define clGetUniformiv CL_OpenGL::functions->getUniformiv
#define clGetVertexAttribdv CL_OpenGL::functions->getVertexAttribdv
#define clGetVertexAttribfv CL_OpenGL::functions->getVertexAttribfv
#define clGetVertexAttribiv CL_OpenGL::functions->getVertexAttribiv
#define clGetVertexAttribPointerv CL_OpenGL::functions->getVertexAttribPointerv
#define clIsProgram CL_OpenGL::functions->isProgram
#define clIsShader CL_OpenGL::functions->isShader
#define clLinkProgram CL_OpenGL::functions->linkProgram
#define clShaderSource CL_OpenGL::functions->shaderSource
#define clUseProgram CL_OpenGL::functions->useProgram
#define clUniform1f CL_OpenGL::functions->uniform1f
#define clUniform2f CL_OpenGL::functions->uniform2f
#define clUniform3f CL_OpenGL::functions->uniform3f
#define clUniform4f CL_OpenGL::functions->uniform4f
#define clUniform1i CL_OpenGL::functions->uniform1i
#define clUniform2i CL_OpenGL::functions->uniform2i
#define clUniform3i CL_OpenGL::functions->uniform3i
#define clUniform4i CL_OpenGL::functions->uniform4i
#define clUniform1fv CL_OpenGL::functions->uniform1fv
#define clUniform2fv CL_OpenGL::functions->uniform2fv
#define clUniform3fv CL_OpenGL::functions->uniform3fv
#define clUniform4fv CL_OpenGL::functions->uniform4fv
#define clUniform1iv CL_OpenGL::functions->uniform1iv
#define clUniform2iv CL_OpenGL::functions->uniform2iv
#define clUniform3iv CL_OpenGL::functions->uniform3iv
#define clUniform4iv CL_OpenGL::functions->uniform4iv
#define clUniformMatrix2fv CL_OpenGL::functions->uniformMatrix2fv
#define clUniformMatrix3fv CL_OpenGL::functions->uniformMatrix3fv
#define clUniformMatrix4fv CL_OpenGL::functions->uniformMatrix4fv
#define clValidateProgram CL_OpenGL::functions->validateProgram
#define clVertexAttrib1d CL_OpenGL::functions->vertexAttrib1d
#define clVertexAttrib1dv CL_OpenGL::functions->vertexAttrib1dv
#define clVertexAttrib1f CL_OpenGL::functions->vertexAttrib1f
#define clVertexAttrib1fv CL_OpenGL::functions->vertexAttrib1fv
#define clVertexAttrib1s CL_OpenGL::functions->vertexAttrib1s
#define clVertexAttrib1sv CL_OpenGL::functions->vertexAttrib1sv
#define clVertexAttrib2d CL_OpenGL::functions->vertexAttrib2d
#define clVertexAttrib2dv CL_OpenGL::functions->vertexAttrib2dv
#define clVertexAttrib2f CL_OpenGL::functions->vertexAttrib2f
#define clVertexAttrib2fv CL_OpenGL::functions->vertexAttrib2fv
#define clVertexAttrib2s CL_OpenGL::functions->vertexAttrib2s
#define clVertexAttrib2sv CL_OpenGL::functions->vertexAttrib2sv
#define clVertexAttrib3d CL_OpenGL::functions->vertexAttrib3d
#define clVertexAttrib3dv CL_OpenGL::functions->vertexAttrib3dv
#define clVertexAttrib3f CL_OpenGL::functions->vertexAttrib3f
#define clVertexAttrib3fv CL_OpenGL::functions->vertexAttrib3fv
#define clVertexAttrib3s CL_OpenGL::functions->vertexAttrib3s
#define clVertexAttrib3sv CL_OpenGL::functions->vertexAttrib3sv
#define clVertexAttrib4Nbv CL_OpenGL::functions->vertexAttrib4Nbv
#define clVertexAttrib4Niv CL_OpenGL::functions->vertexAttrib4Niv
#define clVertexAttrib4Nsv CL_OpenGL::functions->vertexAttrib4Nsv
#define clVertexAttrib4Nub CL_OpenGL::functions->vertexAttrib4Nub
#define clVertexAttrib4Nubv CL_OpenGL::functions->vertexAttrib4Nubv
#define clVertexAttrib4Nuiv CL_OpenGL::functions->vertexAttrib4Nuiv
#define clVertexAttrib4Nusv CL_OpenGL::functions->vertexAttrib4Nusv
#define clVertexAttrib4bv CL_OpenGL::functions->vertexAttrib4bv
#define clVertexAttrib4d CL_OpenGL::functions->vertexAttrib4d
#define clVertexAttrib4dv CL_OpenGL::functions->vertexAttrib4dv
#define clVertexAttrib4f CL_OpenGL::functions->vertexAttrib4f
#define clVertexAttrib4fv CL_OpenGL::functions->vertexAttrib4fv
#define clVertexAttrib4iv CL_OpenGL::functions->vertexAttrib4iv
#define clVertexAttrib4s CL_OpenGL::functions->vertexAttrib4s
#define clVertexAttrib4sv CL_OpenGL::functions->vertexAttrib4sv
#define clVertexAttrib4ubv CL_OpenGL::functions->vertexAttrib4ubv
#define clVertexAttrib4uiv CL_OpenGL::functions->vertexAttrib4uiv
#define clVertexAttrib4usv CL_OpenGL::functions->vertexAttrib4usv
#define clVertexAttribPointer CL_OpenGL::functions->vertexAttribPointer
#define clUniformMatrix2x3fv CL_OpenGL::functions->uniformMatrix2x3fv
#define clUniformMatrix3x2fv CL_OpenGL::functions->uniformMatrix3x2fv
#define clUniformMatrix2x4fv CL_OpenGL::functions->uniformMatrix2x4fv
#define clUniformMatrix4x2fv CL_OpenGL::functions->uniformMatrix4x2fv
#define clUniformMatrix3x4fv CL_OpenGL::functions->uniformMatrix3x4fv
#define clUniformMatrix4x3fv CL_OpenGL::functions->uniformMatrix4x3fv
#define clColorMaski CL_OpenGL::functions->colorMaski
#define clGetBooleani_v CL_OpenGL::functions->getBooleani_v
#define clGetIntegeri_v CL_OpenGL::functions->getIntegeri_v
#define clEnablei CL_OpenGL::functions->enablei
#define clDisablei CL_OpenGL::functions->disablei
#define clIsEnabledi CL_OpenGL::functions->isEnabledi
#define clBeginTransformFeedback CL_OpenGL::functions->beginTransformFeedback
#define clEndTransformFeedback CL_OpenGL::functions->endTransformFeedback
#define clBindBufferRange CL_OpenGL::functions->bindBufferRange
#define clBindBufferBase CL_OpenGL::functions->bindBufferBase
#define clTransformFeedbackVaryings CL_OpenGL::functions->transformFeedbackVaryings
#define clGetTransformFeedbackVarying CL_OpenGL::functions->getTransformFeedbackVarying
#define clClampColor CL_OpenGL::functions->clampColor
#define clBeginConditionalRender CL_OpenGL::functions->beginConditionalRender
#define clEndConditionalRender CL_OpenGL::functions->endConditionalRender
#define clVertexAttribIPointer CL_OpenGL::functions->vertexAttribIPointer
#define clGetVertexAttribIiv CL_OpenGL::functions->getVertexAttribIiv
#define clGetVertexAttribIuiv CL_OpenGL::functions->getVertexAttribIuiv
#define clVertexAttribI1i CL_OpenGL::functions->vertexAttribI1i
#define clVertexAttribI2i CL_OpenGL::functions->vertexAttribI2i
#define clVertexAttribI3i CL_OpenGL::functions->vertexAttribI3i
#define clVertexAttribI4i CL_OpenGL::functions->vertexAttribI4i
#define clVertexAttribI1ui CL_OpenGL::functions->vertexAttribI1ui
#define clVertexAttribI2ui CL_OpenGL::functions->vertexAttribI2ui
#define clVertexAttribI3ui CL_OpenGL::functions->vertexAttribI3ui
#define clVertexAttribI4ui CL_OpenGL::functions->vertexAttribI4ui
#define clVertexAttribI1iv CL_OpenGL::functions->vertexAttribI1iv
#define clVertexAttribI2iv CL_OpenGL::functions->vertexAttribI2iv
#define clVertexAttribI3iv CL_OpenGL::functions->vertexAttribI3iv
#define clVertexAttribI4iv CL_OpenGL::functions->vertexAttribI4iv
#define clVertexAttribI1uiv CL_OpenGL::functions->vertexAttribI1uiv
#define clVertexAttribI2uiv CL_OpenGL::functions->vertexAttribI2uiv
#define clVertexAttribI3uiv CL_OpenGL::functions->vertexAttribI3uiv
#define clVertexAttribI4uiv CL_OpenGL::functions->vertexAttribI4uiv
#define clVertexAttribI4bv CL_OpenGL::functions->vertexAttribI4bv
#define clVertexAttribI4sv CL_OpenGL::functions->vertexAttribI4sv
#define clVertexAttribI4ubv CL_OpenGL::functions->vertexAttribI4ubv
#define clVertexAttribI4usv CL_OpenGL::functions->vertexAttribI4usv
#define clGetUniformuiv CL_OpenGL::functions->getUniformuiv
#define clBindFragDataLocation CL_OpenGL::functions->bindFragDataLocation
#define clGetFragDataLocation CL_OpenGL::functions->getFragDataLocation
#define clUniform1ui CL_OpenGL::functions->uniform1ui
#define clUniform2ui CL_OpenGL::functions->uniform2ui
#define clUniform3ui CL_OpenGL::functions->uniform3ui
#define clUniform4ui CL_OpenGL::functions->uniform4ui
#define clUniform1uiv CL_OpenGL::functions->uniform1uiv
#define clUniform2uiv CL_OpenGL::functions->uniform2uiv
#define clUniform3uiv CL_OpenGL::functions->uniform3uiv
#define clUniform4uiv CL_OpenGL::functions->uniform4uiv
#define clTexParameterIiv CL_OpenGL::functions->texParameterIiv
#define clTexParameterIuiv CL_OpenGL::functions->texParameterIuiv
#define clGetTexParameterIiv CL_OpenGL::functions->getTexParameterIiv
#define clGetTexParameterIuiv CL_OpenGL::functions->getTexParameterIuiv
#define clClearBufferiv CL_OpenGL::functions->clearBufferiv
#define clClearBufferuiv CL_OpenGL::functions->clearBufferuiv
#define clClearBufferfv CL_OpenGL::functions->clearBufferfv
#define clClearBufferfi CL_OpenGL::functions->clearBufferfi
#define clGetStringi CL_OpenGL::functions->getStringi
#define clDrawArraysInstanced CL_OpenGL::functions->drawArraysInstanced
#define clDrawElementsInstanced CL_OpenGL::functions->drawElementsInstanced
#define clTexBuffer CL_OpenGL::functions->texBuffer
#define clPrimitiveRestartIndex CL_OpenGL::functions->primitiveRestartIndex
#define clGetInteger64i_v CL_OpenGL::functions->getInteger64i_v
#define clGetBufferParameteri64v CL_OpenGL::functions->getBufferParameteri64v
#define clProgramParameteri CL_OpenGL::functions->programParameteri
#define clFramebufferTexture CL_OpenGL::functions->framebufferTexture
#define clIsRenderbuffer CL_OpenGL::functions->isRenderbuffer
#define clBindRenderbuffer CL_OpenGL::functions->bindRenderbuffer
#define clDeleteRenderbuffers CL_OpenGL::functions->deleteRenderbuffers
#define clGenRenderbuffers CL_OpenGL::functions->genRenderbuffers
#define clRenderbufferStorage CL_OpenGL::functions->renderbufferStorage
#define clGetRenderbufferParameteriv CL_OpenGL::functions->getRenderbufferParameteriv
#define clIsFramebuffer CL_OpenGL::functions->isFramebuffer
#define clBindFramebuffer CL_OpenGL::functions->bindFramebuffer
#define clDeleteFramebuffers CL_OpenGL::functions->deleteFramebuffers
#define clGenFramebuffers CL_OpenGL::functions->genFramebuffers
#define clCheckFramebufferStatus CL_OpenGL::functions->checkFramebufferStatus
#define clFramebufferTexture1D CL_OpenGL::functions->framebufferTexture1D
#define clFramebufferTexture2D CL_OpenGL::functions->framebufferTexture2D
#define clFramebufferTexture3D CL_OpenGL::functions->framebufferTexture3D
#define clFramebufferRenderbuffer CL_OpenGL::functions->framebufferRenderbuffer
#define clGetFramebufferAttachmentParameteriv CL_OpenGL::functions->getFramebufferAttachmentParameteriv
#define clGenerateMipmap CL_OpenGL::functions->generateMipmap
#define clBlitFramebuffer CL_OpenGL::functions->blitFramebuffer
#define clRenderbufferStorageMultisample CL_OpenGL::functions->renderbufferStorageMultisample
#define clFramebufferTextureLayer CL_OpenGL::functions->framebufferTextureLayer
#define clMapBufferRange CL_OpenGL::functions->mapBufferRange
#define clFlushMappedBufferRange CL_OpenGL::functions->flushMappedBufferRange
#define clBindVertexArray CL_OpenGL::functions->bindVertexArray
#define clDeleteVertexArrays CL_OpenGL::functions->deleteVertexArrays
#define clGenVertexArrays CL_OpenGL::functions->genVertexArrays
#define clIsVertexArray CL_OpenGL::functions->isVertexArray
#define clGetUniformIndices CL_OpenGL::functions->getUniformIndices
#define clGetActiveUniformsiv CL_OpenGL::functions->getActiveUniformsiv
#define clGetActiveUniformName CL_OpenGL::functions->getActiveUniformName
#define clGetUniformBlockIndex CL_OpenGL::functions->getUniformBlockIndex
#define clGetActiveUniformBlockiv CL_OpenGL::functions->getActiveUniformBlockiv
#define clGetActiveUniformBlockName CL_OpenGL::functions->getActiveUniformBlockName
#define clUniformBlockBinding CL_OpenGL::functions->uniformBlockBinding
#define clCopyBufferSubData CL_OpenGL::functions->copyBufferSubData
#define clDrawElementsBaseVertex CL_OpenGL::functions->drawElementsBaseVertex
#define clDrawRangeElementsBaseVertex CL_OpenGL::functions->drawRangeElementsBaseVertex
#define clDrawElementsInstancedBaseVertex CL_OpenGL::functions->drawElementsInstancedBaseVertex
#define clMultiDrawElementsBaseVertex CL_OpenGL::functions->multiDrawElementsBaseVertex
#define clProvokingVertex CL_OpenGL::functions->provokingVertex
#define clFenceSync CL_OpenGL::functions->fenceSync
#define clIsSync CL_OpenGL::functions->isSync
#define clDeleteSync CL_OpenGL::functions->deleteSync
#define clClientWaitSync CL_OpenGL::functions->clientWaitSync
#define clWaitSync CL_OpenGL::functions->waitSync
#define clGetInteger64v CL_OpenGL::functions->getInteger64v
#define clGetSynciv CL_OpenGL::functions->getSynciv
#define clTexImage2DMultisample CL_OpenGL::functions->texImage2DMultisample
#define clTexImage3DMultisample CL_OpenGL::functions->texImage3DMultisample
#define clGetMultisamplefv CL_OpenGL::functions->getMultisamplefv
#define clSampleMaski CL_OpenGL::functions->sampleMaski
#define clBlendEquationi CL_OpenGL::functions->blendEquationi
#define clBlendEquationSeparatei CL_OpenGL::functions->blendEquationSeparatei
#define clBlendFunci CL_OpenGL::functions->blendFunci
#define clBlendFuncSeparatei CL_OpenGL::functions->blendFuncSeparatei
#define clMinSampleShading CL_OpenGL::functions->minSampleShading
#define clBindFragDataLocationIndexed CL_OpenGL::functions->bindFragDataLocationIndexed
#define clGetFragDataIndex CL_OpenGL::functions->getFragDataIndex
#define clGenSamplers CL_OpenGL::functions->genSamplers
#define clDeleteSamplers CL_OpenGL::functions->deleteSamplers
#define clIsSampler CL_OpenGL::functions->isSampler
#define clBindSampler CL_OpenGL::functions->bindSampler
#define clSamplerParameteri CL_OpenGL::functions->samplerParameteri
#define clSamplerParameteriv CL_OpenGL::functions->samplerParameteriv
#define clSamplerParameterf CL_OpenGL::functions->samplerParameterf
#define clSamplerParameterfv CL_OpenGL::functions->samplerParameterfv
#define clSamplerParameterIiv CL_OpenGL::functions->samplerParameterIiv
#define clSamplerParameterIuiv CL_OpenGL::functions->samplerParameterIuiv
#define clGetSamplerParameteriv CL_OpenGL::functions->getSamplerParameteriv
#define clGetSamplerParameterIiv CL_OpenGL::functions->getSamplerParameterIiv
#define clGetSamplerParameterfv CL_OpenGL::functions->getSamplerParameterfv
#define clGetSamplerParameterIfv CL_OpenGL::functions->getSamplerParameterIfv
#define clQueryCounter CL_OpenGL::functions->queryCounter
#define clGetQueryObjecti64v CL_OpenGL::functions->getQueryObjecti64v
#define clGetQueryObjectui64v CL_OpenGL::functions->getQueryObjectui64v
#define clVertexP2ui CL_OpenGL::functions->vertexP2ui
#define clVertexP2uiv CL_OpenGL::functions->vertexP2uiv
#define clVertexP3ui CL_OpenGL::functions->vertexP3ui
#define clVertexP3uiv CL_OpenGL::functions->vertexP3uiv
#define clVertexP4ui CL_OpenGL::functions->vertexP4ui
#define clVertexP4uiv CL_OpenGL::functions->vertexP4uiv
#define clTexCoordP1ui CL_OpenGL::functions->texCoordP1ui
#define clTexCoordP1uiv CL_OpenGL::functions->texCoordP1uiv
#define clTexCoordP2ui CL_OpenGL::functions->texCoordP2ui
#define clTexCoordP2uiv CL_OpenGL::functions->texCoordP2uiv
#define clTexCoordP3ui CL_OpenGL::functions->texCoordP3ui
#define clTexCoordP3uiv CL_OpenGL::functions->texCoordP3uiv
#define clTexCoordP4ui CL_OpenGL::functions->texCoordP4ui
#define clTexCoordP4uiv CL_OpenGL::functions->texCoordP4uiv
#define clMultiTexCoordP1ui CL_OpenGL::functions->multiTexCoordP1ui
#define clMultiTexCoordP1uiv CL_OpenGL::functions->multiTexCoordP1uiv
#define clMultiTexCoordP2ui CL_OpenGL::functions->multiTexCoordP2ui
#define clMultiTexCoordP2uiv CL_OpenGL::functions->multiTexCoordP2uiv
#define clMultiTexCoordP3ui CL_OpenGL::functions->multiTexCoordP3ui
#define clMultiTexCoordP3uiv CL_OpenGL::functions->multiTexCoordP3uiv
#define clMultiTexCoordP4ui CL_OpenGL::functions->multiTexCoordP4ui
#define clMultiTexCoordP4uiv CL_OpenGL::functions->multiTexCoordP4uiv
#define clNormalP3ui CL_OpenGL::functions->normalP3ui
#define clNormalP3uiv CL_OpenGL::functions->normalP3uiv
#define clColorP3ui CL_OpenGL::functions->colorP3ui
#define clColorP3uiv CL_OpenGL::functions->colorP3uiv
#define clColorP4ui CL_OpenGL::functions->colorP4ui
#define clColorP4uiv CL_OpenGL::functions->colorP4uiv
#define clSecondaryColorP3ui CL_OpenGL::functions->secondaryColorP3ui
#define clSecondaryColorP3uiv CL_OpenGL::functions->secondaryColorP3uiv
#define clVertexAttribP1ui CL_OpenGL::functions->vertexAttribP1ui
#define clVertexAttribP1uiv CL_OpenGL::functions->vertexAttribP1uiv
#define clVertexAttribP2ui CL_OpenGL::functions->vertexAttribP2ui
#define clVertexAttribP2uiv CL_OpenGL::functions->vertexAttribP2uiv
#define clVertexAttribP3ui CL_OpenGL::functions->vertexAttribP3ui
#define clVertexAttribP3uiv CL_OpenGL::functions->vertexAttribP3uiv
#define clVertexAttribP4ui CL_OpenGL::functions->vertexAttribP4ui
#define clVertexAttribP4uiv CL_OpenGL::functions->vertexAttribP4uiv
#define clDrawArraysIndirect CL_OpenGL::functions->drawArraysIndirect
#define clDrawElementsIndirect CL_OpenGL::functions->drawElementsIndirect
#define clUniform1d CL_OpenGL::functions->uniform1d
#define clUniform2d CL_OpenGL::functions->uniform2d
#define clUniform3d CL_OpenGL::functions->uniform3d
#define clUniform4d CL_OpenGL::functions->uniform4d
#define clUniform1dv CL_OpenGL::functions->uniform1dv
#define clUniform2dv CL_OpenGL::functions->uniform2dv
#define clUniform3dv CL_OpenGL::functions->uniform3dv
#define clUniform4dv CL_OpenGL::functions->uniform4dv
#define clUniformMatrix2dv CL_OpenGL::functions->uniformMatrix2dv
#define clUniformMatrix3dv CL_OpenGL::functions->uniformMatrix3dv
#define clUniformMatrix4dv CL_OpenGL::functions->uniformMatrix4dv
#define clUniformMatrix2x3dv CL_OpenGL::functions->uniformMatrix2x3dv
#define clUniformMatrix2x4dv CL_OpenGL::functions->uniformMatrix2x4dv
#define clUniformMatrix3x2dv CL_OpenGL::functions->uniformMatrix3x2dv
#define clUniformMatrix3x4dv CL_OpenGL::functions->uniformMatrix3x4dv
#define clUniformMatrix4x2dv CL_OpenGL::functions->uniformMatrix4x2dv
#define clUniformMatrix4x3dv CL_OpenGL::functions->uniformMatrix4x3dv
#define clGetUniformdv CL_OpenGL::functions->getUniformdv
#define clProgramUniform1dEXT CL_OpenGL::functions->programUniform1dEXT
#define clProgramUniform2dEXT CL_OpenGL::functions->programUniform2dEXT
#define clProgramUniform3dEXT CL_OpenGL::functions->programUniform3dEXT
#define clProgramUniform4dEXT CL_OpenGL::functions->programUniform4dEXT
#define clProgramUniform1dvEXT CL_OpenGL::functions->programUniform1dvEXT
#define clProgramUniform2dvEXT CL_OpenGL::functions->programUniform2dvEXT
#define clProgramUniform3dvEXT CL_OpenGL::functions->programUniform3dvEXT
#define clProgramUniform4dvEXT CL_OpenGL::functions->programUniform4dvEXT
#define clProgramUniformMatrix2dvEXT CL_OpenGL::functions->programUniformMatrix2dvEXT
#define clProgramUniformMatrix3dvEXT CL_OpenGL::functions->programUniformMatrix3dvEXT
#define clProgramUniformMatrix4dvEXT CL_OpenGL::functions->programUniformMatrix4dvEXT
#define clProgramUniformMatrix2x3dvEXT CL_OpenGL::functions->programUniformMatrix2x3dvEXT
#define clProgramUniformMatrix2x4dvEXT CL_OpenGL::functions->programUniformMatrix2x4dvEXT
#define clProgramUniformMatrix3x2dvEXT CL_OpenGL::functions->programUniformMatrix3x2dvEXT
#define clProgramUniformMatrix3x4dvEXT CL_OpenGL::functions->programUniformMatrix3x4dvEXT
#define clProgramUniformMatrix4x2dvEXT CL_OpenGL::functions->programUniformMatrix4x2dvEXT
#define clProgramUniformMatrix4x3dvEXT CL_OpenGL::functions->programUniformMatrix4x3dvEXT
#define clGetSubroutineUniformLocation CL_OpenGL::functions->getSubroutineUniformLocation
#define clGetSubroutineIndex CL_OpenGL::functions->getSubroutineIndex
#define clGetActiveSubroutineUniformiv CL_OpenGL::functions->getActiveSubroutineUniformiv
#define clGetActiveSubroutineUniformName CL_OpenGL::functions->getActiveSubroutineUniformName
#define clGetActiveSubroutineName CL_OpenGL::functions->getActiveSubroutineName
#define clUniformSubroutinesuiv CL_OpenGL::functions->uniformSubroutinesuiv
#define clGetUniformSubroutineuiv CL_OpenGL::functions->getUniformSubroutineuiv
#define clGetProgramStageiv CL_OpenGL::functions->getProgramStageiv
#define clPatchParameteri CL_OpenGL::functions->patchParameteri
#define clPatchParameterfv CL_OpenGL::functions->patchParameterfv
#define clBindTransformFeedback CL_OpenGL::functions->bindTransformFeedback
#define clDeleteTransformFeedbacks CL_OpenGL::functions->deleteTransformFeedbacks
#define clGenTransformFeedbacks CL_OpenGL::functions->genTransformFeedbacks
#define clIsTransformFeedback CL_OpenGL::functions->isTransformFeedback
#define clPauseTransformFeedback CL_OpenGL::functions->pauseTransformFeedback
#define clResumeTransformFeedback CL_OpenGL::functions->resumeTransformFeedback
#define clDrawTransformFeedback CL_OpenGL::functions->drawTransformFeedback
#define clDrawTransformFeedbackStream CL_OpenGL::functions->drawTransformFeedbackStream
#define clBeginQueryIndexed CL_OpenGL::functions->beginQueryIndexed
#define clEndQueryIndexed CL_OpenGL::functions->endQueryIndexed
#define clGetQueryIndexediv CL_OpenGL::functions->getQueryIndexediv

/// \}

/// \}


