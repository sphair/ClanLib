/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/// \}

/// \name Typedefs
/// \{

public:
	typedef void (CL_GLFUNC *ptr_glAccum)(CLenum op, CLfloat value);
	typedef void (CL_GLFUNC *ptr_glAlphaFunc)(CLenum func, CLclampf ref);
	typedef CLboolean (CL_GLFUNC *ptr_glAreTexturesResident)(CLsizei n, const CLuint *textures, CLboolean *residences);
	typedef void (CL_GLFUNC *ptr_glArrayElement)(CLint i);
	typedef void (CL_GLFUNC *ptr_glBegin)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glBindTexture)(CLenum target, CLuint texture);
	typedef void (CL_GLFUNC *ptr_glBitmap)(CLsizei width, CLsizei height, CLfloat xorig, CLfloat yorig, CLfloat xmove, CLfloat ymove, const CLubyte *bitmap);
	typedef void (CL_GLFUNC *ptr_glBlendFunc)(CLenum sfactor, CLenum dfactor);
	typedef void (CL_GLFUNC *ptr_glCallList)(CLuint list);
	typedef void (CL_GLFUNC *ptr_glCallLists)(CLsizei n, CLenum type, const CLvoid *lists);
	typedef void (CL_GLFUNC *ptr_glClear)(CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glClearAccum)(CLfloat red, CLfloat green, CLfloat blue, CLfloat alpha);
	typedef void (CL_GLFUNC *ptr_glClearColor)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glClearDepth)(CLclampd depth);
	typedef void (CL_GLFUNC *ptr_glClearIndex)(CLfloat c);
	typedef void (CL_GLFUNC *ptr_glClearStencil)(CLint s);
	typedef void (CL_GLFUNC *ptr_glClipPlane)(CLenum plane, const CLdouble *equation);
	typedef void (CL_GLFUNC *ptr_glColor3b)(CLbyte red, CLbyte green, CLbyte blue);
	typedef void (CL_GLFUNC *ptr_glColor3bv)(const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glColor3d)(CLdouble red, CLdouble green, CLdouble blue);
	typedef void (CL_GLFUNC *ptr_glColor3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glColor3f)(CLfloat red, CLfloat green, CLfloat blue);
	typedef void (CL_GLFUNC *ptr_glColor3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glColor3i)(CLint red, CLint green, CLint blue);
	typedef void (CL_GLFUNC *ptr_glColor3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glColor3s)(CLshort red, CLshort green, CLshort blue);
	typedef void (CL_GLFUNC *ptr_glColor3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glColor3ub)(CLubyte red, CLubyte green, CLubyte blue);
	typedef void (CL_GLFUNC *ptr_glColor3ubv)(const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glColor3ui)(CLuint red, CLuint green, CLuint blue);
	typedef void (CL_GLFUNC *ptr_glColor3uiv)(const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glColor3us)(CLushort red, CLushort green, CLushort blue);
	typedef void (CL_GLFUNC *ptr_glColor3usv)(const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glColor4b)(CLbyte red, CLbyte green, CLbyte blue, CLbyte alpha);
	typedef void (CL_GLFUNC *ptr_glColor4bv)(const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glColor4d)(CLdouble red, CLdouble green, CLdouble blue, CLdouble alpha);
	typedef void (CL_GLFUNC *ptr_glColor4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glColor4f)(CLfloat red, CLfloat green, CLfloat blue, CLfloat alpha);
	typedef void (CL_GLFUNC *ptr_glColor4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glColor4i)(CLint red, CLint green, CLint blue, CLint alpha);
	typedef void (CL_GLFUNC *ptr_glColor4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glColor4s)(CLshort red, CLshort green, CLshort blue, CLshort alpha);
	typedef void (CL_GLFUNC *ptr_glColor4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glColor4ub)(CLubyte red, CLubyte green, CLubyte blue, CLubyte alpha);
	typedef void (CL_GLFUNC *ptr_glColor4ubv)(const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glColor4ui)(CLuint red, CLuint green, CLuint blue, CLuint alpha);
	typedef void (CL_GLFUNC *ptr_glColor4uiv)(const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glColor4us)(CLushort red, CLushort green, CLushort blue, CLushort alpha);
	typedef void (CL_GLFUNC *ptr_glColor4usv)(const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glColorMask)(CLboolean red, CLboolean green, CLboolean blue, CLboolean alpha);
	typedef void (CL_GLFUNC *ptr_glColorMaterial)(CLenum face, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glColorPointer)(CLint size, CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glCopyPixels)(CLint x, CLint y, CLsizei width, CLsizei height, CLenum type);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage1D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage2D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLsizei height, CLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glCullFace)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glDeleteLists)(CLuint list, CLsizei range);
	typedef void (CL_GLFUNC *ptr_glDeleteTextures)(CLsizei n, const CLuint *textures);
	typedef void (CL_GLFUNC *ptr_glDepthFunc)(CLenum func);
	typedef void (CL_GLFUNC *ptr_glDepthMask)(CLboolean flag);
	typedef void (CL_GLFUNC *ptr_glDepthRange)(CLclampd zNear, CLclampd zFar);
	typedef void (CL_GLFUNC *ptr_glDisable)(CLenum cap);
	typedef void (CL_GLFUNC *ptr_glDisableClientState)(CLenum array);
	typedef void (CL_GLFUNC *ptr_glDrawArrays)(CLenum mode, CLint first, CLsizei count);
	typedef void (CL_GLFUNC *ptr_glDrawBuffer)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glDrawElements)(CLenum mode, CLsizei count, CLenum type, const CLvoid *indices);
	typedef void (CL_GLFUNC *ptr_glDrawPixels)(CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glEdgeFlag)(CLboolean flag);
	typedef void (CL_GLFUNC *ptr_glEdgeFlagPointer)(CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glEdgeFlagv)(const CLboolean *flag);
	typedef void (CL_GLFUNC *ptr_glEnable)(CLenum cap);
	typedef void (CL_GLFUNC *ptr_glEnableClientState)(CLenum array);
	typedef void (CL_GLFUNC *ptr_glEnd)(void);
	typedef void (CL_GLFUNC *ptr_glEndList)(void);
	typedef void (CL_GLFUNC *ptr_glEvalCoord1d)(CLdouble u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord1dv)(const CLdouble *u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord1f)(CLfloat u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord1fv)(const CLfloat *u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2d)(CLdouble u, CLdouble v);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2dv)(const CLdouble *u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2f)(CLfloat u, CLfloat v);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2fv)(const CLfloat *u);
	typedef void (CL_GLFUNC *ptr_glEvalMesh1)(CLenum mode, CLint i1, CLint i2);
	typedef void (CL_GLFUNC *ptr_glEvalMesh2)(CLenum mode, CLint i1, CLint i2, CLint j1, CLint j2);
	typedef void (CL_GLFUNC *ptr_glEvalPoint1)(CLint i);
	typedef void (CL_GLFUNC *ptr_glEvalPoint2)(CLint i, CLint j);
	typedef void (CL_GLFUNC *ptr_glFeedbackBuffer)(CLsizei size, CLenum type, CLfloat *buffer);
	typedef void (CL_GLFUNC *ptr_glFinish)(void);
	typedef void (CL_GLFUNC *ptr_glFlush)(void);
	typedef void (CL_GLFUNC *ptr_glFogf)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glFogfv)(CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glFogi)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glFogiv)(CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glFrontFace)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glFrustum)(CLdouble left, CLdouble right, CLdouble bottom, CLdouble top, CLdouble zNear, CLdouble zFar);
	typedef CLuint (CL_GLFUNC *ptr_glGenLists)(CLsizei range);
	typedef void (CL_GLFUNC *ptr_glGenTextures)(CLsizei n, CLuint *textures);
	typedef void (CL_GLFUNC *ptr_glGetBooleanv)(CLenum pname, CLboolean *params);
	typedef void (CL_GLFUNC *ptr_glGetClipPlane)(CLenum plane, CLdouble *equation);
	typedef void (CL_GLFUNC *ptr_glGetDoublev)(CLenum pname, CLdouble *params);
	typedef CLenum (CL_GLFUNC *ptr_glGetError)(void);
	typedef void (CL_GLFUNC *ptr_glGetFloatv)(CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetIntegerv)(CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetLightfv)(CLenum light, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetLightiv)(CLenum light, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetMapdv)(CLenum target, CLenum query, CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glGetMapfv)(CLenum target, CLenum query, CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glGetMapiv)(CLenum target, CLenum query, CLint *v);
	typedef void (CL_GLFUNC *ptr_glGetMaterialfv)(CLenum face, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetMaterialiv)(CLenum face, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetPixelMapfv)(CLenum map, CLfloat *values);
	typedef void (CL_GLFUNC *ptr_glGetPixelMapuiv)(CLenum map, CLuint *values);
	typedef void (CL_GLFUNC *ptr_glGetPixelMapusv)(CLenum map, CLushort *values);
	typedef void (CL_GLFUNC *ptr_glGetPointerv)(CLenum pname, CLvoid* *params);
	typedef void (CL_GLFUNC *ptr_glGetPolygonStipple)(CLubyte *mask);
	typedef const CLubyte *(CL_GLFUNC *ptr_glGetString)(CLenum name);
	typedef void (CL_GLFUNC *ptr_glGetTexEnvfv)(CLenum target, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexEnviv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexGendv)(CLenum coord, CLenum pname, CLdouble *params);
	typedef void (CL_GLFUNC *ptr_glGetTexGenfv)(CLenum coord, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexGeniv)(CLenum coord, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexImage)(CLenum target, CLint level, CLenum format, CLenum type, CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameterfv)(CLenum target, CLint level, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameteriv)(CLenum target, CLint level, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterfv)(CLenum target, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameteriv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glHint)(CLenum target, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glIndexMask)(CLuint mask);
	typedef void (CL_GLFUNC *ptr_glIndexPointer)(CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glIndexd)(CLdouble c);
	typedef void (CL_GLFUNC *ptr_glIndexdv)(const CLdouble *c);
	typedef void (CL_GLFUNC *ptr_glIndexf)(CLfloat c);
	typedef void (CL_GLFUNC *ptr_glIndexfv)(const CLfloat *c);
	typedef void (CL_GLFUNC *ptr_glIndexi)(CLint c);
	typedef void (CL_GLFUNC *ptr_glIndexiv)(const CLint *c);
	typedef void (CL_GLFUNC *ptr_glIndexs)(CLshort c);
	typedef void (CL_GLFUNC *ptr_glIndexsv)(const CLshort *c);
	typedef void (CL_GLFUNC *ptr_glIndexub)(CLubyte c);
	typedef void (CL_GLFUNC *ptr_glIndexubv)(const CLubyte *c);
	typedef void (CL_GLFUNC *ptr_glInitNames)(void);
	typedef void (CL_GLFUNC *ptr_glInterleavedArrays)(CLenum format, CLsizei stride, const CLvoid *pointer);
	typedef CLboolean (CL_GLFUNC *ptr_glIsEnabled)(CLenum cap);
	typedef CLboolean (CL_GLFUNC *ptr_glIsList)(CLuint list);
	typedef CLboolean (CL_GLFUNC *ptr_glIsTexture)(CLuint texture);
	typedef void (CL_GLFUNC *ptr_glLightModelf)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glLightModelfv)(CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glLightModeli)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glLightModeliv)(CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glLightf)(CLenum light, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glLightfv)(CLenum light, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glLighti)(CLenum light, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glLightiv)(CLenum light, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glLineStipple)(CLint factor, CLushort pattern);
	typedef void (CL_GLFUNC *ptr_glLineWidth)(CLfloat width);
	typedef void (CL_GLFUNC *ptr_glListBase)(CLuint base);
	typedef void (CL_GLFUNC *ptr_glLoadIdentity)(void);
	typedef void (CL_GLFUNC *ptr_glLoadMatrixd)(const CLdouble *m);
	typedef void (CL_GLFUNC *ptr_glLoadMatrixf)(const CLfloat *m);
	typedef void (CL_GLFUNC *ptr_glLoadName)(CLuint name);
	typedef void (CL_GLFUNC *ptr_glLogicOp)(CLenum opcode);
	typedef void (CL_GLFUNC *ptr_glMap1d)(CLenum target, CLdouble u1, CLdouble u2, CLint stride, CLint order, const CLdouble *points);
	typedef void (CL_GLFUNC *ptr_glMap1f)(CLenum target, CLfloat u1, CLfloat u2, CLint stride, CLint order, const CLfloat *points);
	typedef void (CL_GLFUNC *ptr_glMap2d)(CLenum target, CLdouble u1, CLdouble u2, CLint ustride, CLint uorder, CLdouble v1, CLdouble v2, CLint vstride, CLint vorder, const CLdouble *points);
	typedef void (CL_GLFUNC *ptr_glMap2f)(CLenum target, CLfloat u1, CLfloat u2, CLint ustride, CLint uorder, CLfloat v1, CLfloat v2, CLint vstride, CLint vorder, const CLfloat *points);
	typedef void (CL_GLFUNC *ptr_glMapGrid1d)(CLint un, CLdouble u1, CLdouble u2);
	typedef void (CL_GLFUNC *ptr_glMapGrid1f)(CLint un, CLfloat u1, CLfloat u2);
	typedef void (CL_GLFUNC *ptr_glMapGrid2d)(CLint un, CLdouble u1, CLdouble u2, CLint vn, CLdouble v1, CLdouble v2);
	typedef void (CL_GLFUNC *ptr_glMapGrid2f)(CLint un, CLfloat u1, CLfloat u2, CLint vn, CLfloat v1, CLfloat v2);
	typedef void (CL_GLFUNC *ptr_glMaterialf)(CLenum face, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glMaterialfv)(CLenum face, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glMateriali)(CLenum face, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glMaterialiv)(CLenum face, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glMatrixMode)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glMultMatrixd)(const CLdouble *m);
	typedef void (CL_GLFUNC *ptr_glMultMatrixf)(const CLfloat *m);
	typedef void (CL_GLFUNC *ptr_glNewList)(CLuint list, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glNormal3b)(CLbyte nx, CLbyte ny, CLbyte nz);
	typedef void (CL_GLFUNC *ptr_glNormal3bv)(const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glNormal3d)(CLdouble nx, CLdouble ny, CLdouble nz);
	typedef void (CL_GLFUNC *ptr_glNormal3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glNormal3f)(CLfloat nx, CLfloat ny, CLfloat nz);
	typedef void (CL_GLFUNC *ptr_glNormal3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glNormal3i)(CLint nx, CLint ny, CLint nz);
	typedef void (CL_GLFUNC *ptr_glNormal3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glNormal3s)(CLshort nx, CLshort ny, CLshort nz);
	typedef void (CL_GLFUNC *ptr_glNormal3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glNormalPointer)(CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glOrtho)(CLdouble left, CLdouble right, CLdouble bottom, CLdouble top, CLdouble zNear, CLdouble zFar);
	typedef void (CL_GLFUNC *ptr_glPassThrough)(CLfloat token);
	typedef void (CL_GLFUNC *ptr_glPixelMapfv)(CLenum map, CLint mapsize, const CLfloat *values);
	typedef void (CL_GLFUNC *ptr_glPixelMapuiv)(CLenum map, CLint mapsize, const CLuint *values);
	typedef void (CL_GLFUNC *ptr_glPixelMapusv)(CLenum map, CLint mapsize, const CLushort *values);
	typedef void (CL_GLFUNC *ptr_glPixelStoref)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glPixelStorei)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glPixelTransferf)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glPixelTransferi)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glPixelZoom)(CLfloat xfactor, CLfloat yfactor);
	typedef void (CL_GLFUNC *ptr_glPointSize)(CLfloat size);
	typedef void (CL_GLFUNC *ptr_glPolygonMode)(CLenum face, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glPolygonOffset)(CLfloat factor, CLfloat units);
	typedef void (CL_GLFUNC *ptr_glPolygonStipple)(const CLubyte *mask);
	typedef void (CL_GLFUNC *ptr_glPopAttrib)(void);
	typedef void (CL_GLFUNC *ptr_glPopClientAttrib)(void);
	typedef void (CL_GLFUNC *ptr_glPopMatrix)(void);
	typedef void (CL_GLFUNC *ptr_glPopName)(void);
	typedef void (CL_GLFUNC *ptr_glPrioritizeTextures)(CLsizei n, const CLuint *textures, const CLclampf *priorities);
	typedef void (CL_GLFUNC *ptr_glPushAttrib)(CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glPushClientAttrib)(CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glPushMatrix)(void);
	typedef void (CL_GLFUNC *ptr_glPushName)(CLuint name);
	typedef void (CL_GLFUNC *ptr_glRasterPos2d)(CLdouble x, CLdouble y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos2f)(CLfloat x, CLfloat y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos2i)(CLint x, CLint y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos2s)(CLshort x, CLshort y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3d)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3f)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3i)(CLint x, CLint y, CLint z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3s)(CLshort x, CLshort y, CLshort z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4d)(CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4f)(CLfloat x, CLfloat y, CLfloat z, CLfloat w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4i)(CLint x, CLint y, CLint z, CLint w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4s)(CLshort x, CLshort y, CLshort z, CLshort w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glReadBuffer)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glReadPixels)(CLint x, CLint y, CLsizei width, CLsizei height, CLenum format, CLenum type, CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glRectd)(CLdouble x1, CLdouble y1, CLdouble x2, CLdouble y2);
	typedef void (CL_GLFUNC *ptr_glRectdv)(const CLdouble *v1, const CLdouble *v2);
	typedef void (CL_GLFUNC *ptr_glRectf)(CLfloat x1, CLfloat y1, CLfloat x2, CLfloat y2);
	typedef void (CL_GLFUNC *ptr_glRectfv)(const CLfloat *v1, const CLfloat *v2);
	typedef void (CL_GLFUNC *ptr_glRecti)(CLint x1, CLint y1, CLint x2, CLint y2);
	typedef void (CL_GLFUNC *ptr_glRectiv)(const CLint *v1, const CLint *v2);
	typedef void (CL_GLFUNC *ptr_glRects)(CLshort x1, CLshort y1, CLshort x2, CLshort y2);
	typedef void (CL_GLFUNC *ptr_glRectsv)(const CLshort *v1, const CLshort *v2);
	typedef CLint (CL_GLFUNC *ptr_glRenderMode)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glRotated)(CLdouble angle, CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glRotatef)(CLfloat angle, CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glScaled)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glScalef)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glScissor)(CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glSelectBuffer)(CLsizei size, CLuint *buffer);
	typedef void (CL_GLFUNC *ptr_glShadeModel)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glStencilFunc)(CLenum func, CLint ref, CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilMask)(CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilMaskSeparate)(CLenum face, CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilOp)(CLenum fail, CLenum zfail, CLenum zpass);
	typedef void (CL_GLFUNC *ptr_glTexCoord1d)(CLdouble s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord1f)(CLfloat s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord1i)(CLint s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord1s)(CLshort s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2d)(CLdouble s, CLdouble t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2f)(CLfloat s, CLfloat t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2i)(CLint s, CLint t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2s)(CLshort s, CLshort t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3d)(CLdouble s, CLdouble t, CLdouble r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3f)(CLfloat s, CLfloat t, CLfloat r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3i)(CLint s, CLint t, CLint r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3s)(CLshort s, CLshort t, CLshort r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4d)(CLdouble s, CLdouble t, CLdouble r, CLdouble q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4f)(CLfloat s, CLfloat t, CLfloat r, CLfloat q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4i)(CLint s, CLint t, CLint r, CLint q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4s)(CLshort s, CLshort t, CLshort r, CLshort q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoordPointer)(CLint size, CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glTexEnvf)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexEnvfv)(CLenum target, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexEnvi)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glTexEnviv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexGend)(CLenum coord, CLenum pname, CLdouble param);
	typedef void (CL_GLFUNC *ptr_glTexGendv)(CLenum coord, CLenum pname, const CLdouble *params);
	typedef void (CL_GLFUNC *ptr_glTexGenf)(CLenum coord, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexGenfv)(CLenum coord, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexGeni)(CLenum coord, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glTexGeniv)(CLenum coord, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexImage1D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLint border, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexImage2D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLsizei height, CLint border, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexParameterf)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexParameterfv)(CLenum target, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexParameteri)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glTexParameteriv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLsizei width, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTranslated)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glTranslatef)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glVertex2d)(CLdouble x, CLdouble y);
	typedef void (CL_GLFUNC *ptr_glVertex2dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertex2f)(CLfloat x, CLfloat y);
	typedef void (CL_GLFUNC *ptr_glVertex2fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertex2i)(CLint x, CLint y);
	typedef void (CL_GLFUNC *ptr_glVertex2iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertex2s)(CLshort x, CLshort y);
	typedef void (CL_GLFUNC *ptr_glVertex2sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertex3d)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glVertex3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertex3f)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glVertex3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertex3i)(CLint x, CLint y, CLint z);
	typedef void (CL_GLFUNC *ptr_glVertex3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertex3s)(CLshort x, CLshort y, CLshort z);
	typedef void (CL_GLFUNC *ptr_glVertex3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertex4d)(CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GLFUNC *ptr_glVertex4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertex4f)(CLfloat x, CLfloat y, CLfloat z, CLfloat w);
	typedef void (CL_GLFUNC *ptr_glVertex4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertex4i)(CLint x, CLint y, CLint z, CLint w);
	typedef void (CL_GLFUNC *ptr_glVertex4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertex4s)(CLshort x, CLshort y, CLshort z, CLshort w);
	typedef void (CL_GLFUNC *ptr_glVertex4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexPointer)(CLint size, CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glViewport)(CLint x, CLint y, CLsizei width, CLsizei height);

	typedef void (CL_GLFUNC *ptr_glDrawRangeElementsEXT)(CLenum mode, CLuint start, CLuint end, CLsizei count, CLenum type, const CLvoid *indices);
	typedef void (CL_GLFUNC *ptr_glTexImage3DEXT)(CLenum target, CLint level, CLenum internalformat, CLsizei width, CLsizei height, CLsizei depth, CLint border, CLenum format, CLenum type, const CLvoid* pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage3DEXT)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLsizei width, CLsizei height, CLsizei depth, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage3DEXT)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glColorTableSGI)(CLenum target, CLenum internalformat, CLsizei width, CLenum format, CLenum type, const CLvoid* table);
	typedef void (CL_GLFUNC *ptr_glCopyColorTableSGI)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glColorTableParameterivSGI)(CLenum target, CLenum pname, const CLint* params);
	typedef void (CL_GLFUNC *ptr_glColorTableParameterfvSGI)(CLenum target, CLenum pname, const CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glGetColorTableSGI)(CLenum target, CLenum format, CLenum type, CLvoid* table);
	typedef void (CL_GLFUNC *ptr_glGetColorTableParameterivSGI)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetColorTableParameterfvSGI)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glColorSubTableEXT)(CLenum target, CLsizei start, CLsizei count, CLenum format, CLenum type, const CLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCopyColorSubTableEXT)(CLenum target, CLsizei start, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glConvolutionFilter1DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLenum format, CLenum type, const CLvoid* image);
	typedef void (CL_GLFUNC *ptr_glConvolutionFilter2DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid* image);
	typedef void (CL_GLFUNC *ptr_glCopyConvolutionFilter1DEXT)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glCopyConvolutionFilter2DEXT)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glGetConvolutionFilterEXT)(CLenum target, CLenum format, CLenum type, CLvoid* image);
	typedef void (CL_GLFUNC *ptr_glSeparableFilter2DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid* row, const CLvoid* column);
	typedef void (CL_GLFUNC *ptr_glGetSeparableFilterEXT)(CLenum target, CLenum format, CLenum type, CLvoid* row, CLvoid* column, CLvoid* span);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameteriEXT)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameterivEXT)(CLenum target, CLenum pname, const CLint* params);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameterfEXT)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameterfvEXT)(CLenum target, CLenum pname, const CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glGetConvolutionParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetConvolutionParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glHistogramEXT)(CLenum target, CLsizei width, CLenum internalformat, CLboolean sink);
	typedef void (CL_GLFUNC *ptr_glResetHistogramEXT)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetHistogramEXT)(CLenum target, CLboolean reset, CLenum format, CLenum type, CLvoid* values);
	typedef void (CL_GLFUNC *ptr_glGetHistogramParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetHistogramParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glMinmaxEXT)(CLenum target, CLenum internalformat, CLboolean sink);
	typedef void (CL_GLFUNC *ptr_glResetMinmaxEXT)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetMinmaxEXT)(CLenum target, CLboolean reset, CLenum format, CLenum type, CLvoid* values);
	typedef void (CL_GLFUNC *ptr_glGetMinmaxParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetMinmaxParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glBlendColorEXT)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glBlendEquationEXT)(CLenum mode);
	typedef CLvoid (CL_GLFUNC *ptr_glActiveTextureARB)(CLenum);
	typedef CLvoid (CL_GLFUNC *ptr_glClientActiveTextureARB)(CLenum);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1dARB)(CLenum, CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1fARB)(CLenum, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1iARB)(CLenum, CLint);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1sARB)(CLenum, CLshort);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord1svARB)(CLenum, CLshort const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2dARB)(CLenum, CLdouble, CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2fARB)(CLenum, CLfloat, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2iARB)(CLenum, CLint, CLint);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2sARB)(CLenum, CLshort, CLshort);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord2svARB)(CLenum, CLshort const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3dARB)(CLenum, CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3fARB)(CLenum, CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3iARB)(CLenum, CLint, CLint, CLint);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3sARB)(CLenum, CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord3svARB)(CLenum, CLshort const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4dARB)(CLenum, CLdouble, CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4fARB)(CLenum, CLfloat, CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4iARB)(CLenum, CLint, CLint, CLint, CLint);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4sARB)(CLenum, CLshort, CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiTexCoord4svARB)(CLenum, CLshort const *);

	typedef CLvoid (CL_GLFUNC *ptr_glCompressedTexImage1DARB)(CLenum, CLint, CLenum, CLsizei, CLint, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glCompressedTexImage2DARB)(CLenum, CLint, CLenum, CLsizei, CLsizei, CLint, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glCompressedTexImage3DARB)(CLenum, CLint, CLenum, CLsizei, CLsizei, CLsizei, CLint, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glCompressedTexSubImage1DARB)(CLenum, CLint, CLint, CLsizei, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glCompressedTexSubImage2DARB)(CLenum, CLint, CLint, CLint, CLsizei, CLsizei, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glCompressedTexSubImage3DARB)(CLenum, CLint, CLint, CLint, CLint, CLsizei, CLsizei, CLsizei, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glGetCompressedTexImageARB)(CLenum, CLint, CLvoid *);
	typedef CLvoid (CL_GLFUNC *ptr_glSampleCoverageARB)(CLclampf, CLboolean);
	typedef CLvoid (CL_GLFUNC *ptr_glLoadTransposeMatrixdARB)(CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glLoadTransposeMatrixfARB)(CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultTransposeMatrixdARB)(CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultTransposeMatrixfARB)(CLfloat const *);

	typedef CLvoid (CL_GLFUNC *ptr_glFogCoorddEXT)(CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glFogCoorddvEXT)(CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glFogCoordfEXT)(CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glFogCoordfvEXT)(CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glFogCoordPointerEXT)(CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiDrawArraysEXT)(CLenum, CLint *, CLsizei *, CLsizei);
	typedef CLvoid (CL_GLFUNC *ptr_glMultiDrawElementsEXT)(CLenum, CLsizei const *, CLenum, CLvoid const * *, CLsizei);
	typedef CLvoid (CL_GLFUNC *ptr_glPointParameterfARB)(CLenum, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glPointParameterfvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3bEXT)(CLbyte, CLbyte, CLbyte);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3bvEXT)(CLbyte const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3dEXT)(CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3dvEXT)(CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3fEXT)(CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3fvEXT)(CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3iEXT)(CLint, CLint, CLint);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3ivEXT)(CLint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3sEXT)(CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3svEXT)(CLshort const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3ubEXT)(CLubyte, CLubyte, CLubyte);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3ubvEXT)(CLubyte const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3uiEXT)(CLuint, CLuint, CLuint);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3uivEXT)(CLuint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3usEXT)(CLushort, CLushort, CLushort);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColor3usvEXT)(CLushort const *);
	typedef CLvoid (CL_GLFUNC *ptr_glSecondaryColorPointerEXT)(CLint, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GLFUNC *ptr_glBlendFuncSeparateEXT)(CLenum, CLenum, CLenum, CLenum);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2dARB)(CLdouble, CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2dvARB)(CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2fARB)(CLfloat, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2fvARB)(CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2iARB)(CLint, CLint);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2ivARB)(CLint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2sARB)(CLshort, CLshort);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos2svARB)(CLshort const *);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3dARB)(CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3dvARB)(CLdouble const *);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3fARB)(CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3fvARB)(CLfloat const *);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3iARB)(CLint, CLint, CLint);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3ivARB)(CLint const *);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3sARB)(CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GLFUNC *ptr_glWindowPos3svARB)(CLshort const *);

	typedef CLvoid (CL_GLFUNC *ptr_glBindBufferARB)(CLenum target, CLuint buffer);
	typedef CLvoid (CL_GLFUNC *ptr_glDeleteBuffersARB)(CLsizei n, const CLuint *buffers);
	typedef CLvoid (CL_GLFUNC *ptr_glGenBuffersARB)(CLsizei n, CLuint *buffers);
	typedef CLboolean (CL_GLFUNC *ptr_glIsBufferARB)(CLuint buffer);
	typedef CLvoid (CL_GLFUNC *ptr_glBufferDataARB)(CLenum target, CLsizeiptr size, const CLvoid *data, CLenum usage);
	typedef CLvoid (CL_GLFUNC *ptr_glBufferSubDataARB)(CLenum target, CLintptr offset, CLsizeiptr size, const CLvoid *data);
	typedef CLvoid (CL_GLFUNC *ptr_glGetBufferSubDataARB)(CLenum target, CLintptr offset, CLsizeiptr size, CLvoid *data);
	typedef CLvoid *(CL_GLFUNC *ptr_glMapBufferARB)(CLenum target, CLenum access);
	typedef CLboolean (CL_GLFUNC *ptr_glUnmapBufferARB)(CLenum target);
	typedef CLvoid (CL_GLFUNC *ptr_glGetBufferParameterivARB)(CLenum target, CLenum pname, CLint *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetBufferPointervARB)(CLenum target, CLenum pname, CLvoid **params);
	typedef CLvoid (CL_GLFUNC *ptr_glGenQueriesARB)(CLsizei n, CLuint *ids);
	typedef CLvoid (CL_GLFUNC *ptr_glDeleteQueriesARB)(CLsizei n, const CLuint *ids);
	typedef CLboolean (CL_GLFUNC *ptr_glIsQueryARB)(CLuint id);
	typedef CLvoid (CL_GLFUNC *ptr_glBeginQueryARB)(CLenum target, CLuint id);
	typedef CLvoid (CL_GLFUNC *ptr_glEndQueryARB)(CLenum target);
	typedef CLvoid (CL_GLFUNC *ptr_glGetQueryivARB)(CLenum target, CLenum pname, CLint *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetQueryObjectivARB)(CLuint id, CLenum pname, CLint *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetQueryObjectuivARB)(CLuint id, CLenum pname, CLuint *params);

	typedef CLvoid (CL_GLFUNC *ptr_glDeleteObjectARB)(CLuint obj);
	typedef CLuint (CL_GLFUNC *ptr_glGetHandleARB)(CLenum pname);
	typedef CLvoid (CL_GLFUNC *ptr_glDetachObjectARB)(CLuint containerObj, CLuint attachedObj);
	typedef CLuint (CL_GLFUNC *ptr_glCreateShaderObjectARB )(CLenum shaderType);
	typedef CLvoid (CL_GLFUNC *ptr_glShaderSourceARB)(CLuint shaderObj, CLsizei count, const CLchar* *string, const CLint *length);
	typedef CLvoid (CL_GLFUNC *ptr_glCompileShaderARB)(CLuint shaderObj);
	typedef CLuint (CL_GLFUNC *ptr_glCreateProgramObjectARB)();
	typedef CLvoid (CL_GLFUNC *ptr_glAttachObjectARB)(CLuint containerObj, CLuint attachedObj);
	typedef CLvoid (CL_GLFUNC *ptr_glLinkProgramARB)(CLuint programObj);
	typedef CLvoid (CL_GLFUNC *ptr_glUseProgramObjectARB)(CLuint programObj);
	typedef CLvoid (CL_GLFUNC *ptr_glValidateProgramARB)(CLuint programObj);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform1fARB)(CLint location, CLfloat v0);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform2fARB)(CLint location, CLfloat v0, CLfloat v1);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform3fARB)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform4fARB)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2, CLfloat v3);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform1iARB)(CLint location, CLint v0);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform2iARB)(CLint location, CLint v0, CLint v1);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform3iARB)(CLint location, CLint v0, CLint v1, CLint v2);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform4iARB)(CLint location, CLint v0, CLint v1, CLint v2, CLint v3);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform1fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform2fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform3fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform4fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform1ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform2ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform3ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniform4ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniformMatrix2fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniformMatrix3fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef CLvoid (CL_GLFUNC *ptr_glUniformMatrix4fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef CLvoid (CL_GLFUNC *ptr_glGetObjectParameterfvARB)(CLuint obj, CLenum pname, CLfloat *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetObjectParameterivARB)(CLuint obj, CLenum pname, CLint *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetInfoLogARB)(CLuint obj, CLsizei maxLength, CLsizei *length, CLchar *infoLog);
	typedef CLvoid (CL_GLFUNC *ptr_glGetAttachedObjectsARB)(CLuint containerObj, CLsizei maxCount, CLsizei *count, CLuint *obj);
	typedef CLint (CL_GLFUNC *ptr_glGetUniformLocationARB)(CLuint programObj, const CLchar *name);
	typedef CLvoid (CL_GLFUNC *ptr_glGetActiveUniformARB)(CLuint programObj, CLuint index, CLsizei maxLength, CLsizei *length, CLsizei *size, CLenum *type, CLchar *name);
	typedef CLvoid (CL_GLFUNC *ptr_glGetUniformfvARB)(CLuint programObj, CLint location, CLfloat *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetUniformivARB)(CLuint programObj, CLint location, CLint *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetShaderSourceARB)(CLuint obj, CLsizei maxLength, CLsizei *length, CLchar *source);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib1fARB)(CLuint index, CLfloat v0);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib1sARB)(CLuint index, CLshort v0);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib1dARB)(CLuint index, CLdouble v0);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib2fARB)(CLuint index, CLfloat v0, CLfloat v1);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib2sARB)(CLuint index, CLshort v0, CLshort v1);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib2dARB)(CLuint index, CLdouble v0, CLdouble v1);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib3fARB)(CLuint index, CLfloat v0, CLfloat v1, CLfloat v2);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib3sARB)(CLuint index, CLshort v0, CLshort v1, CLshort v2);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib3dARB)(CLuint index, CLdouble v0, CLdouble v1, CLdouble v2);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4fARB)(CLuint index, CLfloat v0, CLfloat v1, CLfloat v2, CLfloat v3);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4sARB)(CLuint index, CLshort v0, CLshort v1, CLshort v2, CLshort v3);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4dARB)(CLuint index, CLdouble v0, CLdouble v1, CLdouble v2, CLdouble v3);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4NubARB)(CLuint index, CLubyte x, CLubyte y, CLubyte z, CLubyte w);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib1fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib1svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib1dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib2fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib2svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib2dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib3fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib3svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib3dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4ivARB)(CLuint index, const CLint *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4bvARB)(CLuint index, const CLbyte *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4ubvARB)(CLuint index, const CLubyte *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4usvARB)(CLuint index, const CLushort *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4uivARB)(CLuint index, const CLuint *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4NbvARB)(CLuint index, const CLbyte *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4NsvARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4NivARB)(CLuint index, const CLint *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4NubvARB)(CLuint index, const CLubyte *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4NusvARB)(CLuint index, const CLushort *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttrib4NuivARB)(CLuint index, const CLuint *v);
	typedef CLvoid (CL_GLFUNC *ptr_glVertexAttribPointerARB)(CLuint index, CLint size, CLenum type, CLboolean normalized, CLsizei stride, const CLvoid *pointer);
	typedef CLvoid (CL_GLFUNC *ptr_glEnableVertexAttribArrayARB)(CLuint index);
	typedef CLvoid (CL_GLFUNC *ptr_glDisableVertexAttribArrayARB)(CLuint index);
	typedef CLvoid (CL_GLFUNC *ptr_glBindAttribLocationARB)(CLuint programObj, CLuint index, const char *name);
	typedef CLvoid (CL_GLFUNC *ptr_glGetActiveAttribARB)(CLuint programObj, CLuint index, CLsizei maxLength, CLsizei *length, CLint *size, CLenum *type, char *name);
	typedef CLint (CL_GLFUNC *ptr_glGetAttribLocationARB)(CLuint programObj, const char *name);
	typedef CLvoid (CL_GLFUNC *ptr_glGetVertexAttribdvARB)(CLuint index, CLenum pname, CLdouble *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetVertexAttribfvARB)(CLuint index, CLenum pname, CLfloat *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetVertexAttribivARB)(CLuint index, CLenum pname, CLint *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGetVertexAttribPointervARB)(CLuint index, CLenum pname, CLvoid **pointer);
	typedef CLvoid (CL_GLFUNC *ptr_glDrawBuffersARB)(CLsizei n, const CLenum *bufs);
	typedef CLvoid (CL_GLFUNC *ptr_glStencilFuncSeparateATI)(CLenum frontfunc, CLenum backfunc, CLint ref, CLuint mask);
	typedef CLvoid (CL_GLFUNC *ptr_glStencilOpSeparateATI)(CLenum face, CLenum sfail, CLenum dpfail, CLenum dppass);

	typedef CLboolean (CL_GLFUNC *ptr_glIsRenderbufferEXT)(CLuint renderbuffer);
	typedef CLvoid (CL_GLFUNC *ptr_glBindRenderbufferEXT)(CLenum target, CLuint renderbuffer);
	typedef CLvoid (CL_GLFUNC *ptr_glDeleteRenderbuffersEXT)(CLsizei n, const CLuint *renderbuffers);
	typedef CLvoid (CL_GLFUNC *ptr_glGenRenderbuffersEXT)(CLsizei n, CLuint *renderbuffers);
	typedef CLvoid (CL_GLFUNC *ptr_glRenderbufferStorageEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height);
	typedef CLvoid (CL_GLFUNC *ptr_glGetRenderbufferParameterivEXT)(CLenum target, CLenum pname, CLint *params);
	typedef CLboolean (CL_GLFUNC *ptr_glIsFramebufferEXT)(CLuint framebuffer);
	typedef CLvoid (CL_GLFUNC *ptr_glBindFramebufferEXT)(CLenum target, CLuint framebuffer);
	typedef CLvoid (CL_GLFUNC *ptr_glDeleteFramebuffersEXT)(CLsizei n, const CLuint *framebuffers);
	typedef CLvoid (CL_GLFUNC *ptr_glGenFramebuffersEXT)(CLsizei n, CLuint *framebuffers);
	typedef CLenum (CL_GLFUNC *ptr_glCheckFramebufferStatusEXT)(CLenum target);
	typedef CLvoid (CL_GLFUNC *ptr_glFramebufferTexture1DEXT)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level);
	typedef CLvoid (CL_GLFUNC *ptr_glFramebufferTexture2DEXT)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level);
	typedef CLvoid (CL_GLFUNC *ptr_glFramebufferTexture3DEXT)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level, CLint zoffset);
	typedef CLvoid (CL_GLFUNC *ptr_glFramebufferRenderbufferEXT)(CLenum target, CLenum attachment, CLenum renderbuffertarget, CLuint renderbuffer);
	typedef CLvoid (CL_GLFUNC *ptr_glGetFramebufferAttachmentParameterivEXT)(CLenum target, CLenum attachment, CLenum pname, CLint *params);
	typedef CLvoid (CL_GLFUNC *ptr_glGenerateMipmapEXT)(CLenum target);

	typedef CLvoid (CL_GLFUNC *ptr_glBlitFramebufferEXT)(CLint srcX0, CLint srcY0, CLint srcX1, CLint srcY1, CLint dstX0, CLint dstY0, CLint dstX1, CLint dstY1, CLbitfield mask, CLenum filter);

/// \}

/// \name Functions
/// \{

public:
	/// \brief glAccum binding
	ptr_glAccum accum;

	/// \brief glAplhaFunc binding
	ptr_glAlphaFunc alphaFunc;

	/// \brief glAreTexturesResident binding
	ptr_glAreTexturesResident areTexturesResident;

	/// \brief glArrayElement binding
	ptr_glArrayElement arrayElement;

	/// \brief glBegin binding
	ptr_glBegin begin;

	/// \brief glBindTexture binding
	ptr_glBindTexture bindTexture;

	/// \brief glBitmap binding
	ptr_glBitmap bitmap;

	/// \brief glBlendFunc binding
	ptr_glBlendFunc blendFunc;

	/// \brief glCallList binding
	ptr_glCallList callList;

	/// \brief glCallLists binding
	ptr_glCallLists callLists;

	/// \brief glClear binding
	ptr_glClear clear;

	/// \brief glClearAccum binding
	ptr_glClearAccum clearAccum;

	/// \brief glClearColor binding
	ptr_glClearColor clearColor;

	/// \brief glClearDepth binding
	ptr_glClearDepth clearDepth;

	/// \brief glClearIndex binding
	ptr_glClearIndex clearIndex;

	/// \brief glClearStencil binding
	ptr_glClearStencil clearStencil;

	/// \brief glClipPlane binding
	ptr_glClipPlane clipPlane;

	/// \brief glColor3b binding
	ptr_glColor3b color3b;

	/// \brief glColor3bv binding
	ptr_glColor3bv color3bv;

	/// \brief glColor3d binding
	ptr_glColor3d color3d;

	/// \brief glColor3dv binding
	ptr_glColor3dv color3dv;

	/// \brief glColor3f binding
	ptr_glColor3f color3f;

	/// \brief glColor3fv binding
	ptr_glColor3fv color3fv;

	/// \brief glColor3i binding
	ptr_glColor3i color3i;

	/// \brief glColor3iv binding
	ptr_glColor3iv color3iv;

	/// \brief glColor3s binding
	ptr_glColor3s color3s;

	/// \brief glColor3sv binding
	ptr_glColor3sv color3sv;

	/// \brief glColor3ub binding
	ptr_glColor3ub color3ub;

	/// \brief glColor3ubv binding
	ptr_glColor3ubv color3ubv;

	/// \brief glColor3ui binding
	ptr_glColor3ui color3ui;

	/// \brief glColor3uiv binding
	ptr_glColor3uiv color3uiv;

	/// \brief glColor3us binding
	ptr_glColor3us color3us;

	/// \brief glColor3usv binding
	ptr_glColor3usv color3usv;

	/// \brief glColor4b binding
	ptr_glColor4b color4b;

	/// \brief glColor4bv binding
	ptr_glColor4bv color4bv;

	/// \brief glColor4d binding
	ptr_glColor4d color4d;

	/// \brief glColor4dv binding
	ptr_glColor4dv color4dv;

	/// \brief glColor4f binding
	ptr_glColor4f color4f;

	/// \brief glColor4fv binding
	ptr_glColor4fv color4fv;

	/// \brief glColor4i binding
	ptr_glColor4i color4i;

	/// \brief glColor4iv binding
	ptr_glColor4iv color4iv;

	/// \brief glColor4s binding
	ptr_glColor4s color4s;

	/// \brief glColor4sv binding
	ptr_glColor4sv color4sv;

	/// \brief glColor4ub binding
	ptr_glColor4ub color4ub;

	/// \brief glColor4ubv binding
	ptr_glColor4ubv color4ubv;

	/// \brief glColor4ui binding
	ptr_glColor4ui color4ui;

	/// \brief glColor4uiv binding
	ptr_glColor4uiv color4uiv;

	/// \brief glColor4us binding
	ptr_glColor4us color4us;

	/// \brief glColor4usv binding
	ptr_glColor4usv color4usv;

	/// \brief glColorMask binding
	ptr_glColorMask colorMask;

	/// \brief glColorMaterial binding
	ptr_glColorMaterial colorMaterial;

	/// \brief glColorPointer binding
	ptr_glColorPointer colorPointer;

	/// \brief glCopyPixels binding
	ptr_glCopyPixels copyPixels;

	/// \brief glCopyTexImage1D binding
	ptr_glCopyTexImage1D copyTexImage1D;

	/// \brief glCopyTexImage2D binding
	ptr_glCopyTexImage2D copyTexImage2D;

	/// \brief glCopyTexSubImage1D binding
	ptr_glCopyTexSubImage1D copyTexSubImage1D;

	/// \brief glCopyTexSubImage2D binding
	ptr_glCopyTexSubImage2D copyTexSubImage2D;

	/// \brief glCullFace binding
	ptr_glCullFace cullFace;

	/// \brief glDeleteLists binding
	ptr_glDeleteLists deleteLists;

	/// \brief glDeleteTextures binding
	ptr_glDeleteTextures deleteTextures;

	/// \brief glDepthFunc binding
	ptr_glDepthFunc depthFunc;

	/// \brief glDepthMask binding
	ptr_glDepthMask depthMask;

	/// \brief glDepthRange binding
	ptr_glDepthRange depthRange;

	/// \brief glDisable binding
	ptr_glDisable disable;

	/// \brief glDisableClientState binding
	ptr_glDisableClientState disableClientState;

	/// \brief glDrawArrays binding
	ptr_glDrawArrays drawArrays;

	/// \brief glDrawBuffer binding
	ptr_glDrawBuffer drawBuffer;

	/// \brief glDrawElements binding
	ptr_glDrawElements drawElements;

	/// \brief glDrawPixels binding
	ptr_glDrawPixels drawPixels;

	/// \brief glEdgeFlag binding
	ptr_glEdgeFlag edgeFlag;

	/// \brief glEdgeFlagPointer binding
	ptr_glEdgeFlagPointer edgeFlagPointer;

	/// \brief glEdgeFlagv binding
	ptr_glEdgeFlagv edgeFlagv;

	/// \brief glEnable binding
	ptr_glEnable enable;

	/// \brief glEnableClientState binding
	ptr_glEnableClientState enableClientState;

	/// \brief glEnd binding
	ptr_glEnd end;

	/// \brief glEndList binding
	ptr_glEndList endList;

	/// \brief glEvalCoord1d binding
	ptr_glEvalCoord1d evalCoord1d;

	/// \brief glEvalCoord1dv binding
	ptr_glEvalCoord1dv evalCoord1dv;

	/// \brief glEvalCoord1f binding
	ptr_glEvalCoord1f evalCoord1f;

	/// \brief glEvalCoord1fv binding
	ptr_glEvalCoord1fv evalCoord1fv;

	/// \brief glEvalCoord2d binding
	ptr_glEvalCoord2d evalCoord2d;

	/// \brief glEvalCoord2dv binding
	ptr_glEvalCoord2dv evalCoord2dv;

	/// \brief glEvalCoord2f binding
	ptr_glEvalCoord2f evalCoord2f;

	/// \brief glEvalCoord2fv binding
	ptr_glEvalCoord2fv evalCoord2fv;

	/// \brief glEvalMesh1 binding
	ptr_glEvalMesh1 evalMesh1;

	/// \brief glEvalMesh2 binding
	ptr_glEvalMesh2 evalMesh2;

	/// \brief glEvalPoint1 binding
	ptr_glEvalPoint1 evalPoint1;

	/// \brief glEvalPoint2 binding
	ptr_glEvalPoint2 evalPoint2;

	/// \brief glFeedbackBuffer binding
	ptr_glFeedbackBuffer feedbackBuffer;

	/// \brief glFinish binding
	ptr_glFinish finish;

	/// \brief glFlush binding
	ptr_glFlush flush;

	/// \brief glFogf binding
	ptr_glFogf fogf;

	/// \brief glFogfv binding
	ptr_glFogfv fogfv;

	/// \brief glFogi binding
	ptr_glFogi fogi;

	/// \brief glFogiv binding
	ptr_glFogiv fogiv;

	/// \brief glFrontFace binding
	ptr_glFrontFace frontFace;

	/// \brief glFrustum binding
	ptr_glFrustum frustum;

	/// \brief glGenLists binding
	ptr_glGenLists genLists;

	/// \brief glGenTextures binding
	ptr_glGenTextures genTextures;

	/// \brief glGetBooleanv binding
	ptr_glGetBooleanv getBooleanv;

	/// \brief glGetClipPlane binding
	ptr_glGetClipPlane getClipPlane;

	/// \brief glGetDoublev binding
	ptr_glGetDoublev getDoublev;

	/// \brief glGetError binding
	ptr_glGetError getError;

	/// \brief glGetFloatv binding
	ptr_glGetFloatv getFloatv;

	/// \brief glGetIntegerv binding
	ptr_glGetIntegerv getIntegerv;

	/// \brief glGetLightfv binding
	ptr_glGetLightfv getLightfv;

	/// \brief glGetLightiv binding
	ptr_glGetLightiv getLightiv;

	/// \brief glGetMapdv binding
	ptr_glGetMapdv getMapdv;

	/// \brief glGetMapfv binding
	ptr_glGetMapfv getMapfv;

	/// \brief glGetMapiv binding
	ptr_glGetMapiv getMapiv;

	/// \brief glGetMaterialfv binding
	ptr_glGetMaterialfv getMaterialfv;

	/// \brief glGetMaterialiv binding
	ptr_glGetMaterialiv getMaterialiv;

	/// \brief glGetPixelMapfv binding
	ptr_glGetPixelMapfv getPixelMapfv;

	/// \brief glGetPixelMapuiv binding
	ptr_glGetPixelMapuiv getPixelMapuiv;

	/// \brief glGetPixelMapusv binding
	ptr_glGetPixelMapusv getPixelMapusv;

	/// \brief glGetPointerv binding
	ptr_glGetPointerv getPointerv;

	/// \brief glGetPolygonStipple binding
	ptr_glGetPolygonStipple getPolygonStipple;

	/// \brief glGetString binding
	ptr_glGetString getString;

	/// \brief glGetTexEnvfv binding
	ptr_glGetTexEnvfv getTexEnvfv;

	/// \brief glGetTexEnviv binding
	ptr_glGetTexEnviv getTexEnviv;

	/// \brief glGetTexGendv binding
	ptr_glGetTexGendv getTexGendv;

	/// \brief glGetTexGenfv binding
	ptr_glGetTexGenfv getTexGenfv;

	/// \brief glGetTexGeniv binding
	ptr_glGetTexGeniv getTexGeniv;

	/// \brief glGetTexImage binding
	ptr_glGetTexImage getTexImage;

	/// \brief glGetTexLevelParameterfv binding
	ptr_glGetTexLevelParameterfv getTexLevelParameterfv;

	/// \brief glGetTexLevelParameteriv binding
	ptr_glGetTexLevelParameteriv getTexLevelParameteriv;

	/// \brief glGetTexParameterfv binding
	ptr_glGetTexParameterfv getTexParameterfv;

	/// \brief glGetTexParameteriv binding
	ptr_glGetTexParameteriv getTexParameteriv;

	/// \brief glHint binding
	ptr_glHint hint;

	/// \brief glIndexMask binding
	ptr_glIndexMask indexMask;

	/// \brief glIndexPointer binding
	ptr_glIndexPointer indexPointer;

	/// \brief glIndexd binding
	ptr_glIndexd indexd;

	/// \brief glIndexdv binding
	ptr_glIndexdv indexdv;

	/// \brief glIndexf binding
	ptr_glIndexf indexf;

	/// \brief glIndexfv binding
	ptr_glIndexfv indexfv;

	/// \brief glIndexi binding
	ptr_glIndexi indexi;

	/// \brief glIndexiv binding
	ptr_glIndexiv indexiv;

	/// \brief glIndexs binding
	ptr_glIndexs indexs;

	/// \brief glIndexsv binding
	ptr_glIndexsv indexsv;

	/// \brief glIndexub binding
	ptr_glIndexub indexub;

	/// \brief glIndexubv binding
	ptr_glIndexubv indexubv;

	/// \brief glInitNames binding
	ptr_glInitNames initNames;

	/// \brief glInterleavedArrays binding
	ptr_glInterleavedArrays interleavedArrays;

	/// \brief glIsEnabled binding
	ptr_glIsEnabled isEnabled;

	/// \brief glIsList binding
	ptr_glIsList isList;

	/// \brief glIsTexture binding
	ptr_glIsTexture isTexture;

	/// \brief glLightModelf binding
	ptr_glLightModelf lightModelf;

	/// \brief glLightModelfv binding
	ptr_glLightModelfv lightModelfv;

	/// \brief glLightModeli binding
	ptr_glLightModeli lightModeli;

	/// \brief glLightModeliv binding
	ptr_glLightModeliv lightModeliv;

	/// \brief glLightf binding
	ptr_glLightf lightf;

	/// \brief glLightfv binding
	ptr_glLightfv lightfv;

	/// \brief glLighti binding
	ptr_glLighti lighti;

	/// \brief glLightiv binding
	ptr_glLightiv lightiv;

	/// \brief glLineStipple binding
	ptr_glLineStipple lineStipple;

	/// \brief glLineWidth binding
	ptr_glLineWidth lineWidth;

	/// \brief glListBase binding
	ptr_glListBase listBase;

	/// \brief glLoadIdentity binding
	ptr_glLoadIdentity loadIdentity;

	/// \brief glLoadMatrixd binding
	ptr_glLoadMatrixd loadMatrixd;

	/// \brief glLoadMatrixf binding
	ptr_glLoadMatrixf loadMatrixf;

	/// \brief glLoadName binding
	ptr_glLoadName loadName;

	/// \brief glLogicOp binding
	ptr_glLogicOp logicOp;

	/// \brief glMap1d binding
	ptr_glMap1d map1d;

	/// \brief glMap1f binding
	ptr_glMap1f map1f;

	/// \brief glMap2d binding
	ptr_glMap2d map2d;

	/// \brief glMap2f binding
	ptr_glMap2f map2f;

	/// \brief glMapGrid1d binding
	ptr_glMapGrid1d mapGrid1d;

	/// \brief glMapGrid1f binding
	ptr_glMapGrid1f mapGrid1f;

	/// \brief glMapGrid2d binding
	ptr_glMapGrid2d mapGrid2d;

	/// \brief glMapGrid2f binding
	ptr_glMapGrid2f mapGrid2f;

	/// \brief glMaterialf binding
	ptr_glMaterialf materialf;

	/// \brief glMaterialfv binding
	ptr_glMaterialfv materialfv;

	/// \brief glMateriali binding
	ptr_glMateriali materiali;

	/// \brief glMaterialiv binding
	ptr_glMaterialiv materialiv;

	/// \brief glMatrixMode binding
	ptr_glMatrixMode matrixMode;

	/// \brief glMultMatrixd binding
	ptr_glMultMatrixd multMatrixd;

	/// \brief glMultMatrixf binding
	ptr_glMultMatrixf multMatrixf;

	/// \brief glNewList binding
	ptr_glNewList newList;

	/// \brief glNormal3b binding
	ptr_glNormal3b normal3b;

	/// \brief glNormal3bv binding
	ptr_glNormal3bv normal3bv;

	/// \brief glNormal3d binding
	ptr_glNormal3d normal3d;

	/// \brief glNormal3dv binding
	ptr_glNormal3dv normal3dv;

	/// \brief glNormal3f binding
	ptr_glNormal3f normal3f;

	/// \brief glNormal3fv binding
	ptr_glNormal3fv normal3fv;

	/// \brief glNormal3i binding
	ptr_glNormal3i normal3i;

	/// \brief glNormal3iv binding
	ptr_glNormal3iv normal3iv;

	/// \brief glNormal3s binding
	ptr_glNormal3s normal3s;

	/// \brief glNormal3sv binding
	ptr_glNormal3sv normal3sv;

	/// \brief glNormalPointer binding
	ptr_glNormalPointer normalPointer;

	/// \brief glOrtho binding
	ptr_glOrtho ortho;

	/// \brief glPassThrough binding
	ptr_glPassThrough passThrough;

	/// \brief glPixelMapfv binding
	ptr_glPixelMapfv pixelMapfv;

	/// \brief glPixelMapuiv binding
	ptr_glPixelMapuiv pixelMapuiv;

	/// \brief glPixelMapusv binding
	ptr_glPixelMapusv pixelMapusv;

	/// \brief glPixelStoref binding
	ptr_glPixelStoref pixelStoref;

	/// \brief glPixelStorei binding
	ptr_glPixelStorei pixelStorei;

	/// \brief glPixelTransferf binding
	ptr_glPixelTransferf pixelTransferf;

	/// \brief glPixelTransferi binding
	ptr_glPixelTransferi pixelTransferi;

	/// \brief glPixelZoom binding
	ptr_glPixelZoom pixelZoom;

	/// \brief glPointSize binding
	ptr_glPointSize pointSize;

	/// \brief glPolygonMode binding
	ptr_glPolygonMode polygonMode;

	/// \brief glPolygonOffset binding
	ptr_glPolygonOffset polygonOffset;

	/// \brief glPolygonStipple binding
	ptr_glPolygonStipple polygonStipple;

	/// \brief glPopAttrib binding
	ptr_glPopAttrib popAttrib;

	/// \brief glPopClientAttrib binding
	ptr_glPopClientAttrib popClientAttrib;

	/// \brief glPopMatrix binding
	ptr_glPopMatrix popMatrix;

	/// \brief glPopName binding
	ptr_glPopName popName;

	/// \brief glPrioritizeTextures binding
	ptr_glPrioritizeTextures prioritizeTextures;

	/// \brief glPushAttrib binding
	ptr_glPushAttrib pushAttrib;

	/// \brief glPushClientAttrib binding
	ptr_glPushClientAttrib pushClientAttrib;

	/// \brief glPushMatrix binding
	ptr_glPushMatrix pushMatrix;

	/// \brief glPushName binding
	ptr_glPushName pushName;

	/// \brief glRasterPos2d binding
	ptr_glRasterPos2d rasterPos2d;

	/// \brief glRasterPos2dv binding
	ptr_glRasterPos2dv rasterPos2dv;

	/// \brief glRasterPos2f binding
	ptr_glRasterPos2f rasterPos2f;

	/// \brief glRasterPos2fv binding
	ptr_glRasterPos2fv rasterPos2fv;

	/// \brief glRasterPos2i binding
	ptr_glRasterPos2i rasterPos2i;

	/// \brief glRasterPos2iv binding
	ptr_glRasterPos2iv rasterPos2iv;

	/// \brief glRasterPos2s binding
	ptr_glRasterPos2s rasterPos2s;

	/// \brief glRasterPos2sv binding
	ptr_glRasterPos2sv rasterPos2sv;

	/// \brief glRasterPos3d binding
	ptr_glRasterPos3d rasterPos3d;

	/// \brief glRasterPos3dv binding
	ptr_glRasterPos3dv rasterPos3dv;

	/// \brief glRasterPos3f binding
	ptr_glRasterPos3f rasterPos3f;

	/// \brief glRasterPos3fv binding
	ptr_glRasterPos3fv rasterPos3fv;

	/// \brief glRasterPos3i binding
	ptr_glRasterPos3i rasterPos3i;

	/// \brief glRasterPos3iv binding
	ptr_glRasterPos3iv rasterPos3iv;

	/// \brief glRasterPos3s binding
	ptr_glRasterPos3s rasterPos3s;

	/// \brief glRasterPos3sv binding
	ptr_glRasterPos3sv rasterPos3sv;

	/// \brief glRasterPos4d binding
	ptr_glRasterPos4d rasterPos4d;

	/// \brief glRasterPos4dv binding
	ptr_glRasterPos4dv rasterPos4dv;

	/// \brief glRasterPos4f binding
	ptr_glRasterPos4f rasterPos4f;

	/// \brief glRasterPos4fv binding
	ptr_glRasterPos4fv rasterPos4fv;

	/// \brief glRasterPos4i binding
	ptr_glRasterPos4i rasterPos4i;

	/// \brief glRasterPos4iv binding
	ptr_glRasterPos4iv rasterPos4iv;

	/// \brief glRasterPos4s binding
	ptr_glRasterPos4s rasterPos4s;

	/// \brief glRasterPos4sv binding
	ptr_glRasterPos4sv rasterPos4sv;

	/// \brief glReadBuffer binding
	ptr_glReadBuffer readBuffer;

	/// \brief glReadPixels binding
	ptr_glReadPixels readPixels;

	/// \brief glRectd binding
	ptr_glRectd rectd;

	/// \brief glRectdv binding
	ptr_glRectdv rectdv;

	/// \brief glRectf binding
	ptr_glRectf rectf;

	/// \brief glRectfv binding
	ptr_glRectfv rectfv;

	/// \brief glRecti binding
	ptr_glRecti recti;

	/// \brief glRectiv binding
	ptr_glRectiv rectiv;

	/// \brief glRects binding
	ptr_glRects rects;

	/// \brief glRectsv binding
	ptr_glRectsv rectsv;

	/// \brief glRenderMode binding
	ptr_glRenderMode renderMode;

	/// \brief glRotated binding
	ptr_glRotated rotated;

	/// \brief glRotatef binding
	ptr_glRotatef rotatef;

	/// \brief glScaled binding
	ptr_glScaled scaled;

	/// \brief glScalef binding
	ptr_glScalef scalef;

	/// \brief glScissor binding
	ptr_glScissor scissor;

	/// \brief glSelectBuffer binding
	ptr_glSelectBuffer selectBuffer;

	/// \brief glShadeModel binding
	ptr_glShadeModel shadeModel;

	/// \brief glStencilFunc binding
	ptr_glStencilFunc stencilFunc;

	/// \brief glStencilMask binding
	ptr_glStencilMask stencilMask;

	/// \brief glStencilMaskSeparate binding
	ptr_glStencilMaskSeparate stencilMaskSeparate;

	/// \brief glStencilOp binding
	ptr_glStencilOp stencilOp;

	/// \brief glTexCoord1d binding
	ptr_glTexCoord1d texCoord1d;

	/// \brief glTexCoord1dv binding
	ptr_glTexCoord1dv texCoord1dv;

	/// \brief glTexCoord1f binding
	ptr_glTexCoord1f texCoord1f;

	/// \brief glTexCoord1fv binding
	ptr_glTexCoord1fv texCoord1fv;

	/// \brief glTexCoord1i binding
	ptr_glTexCoord1i texCoord1i;

	/// \brief glTexCoord1iv binding
	ptr_glTexCoord1iv texCoord1iv;

	/// \brief glTexCoord1s binding
	ptr_glTexCoord1s texCoord1s;

	/// \brief glTexCoord1sv binding
	ptr_glTexCoord1sv texCoord1sv;

	/// \brief glTexCoord2d binding
	ptr_glTexCoord2d texCoord2d;

	/// \brief glTexCoord2dv binding
	ptr_glTexCoord2dv texCoord2dv;

	/// \brief glTexCoord2f binding
	ptr_glTexCoord2f texCoord2f;

	/// \brief glTexCoord2fv binding
	ptr_glTexCoord2fv texCoord2fv;

	/// \brief glTexCoord2i binding
	ptr_glTexCoord2i texCoord2i;

	/// \brief glTexCoord2iv binding
	ptr_glTexCoord2iv texCoord2iv;

	/// \brief glTexCoord2s binding
	ptr_glTexCoord2s texCoord2s;

	/// \brief glTexCoord2sv binding
	ptr_glTexCoord2sv texCoord2sv;

	/// \brief glTexCoord3d binding
	ptr_glTexCoord3d texCoord3d;

	/// \brief glTexCoord3dv binding
	ptr_glTexCoord3dv texCoord3dv;

	/// \brief glTexCoord3f binding
	ptr_glTexCoord3f texCoord3f;

	/// \brief glTexCoord3fv binding
	ptr_glTexCoord3fv texCoord3fv;

	/// \brief glTexCoord3i binding
	ptr_glTexCoord3i texCoord3i;

	/// \brief glTexCoord3iv binding
	ptr_glTexCoord3iv texCoord3iv;

	/// \brief glTexCoord3s binding
	ptr_glTexCoord3s texCoord3s;

	/// \brief glTexCoord3sv binding
	ptr_glTexCoord3sv texCoord3sv;

	/// \brief glTexCoord4d binding
	ptr_glTexCoord4d texCoord4d;

	/// \brief glTexCoord4dv binding
	ptr_glTexCoord4dv texCoord4dv;

	/// \brief glTexCoord4f binding
	ptr_glTexCoord4f texCoord4f;

	/// \brief glTexCoord4fv binding
	ptr_glTexCoord4fv texCoord4fv;

	/// \brief glTexCoord4i binding
	ptr_glTexCoord4i texCoord4i;

	/// \brief glTexCoord4iv binding
	ptr_glTexCoord4iv texCoord4iv;

	/// \brief glTexCoord4s binding
	ptr_glTexCoord4s texCoord4s;

	/// \brief glTexCoord4sv binding
	ptr_glTexCoord4sv texCoord4sv;

	/// \brief glTexCoordPointer binding
	ptr_glTexCoordPointer texCoordPointer;

	/// \brief glTexEnvf binding
	ptr_glTexEnvf texEnvf;

	/// \brief glTexEnvfv binding
	ptr_glTexEnvfv texEnvfv;

	/// \brief glTexEnvi binding
	ptr_glTexEnvi texEnvi;

	/// \brief glTexEnviv binding
	ptr_glTexEnviv texEnviv;

	/// \brief glTexGend binding
	ptr_glTexGend texGend;

	/// \brief glTexGendv binding
	ptr_glTexGendv texGendv;

	/// \brief glTexGenf binding
	ptr_glTexGenf texGenf;

	/// \brief glTexGenfv binding
	ptr_glTexGenfv texGenfv;

	/// \brief glTexGeni binding
	ptr_glTexGeni texGeni;

	/// \brief glTexGeniv binding
	ptr_glTexGeniv texGeniv;

	/// \brief glTexImage1D binding
	ptr_glTexImage1D texImage1D;

	/// \brief glTexImage2D binding
	ptr_glTexImage2D texImage2D;

	/// \brief glTexParameterf binding
	ptr_glTexParameterf texParameterf;

	/// \brief glTexParameterfv binding
	ptr_glTexParameterfv texParameterfv;

	/// \brief glTexParameteri binding
	ptr_glTexParameteri texParameteri;

	/// \brief glTexParameteriv binding
	ptr_glTexParameteriv texParameteriv;

	/// \brief glTexSubImage1D binding
	ptr_glTexSubImage1D texSubImage1D;

	/// \brief glTexSubImage2D binding
	ptr_glTexSubImage2D texSubImage2D;

	/// \brief glTranslated binding
	ptr_glTranslated translated;

	/// \brief glTranslatef binding
	ptr_glTranslatef translatef;

	/// \brief glVertex2d binding
	ptr_glVertex2d vertex2d;

	/// \brief glVertex2dv binding
	ptr_glVertex2dv vertex2dv;

	/// \brief glVertex2f binding
	ptr_glVertex2f vertex2f;

	/// \brief glVertex2fv binding
	ptr_glVertex2fv vertex2fv;

	/// \brief glVertex2i binding
	ptr_glVertex2i vertex2i;

	/// \brief glVertex2iv binding
	ptr_glVertex2iv vertex2iv;

	/// \brief glVertex2s binding
	ptr_glVertex2s vertex2s;

	/// \brief glVertex2sv binding
	ptr_glVertex2sv vertex2sv;

	/// \brief glVertex3d binding
	ptr_glVertex3d vertex3d;

	/// \brief glVertex3dv binding
	ptr_glVertex3dv vertex3dv;

	/// \brief glVertex3f binding
	ptr_glVertex3f vertex3f;

	/// \brief glVertex3fv binding
	ptr_glVertex3fv vertex3fv;

	/// \brief glVertex3i binding
	ptr_glVertex3i vertex3i;

	/// \brief glVertex3iv binding
	ptr_glVertex3iv vertex3iv;

	/// \brief glVertex3s binding
	ptr_glVertex3s vertex3s;

	/// \brief glVertex3sv binding
	ptr_glVertex3sv vertex3sv;

	/// \brief glVertex4d binding
	ptr_glVertex4d vertex4d;

	/// \brief glVertex4dv binding
	ptr_glVertex4dv vertex4dv;

	/// \brief glVertex4f binding
	ptr_glVertex4f vertex4f;

	/// \brief glVertex4fv binding
	ptr_glVertex4fv vertex4fv;

	/// \brief glVertex4i binding
	ptr_glVertex4i vertex4i;

	/// \brief glVertex4iv binding
	ptr_glVertex4iv vertex4iv;

	/// \brief glVertex4s binding
	ptr_glVertex4s vertex4s;

	/// \brief glVertex4sv binding
	ptr_glVertex4sv vertex4sv;

	/// \brief glVertexPointer binding
	ptr_glVertexPointer vertexPointer;

	/// \brief glViewport binding
	ptr_glViewport viewport;

	/// \brief glDrawRangeElements binding (OpenGL 1.2, EXT_draw_range_elements)
	ptr_glDrawRangeElementsEXT drawRangeElements;

	/// \brief glTexImage3D binding (OpenGL 1.2, EXT_texture3D)
	ptr_glTexImage3DEXT texImage3D;

	/// \brief glTexSubImage3D binding (OpenGL 1.2, EXT_texture3D)
	ptr_glTexSubImage3DEXT texSubImage3D;

	/// \brief glCopyTexSubImage3D binding (OpenGL 1.2, EXT_texture3D)
	ptr_glCopyTexSubImage3DEXT copyTexSubImage3D;

	/// \brief glColorTable (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glColorTableSGI colorTable;

	/// \brief glCopyColorTable (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glCopyColorTableSGI copyColorTable;

	/// \brief glColorTableParameteriv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glColorTableParameterivSGI colorTableParameteriv;

	/// \brief glColorTableParameterfv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glColorTableParameterfvSGI colorTableParameterfv;

	/// \brief glGetColorTable (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glGetColorTableSGI getColorTable;

	/// \brief glGetColorTableParameteriv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glGetColorTableParameterivSGI getColorTableParameteriv;

	/// \brief glGetColorTableParameterfv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glGetColorTableParameterfvSGI getColorTableParameterfv;

	/// \brief glColorSubTable (OpenGL 1.2 Imaging subset, EXT_color_subtable)
	ptr_glColorSubTableEXT colorSubTable;

	/// \brief glCopyColorSubTable (OpenGL 1.2 Imaging subset, EXT_color_subtable)
	ptr_glCopyColorSubTableEXT copyColorSubTable;

	/// \brief glConvolutionFilter1D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionFilter1DEXT convolutionFilter1D;

	/// \brief glConvolutionFilter2D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionFilter2DEXT convolutionFilter2D;

	/// \brief glCopyConvolutionFilter1D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glCopyConvolutionFilter1DEXT copyConvolutionFilter1D;

	/// \brief glCopyConvolutionFilter2D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glCopyConvolutionFilter2DEXT copyConvolutionFilter2D;

	/// \brief glGetConvolutionFilter (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetConvolutionFilterEXT getConvolutionFilter;

	/// \brief glSeparableFilter2D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glSeparableFilter2DEXT separableFilter2D;

	/// \brief glGetSeparableFilter (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetSeparableFilterEXT getSeparableFilter;

	/// \brief glConvolutionParameteri (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameteriEXT convolutionParameteri;

	/// \brief glConvolutionParameterv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameterivEXT convolutionParameteriv;

	/// \brief glConvolutionParameterf (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameterfEXT convolutionParameterf;

	/// \brief glConvolutionParameterfv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameterfvEXT convolutionParameterfv;

	/// \brief glGetConvolutionParameteriv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetConvolutionParameterivEXT getConvolutionParameteriv;

	/// \brief glGetConvolutionParameterfv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetConvolutionParameterfvEXT getConvolutionParameterfv;

	/// \brief glHistogram (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glHistogramEXT histogram;

	/// \brief glResetHistogram (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glResetHistogramEXT resetHistogram;

	/// \brief glGetHistogram (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetHistogramEXT getHistogram;

	/// \brief glHistogramParameteriv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetHistogramParameterivEXT getHistogramParameteriv;

	/// \brief glHistogramParameterfv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetHistogramParameterfvEXT getHistogramParameterfv;

	/// \brief glMinmax (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glMinmaxEXT minmax;

	/// \brief glResetMinmax (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glResetMinmaxEXT resetMinmax;

	/// \brief glGetMinmax (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetMinmaxEXT getMinmax;

	/// \brief glGetMinmaxParameteriv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetMinmaxParameterivEXT getMinmaxParameteriv;

	/// \brief glGetMinmaxParameterfv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetMinmaxParameterfvEXT getMinmaxParameterfv;

	/// \brief glBlendColor (OpenGL 1.2 Imaging subset, OpenGL 1.4, EXT_blend_color)
	ptr_glBlendColorEXT blendColor;

	/// \brief glBlendEquation (OpenGL 1.2 Imaging subset, EXT_blend_minmax)
	ptr_glBlendEquationEXT blendEquation;

	/// \brief glActiveTexture (OpenGL 1.2.1, ARB_multitexture)
	ptr_glActiveTextureARB activeTexture;

	/// \brief glClientActiveTexture (OpenGL 1.2.1, ARB_multitexture)
	ptr_glClientActiveTextureARB clientActiveTexture;

	/// \brief glMultiTexCoord1d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1dARB multiTexCoord1d;

	/// \brief glMultiTexCoord1dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1dvARB multiTexCoord1dv;

	/// \brief glMultiTexCoord1f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1fARB multiTexCoord1f;

	/// \brief glMultiTexCoord1fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1fvARB multiTexCoord1fv;

	/// \brief glMultiTexCoord1i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1iARB multiTexCoord1i;

	/// \brief glMultiTexCoord1iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1ivARB multiTexCoord1iv;

	/// \brief glMultiTexCoord1s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1sARB multiTexCoord1s;

	/// \brief glMultiTexCoord1sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1svARB multiTexCoord1sv;

	/// \brief glMultiTexCoord2d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2dARB multiTexCoord2d;

	/// \brief glMultiTexCoord2dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2dvARB multiTexCoord2dv;

	/// \brief glMultiTexCoord2f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2fARB multiTexCoord2f;

	/// \brief glMultiTexCoord2fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2fvARB multiTexCoord2fv;

	/// \brief glMultiTexCoord2i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2iARB multiTexCoord2i;

	/// \brief glMultiTexCoord2iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2ivARB multiTexCoord2iv;

	/// \brief glMultiTexCoord2s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2sARB multiTexCoord2s;

	/// \brief glMultiTexCoord2sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2svARB multiTexCoord2sv;

	/// \brief glMultiTexCoord3d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3dARB multiTexCoord3d;

	/// \brief glMultiTexCoord3dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3dvARB multiTexCoord3dv;

	/// \brief glMultiTexCoord3f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3fARB multiTexCoord3f;

	/// \brief glMultiTexCoord3fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3fvARB multiTexCoord3fv;

	/// \brief glMultiTexCoord3i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3iARB multiTexCoord3i;

	/// \brief glMultiTexCoord3iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3ivARB multiTexCoord3iv;

	/// \brief glMultiTexCoord3s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3sARB multiTexCoord3s;

	/// \brief glMultiTexCoord3sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3svARB multiTexCoord3sv;

	/// \brief glMultiTexCoord4d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4dARB multiTexCoord4d;

	/// \brief glMultiTexCoord4dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4dvARB multiTexCoord4dv;

	/// \brief glMultiTexCoord4f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4fARB multiTexCoord4f;

	/// \brief glMultiTexCoord4fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4fvARB multiTexCoord4fv;

	/// \brief glMultiTexCoord4i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4iARB multiTexCoord4i;

	/// \brief glMultiTexCoord4iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4ivARB multiTexCoord4iv;

	/// \brief glMultiTexCoord4s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4sARB multiTexCoord4s;

	/// \brief glMultiTexCoord4sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4svARB multiTexCoord4sv;

	/// \brief glCompressedTexImage1D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexImage1DARB compressedTexImage1D;

	/// \brief glCompressedTexImage2D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexImage2DARB compressedTexImage2D;

	/// \brief glCompressedTexImage3D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexImage3DARB compressedTexImage3D;

	/// \brief glCompressedTexSubImage1D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexSubImage1DARB compressedTexSubImage1D;

	/// \brief glCompressedTexSubImage2D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexSubImage2DARB compressedTexSubImage2D;

	/// \brief glCompressedTexSubImage3D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexSubImage3DARB compressedTexSubImage3D;

	/// \brief glGetCompressedTexImage (OpenGL 1.3, ARB_texture_compression)
	ptr_glGetCompressedTexImageARB getCompressedTexImage;

	/// \brief glSampleCoverage (OpenGL 1.3, ARB_multisample)
	ptr_glSampleCoverageARB sampleCoverage;

	/// \brief glLoadTransposeMatrixd (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glLoadTransposeMatrixdARB glLoadTransposeMatrixd;

	/// \brief glLoadTransposeMatrixf (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glLoadTransposeMatrixfARB glLoadTransposeMatrixf;

	/// \brief glMultTransposeMatrixd (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glMultTransposeMatrixdARB glMultTransposeMatrixd;

	/// \brief glMultTransposeMatrixf (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glMultTransposeMatrixfARB glMultTransposeMatrixf;

	/// \brief glFogCoordd (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoorddEXT fogCoordd;

	/// \brief glFogCoorddv (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoorddvEXT fogCoorddv;

	/// \brief glFogCoordf (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoordfEXT fogCoordf;

	/// \brief glFogCoordfv (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoordfvEXT fogCoordfv;

	/// \brief glFogCoordPointer (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoordPointerEXT fogCoordPointer;

	/// \brief glMultiDrawArrays (OpenGL 1.4, EXT_multi_draw_arrays)
	ptr_glMultiDrawArraysEXT multiDrawArrays;

	/// \brief glMultiDrawElements (OpenGL 1.4, EXT_multi_draw_arrays)
	ptr_glMultiDrawElementsEXT multiDrawElementsEXT;

	/// \brief glPointParameterf (OpenGL 1.4, ARB_point_parameters)
	ptr_glPointParameterfARB pointParameterf;

	/// \brief glPointParameterfv (OpenGL 1.4, ARB_point_parameters)
	ptr_glPointParameterfvARB pointParameterfv;

	/// \brief glSecondaryColor3b (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3bEXT secondaryColor3b;

	/// \brief glSecondaryColor3bv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3bvEXT secondaryColor3bv;

	/// \brief glSecondaryColor3d (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3dEXT secondaryColor3d;

	/// \brief glSecondaryColor3dv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3dvEXT secondaryColor3dv;

	/// \brief glSecondaryColor3f (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3fEXT secondaryColor3f;

	/// \brief glSecondaryColor3fv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3fvEXT secondaryColor3fv;

	/// \brief glSecondaryColor3i (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3iEXT secondaryColor3i;

	/// \brief glSecondaryColor3iv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3ivEXT secondaryColor3iv;

	/// \brief glSecondaryColor3s (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3sEXT secondaryColor3s;

	/// \brief glSecondaryColor3sv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3svEXT secondaryColor3sv;

	/// \brief glSecondaryColor3ub (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3ubEXT secondaryColor3ub;

	/// \brief glSecondaryColor3ubv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3ubvEXT secondaryColor3ubv;

	/// \brief glSecondaryColor3ui (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3uiEXT secondaryColor3ui;

	/// \brief glSecondaryColor3uiv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3uivEXT secondaryColor3uiv;

	/// \brief glSecondaryColor3us (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3usEXT secondaryColor3us;

	/// \brief glSecondaryColor3usv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3usvEXT secondaryColor3usv;

	/// \brief glSecondaryColorPointer (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColorPointerEXT secondaryColorPointer;

	/// \brief glBlendFuncSeparate (OpenGL 1.4, EXT_blend_func_separate)
	ptr_glBlendFuncSeparateEXT blendFuncSeparate;

	/// \brief glWindowPos2d (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2dARB windowPos2d;

	/// \brief glWindowPos2dv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2dvARB windowPos2dv;

	/// \brief glWindowPos2f (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2fARB windowPos2f;

	/// \brief glWindowPos2fv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2fvARB windowPos2fv;

	/// \brief glWindowPos2i (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2iARB windowPos2i;

	/// \brief glWindowPos2iv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2ivARB windowPos2iv;

	/// \brief glWindowPos2s (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2sARB windowPos2s;

	/// \brief glWindowPos2sv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2svARB windowPos2sv;

	/// \brief glWindowPos3d (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3dARB windowPos3d;

	/// \brief glWindowPos3dv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3dvARB windowPos3dv;

	/// \brief glWindowPos3f (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3fARB windowPos3f;

	/// \brief glWindowPos3fv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3fvARB windowPos3fv;

	/// \brief glWindowPos3i (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3iARB windowPos3i;

	/// \brief glWindowPos3iv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3ivARB windowPos3iv;

	/// \brief glWindowPos3s (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3sARB windowPos3s;

	/// \brief glWindowPos3sv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3svARB windowPos3sv;

	/// \brief glBindBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glBindBufferARB bindBuffer;

	/// \brief glDeleteBuffers (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glDeleteBuffersARB deleteBuffers;

	/// \brief glGenBuffers (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGenBuffersARB genBuffers;

	/// \brief glIsBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glIsBufferARB isBuffer;

	/// \brief glBufferData (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glBufferDataARB bufferData;

	/// \brief glBufferData (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glBufferSubDataARB bufferSubData;

	/// \brief glBufferSubData (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGetBufferSubDataARB getBufferSubData;

	/// \brief glMapBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glMapBufferARB mapBuffer;

	/// \brief glUnmapBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glUnmapBufferARB unmapBuffer;

	/// \brief glGetBufferParameteriv (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGetBufferParameterivARB getBufferParameteriv;

	/// \brief glGetBufferPointerv (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGetBufferPointervARB getBufferPointerv;

	/// \brief glGenQueries (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGenQueriesARB genQueries;

	/// \brief glDeleteQueries (OpenGL 1.5, ARB_occlusion_query)
	ptr_glDeleteQueriesARB deleteQueries;

	/// \brief glIsQuery (OpenGL 1.5, ARB_occlusion_query)
	ptr_glIsQueryARB isQuery;

	/// \brief glBeginQuery (OpenGL 1.5, ARB_occlusion_query)
	ptr_glBeginQueryARB beginQuery;

	/// \brief glEndQuery (OpenGL 1.5, ARB_occlusion_query)
	ptr_glEndQueryARB endQuery;

	/// \brief glGetQueryiv (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGetQueryivARB getQueryiv;

	/// \brief glGetQueryObjectiv (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGetQueryObjectivARB getQueryObjectiv;

	/// \brief glGetQueryObjectuiv (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGetQueryObjectuivARB getQueryObjectuiv;

	/// \brief glDeleteShader (OpenGL 2.0, ARB_shader_objects)
	ptr_glDeleteObjectARB deleteShader;

	/// \brief glDeleteProgram (OpenGL 2.0, ARB_shader_objects)
	ptr_glDeleteObjectARB deleteProgram;

	/// \brief glGetHandle (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetHandleARB getHandle;

	/// \brief glDetachObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glDetachObjectARB detachShader;

	/// \brief glCreateShader (OpenGL 2.0, ARB_shader_objects)
	ptr_glCreateShaderObjectARB createShader;

	/// \brief glShaderSource (OpenGL 2.0, ARB_shader_objects)
	ptr_glShaderSourceARB shaderSource;

	/// \brief glCompileShader (OpenGL 2.0, ARB_shader_objects)
	ptr_glCompileShaderARB compileShader;

	/// \brief glCreateProgramObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glCreateProgramObjectARB createProgram;

	/// \brief glAttachObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glAttachObjectARB attachShader;

	/// \brief glLinkProgram (OpenGL 2.0, ARB_shader_objects)
	ptr_glLinkProgramARB linkProgram;

	/// \brief glUseProgramObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glUseProgramObjectARB useProgram;

	/// \brief glValidateProgram (OpenGL 2.0, ARB_shader_objects)
	ptr_glValidateProgramARB validateProgram;

	/// \brief glUniform1f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1fARB uniform1f;

	/// \brief glUniform2f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2fARB uniform2f;

	/// \brief glUniform3f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3fARB uniform3f;

	/// \brief glUniform4f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4fARB uniform4f;

	/// \brief glUniform1i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1iARB uniform1i;

	/// \brief glUniform2i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2iARB uniform2i;

	/// \brief glUniform3i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3iARB uniform3i;

	/// \brief glUniform4i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4iARB uniform4i;

	/// \brief glUniform1fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1fvARB uniform1fv;

	/// \brief glUniform2fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2fvARB uniform2fv;

	/// \brief glUniform3fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3fvARB uniform3fv;

	/// \brief glUniform4fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4fvARB uniform4fv;

	/// \brief glUniform1iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1ivARB uniform1iv;

	/// \brief glUniform2iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2ivARB uniform2iv;

	/// \brief glUniform3iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3ivARB uniform3iv;

	/// \brief glUniform4iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4ivARB uniform4iv;

	/// \brief glUniformMatrix2fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniformMatrix2fvARB uniformMatrix2fv;

	/// \brief glUniformMatrix3fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniformMatrix3fvARB uniformMatrix3fv;

	/// \brief glUniformMatrix4fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniformMatrix4fvARB uniformMatrix4fv;

	/// \brief glGetProgramfv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterfvARB getProgramfv;

	/// \brief glGetProgramiv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterivARB getProgramiv;

	/// \brief glGetShaderfv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterfvARB getShaderfv;

	/// \brief glGetShaderiv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterivARB getShaderiv;

	/// \brief glGetShaderInfoLog (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetInfoLogARB getShaderInfoLog;

	/// \brief glGetProgramInfoLog (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetInfoLogARB getProgramInfoLog;

	/// \brief glGetAttachedObjects (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetAttachedObjectsARB getAttachedShaders;

	/// \brief glGetUniformLocation (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetUniformLocationARB getUniformLocation;

	/// \brief glGetActiveUniform (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetActiveUniformARB getActiveUniform;

	/// \brief glGetUniformfv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetUniformfvARB getUniformfv;

	/// \brief glGetUniformiv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetUniformivARB getUniformiv;

	/// \brief glGetShaderSource (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetShaderSourceARB getShaderSource;

	/// \brief glVertexAttrib1f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1fARB vertexAttrib1f;

	/// \brief glVertexAttrib1s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1sARB vertexAttrib1s;

	/// \brief glVertexAttrib1d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1dARB vertexAttrib1d;

	/// \brief glVertexAttrib2f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2fARB vertexAttrib2f;

	/// \brief glVertexAttrib2s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2sARB vertexAttrib2s;

	/// \brief glVertexAttrib2d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2dARB vertexAttrib2d;

	/// \brief glVertexAttrib3f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3fARB vertexAttrib3f;

	/// \brief glVertexAttrib3s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3sARB vertexAttrib3s;

	/// \brief glVertexAttrib3d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3dARB vertexAttrib3d;

	/// \brief glVertexAttrib4f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4fARB vertexAttrib4f;

	/// \brief glVertexAttrib4s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4sARB vertexAttrib4s;

	/// \brief glVertexAttrib4d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4dARB vertexAttrib4d;

	/// \brief glVertexAttrib4Nub (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NubARB vertexAttrib4Nub;

	/// \brief glVertexAttrib1fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1fvARB vertexAttrib1fv;

	/// \brief glVertexAttrib1sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1svARB vertexAttrib1sv;

	/// \brief glVertexAttrib1dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1dvARB vertexAttrib1dv;

	/// \brief glVertexAttrib2fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2fvARB vertexAttrib2fv;

	/// \brief glVertexAttrib2sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2svARB vertexAttrib2sv;

	/// \brief glVertexAttrib2dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2dvARB vertexAttrib2dv;

	/// \brief glVertexAttrib3fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3fvARB vertexAttrib3fv;

	/// \brief glVertexAttrib3sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3svARB vertexAttrib3sv;

	/// \brief glVertexAttrib3dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3dvARB vertexAttrib3dv;

	/// \brief glVertexAttrib4fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4fvARB vertexAttrib4fv;

	/// \brief glVertexAttrib4sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4svARB vertexAttrib4sv;

	/// \brief glVertexAttrib4dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4dvARB vertexAttrib4dv;

	/// \brief glVertexAttrib4iv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4ivARB vertexAttrib4iv;

	/// \brief glVertexAttrib4bv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4bvARB vertexAttrib4bv;

	/// \brief glVertexAttrib4ubv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4ubvARB vertexAttrib4ubv;

	/// \brief glVertexAttrib4usv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4usvARB vertexAttrib4usv;

	/// \brief glVertexAttrib4uiv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4uivARB vertexAttrib4uiv;

	/// \brief glVertexAttrib4Nbv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NbvARB vertexAttrib4Nbv;

	/// \brief glVertexAttrib4Nsv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NsvARB vertexAttrib4Nsv;

	/// \brief glVertexAttrib4Niv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NivARB vertexAttrib4Niv;

	/// \brief glVertexAttrib4Nubv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NubvARB vertexAttrib4Nubv;

	/// \brief glVertexAttrib4Nusv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NusvARB vertexAttrib4Nusv;

	/// \brief glVertexAttrib4Nuiv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NuivARB vertexAttrib4Nuiv;

	/// \brief glVertexAttribPointer (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttribPointerARB vertexAttribPointer;

	/// \brief glEnableVertexAttribArray (OpenGL 2.0, ARB_vertex_shader)
	ptr_glEnableVertexAttribArrayARB enableVertexAttribArray;

	/// \brief glDisableVertexAttribArray (OpenGL 2.0, ARB_vertex_shader)
	ptr_glDisableVertexAttribArrayARB disableVertexAttribArray;

	/// \brief glBindAttribLocation (OpenGL 2.0, ARB_vertex_shader)
	ptr_glBindAttribLocationARB bindAttribLocation;

	/// \brief glGetActiveAttrib (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetActiveAttribARB getActiveAttrib;

	/// \brief glGetAttribLocation (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetAttribLocationARB getAttribLocation;

	/// \brief glGetVertexAttribdv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribdvARB getVertexAttribdv;

	/// \brief glGetVertexAttribfv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribfvARB getVertexAttribfv;

	/// \brief glGetVertexAttribiv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribivARB getVertexAttribiv;

	/// \brief glGetVertexAttribPointerv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribPointervARB getVertexAttribPointerv;

	/// \brief glDrawBuffers (OpenGL 2.0, ARB_draw_buffers)
	ptr_glDrawBuffersARB drawBuffers;

	/// \brief glStencilFuncSeparate (OpenGL 2.0, ATI_separate_stencil)
	ptr_glStencilFuncSeparateATI stencilFuncSeparate;

	/// \brief glStencilOpSeparate (OpenGL 2.0, ATI_separate_stencil)
	ptr_glStencilOpSeparateATI stencilOpSeparate;

	/// \brief glIsRenderbuffer (EXT_framebuffer_object)
	ptr_glIsRenderbufferEXT isRenderbuffer;

	/// \brief glBindRenderbuffer (EXT_framebuffer_object)
	ptr_glBindRenderbufferEXT bindRenderbuffer;

	/// \brief glDeleteRenderbuffer (EXT_framebuffer_object)
	ptr_glDeleteRenderbuffersEXT deleteRenderbuffers;

	/// \brief glGenRenderbuffers (EXT_framebuffer_object)
	ptr_glGenRenderbuffersEXT genRenderbuffers;

	/// \brief glRenderbufferStorage (EXT_framebuffer_object)
	ptr_glRenderbufferStorageEXT renderbufferStorage;

	/// \brief glGetRenderbufferParameteriv (EXT_framebuffer_object)
	ptr_glGetRenderbufferParameterivEXT getRenderbufferParameteriv;

	/// \brief glIsFramebuffer (EXT_framebuffer_object)
	ptr_glIsFramebufferEXT isFramebuffer;

	/// \brief glBindFramebuffer (EXT_framebuffer_object)
	ptr_glBindFramebufferEXT bindFramebuffer;

	/// \brief glDeleteFramebuffers (EXT_framebuffer_object)
	ptr_glDeleteFramebuffersEXT deleteFramebuffers;

	/// \brief glGenFramebuffers (EXT_framebuffer_object)
	ptr_glGenFramebuffersEXT genFramebuffers;

	/// \brief glCheckFramebufferStatus (EXT_framebuffer_object)
	ptr_glCheckFramebufferStatusEXT checkFramebufferStatus;

	/// \brief glFramebufferTexture1D (EXT_framebuffer_object)
	ptr_glFramebufferTexture1DEXT framebufferTexture1D;

	/// \brief glFramebufferTexture2D (EXT_framebuffer_object)
	ptr_glFramebufferTexture2DEXT framebufferTexture2D;

	/// \brief glFramebufferTexture3D (EXT_framebuffer_object)
	ptr_glFramebufferTexture3DEXT framebufferTexture3D;

	/// \brief glFramebufferRenderbuffer (EXT_framebuffer_object)
	ptr_glFramebufferRenderbufferEXT framebufferRenderbuffer;

	/// \brief glGetFramebufferAttachmentParameteriv (EXT_framebuffer_object)
	ptr_glGetFramebufferAttachmentParameterivEXT getFramebufferAttachmentParameteriv;

	/// \brief glGenerateMipmap (EXT_framebuffer_object)
	ptr_glGenerateMipmapEXT generateMipmap;

	/// \brief glGenerateMipmap (EXT_framebuffer_blit)
	ptr_glBlitFramebufferEXT blitFramebuffer;

};
/// \}

#define clAccum CL_OpenGL::functions->accum
#define clAlphaFunc CL_OpenGL::functions->alphaFunc
#define clAreTexturesResident CL_OpenGL::functions->areTexturesResident
#define clArrayElement CL_OpenGL::functions->arrayElement
#define clBegin CL_OpenGL::functions->begin
#define clBindTexture CL_OpenGL::functions->bindTexture
#define clBitmap CL_OpenGL::functions->bitmap
#define clBlendFunc CL_OpenGL::functions->blendFunc
#define clCallList CL_OpenGL::functions->callList
#define clCallLists CL_OpenGL::functions->callLists
#define clClear CL_OpenGL::functions->clear
#define clClearAccum CL_OpenGL::functions->clearAccum
#define clClearColor CL_OpenGL::functions->clearColor
#define clClearDepth CL_OpenGL::functions->clearDepth
#define clClearIndex CL_OpenGL::functions->clearIndex
#define clClearStencil CL_OpenGL::functions->clearStencil
#define clClipPlane CL_OpenGL::functions->clipPlane
#define clColor3b CL_OpenGL::functions->color3b
#define clColor3bv CL_OpenGL::functions->color3bv
#define clColor3d CL_OpenGL::functions->color3d
#define clColor3dv CL_OpenGL::functions->color3dv
#define clColor3f CL_OpenGL::functions->color3f
#define clColor3fv CL_OpenGL::functions->color3fv
#define clColor3i CL_OpenGL::functions->color3i
#define clColor3iv CL_OpenGL::functions->color3iv
#define clColor3s CL_OpenGL::functions->color3s
#define clColor3sv CL_OpenGL::functions->color3sv
#define clColor3ub CL_OpenGL::functions->color3ub
#define clColor3ubv CL_OpenGL::functions->color3ubv
#define clColor3ui CL_OpenGL::functions->color3ui
#define clColor3uiv CL_OpenGL::functions->color3uiv
#define clColor3us CL_OpenGL::functions->color3us
#define clColor3usv CL_OpenGL::functions->color3usv
#define clColor4b CL_OpenGL::functions->color4b
#define clColor4bv CL_OpenGL::functions->color4bv
#define clColor4d CL_OpenGL::functions->color4d
#define clColor4dv CL_OpenGL::functions->color4dv
#define clColor4f CL_OpenGL::functions->color4f
#define clColor4fv CL_OpenGL::functions->color4fv
#define clColor4i CL_OpenGL::functions->color4i
#define clColor4iv CL_OpenGL::functions->color4iv
#define clColor4s CL_OpenGL::functions->color4s
#define clColor4sv CL_OpenGL::functions->color4sv
#define clColor4ub CL_OpenGL::functions->color4ub
#define clColor4ubv CL_OpenGL::functions->color4ubv
#define clColor4ui CL_OpenGL::functions->color4ui
#define clColor4uiv CL_OpenGL::functions->color4uiv
#define clColor4us CL_OpenGL::functions->color4us
#define clColor4usv CL_OpenGL::functions->color4usv
#define clColorMask CL_OpenGL::functions->colorMask
#define clColorMaterial CL_OpenGL::functions->colorMaterial
#define clColorPointer CL_OpenGL::functions->colorPointer
#define clCopyPixels CL_OpenGL::functions->copyPixels
#define clCopyTexImage1D CL_OpenGL::functions->copyTexImage1D
#define clCopyTexImage2D CL_OpenGL::functions->copyTexImage2D
#define clCopyTexSubImage1D CL_OpenGL::functions->copyTexSubImage1D
#define clCopyTexSubImage2D CL_OpenGL::functions->copyTexSubImage2D
#define clCopyTexSubImage3D CL_OpenGL::functions->copyTexSubImage3D
#define clCullFace CL_OpenGL::functions->cullFace
#define clDeleteLists CL_OpenGL::functions->deleteLists
#define clDeleteTextures CL_OpenGL::functions->deleteTextures
#define clDepthFunc CL_OpenGL::functions->depthFunc
#define clDepthMask CL_OpenGL::functions->depthMask
#define clDepthRange CL_OpenGL::functions->depthRange
#define clDisable CL_OpenGL::functions->disable
#define clDisableClientState CL_OpenGL::functions->disableClientState
#define clDrawArrays CL_OpenGL::functions->drawArrays
#define clDrawBuffer CL_OpenGL::functions->drawBuffer
#define clDrawElements CL_OpenGL::functions->drawElements
#define clDrawPixels CL_OpenGL::functions->drawPixels
#define clEdgeFlag CL_OpenGL::functions->edgeFlag
#define clEdgeFlagPointer CL_OpenGL::functions->edgeFlagPointer
#define clEdgeFlagv CL_OpenGL::functions->edgeFlagv
#define clEnable CL_OpenGL::functions->enable
#define clEnableClientState CL_OpenGL::functions->enableClientState
#define clEnd CL_OpenGL::functions->end
#define clEndList CL_OpenGL::functions->endList
#define clEvalCoord1d CL_OpenGL::functions->evalCoord1d
#define clEvalCoord1dv CL_OpenGL::functions->evalCoord1dv
#define clEvalCoord1f CL_OpenGL::functions->evalCoord1f
#define clEvalCoord1fv CL_OpenGL::functions->evalCoord1fv
#define clEvalCoord2d CL_OpenGL::functions->evalCoord2d
#define clEvalCoord2dv CL_OpenGL::functions->evalCoord2dv
#define clEvalCoord2f CL_OpenGL::functions->evalCoord2f
#define clEvalCoord2fv CL_OpenGL::functions->evalCoord2fv
#define clEvalMesh1 CL_OpenGL::functions->evalMesh1
#define clEvalMesh2 CL_OpenGL::functions->evalMesh2
#define clEvalPoint1 CL_OpenGL::functions->evalPoint1
#define clEvalPoint2 CL_OpenGL::functions->evalPoint2
#define clFeedbackBuffer CL_OpenGL::functions->feedbackBuffer
#define clFinish CL_OpenGL::functions->finish
#define clFlush CL_OpenGL::functions->flush
#define clFogf CL_OpenGL::functions->fogf
#define clFogfv CL_OpenGL::functions->fogfv
#define clFogi CL_OpenGL::functions->fogi
#define clFogiv CL_OpenGL::functions->fogiv
#define clFrontFace CL_OpenGL::functions->frontFace
#define clFrustum CL_OpenGL::functions->frustum
#define clGenLists CL_OpenGL::functions->genLists
#define clGenTextures CL_OpenGL::functions->genTextures
#define clGetBooleanv CL_OpenGL::functions->getBooleanv
#define clGetClipPlane CL_OpenGL::functions->getClipPlane
#define clGetDoublev CL_OpenGL::functions->getDoublev
#define clGetError CL_OpenGL::functions->getError
#define clGetFloatv CL_OpenGL::functions->getFloatv
#define clGetIntegerv CL_OpenGL::functions->getIntegerv
#define clGetLightfv CL_OpenGL::functions->getLightfv
#define clGetLightiv CL_OpenGL::functions->getLightiv
#define clGetMapdv CL_OpenGL::functions->getMapdv
#define clGetMapfv CL_OpenGL::functions->getMapfv
#define clGetMapiv CL_OpenGL::functions->getMapiv
#define clGetMaterialfv CL_OpenGL::functions->getMaterialfv
#define clGetMaterialiv CL_OpenGL::functions->getMaterialiv
#define clGetPixelMapfv CL_OpenGL::functions->getPixelMapfv
#define clGetPixelMapuiv CL_OpenGL::functions->getPixelMapuiv
#define clGetPixelMapusv CL_OpenGL::functions->getPixelMapusv
#define clGetPointerv CL_OpenGL::functions->getPointerv
#define clGetPolygonStipple CL_OpenGL::functions->getPolygonStipple
#define clGetString CL_OpenGL::functions->getString
#define clGetTexEnvfv CL_OpenGL::functions->getTexEnvfv
#define clGetTexEnviv CL_OpenGL::functions->getTexEnviv
#define clGetTexGendv CL_OpenGL::functions->getTexGendv
#define clGetTexGenfv CL_OpenGL::functions->getTexGenfv
#define clGetTexGeniv CL_OpenGL::functions->getTexGeniv
#define clGetTexImage CL_OpenGL::functions->getTexImage
#define clGetTexLevelParameterfv CL_OpenGL::functions->getTexLevelParameterfv
#define clGetTexLevelParameteriv CL_OpenGL::functions->getTexLevelParameteriv
#define clGetTexParameterfv CL_OpenGL::functions->getTexParameterfv
#define clGetTexParameteriv CL_OpenGL::functions->getTexParameteriv
#define clHint CL_OpenGL::functions->hint
#define clIndexMask CL_OpenGL::functions->indexMask
#define clIndexPointer CL_OpenGL::functions->indexPointer
#define clIndexd CL_OpenGL::functions->indexd
#define clIndexdv CL_OpenGL::functions->indexdv
#define clIndexf CL_OpenGL::functions->indexf
#define clIndexfv CL_OpenGL::functions->indexfv
#define clIndexi CL_OpenGL::functions->indexi
#define clIndexiv CL_OpenGL::functions->indexiv
#define clIndexs CL_OpenGL::functions->indexs
#define clIndexsv CL_OpenGL::functions->indexsv
#define clIndexub CL_OpenGL::functions->indexub
#define clIndexubv CL_OpenGL::functions->indexubv
#define clInitNames CL_OpenGL::functions->initNames
#define clInterleavedArrays CL_OpenGL::functions->interleavedArrays
#define clIsEnabled CL_OpenGL::functions->isEnabled
#define clIsList CL_OpenGL::functions->isList
#define clIsTexture CL_OpenGL::functions->isTexture
#define clLightModelf CL_OpenGL::functions->lightModelf
#define clLightModelfv CL_OpenGL::functions->lightModelfv
#define clLightModeli CL_OpenGL::functions->lightModeli
#define clLightModeliv CL_OpenGL::functions->lightModeliv
#define clLightf CL_OpenGL::functions->lightf
#define clLightfv CL_OpenGL::functions->lightfv
#define clLighti CL_OpenGL::functions->lighti
#define clLightiv CL_OpenGL::functions->lightiv
#define clLineStipple CL_OpenGL::functions->lineStipple
#define clLineWidth CL_OpenGL::functions->lineWidth
#define clListBase CL_OpenGL::functions->listBase
#define clLoadIdentity CL_OpenGL::functions->loadIdentity
#define clLoadMatrixd CL_OpenGL::functions->loadMatrixd
#define clLoadMatrixf CL_OpenGL::functions->loadMatrixf
#define clLoadName CL_OpenGL::functions->loadName
#define clLogicOp CL_OpenGL::functions->logicOp
#define clMap1d CL_OpenGL::functions->map1d
#define clMap1f CL_OpenGL::functions->map1f
#define clMap2d CL_OpenGL::functions->map2d
#define clMap2f CL_OpenGL::functions->map2f
#define clMapGrid1d CL_OpenGL::functions->mapGrid1d
#define clMapGrid1f CL_OpenGL::functions->mapGrid1f
#define clMapGrid2d CL_OpenGL::functions->mapGrid2d
#define clMapGrid2f CL_OpenGL::functions->mapGrid2f
#define clMaterialf CL_OpenGL::functions->materialf
#define clMaterialfv CL_OpenGL::functions->materialfv
#define clMateriali CL_OpenGL::functions->materiali
#define clMaterialiv CL_OpenGL::functions->materialiv
#define clMatrixMode CL_OpenGL::functions->matrixMode
#define clMultMatrixd CL_OpenGL::functions->multMatrixd
#define clMultMatrixf CL_OpenGL::functions->multMatrixf
#define clNewList CL_OpenGL::functions->newList
#define clNormal3b CL_OpenGL::functions->normal3b
#define clNormal3bv CL_OpenGL::functions->normal3bv
#define clNormal3d CL_OpenGL::functions->normal3d
#define clNormal3dv CL_OpenGL::functions->normal3dv
#define clNormal3f CL_OpenGL::functions->normal3f
#define clNormal3fv CL_OpenGL::functions->normal3fv
#define clNormal3i CL_OpenGL::functions->normal3i
#define clNormal3iv CL_OpenGL::functions->normal3iv
#define clNormal3s CL_OpenGL::functions->normal3s
#define clNormal3sv CL_OpenGL::functions->normal3sv
#define clNormalPointer CL_OpenGL::functions->normalPointer
#define clOrtho CL_OpenGL::functions->ortho
#define clPassThrough CL_OpenGL::functions->passThrough
#define clPixelMapfv CL_OpenGL::functions->pixelMapfv
#define clPixelMapuiv CL_OpenGL::functions->pixelMapuiv
#define clPixelMapusv CL_OpenGL::functions->pixelMapusv
#define clPixelStoref CL_OpenGL::functions->pixelStoref
#define clPixelStorei CL_OpenGL::functions->pixelStorei
#define clPixelTransferf CL_OpenGL::functions->pixelTransferf
#define clPixelTransferi CL_OpenGL::functions->pixelTransferi
#define clPixelZoom CL_OpenGL::functions->pixelZoom
#define clPointSize CL_OpenGL::functions->pointSize
#define clPolygonMode CL_OpenGL::functions->polygonMode
#define clPolygonOffset CL_OpenGL::functions->polygonOffset
#define clPolygonStipple CL_OpenGL::functions->polygonStipple
#define clPopAttrib CL_OpenGL::functions->popAttrib
#define clPopClientAttrib CL_OpenGL::functions->popClientAttrib
#define clPopMatrix CL_OpenGL::functions->popMatrix
#define clPopName CL_OpenGL::functions->popName
#define clPrioritizeTextures CL_OpenGL::functions->prioritizeTextures
#define clPushAttrib CL_OpenGL::functions->pushAttrib
#define clPushClientAttrib CL_OpenGL::functions->pushClientAttrib
#define clPushMatrix CL_OpenGL::functions->pushMatrix
#define clPushName CL_OpenGL::functions->pushName
#define clRasterPos2d CL_OpenGL::functions->rasterPos2d
#define clRasterPos2dv CL_OpenGL::functions->rasterPos2dv
#define clRasterPos2f CL_OpenGL::functions->rasterPos2f
#define clRasterPos2fv CL_OpenGL::functions->rasterPos2fv
#define clRasterPos2i CL_OpenGL::functions->rasterPos2i
#define clRasterPos2iv CL_OpenGL::functions->rasterPos2iv
#define clRasterPos2s CL_OpenGL::functions->rasterPos2s
#define clRasterPos2sv CL_OpenGL::functions->rasterPos2sv
#define clRasterPos3d CL_OpenGL::functions->rasterPos3d
#define clRasterPos3dv CL_OpenGL::functions->rasterPos3dv
#define clRasterPos3f CL_OpenGL::functions->rasterPos3f
#define clRasterPos3fv CL_OpenGL::functions->rasterPos3fv
#define clRasterPos3i CL_OpenGL::functions->rasterPos3i
#define clRasterPos3iv CL_OpenGL::functions->rasterPos3iv
#define clRasterPos3s CL_OpenGL::functions->rasterPos3s
#define clRasterPos3sv CL_OpenGL::functions->rasterPos3sv
#define clRasterPos4d CL_OpenGL::functions->rasterPos4d
#define clRasterPos4dv CL_OpenGL::functions->rasterPos4dv
#define clRasterPos4f CL_OpenGL::functions->rasterPos4f
#define clRasterPos4fv CL_OpenGL::functions->rasterPos4fv
#define clRasterPos4i CL_OpenGL::functions->rasterPos4i
#define clRasterPos4iv CL_OpenGL::functions->rasterPos4iv
#define clRasterPos4s CL_OpenGL::functions->rasterPos4s
#define clRasterPos4sv CL_OpenGL::functions->rasterPos4sv
#define clReadBuffer CL_OpenGL::functions->readBuffer
#define clReadPixels CL_OpenGL::functions->readPixels
#define clRectd CL_OpenGL::functions->rectd
#define clRectdv CL_OpenGL::functions->rectdv
#define clRectf CL_OpenGL::functions->rectf
#define clRectfv CL_OpenGL::functions->rectfv
#define clRecti CL_OpenGL::functions->recti
#define clRectiv CL_OpenGL::functions->rectiv
#define clRects CL_OpenGL::functions->rects
#define clRectsv CL_OpenGL::functions->rectsv
#define clRenderMode CL_OpenGL::functions->renderMode
#define clRotated CL_OpenGL::functions->rotated
#define clRotatef CL_OpenGL::functions->rotatef
#define clScaled CL_OpenGL::functions->scaled
#define clScalef CL_OpenGL::functions->scalef
#define clScissor CL_OpenGL::functions->scissor
#define clSelectBuffer CL_OpenGL::functions->selectBuffer
#define clShadeModel CL_OpenGL::functions->shadeModel
#define clStencilFunc CL_OpenGL::functions->stencilFunc
#define clStencilMask CL_OpenGL::functions->stencilMask
#define clStencilMaskSeparate CL_OpenGL::functions->stencilMaskSeparate
#define clStencilOp CL_OpenGL::functions->stencilOp
#define clTexCoord1d CL_OpenGL::functions->texCoord1d
#define clTexCoord1dv CL_OpenGL::functions->texCoord1dv
#define clTexCoord1f CL_OpenGL::functions->texCoord1f
#define clTexCoord1fv CL_OpenGL::functions->texCoord1fv
#define clTexCoord1i CL_OpenGL::functions->texCoord1i
#define clTexCoord1iv CL_OpenGL::functions->texCoord1iv
#define clTexCoord1s CL_OpenGL::functions->texCoord1s
#define clTexCoord1sv CL_OpenGL::functions->texCoord1sv
#define clTexCoord2d CL_OpenGL::functions->texCoord2d
#define clTexCoord2dv CL_OpenGL::functions->texCoord2dv
#define clTexCoord2f CL_OpenGL::functions->texCoord2f
#define clTexCoord2fv CL_OpenGL::functions->texCoord2fv
#define clTexCoord2i CL_OpenGL::functions->texCoord2i
#define clTexCoord2iv CL_OpenGL::functions->texCoord2iv
#define clTexCoord2s CL_OpenGL::functions->texCoord2s
#define clTexCoord2sv CL_OpenGL::functions->texCoord2sv
#define clTexCoord3d CL_OpenGL::functions->texCoord3d
#define clTexCoord3dv CL_OpenGL::functions->texCoord3dv
#define clTexCoord3f CL_OpenGL::functions->texCoord3f
#define clTexCoord3fv CL_OpenGL::functions->texCoord3fv
#define clTexCoord3i CL_OpenGL::functions->texCoord3i
#define clTexCoord3iv CL_OpenGL::functions->texCoord3iv
#define clTexCoord3s CL_OpenGL::functions->texCoord3s
#define clTexCoord3sv CL_OpenGL::functions->texCoord3sv
#define clTexCoord4d CL_OpenGL::functions->texCoord4d
#define clTexCoord4dv CL_OpenGL::functions->texCoord4dv
#define clTexCoord4f CL_OpenGL::functions->texCoord4f
#define clTexCoord4fv CL_OpenGL::functions->texCoord4fv
#define clTexCoord4i CL_OpenGL::functions->texCoord4i
#define clTexCoord4iv CL_OpenGL::functions->texCoord4iv
#define clTexCoord4s CL_OpenGL::functions->texCoord4s
#define clTexCoord4sv CL_OpenGL::functions->texCoord4sv
#define clTexCoordPointer CL_OpenGL::functions->texCoordPointer
#define clTexEnvf CL_OpenGL::functions->texEnvf
#define clTexEnvfv CL_OpenGL::functions->texEnvfv
#define clTexEnvi CL_OpenGL::functions->texEnvi
#define clTexEnviv CL_OpenGL::functions->texEnviv
#define clTexGend CL_OpenGL::functions->texGend
#define clTexGendv CL_OpenGL::functions->texGendv
#define clTexGenf CL_OpenGL::functions->texGenf
#define clTexGenfv CL_OpenGL::functions->texGenfv
#define clTexGeni CL_OpenGL::functions->texGeni
#define clTexGeniv CL_OpenGL::functions->texGeniv
#define clTexImage1D CL_OpenGL::functions->texImage1D
#define clTexImage2D CL_OpenGL::functions->texImage2D
#define clTexParameterf CL_OpenGL::functions->texParameterf
#define clTexParameterfv CL_OpenGL::functions->texParameterfv
#define clTexParameteri CL_OpenGL::functions->texParameteri
#define clTexParameteriv CL_OpenGL::functions->texParameteriv
#define clTexSubImage1D CL_OpenGL::functions->texSubImage1D
#define clTexSubImage2D CL_OpenGL::functions->texSubImage2D
#define clTexSubImage3D CL_OpenGL::functions->texSubImage3D
#define clTranslated CL_OpenGL::functions->translated
#define clTranslatef CL_OpenGL::functions->translatef
#define clVertex2d CL_OpenGL::functions->vertex2d
#define clVertex2dv CL_OpenGL::functions->vertex2dv
#define clVertex2f CL_OpenGL::functions->vertex2f
#define clVertex2fv CL_OpenGL::functions->vertex2fv
#define clVertex2i CL_OpenGL::functions->vertex2i
#define clVertex2iv CL_OpenGL::functions->vertex2iv
#define clVertex2s CL_OpenGL::functions->vertex2s
#define clVertex2sv CL_OpenGL::functions->vertex2sv
#define clVertex3d CL_OpenGL::functions->vertex3d
#define clVertex3dv CL_OpenGL::functions->vertex3dv
#define clVertex3f CL_OpenGL::functions->vertex3f
#define clVertex3fv CL_OpenGL::functions->vertex3fv
#define clVertex3i CL_OpenGL::functions->vertex3i
#define clVertex3iv CL_OpenGL::functions->vertex3iv
#define clVertex3s CL_OpenGL::functions->vertex3s
#define clVertex3sv CL_OpenGL::functions->vertex3sv
#define clVertex4d CL_OpenGL::functions->vertex4d
#define clVertex4dv CL_OpenGL::functions->vertex4dv
#define clVertex4f CL_OpenGL::functions->vertex4f
#define clVertex4fv CL_OpenGL::functions->vertex4fv
#define clVertex4i CL_OpenGL::functions->vertex4i
#define clVertex4iv CL_OpenGL::functions->vertex4iv
#define clVertex4s CL_OpenGL::functions->vertex4s
#define clVertex4sv CL_OpenGL::functions->vertex4sv
#define clVertexPointer CL_OpenGL::functions->vertexPointer
#define clViewport CL_OpenGL::functions->viewport
#define clDrawRangeElements CL_OpenGL::functions->drawRangeElements
#define clTexImage3D CL_OpenGL::functions->texImage3D
#define clColorTable CL_OpenGL::functions->colorTable
#define clCopyColorTable CL_OpenGL::functions->copyColorTable
#define clColorTableParameteriv CL_OpenGL::functions->colorTableParameteriv
#define clColorTableParameterfv CL_OpenGL::functions->colorTableParameterfv
#define clGetColorTable CL_OpenGL::functions->getColorTable
#define clGetColorTableParameteriv CL_OpenGL::functions->getColorTableParameteriv
#define clGetColorTableParameterfv CL_OpenGL::functions->getColorTableParameterfv
#define clColorSubTable CL_OpenGL::functions->colorSubTable
#define clCopyColorSubTable CL_OpenGL::functions->copyColorSubTable
#define clConvolutionFilter1D CL_OpenGL::functions->convolutionFilter1D
#define clConvolutionFilter2D CL_OpenGL::functions->convolutionFilter2D
#define clCopyConvolutionFilter1D CL_OpenGL::functions->copyConvolutionFilter1D
#define clCopyConvolutionFilter2D CL_OpenGL::functions->copyConvolutionFilter2D
#define clGetConvolutionFilter CL_OpenGL::functions->getConvolutionFilter
#define clSeparableFilter2D CL_OpenGL::functions->separableFilter2D
#define clGetSeparableFilter CL_OpenGL::functions->getSeparableFilter
#define clConvolutionParameteri CL_OpenGL::functions->convolutionParameteri
#define clConvolutionParameteriv CL_OpenGL::functions->convolutionParameteriv
#define clConvolutionParameterf CL_OpenGL::functions->convolutionParameterf
#define clConvolutionParameterfv CL_OpenGL::functions->convolutionParameterfv
#define clGetConvolutionParameteriv CL_OpenGL::functions->getConvolutionParameteriv
#define clGetConvolutionParameterfv CL_OpenGL::functions->getConvolutionParameterfv
#define clHistogram CL_OpenGL::functions->histogram
#define clResetHistogram CL_OpenGL::functions->resetHistogram
#define clGetHistogram CL_OpenGL::functions->getHistogram
#define clGetHistogramParameteriv CL_OpenGL::functions->getHistogramParameteriv
#define clGetHistogramParameterfv CL_OpenGL::functions->getHistogramParameterfv
#define clMinmax CL_OpenGL::functions->minmax
#define clResetMinmax CL_OpenGL::functions->resetMinmax
#define clGetMinmax CL_OpenGL::functions->getMinmax
#define clGetMinmaxParameteriv CL_OpenGL::functions->getMinmaxParameteriv
#define clGetMinmaxParameterfv CL_OpenGL::functions->getMinmaxParameterfv
#define clBlendColor CL_OpenGL::functions->blendColor
#define clBlendEquation CL_OpenGL::functions->blendEquation
#define clActiveTexture CL_OpenGL::functions->activeTexture
#define clClientActiveTexture CL_OpenGL::functions->clientActiveTexture
#define clMultiTexCoord1d CL_OpenGL::functions->multiTexCoord1d
#define clMultiTexCoord1dv CL_OpenGL::functions->multiTexCoord1dv
#define clMultiTexCoord1f CL_OpenGL::functions->multiTexCoord1f
#define clMultiTexCoord1fv CL_OpenGL::functions->multiTexCoord1fv
#define clMultiTexCoord1i CL_OpenGL::functions->multiTexCoord1i
#define clMultiTexCoord1iv CL_OpenGL::functions->multiTexCoord1iv
#define clMultiTexCoord1s CL_OpenGL::functions->multiTexCoord1s
#define clMultiTexCoord1sv CL_OpenGL::functions->multiTexCoord1sv
#define clMultiTexCoord2d CL_OpenGL::functions->multiTexCoord2d
#define clMultiTexCoord2dv CL_OpenGL::functions->multiTexCoord2dv
#define clMultiTexCoord2f CL_OpenGL::functions->multiTexCoord2f
#define clMultiTexCoord2fv CL_OpenGL::functions->multiTexCoord2fv
#define clMultiTexCoord2i CL_OpenGL::functions->multiTexCoord2i
#define clMultiTexCoord2iv CL_OpenGL::functions->multiTexCoord2iv
#define clMultiTexCoord2s CL_OpenGL::functions->multiTexCoord2s
#define clMultiTexCoord2sv CL_OpenGL::functions->multiTexCoord2sv
#define clMultiTexCoord3d CL_OpenGL::functions->multiTexCoord3d
#define clMultiTexCoord3dv CL_OpenGL::functions->multiTexCoord3dv
#define clMultiTexCoord3f CL_OpenGL::functions->multiTexCoord3f
#define clMultiTexCoord3fv CL_OpenGL::functions->multiTexCoord3fv
#define clMultiTexCoord3i CL_OpenGL::functions->multiTexCoord3i
#define clMultiTexCoord3iv CL_OpenGL::functions->multiTexCoord3iv
#define clMultiTexCoord3s CL_OpenGL::functions->multiTexCoord3s
#define clMultiTexCoord3sv CL_OpenGL::functions->multiTexCoord3sv
#define clMultiTexCoord4d CL_OpenGL::functions->multiTexCoord4d
#define clMultiTexCoord4dv CL_OpenGL::functions->multiTexCoord4dv
#define clMultiTexCoord4f CL_OpenGL::functions->multiTexCoord4f
#define clMultiTexCoord4fv CL_OpenGL::functions->multiTexCoord4fv
#define clMultiTexCoord4i CL_OpenGL::functions->multiTexCoord4i
#define clMultiTexCoord4iv CL_OpenGL::functions->multiTexCoord4iv
#define clMultiTexCoord4s CL_OpenGL::functions->multiTexCoord4s
#define clMultiTexCoord4sv CL_OpenGL::functions->multiTexCoord4sv
#define clCompressedTexImage1D CL_OpenGL::functions->compressedTexImage1D
#define clCompressedTexImage2D CL_OpenGL::functions->compressedTexImage2D
#define clCompressedTexImage3D CL_OpenGL::functions->compressedTexImage3D
#define clCompressedTexSubImage1D CL_OpenGL::functions->compressedTexSubImage1D
#define clCompressedTexSubImage2D CL_OpenGL::functions->compressedTexSubImage2D
#define clCompressedTexSubImage3D CL_OpenGL::functions->compressedTexSubImage3D
#define clGetCompressedTexImage CL_OpenGL::functions->getCompressedTexImage
#define clSampleCoverage CL_OpenGL::functions->sampleCoverage
#define clLoadTransposeMatrixd CL_OpenGL::functions->glLoadTransposeMatrixd
#define clLoadTransposeMatrixf CL_OpenGL::functions->glLoadTransposeMatrixf
#define clMultTransposeMatrixd CL_OpenGL::functions->glMultTransposeMatrixd
#define clMultTransposeMatrixf CL_OpenGL::functions->glMultTransposeMatrixf
#define clFogCoordd CL_OpenGL::functions->fogCoordd
#define clFogCoorddv CL_OpenGL::functions->fogCoorddv
#define clFogCoordf CL_OpenGL::functions->fogCoordf
#define clFogCoordfv CL_OpenGL::functions->fogCoordfv
#define clFogCoordPointer CL_OpenGL::functions->fogCoordPointer
#define clMultiDrawArrays CL_OpenGL::functions->multiDrawArrays
#define clMultiDrawElements CL_OpenGL::functions->multiDrawElementsEXT
#define clPointParameterf CL_OpenGL::functions->pointParameterf
#define clPointParameterfv CL_OpenGL::functions->pointParameterfv
#define clSecondaryColor3b CL_OpenGL::functions->secondaryColor3b
#define clSecondaryColor3bv CL_OpenGL::functions->secondaryColor3bv
#define clSecondaryColor3d CL_OpenGL::functions->secondaryColor3d
#define clSecondaryColor3dv CL_OpenGL::functions->secondaryColor3dv
#define clSecondaryColor3f CL_OpenGL::functions->secondaryColor3f
#define clSecondaryColor3fv CL_OpenGL::functions->secondaryColor3fv
#define clSecondaryColor3i CL_OpenGL::functions->secondaryColor3i
#define clSecondaryColor3iv CL_OpenGL::functions->secondaryColor3iv
#define clSecondaryColor3s CL_OpenGL::functions->secondaryColor3s
#define clSecondaryColor3sv CL_OpenGL::functions->secondaryColor3sv
#define clSecondaryColor3ub CL_OpenGL::functions->secondaryColor3ub
#define clSecondaryColor3ubv CL_OpenGL::functions->secondaryColor3ubv
#define clSecondaryColor3ui CL_OpenGL::functions->secondaryColor3ui
#define clSecondaryColor3uiv CL_OpenGL::functions->secondaryColor3uiv
#define clSecondaryColor3us CL_OpenGL::functions->secondaryColor3us
#define clSecondaryColor3usv CL_OpenGL::functions->secondaryColor3usv
#define clSecondaryColorPointer CL_OpenGL::functions->secondaryColorPointer
#define clBlendFuncSeparate CL_OpenGL::functions->blendFuncSeparate
#define clWindowPos2d CL_OpenGL::functions->windowPos2d
#define clWindowPos2dv CL_OpenGL::functions->windowPos2dv
#define clWindowPos2f CL_OpenGL::functions->windowPos2f
#define clWindowPos2fv CL_OpenGL::functions->windowPos2fv
#define clWindowPos2i CL_OpenGL::functions->windowPos2i
#define clWindowPos2iv CL_OpenGL::functions->windowPos2iv
#define clWindowPos2s CL_OpenGL::functions->windowPos2s
#define clWindowPos2sv CL_OpenGL::functions->windowPos2sv
#define clWindowPos3d CL_OpenGL::functions->windowPos3d
#define clWindowPos3dv CL_OpenGL::functions->windowPos3dv
#define clWindowPos3f CL_OpenGL::functions->windowPos3f
#define clWindowPos3fv CL_OpenGL::functions->windowPos3fv
#define clWindowPos3i CL_OpenGL::functions->windowPos3i
#define clWindowPos3iv CL_OpenGL::functions->windowPos3iv
#define clWindowPos3s CL_OpenGL::functions->windowPos3s
#define clWindowPos3sv CL_OpenGL::functions->windowPos3sv
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
#define clGenQueries CL_OpenGL::functions->genQueries
#define clDeleteQueries CL_OpenGL::functions->deleteQueries
#define clIsQuery CL_OpenGL::functions->isQuery
#define clBeginQuery CL_OpenGL::functions->beginQuery
#define clEndQuery CL_OpenGL::functions->endQuery
#define clGetQueryiv CL_OpenGL::functions->getQueryiv
#define clGetQueryObjectiv CL_OpenGL::functions->getQueryObjectiv
#define clGetQueryObjectuiv CL_OpenGL::functions->getQueryObjectuiv
#define clDeleteShader CL_OpenGL::functions->deleteShader
#define clDeleteProgram CL_OpenGL::functions->deleteProgram
#define clGetHandle CL_OpenGL::functions->getHandle
#define clDetachShader CL_OpenGL::functions->detachShader
#define clCreateShader CL_OpenGL::functions->createShader
#define clShaderSource CL_OpenGL::functions->shaderSource
#define clCompileShader CL_OpenGL::functions->compileShader
#define clCreateProgram CL_OpenGL::functions->createProgram
#define clAttachShader CL_OpenGL::functions->attachShader
#define clLinkProgram CL_OpenGL::functions->linkProgram
#define clUseProgram CL_OpenGL::functions->useProgram
#define clValidateProgram CL_OpenGL::functions->validateProgram
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
#define clGetProgramfv CL_OpenGL::functions->getProgramfv
#define clGetProgramiv CL_OpenGL::functions->getProgramiv
#define clGetShaderfv CL_OpenGL::functions->getShaderfv
#define clGetShaderiv CL_OpenGL::functions->getShaderiv
#define clGetShaderInfoLog CL_OpenGL::functions->getShaderInfoLog
#define clGetProgramInfoLog CL_OpenGL::functions->getProgramInfoLog
#define clGetAttachedShaders CL_OpenGL::functions->getAttachedShaders
#define clGetUniformLocation CL_OpenGL::functions->getUniformLocation
#define clGetActiveUniform CL_OpenGL::functions->getActiveUniform
#define clGetUniformfv CL_OpenGL::functions->getUniformfv
#define clGetUniformiv CL_OpenGL::functions->getUniformiv
#define clGetShaderSource CL_OpenGL::functions->getShaderSource
#define clVertexAttrib1f CL_OpenGL::functions->vertexAttrib1f
#define clVertexAttrib1s CL_OpenGL::functions->vertexAttrib1s
#define clVertexAttrib1d CL_OpenGL::functions->vertexAttrib1d
#define clVertexAttrib2f CL_OpenGL::functions->vertexAttrib2f
#define clVertexAttrib2s CL_OpenGL::functions->vertexAttrib2s
#define clVertexAttrib2d CL_OpenGL::functions->vertexAttrib2d
#define clVertexAttrib3f CL_OpenGL::functions->vertexAttrib3f
#define clVertexAttrib3s CL_OpenGL::functions->vertexAttrib3s
#define clVertexAttrib3d CL_OpenGL::functions->vertexAttrib3d
#define clVertexAttrib4f CL_OpenGL::functions->vertexAttrib4f
#define clVertexAttrib4s CL_OpenGL::functions->vertexAttrib4s
#define clVertexAttrib4d CL_OpenGL::functions->vertexAttrib4d
#define clVertexAttrib4Nub CL_OpenGL::functions->vertexAttrib4Nub
#define clVertexAttrib1fv CL_OpenGL::functions->vertexAttrib1fv
#define clVertexAttrib1sv CL_OpenGL::functions->vertexAttrib1sv
#define clVertexAttrib1dv CL_OpenGL::functions->vertexAttrib1dv
#define clVertexAttrib2fv CL_OpenGL::functions->vertexAttrib2fv
#define clVertexAttrib2sv CL_OpenGL::functions->vertexAttrib2sv
#define clVertexAttrib2dv CL_OpenGL::functions->vertexAttrib2dv
#define clVertexAttrib3fv CL_OpenGL::functions->vertexAttrib3fv
#define clVertexAttrib3sv CL_OpenGL::functions->vertexAttrib3sv
#define clVertexAttrib3dv CL_OpenGL::functions->vertexAttrib3dv
#define clVertexAttrib4fv CL_OpenGL::functions->vertexAttrib4fv
#define clVertexAttrib4sv CL_OpenGL::functions->vertexAttrib4sv
#define clVertexAttrib4dv CL_OpenGL::functions->vertexAttrib4dv
#define clVertexAttrib4iv CL_OpenGL::functions->vertexAttrib4iv
#define clVertexAttrib4bv CL_OpenGL::functions->vertexAttrib4bv
#define clVertexAttrib4ubv CL_OpenGL::functions->vertexAttrib4ubv
#define clVertexAttrib4usv CL_OpenGL::functions->vertexAttrib4usv
#define clVertexAttrib4uiv CL_OpenGL::functions->vertexAttrib4uiv
#define clVertexAttrib4Nbv CL_OpenGL::functions->vertexAttrib4Nbv
#define clVertexAttrib4Nsv CL_OpenGL::functions->vertexAttrib4Nsv
#define clVertexAttrib4Niv CL_OpenGL::functions->vertexAttrib4Niv
#define clVertexAttrib4Nubv CL_OpenGL::functions->vertexAttrib4Nubv
#define clVertexAttrib4Nusv CL_OpenGL::functions->vertexAttrib4Nusv
#define clVertexAttrib4Nuiv CL_OpenGL::functions->vertexAttrib4Nuiv
#define clVertexAttribPointer CL_OpenGL::functions->vertexAttribPointer
#define clEnableVertexAttribArray CL_OpenGL::functions->enableVertexAttribArray
#define clDisableVertexAttribArray CL_OpenGL::functions->disableVertexAttribArray
#define clBindAttribLocation CL_OpenGL::functions->bindAttribLocation
#define clGetActiveAttrib CL_OpenGL::functions->getActiveAttrib
#define clGetAttribLocation CL_OpenGL::functions->getAttribLocation
#define clGetVertexAttribdv CL_OpenGL::functions->getVertexAttribdv
#define clGetVertexAttribfv CL_OpenGL::functions->getVertexAttribfv
#define clGetVertexAttribiv CL_OpenGL::functions->getVertexAttribiv
#define clGetVertexAttribPointerv CL_OpenGL::functions->getVertexAttribPointerv
#define clDrawBuffers CL_OpenGL::functions->drawBuffers
#define clStencilFuncSeparate CL_OpenGL::functions->stencilFuncSeparate
#define clStencilOpSeparate CL_OpenGL::functions->stencilOpSeparate
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

/// \}

