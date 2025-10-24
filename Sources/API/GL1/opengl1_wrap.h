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
**     cl1aim that you wrote the original software. If you use this software
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

/// \addtogroup clanGL1_System clanGL1 System
/// \{

#pragma once

#include "opengl1.h"
#include "opengl1_defines.h"

#ifdef WIN32
#define CL_GL1FUNC WINAPI
#else
#define CL_GL1FUNC
#endif

/// \name OpenGL 1.3 binds
/// \{

/// \brief CL_GL1Functions
///
/// \xmlonly !group=GL1/System! !header=gl1.h! \endxmlonly
class CL_GL1Functions
{

/// \name Typedefs
/// \{

public:
	typedef void (CL_GL1FUNC *ptr_glAccum)(CLenum op, CLfloat value);
	typedef void (CL_GL1FUNC *ptr_glAlphaFunc)(CLenum func, CLclampf ref);
	typedef CLboolean (CL_GL1FUNC *ptr_glAreTexturesResident)(CLsizei n, const CLuint *textures, CLboolean *residences);
	typedef void (CL_GL1FUNC *ptr_glArrayElement)(CLint i);
	typedef void (CL_GL1FUNC *ptr_glBegin)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glBindTexture)(CLenum target, CLuint texture);
	typedef void (CL_GL1FUNC *ptr_glBitmap)(CLsizei width, CLsizei height, CLfloat xorig, CLfloat yorig, CLfloat xmove, CLfloat ymove, const CLubyte *bitmap);
	typedef void (CL_GL1FUNC *ptr_glBlendFunc)(CLenum sfactor, CLenum dfactor);
	typedef void (CL_GL1FUNC *ptr_glCallList)(CLuint list);
	typedef void (CL_GL1FUNC *ptr_glCallLists)(CLsizei n, CLenum type, const CLvoid *lists);
	typedef void (CL_GL1FUNC *ptr_glClear)(CLbitfield mask);
	typedef void (CL_GL1FUNC *ptr_glClearAccum)(CLfloat red, CLfloat green, CLfloat blue, CLfloat alpha);
	typedef void (CL_GL1FUNC *ptr_glClearColor)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GL1FUNC *ptr_glClearDepth)(CLclampd depth);
	typedef void (CL_GL1FUNC *ptr_glClearIndex)(CLfloat c);
	typedef void (CL_GL1FUNC *ptr_glClearStencil)(CLint s);
	typedef void (CL_GL1FUNC *ptr_glClipPlane)(CLenum plane, const CLdouble *equation);
	typedef void (CL_GL1FUNC *ptr_glColor3b)(CLbyte red, CLbyte green, CLbyte blue);
	typedef void (CL_GL1FUNC *ptr_glColor3bv)(const CLbyte *v);
	typedef void (CL_GL1FUNC *ptr_glColor3d)(CLdouble red, CLdouble green, CLdouble blue);
	typedef void (CL_GL1FUNC *ptr_glColor3dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glColor3f)(CLfloat red, CLfloat green, CLfloat blue);
	typedef void (CL_GL1FUNC *ptr_glColor3fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glColor3i)(CLint red, CLint green, CLint blue);
	typedef void (CL_GL1FUNC *ptr_glColor3iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glColor3s)(CLshort red, CLshort green, CLshort blue);
	typedef void (CL_GL1FUNC *ptr_glColor3sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glColor3ub)(CLubyte red, CLubyte green, CLubyte blue);
	typedef void (CL_GL1FUNC *ptr_glColor3ubv)(const CLubyte *v);
	typedef void (CL_GL1FUNC *ptr_glColor3ui)(CLuint red, CLuint green, CLuint blue);
	typedef void (CL_GL1FUNC *ptr_glColor3uiv)(const CLuint *v);
	typedef void (CL_GL1FUNC *ptr_glColor3us)(CLushort red, CLushort green, CLushort blue);
	typedef void (CL_GL1FUNC *ptr_glColor3usv)(const CLushort *v);
	typedef void (CL_GL1FUNC *ptr_glColor4b)(CLbyte red, CLbyte green, CLbyte blue, CLbyte alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4bv)(const CLbyte *v);
	typedef void (CL_GL1FUNC *ptr_glColor4d)(CLdouble red, CLdouble green, CLdouble blue, CLdouble alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glColor4f)(CLfloat red, CLfloat green, CLfloat blue, CLfloat alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glColor4i)(CLint red, CLint green, CLint blue, CLint alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glColor4s)(CLshort red, CLshort green, CLshort blue, CLshort alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glColor4ub)(CLubyte red, CLubyte green, CLubyte blue, CLubyte alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4ubv)(const CLubyte *v);
	typedef void (CL_GL1FUNC *ptr_glColor4ui)(CLuint red, CLuint green, CLuint blue, CLuint alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4uiv)(const CLuint *v);
	typedef void (CL_GL1FUNC *ptr_glColor4us)(CLushort red, CLushort green, CLushort blue, CLushort alpha);
	typedef void (CL_GL1FUNC *ptr_glColor4usv)(const CLushort *v);
	typedef void (CL_GL1FUNC *ptr_glColorMask)(CLboolean red, CLboolean green, CLboolean blue, CLboolean alpha);
	typedef void (CL_GL1FUNC *ptr_glColorMaterial)(CLenum face, CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glColorPointer)(CLint size, CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GL1FUNC *ptr_glCopyPixels)(CLint x, CLint y, CLsizei width, CLsizei height, CLenum type);
	typedef void (CL_GL1FUNC *ptr_glCopyTexImage1D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLint border);
	typedef void (CL_GL1FUNC *ptr_glCopyTexImage2D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLsizei height, CLint border);
	typedef void (CL_GL1FUNC *ptr_glCopyTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLint x, CLint y, CLsizei width);
	typedef void (CL_GL1FUNC *ptr_glCopyTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GL1FUNC *ptr_glCullFace)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glDeleteLists)(CLuint list, CLsizei range);
	typedef void (CL_GL1FUNC *ptr_glDeleteTextures)(CLsizei n, const CLuint *textures);
	typedef void (CL_GL1FUNC *ptr_glDepthFunc)(CLenum func);
	typedef void (CL_GL1FUNC *ptr_glDepthMask)(CLboolean flag);
	typedef void (CL_GL1FUNC *ptr_glDepthRange)(CLclampd zNear, CLclampd zFar);
	typedef void (CL_GL1FUNC *ptr_glDisable)(CLenum cap);
	typedef void (CL_GL1FUNC *ptr_glDisableClientState)(CLenum array);
	typedef void (CL_GL1FUNC *ptr_glDrawArrays)(CLenum mode, CLint first, CLsizei count);
	typedef void (CL_GL1FUNC *ptr_glDrawBuffer)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glDrawElements)(CLenum mode, CLsizei count, CLenum type, const CLvoid *indices);
	typedef void (CL_GL1FUNC *ptr_glDrawPixels)(CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glEdgeFlag)(CLboolean flag);
	typedef void (CL_GL1FUNC *ptr_glEdgeFlagPointer)(CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GL1FUNC *ptr_glEdgeFlagv)(const CLboolean *flag);
	typedef void (CL_GL1FUNC *ptr_glEnable)(CLenum cap);
	typedef void (CL_GL1FUNC *ptr_glEnableClientState)(CLenum array);
	typedef void (CL_GL1FUNC *ptr_glEnd)(void);
	typedef void (CL_GL1FUNC *ptr_glEndList)(void);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord1d)(CLdouble u);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord1dv)(const CLdouble *u);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord1f)(CLfloat u);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord1fv)(const CLfloat *u);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord2d)(CLdouble u, CLdouble v);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord2dv)(const CLdouble *u);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord2f)(CLfloat u, CLfloat v);
	typedef void (CL_GL1FUNC *ptr_glEvalCoord2fv)(const CLfloat *u);
	typedef void (CL_GL1FUNC *ptr_glEvalMesh1)(CLenum mode, CLint i1, CLint i2);
	typedef void (CL_GL1FUNC *ptr_glEvalMesh2)(CLenum mode, CLint i1, CLint i2, CLint j1, CLint j2);
	typedef void (CL_GL1FUNC *ptr_glEvalPoint1)(CLint i);
	typedef void (CL_GL1FUNC *ptr_glEvalPoint2)(CLint i, CLint j);
	typedef void (CL_GL1FUNC *ptr_glFeedbackBuffer)(CLsizei size, CLenum type, CLfloat *buffer);
	typedef void (CL_GL1FUNC *ptr_glFinish)(void);
	typedef void (CL_GL1FUNC *ptr_glFlush)(void);
	typedef void (CL_GL1FUNC *ptr_glFogf)(CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glFogfv)(CLenum pname, const CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glFogi)(CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glFogiv)(CLenum pname, const CLint *params);
	typedef void (CL_GL1FUNC *ptr_glFrontFace)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glFrustum)(CLdouble left, CLdouble right, CLdouble bottom, CLdouble top, CLdouble zNear, CLdouble zFar);
	typedef CLuint (CL_GL1FUNC *ptr_glGenLists)(CLsizei range);
	typedef void (CL_GL1FUNC *ptr_glGenTextures)(CLsizei n, CLuint *textures);
	typedef void (CL_GL1FUNC *ptr_glGetBooleanv)(CLenum pname, CLboolean *params);
	typedef void (CL_GL1FUNC *ptr_glGetClipPlane)(CLenum plane, CLdouble *equation);
	typedef void (CL_GL1FUNC *ptr_glGetDoublev)(CLenum pname, CLdouble *params);
	typedef CLenum (CL_GL1FUNC *ptr_glGetError)(void);
	typedef void (CL_GL1FUNC *ptr_glGetFloatv)(CLenum pname, CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glGetIntegerv)(CLenum pname, CLint *params);
	typedef void (CL_GL1FUNC *ptr_glGetLightfv)(CLenum light, CLenum pname, CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glGetLightiv)(CLenum light, CLenum pname, CLint *params);
	typedef void (CL_GL1FUNC *ptr_glGetMapdv)(CLenum target, CLenum query, CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glGetMapfv)(CLenum target, CLenum query, CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glGetMapiv)(CLenum target, CLenum query, CLint *v);
	typedef void (CL_GL1FUNC *ptr_glGetMaterialfv)(CLenum face, CLenum pname, CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glGetMaterialiv)(CLenum face, CLenum pname, CLint *params);
	typedef void (CL_GL1FUNC *ptr_glGetPixelMapfv)(CLenum map, CLfloat *values);
	typedef void (CL_GL1FUNC *ptr_glGetPixelMapuiv)(CLenum map, CLuint *values);
	typedef void (CL_GL1FUNC *ptr_glGetPixelMapusv)(CLenum map, CLushort *values);
	typedef void (CL_GL1FUNC *ptr_glGetPointerv)(CLenum pname, CLvoid* *params);
	typedef void (CL_GL1FUNC *ptr_glGetPolygonStipple)(CLubyte *mask);
	typedef const CLubyte *(CL_GL1FUNC *ptr_glGetString)(CLenum name);
	typedef void (CL_GL1FUNC *ptr_glGetTexEnvfv)(CLenum target, CLenum pname, CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexEnviv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexGendv)(CLenum coord, CLenum pname, CLdouble *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexGenfv)(CLenum coord, CLenum pname, CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexGeniv)(CLenum coord, CLenum pname, CLint *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexImage)(CLenum target, CLint level, CLenum format, CLenum type, CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glGetTexLevelParameterfv)(CLenum target, CLint level, CLenum pname, CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexLevelParameteriv)(CLenum target, CLint level, CLenum pname, CLint *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexParameterfv)(CLenum target, CLenum pname, CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glGetTexParameteriv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GL1FUNC *ptr_glHint)(CLenum target, CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glIndexMask)(CLuint mask);
	typedef void (CL_GL1FUNC *ptr_glIndexPointer)(CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GL1FUNC *ptr_glIndexd)(CLdouble c);
	typedef void (CL_GL1FUNC *ptr_glIndexdv)(const CLdouble *c);
	typedef void (CL_GL1FUNC *ptr_glIndexf)(CLfloat c);
	typedef void (CL_GL1FUNC *ptr_glIndexfv)(const CLfloat *c);
	typedef void (CL_GL1FUNC *ptr_glIndexi)(CLint c);
	typedef void (CL_GL1FUNC *ptr_glIndexiv)(const CLint *c);
	typedef void (CL_GL1FUNC *ptr_glIndexs)(CLshort c);
	typedef void (CL_GL1FUNC *ptr_glIndexsv)(const CLshort *c);
	typedef void (CL_GL1FUNC *ptr_glIndexub)(CLubyte c);
	typedef void (CL_GL1FUNC *ptr_glIndexubv)(const CLubyte *c);
	typedef void (CL_GL1FUNC *ptr_glInitNames)(void);
	typedef void (CL_GL1FUNC *ptr_glInterleavedArrays)(CLenum format, CLsizei stride, const CLvoid *pointer);
	typedef CLboolean (CL_GL1FUNC *ptr_glIsEnabled)(CLenum cap);
	typedef CLboolean (CL_GL1FUNC *ptr_glIsList)(CLuint list);
	typedef CLboolean (CL_GL1FUNC *ptr_glIsTexture)(CLuint texture);
	typedef void (CL_GL1FUNC *ptr_glLightModelf)(CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glLightModelfv)(CLenum pname, const CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glLightModeli)(CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glLightModeliv)(CLenum pname, const CLint *params);
	typedef void (CL_GL1FUNC *ptr_glLightf)(CLenum light, CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glLightfv)(CLenum light, CLenum pname, const CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glLighti)(CLenum light, CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glLightiv)(CLenum light, CLenum pname, const CLint *params);
	typedef void (CL_GL1FUNC *ptr_glLineStipple)(CLint factor, CLushort pattern);
	typedef void (CL_GL1FUNC *ptr_glLineWidth)(CLfloat width);
	typedef void (CL_GL1FUNC *ptr_glListBase)(CLuint base);
	typedef void (CL_GL1FUNC *ptr_glLoadIdentity)(void);
	typedef void (CL_GL1FUNC *ptr_glLoadMatrixd)(const CLdouble *m);
	typedef void (CL_GL1FUNC *ptr_glLoadMatrixf)(const CLfloat *m);
	typedef void (CL_GL1FUNC *ptr_glLoadName)(CLuint name);
	typedef void (CL_GL1FUNC *ptr_glLogicOp)(CLenum opcode);
	typedef void (CL_GL1FUNC *ptr_glMap1d)(CLenum target, CLdouble u1, CLdouble u2, CLint stride, CLint order, const CLdouble *points);
	typedef void (CL_GL1FUNC *ptr_glMap1f)(CLenum target, CLfloat u1, CLfloat u2, CLint stride, CLint order, const CLfloat *points);
	typedef void (CL_GL1FUNC *ptr_glMap2d)(CLenum target, CLdouble u1, CLdouble u2, CLint ustride, CLint uorder, CLdouble v1, CLdouble v2, CLint vstride, CLint vorder, const CLdouble *points);
	typedef void (CL_GL1FUNC *ptr_glMap2f)(CLenum target, CLfloat u1, CLfloat u2, CLint ustride, CLint uorder, CLfloat v1, CLfloat v2, CLint vstride, CLint vorder, const CLfloat *points);
	typedef void (CL_GL1FUNC *ptr_glMapGrid1d)(CLint un, CLdouble u1, CLdouble u2);
	typedef void (CL_GL1FUNC *ptr_glMapGrid1f)(CLint un, CLfloat u1, CLfloat u2);
	typedef void (CL_GL1FUNC *ptr_glMapGrid2d)(CLint un, CLdouble u1, CLdouble u2, CLint vn, CLdouble v1, CLdouble v2);
	typedef void (CL_GL1FUNC *ptr_glMapGrid2f)(CLint un, CLfloat u1, CLfloat u2, CLint vn, CLfloat v1, CLfloat v2);
	typedef void (CL_GL1FUNC *ptr_glMaterialf)(CLenum face, CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glMaterialfv)(CLenum face, CLenum pname, const CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glMateriali)(CLenum face, CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glMaterialiv)(CLenum face, CLenum pname, const CLint *params);
	typedef void (CL_GL1FUNC *ptr_glMatrixMode)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glMultMatrixd)(const CLdouble *m);
	typedef void (CL_GL1FUNC *ptr_glMultMatrixf)(const CLfloat *m);
	typedef void (CL_GL1FUNC *ptr_glNewList)(CLuint list, CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glNormal3b)(CLbyte nx, CLbyte ny, CLbyte nz);
	typedef void (CL_GL1FUNC *ptr_glNormal3bv)(const CLbyte *v);
	typedef void (CL_GL1FUNC *ptr_glNormal3d)(CLdouble nx, CLdouble ny, CLdouble nz);
	typedef void (CL_GL1FUNC *ptr_glNormal3dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glNormal3f)(CLfloat nx, CLfloat ny, CLfloat nz);
	typedef void (CL_GL1FUNC *ptr_glNormal3fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glNormal3i)(CLint nx, CLint ny, CLint nz);
	typedef void (CL_GL1FUNC *ptr_glNormal3iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glNormal3s)(CLshort nx, CLshort ny, CLshort nz);
	typedef void (CL_GL1FUNC *ptr_glNormal3sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glNormalPointer)(CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GL1FUNC *ptr_glOrtho)(CLdouble left, CLdouble right, CLdouble bottom, CLdouble top, CLdouble zNear, CLdouble zFar);
	typedef void (CL_GL1FUNC *ptr_glPassThrough)(CLfloat token);
	typedef void (CL_GL1FUNC *ptr_glPixelMapfv)(CLenum map, CLint mapsize, const CLfloat *values);
	typedef void (CL_GL1FUNC *ptr_glPixelMapuiv)(CLenum map, CLint mapsize, const CLuint *values);
	typedef void (CL_GL1FUNC *ptr_glPixelMapusv)(CLenum map, CLint mapsize, const CLushort *values);
	typedef void (CL_GL1FUNC *ptr_glPixelStoref)(CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glPixelStorei)(CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glPixelTransferf)(CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glPixelTransferi)(CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glPixelZoom)(CLfloat xfactor, CLfloat yfactor);
	typedef void (CL_GL1FUNC *ptr_glPointSize)(CLfloat size);
	typedef void (CL_GL1FUNC *ptr_glPolygonMode)(CLenum face, CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glPolygonOffset)(CLfloat factor, CLfloat units);
	typedef void (CL_GL1FUNC *ptr_glPolygonStipple)(const CLubyte *mask);
	typedef void (CL_GL1FUNC *ptr_glPopAttrib)(void);
	typedef void (CL_GL1FUNC *ptr_glPopClientAttrib)(void);
	typedef void (CL_GL1FUNC *ptr_glPopMatrix)(void);
	typedef void (CL_GL1FUNC *ptr_glPopName)(void);
	typedef void (CL_GL1FUNC *ptr_glPrioritizeTextures)(CLsizei n, const CLuint *textures, const CLclampf *priorities);
	typedef void (CL_GL1FUNC *ptr_glPushAttrib)(CLbitfield mask);
	typedef void (CL_GL1FUNC *ptr_glPushClientAttrib)(CLbitfield mask);
	typedef void (CL_GL1FUNC *ptr_glPushMatrix)(void);
	typedef void (CL_GL1FUNC *ptr_glPushName)(CLuint name);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2d)(CLdouble x, CLdouble y);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2f)(CLfloat x, CLfloat y);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2i)(CLint x, CLint y);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2s)(CLshort x, CLshort y);
	typedef void (CL_GL1FUNC *ptr_glRasterPos2sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3d)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3f)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3i)(CLint x, CLint y, CLint z);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3s)(CLshort x, CLshort y, CLshort z);
	typedef void (CL_GL1FUNC *ptr_glRasterPos3sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4d)(CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4f)(CLfloat x, CLfloat y, CLfloat z, CLfloat w);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4i)(CLint x, CLint y, CLint z, CLint w);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4s)(CLshort x, CLshort y, CLshort z, CLshort w);
	typedef void (CL_GL1FUNC *ptr_glRasterPos4sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glReadBuffer)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glReadPixels)(CLint x, CLint y, CLsizei width, CLsizei height, CLenum format, CLenum type, CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glRectd)(CLdouble x1, CLdouble y1, CLdouble x2, CLdouble y2);
	typedef void (CL_GL1FUNC *ptr_glRectdv)(const CLdouble *v1, const CLdouble *v2);
	typedef void (CL_GL1FUNC *ptr_glRectf)(CLfloat x1, CLfloat y1, CLfloat x2, CLfloat y2);
	typedef void (CL_GL1FUNC *ptr_glRectfv)(const CLfloat *v1, const CLfloat *v2);
	typedef void (CL_GL1FUNC *ptr_glRecti)(CLint x1, CLint y1, CLint x2, CLint y2);
	typedef void (CL_GL1FUNC *ptr_glRectiv)(const CLint *v1, const CLint *v2);
	typedef void (CL_GL1FUNC *ptr_glRects)(CLshort x1, CLshort y1, CLshort x2, CLshort y2);
	typedef void (CL_GL1FUNC *ptr_glRectsv)(const CLshort *v1, const CLshort *v2);
	typedef CLint (CL_GL1FUNC *ptr_glRenderMode)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glRotated)(CLdouble angle, CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GL1FUNC *ptr_glRotatef)(CLfloat angle, CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GL1FUNC *ptr_glScaled)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GL1FUNC *ptr_glScalef)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GL1FUNC *ptr_glScissor)(CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GL1FUNC *ptr_glSelectBuffer)(CLsizei size, CLuint *buffer);
	typedef void (CL_GL1FUNC *ptr_glShadeModel)(CLenum mode);
	typedef void (CL_GL1FUNC *ptr_glStencilFunc)(CLenum func, CLint ref, CLuint mask);
	typedef void (CL_GL1FUNC *ptr_glStencilMask)(CLuint mask);
	typedef void (CL_GL1FUNC *ptr_glStencilMaskSeparate)(CLenum face, CLuint mask);
	typedef void (CL_GL1FUNC *ptr_glStencilOp)(CLenum fail, CLenum zfail, CLenum zpass);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1d)(CLdouble s);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1f)(CLfloat s);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1i)(CLint s);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1s)(CLshort s);
	typedef void (CL_GL1FUNC *ptr_glTexCoord1sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2d)(CLdouble s, CLdouble t);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2f)(CLfloat s, CLfloat t);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2i)(CLint s, CLint t);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2s)(CLshort s, CLshort t);
	typedef void (CL_GL1FUNC *ptr_glTexCoord2sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3d)(CLdouble s, CLdouble t, CLdouble r);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3f)(CLfloat s, CLfloat t, CLfloat r);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3i)(CLint s, CLint t, CLint r);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3s)(CLshort s, CLshort t, CLshort r);
	typedef void (CL_GL1FUNC *ptr_glTexCoord3sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4d)(CLdouble s, CLdouble t, CLdouble r, CLdouble q);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4f)(CLfloat s, CLfloat t, CLfloat r, CLfloat q);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4i)(CLint s, CLint t, CLint r, CLint q);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4s)(CLshort s, CLshort t, CLshort r, CLshort q);
	typedef void (CL_GL1FUNC *ptr_glTexCoord4sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glTexCoordPointer)(CLint size, CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GL1FUNC *ptr_glTexEnvf)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glTexEnvfv)(CLenum target, CLenum pname, const CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glTexEnvi)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glTexEnviv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GL1FUNC *ptr_glTexGend)(CLenum coord, CLenum pname, CLdouble param);
	typedef void (CL_GL1FUNC *ptr_glTexGendv)(CLenum coord, CLenum pname, const CLdouble *params);
	typedef void (CL_GL1FUNC *ptr_glTexGenf)(CLenum coord, CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glTexGenfv)(CLenum coord, CLenum pname, const CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glTexGeni)(CLenum coord, CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glTexGeniv)(CLenum coord, CLenum pname, const CLint *params);
	typedef void (CL_GL1FUNC *ptr_glTexImage1D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLint border, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glTexImage2D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLsizei height, CLint border, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glTexParameterf)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glTexParameterfv)(CLenum target, CLenum pname, const CLfloat *params);
	typedef void (CL_GL1FUNC *ptr_glTexParameteri)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glTexParameteriv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GL1FUNC *ptr_glTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLsizei width, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glTranslated)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GL1FUNC *ptr_glTranslatef)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GL1FUNC *ptr_glVertex2d)(CLdouble x, CLdouble y);
	typedef void (CL_GL1FUNC *ptr_glVertex2dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glVertex2f)(CLfloat x, CLfloat y);
	typedef void (CL_GL1FUNC *ptr_glVertex2fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glVertex2i)(CLint x, CLint y);
	typedef void (CL_GL1FUNC *ptr_glVertex2iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glVertex2s)(CLshort x, CLshort y);
	typedef void (CL_GL1FUNC *ptr_glVertex2sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glVertex3d)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GL1FUNC *ptr_glVertex3dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glVertex3f)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GL1FUNC *ptr_glVertex3fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glVertex3i)(CLint x, CLint y, CLint z);
	typedef void (CL_GL1FUNC *ptr_glVertex3iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glVertex3s)(CLshort x, CLshort y, CLshort z);
	typedef void (CL_GL1FUNC *ptr_glVertex3sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glVertex4d)(CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GL1FUNC *ptr_glVertex4dv)(const CLdouble *v);
	typedef void (CL_GL1FUNC *ptr_glVertex4f)(CLfloat x, CLfloat y, CLfloat z, CLfloat w);
	typedef void (CL_GL1FUNC *ptr_glVertex4fv)(const CLfloat *v);
	typedef void (CL_GL1FUNC *ptr_glVertex4i)(CLint x, CLint y, CLint z, CLint w);
	typedef void (CL_GL1FUNC *ptr_glVertex4iv)(const CLint *v);
	typedef void (CL_GL1FUNC *ptr_glVertex4s)(CLshort x, CLshort y, CLshort z, CLshort w);
	typedef void (CL_GL1FUNC *ptr_glVertex4sv)(const CLshort *v);
	typedef void (CL_GL1FUNC *ptr_glVertexPointer)(CLint size, CLenum type, CLsizei stride, const CLvoid *pointer);
	typedef void (CL_GL1FUNC *ptr_glViewport)(CLint x, CLint y, CLsizei width, CLsizei height);

	typedef void (CL_GL1FUNC *ptr_glDrawRangeElementsEXT)(CLenum mode, CLuint start, CLuint end, CLsizei count, CLenum type, const CLvoid *indices);
	typedef void (CL_GL1FUNC *ptr_glTexImage3DEXT)(CLenum target, CLint level, CLenum internalformat, CLsizei width, CLsizei height, CLsizei depth, CLint border, CLenum format, CLenum type, const CLvoid* pixels);
	typedef void (CL_GL1FUNC *ptr_glTexSubImage3DEXT)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLsizei width, CLsizei height, CLsizei depth, CLenum format, CLenum type, const CLvoid *pixels);
	typedef void (CL_GL1FUNC *ptr_glCopyTexSubImage3DEXT)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GL1FUNC *ptr_glColorTableSGI)(CLenum target, CLenum internalformat, CLsizei width, CLenum format, CLenum type, const CLvoid* table);
	typedef void (CL_GL1FUNC *ptr_glCopyColorTableSGI)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width);
	typedef void (CL_GL1FUNC *ptr_glColorTableParameterivSGI)(CLenum target, CLenum pname, const CLint* params);
	typedef void (CL_GL1FUNC *ptr_glColorTableParameterfvSGI)(CLenum target, CLenum pname, const CLfloat* params);
	typedef void (CL_GL1FUNC *ptr_glGetColorTableSGI)(CLenum target, CLenum format, CLenum type, CLvoid* table);
	typedef void (CL_GL1FUNC *ptr_glGetColorTableParameterivSGI)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GL1FUNC *ptr_glGetColorTableParameterfvSGI)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GL1FUNC *ptr_glColorSubTableEXT)(CLenum target, CLsizei start, CLsizei count, CLenum format, CLenum type, const CLvoid *data);
	typedef void (CL_GL1FUNC *ptr_glCopyColorSubTableEXT)(CLenum target, CLsizei start, CLint x, CLint y, CLsizei width);
	typedef void (CL_GL1FUNC *ptr_glConvolutionFilter1DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLenum format, CLenum type, const CLvoid* image);
	typedef void (CL_GL1FUNC *ptr_glConvolutionFilter2DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid* image);
	typedef void (CL_GL1FUNC *ptr_glCopyConvolutionFilter1DEXT)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width);
	typedef void (CL_GL1FUNC *ptr_glCopyConvolutionFilter2DEXT)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GL1FUNC *ptr_glGetConvolutionFilterEXT)(CLenum target, CLenum format, CLenum type, CLvoid* image);
	typedef void (CL_GL1FUNC *ptr_glSeparableFilter2DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height, CLenum format, CLenum type, const CLvoid* row, const CLvoid* column);
	typedef void (CL_GL1FUNC *ptr_glGetSeparableFilterEXT)(CLenum target, CLenum format, CLenum type, CLvoid* row, CLvoid* column, CLvoid* span);
	typedef void (CL_GL1FUNC *ptr_glConvolutionParameteriEXT)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GL1FUNC *ptr_glConvolutionParameterivEXT)(CLenum target, CLenum pname, const CLint* params);
	typedef void (CL_GL1FUNC *ptr_glConvolutionParameterfEXT)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GL1FUNC *ptr_glConvolutionParameterfvEXT)(CLenum target, CLenum pname, const CLfloat* params);
	typedef void (CL_GL1FUNC *ptr_glGetConvolutionParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GL1FUNC *ptr_glGetConvolutionParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GL1FUNC *ptr_glHistogramEXT)(CLenum target, CLsizei width, CLenum internalformat, CLboolean sink);
	typedef void (CL_GL1FUNC *ptr_glResetHistogramEXT)(CLenum target);
	typedef void (CL_GL1FUNC *ptr_glGetHistogramEXT)(CLenum target, CLboolean reset, CLenum format, CLenum type, CLvoid* values);
	typedef void (CL_GL1FUNC *ptr_glGetHistogramParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GL1FUNC *ptr_glGetHistogramParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GL1FUNC *ptr_glMinmaxEXT)(CLenum target, CLenum internalformat, CLboolean sink);
	typedef void (CL_GL1FUNC *ptr_glResetMinmaxEXT)(CLenum target);
	typedef void (CL_GL1FUNC *ptr_glGetMinmaxEXT)(CLenum target, CLboolean reset, CLenum format, CLenum type, CLvoid* values);
	typedef void (CL_GL1FUNC *ptr_glGetMinmaxParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GL1FUNC *ptr_glGetMinmaxParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GL1FUNC *ptr_glBlendColorEXT)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GL1FUNC *ptr_glBlendEquationEXT)(CLenum mode);
	typedef CLvoid (CL_GL1FUNC *ptr_glActiveTextureARB)(CLenum);
	typedef CLvoid (CL_GL1FUNC *ptr_glClientActiveTextureARB)(CLenum);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1dARB)(CLenum, CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1fARB)(CLenum, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1iARB)(CLenum, CLint);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1sARB)(CLenum, CLshort);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord1svARB)(CLenum, CLshort const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2dARB)(CLenum, CLdouble, CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2fARB)(CLenum, CLfloat, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2iARB)(CLenum, CLint, CLint);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2sARB)(CLenum, CLshort, CLshort);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord2svARB)(CLenum, CLshort const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3dARB)(CLenum, CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3fARB)(CLenum, CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3iARB)(CLenum, CLint, CLint, CLint);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3sARB)(CLenum, CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord3svARB)(CLenum, CLshort const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4dARB)(CLenum, CLdouble, CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4dvARB)(CLenum, CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4fARB)(CLenum, CLfloat, CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4fvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4iARB)(CLenum, CLint, CLint, CLint, CLint);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4ivARB)(CLenum, CLint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4sARB)(CLenum, CLshort, CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiTexCoord4svARB)(CLenum, CLshort const *);

	typedef CLvoid (CL_GL1FUNC *ptr_glCompressedTexImage1DARB)(CLenum, CLint, CLenum, CLsizei, CLint, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glCompressedTexImage2DARB)(CLenum, CLint, CLenum, CLsizei, CLsizei, CLint, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glCompressedTexImage3DARB)(CLenum, CLint, CLenum, CLsizei, CLsizei, CLsizei, CLint, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glCompressedTexSubImage1DARB)(CLenum, CLint, CLint, CLsizei, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glCompressedTexSubImage2DARB)(CLenum, CLint, CLint, CLint, CLsizei, CLsizei, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glCompressedTexSubImage3DARB)(CLenum, CLint, CLint, CLint, CLint, CLsizei, CLsizei, CLsizei, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetCompressedTexImageARB)(CLenum, CLint, CLvoid *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSampleCoverageARB)(CLclampf, CLboolean);
	typedef CLvoid (CL_GL1FUNC *ptr_glLoadTransposeMatrixdARB)(CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glLoadTransposeMatrixfARB)(CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultTransposeMatrixdARB)(CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultTransposeMatrixfARB)(CLfloat const *);

	typedef CLvoid (CL_GL1FUNC *ptr_glFogCoorddEXT)(CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glFogCoorddvEXT)(CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glFogCoordfEXT)(CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glFogCoordfvEXT)(CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glFogCoordPointerEXT)(CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiDrawArraysEXT)(CLenum, CLint *, CLsizei *, CLsizei);
	typedef CLvoid (CL_GL1FUNC *ptr_glMultiDrawElementsEXT)(CLenum, CLsizei const *, CLenum, CLvoid const * *, CLsizei);
	typedef CLvoid (CL_GL1FUNC *ptr_glPointParameterfARB)(CLenum, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glPointParameterfvARB)(CLenum, CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3bEXT)(CLbyte, CLbyte, CLbyte);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3bvEXT)(CLbyte const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3dEXT)(CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3dvEXT)(CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3fEXT)(CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3fvEXT)(CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3iEXT)(CLint, CLint, CLint);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3ivEXT)(CLint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3sEXT)(CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3svEXT)(CLshort const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3ubEXT)(CLubyte, CLubyte, CLubyte);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3ubvEXT)(CLubyte const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3uiEXT)(CLuint, CLuint, CLuint);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3uivEXT)(CLuint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3usEXT)(CLushort, CLushort, CLushort);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColor3usvEXT)(CLushort const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glSecondaryColorPointerEXT)(CLint, CLenum, CLsizei, CLvoid const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glBlendFuncSeparateEXT)(CLenum, CLenum, CLenum, CLenum);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2dARB)(CLdouble, CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2dvARB)(CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2fARB)(CLfloat, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2fvARB)(CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2iARB)(CLint, CLint);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2ivARB)(CLint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2sARB)(CLshort, CLshort);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos2svARB)(CLshort const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3dARB)(CLdouble, CLdouble, CLdouble);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3dvARB)(CLdouble const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3fARB)(CLfloat, CLfloat, CLfloat);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3fvARB)(CLfloat const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3iARB)(CLint, CLint, CLint);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3ivARB)(CLint const *);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3sARB)(CLshort, CLshort, CLshort);
	typedef CLvoid (CL_GL1FUNC *ptr_glWindowPos3svARB)(CLshort const *);

	typedef CLvoid (CL_GL1FUNC *ptr_glBindBufferARB)(CLenum target, CLuint buffer);
	typedef CLvoid (CL_GL1FUNC *ptr_glDeleteBuffersARB)(CLsizei n, const CLuint *buffers);
	typedef CLvoid (CL_GL1FUNC *ptr_glGenBuffersARB)(CLsizei n, CLuint *buffers);
	typedef CLboolean (CL_GL1FUNC *ptr_glIsBufferARB)(CLuint buffer);
	typedef CLvoid (CL_GL1FUNC *ptr_glBufferDataARB)(CLenum target, CLsizeiptr size, const CLvoid *data, CLenum usage);
	typedef CLvoid (CL_GL1FUNC *ptr_glBufferSubDataARB)(CLenum target, CLintptr offset, CLsizeiptr size, const CLvoid *data);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetBufferSubDataARB)(CLenum target, CLintptr offset, CLsizeiptr size, CLvoid *data);
	typedef CLvoid *(CL_GL1FUNC *ptr_glMapBufferARB)(CLenum target, CLenum access);
	typedef CLboolean (CL_GL1FUNC *ptr_glUnmapBufferARB)(CLenum target);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetBufferParameterivARB)(CLenum target, CLenum pname, CLint *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetBufferPointervARB)(CLenum target, CLenum pname, CLvoid **params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGenQueriesARB)(CLsizei n, CLuint *ids);
	typedef CLvoid (CL_GL1FUNC *ptr_glDeleteQueriesARB)(CLsizei n, const CLuint *ids);
	typedef CLboolean (CL_GL1FUNC *ptr_glIsQueryARB)(CLuint id);
	typedef CLvoid (CL_GL1FUNC *ptr_glBeginQueryARB)(CLenum target, CLuint id);
	typedef CLvoid (CL_GL1FUNC *ptr_glEndQueryARB)(CLenum target);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetQueryivARB)(CLenum target, CLenum pname, CLint *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetQueryObjectivARB)(CLuint id, CLenum pname, CLint *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetQueryObjectuivARB)(CLuint id, CLenum pname, CLuint *params);

	typedef CLvoid (CL_GL1FUNC *ptr_glDeleteObjectARB)(CLuint obj);
	typedef CLuint (CL_GL1FUNC *ptr_glGetHandleARB)(CLenum pname);
	typedef CLvoid (CL_GL1FUNC *ptr_glDetachObjectARB)(CLuint containerObj, CLuint attachedObj);
	typedef CLuint (CL_GL1FUNC *ptr_glCreateShaderObjectARB )(CLenum shaderType);
	typedef CLvoid (CL_GL1FUNC *ptr_glShaderSourceARB)(CLuint shaderObj, CLsizei count, const CLchar* *string, const CLint *length);
	typedef CLvoid (CL_GL1FUNC *ptr_glCompileShaderARB)(CLuint shaderObj);
	typedef CLuint (CL_GL1FUNC *ptr_glCreateProgramObjectARB)();
	typedef CLvoid (CL_GL1FUNC *ptr_glAttachObjectARB)(CLuint containerObj, CLuint attachedObj);
	typedef CLvoid (CL_GL1FUNC *ptr_glLinkProgramARB)(CLuint programObj);
	typedef CLvoid (CL_GL1FUNC *ptr_glUseProgramObjectARB)(CLuint programObj);
	typedef CLvoid (CL_GL1FUNC *ptr_glValidateProgramARB)(CLuint programObj);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform1fARB)(CLint location, CLfloat v0);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform2fARB)(CLint location, CLfloat v0, CLfloat v1);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform3fARB)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform4fARB)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2, CLfloat v3);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform1iARB)(CLint location, CLint v0);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform2iARB)(CLint location, CLint v0, CLint v1);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform3iARB)(CLint location, CLint v0, CLint v1, CLint v2);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform4iARB)(CLint location, CLint v0, CLint v1, CLint v2, CLint v3);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform1fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform2fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform3fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform4fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform1ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform2ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform3ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniform4ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniformMatrix2fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniformMatrix3fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glUniformMatrix4fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetObjectParameterfvARB)(CLuint obj, CLenum pname, CLfloat *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetObjectParameterivARB)(CLuint obj, CLenum pname, CLint *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetInfoLogARB)(CLuint obj, CLsizei maxLength, CLsizei *length, CLchar *infoLog);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetAttachedObjectsARB)(CLuint containerObj, CLsizei maxCount, CLsizei *count, CLuint *obj);
	typedef CLint (CL_GL1FUNC *ptr_glGetUniformLocationARB)(CLuint programObj, const CLchar *name);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetActiveUniformARB)(CLuint programObj, CLuint index, CLsizei maxLength, CLsizei *length, CLsizei *size, CLenum *type, CLchar *name);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetUniformfvARB)(CLuint programObj, CLint location, CLfloat *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetUniformivARB)(CLuint programObj, CLint location, CLint *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetShaderSourceARB)(CLuint obj, CLsizei maxLength, CLsizei *length, CLchar *source);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib1fARB)(CLuint index, CLfloat v0);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib1sARB)(CLuint index, CLshort v0);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib1dARB)(CLuint index, CLdouble v0);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib2fARB)(CLuint index, CLfloat v0, CLfloat v1);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib2sARB)(CLuint index, CLshort v0, CLshort v1);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib2dARB)(CLuint index, CLdouble v0, CLdouble v1);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib3fARB)(CLuint index, CLfloat v0, CLfloat v1, CLfloat v2);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib3sARB)(CLuint index, CLshort v0, CLshort v1, CLshort v2);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib3dARB)(CLuint index, CLdouble v0, CLdouble v1, CLdouble v2);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4fARB)(CLuint index, CLfloat v0, CLfloat v1, CLfloat v2, CLfloat v3);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4sARB)(CLuint index, CLshort v0, CLshort v1, CLshort v2, CLshort v3);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4dARB)(CLuint index, CLdouble v0, CLdouble v1, CLdouble v2, CLdouble v3);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4NubARB)(CLuint index, CLubyte x, CLubyte y, CLubyte z, CLubyte w);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib1fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib1svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib1dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib2fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib2svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib2dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib3fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib3svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib3dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4fvARB)(CLuint index, const CLfloat *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4svARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4dvARB)(CLuint index, const CLdouble *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4ivARB)(CLuint index, const CLint *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4bvARB)(CLuint index, const CLbyte *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4ubvARB)(CLuint index, const CLubyte *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4usvARB)(CLuint index, const CLushort *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4uivARB)(CLuint index, const CLuint *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4NbvARB)(CLuint index, const CLbyte *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4NsvARB)(CLuint index, const CLshort *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4NivARB)(CLuint index, const CLint *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4NubvARB)(CLuint index, const CLubyte *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4NusvARB)(CLuint index, const CLushort *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttrib4NuivARB)(CLuint index, const CLuint *v);
	typedef CLvoid (CL_GL1FUNC *ptr_glVertexAttribPointerARB)(CLuint index, CLint size, CLenum type, CLboolean normalized, CLsizei stride, const CLvoid *pointer);
	typedef CLvoid (CL_GL1FUNC *ptr_glEnableVertexAttribArrayARB)(CLuint index);
	typedef CLvoid (CL_GL1FUNC *ptr_glDisableVertexAttribArrayARB)(CLuint index);
	typedef CLvoid (CL_GL1FUNC *ptr_glBindAttribLocationARB)(CLuint programObj, CLuint index, const char *name);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetActiveAttribARB)(CLuint programObj, CLuint index, CLsizei maxLength, CLsizei *length, CLint *size, CLenum *type, char *name);
	typedef CLint (CL_GL1FUNC *ptr_glGetAttribLocationARB)(CLuint programObj, const char *name);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetVertexAttribdvARB)(CLuint index, CLenum pname, CLdouble *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetVertexAttribfvARB)(CLuint index, CLenum pname, CLfloat *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetVertexAttribivARB)(CLuint index, CLenum pname, CLint *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetVertexAttribPointervARB)(CLuint index, CLenum pname, CLvoid **pointer);
	typedef CLvoid (CL_GL1FUNC *ptr_glDrawBuffersARB)(CLsizei n, const CLenum *bufs);
	typedef CLvoid (CL_GL1FUNC *ptr_glStencilFuncSeparateATI)(CLenum frontfunc, CLenum backfunc, CLint ref, CLuint mask);
	typedef CLvoid (CL_GL1FUNC *ptr_glStencilOpSeparateATI)(CLenum face, CLenum sfail, CLenum dpfail, CLenum dppass);

	typedef CLboolean (CL_GL1FUNC *ptr_glIsRenderbufferEXT)(CLuint renderbuffer);
	typedef CLvoid (CL_GL1FUNC *ptr_glBindRenderbufferEXT)(CLenum target, CLuint renderbuffer);
	typedef CLvoid (CL_GL1FUNC *ptr_glDeleteRenderbuffersEXT)(CLsizei n, const CLuint *renderbuffers);
	typedef CLvoid (CL_GL1FUNC *ptr_glGenRenderbuffersEXT)(CLsizei n, CLuint *renderbuffers);
	typedef CLvoid (CL_GL1FUNC *ptr_glRenderbufferStorageEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetRenderbufferParameterivEXT)(CLenum target, CLenum pname, CLint *params);
	typedef CLboolean (CL_GL1FUNC *ptr_glIsFramebufferEXT)(CLuint framebuffer);
	typedef CLvoid (CL_GL1FUNC *ptr_glBindFramebufferEXT)(CLenum target, CLuint framebuffer);
	typedef CLvoid (CL_GL1FUNC *ptr_glDeleteFramebuffersEXT)(CLsizei n, const CLuint *framebuffers);
	typedef CLvoid (CL_GL1FUNC *ptr_glGenFramebuffersEXT)(CLsizei n, CLuint *framebuffers);
	typedef CLenum (CL_GL1FUNC *ptr_glCheckFramebufferStatusEXT)(CLenum target);
	typedef CLvoid (CL_GL1FUNC *ptr_glFramebufferTexture1DEXT)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level);
	typedef CLvoid (CL_GL1FUNC *ptr_glFramebufferTexture2DEXT)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level);
	typedef CLvoid (CL_GL1FUNC *ptr_glFramebufferTexture3DEXT)(CLenum target, CLenum attachment, CLenum textarget, CLuint texture, CLint level, CLint zoffset);
	typedef CLvoid (CL_GL1FUNC *ptr_glFramebufferRenderbufferEXT)(CLenum target, CLenum attachment, CLenum renderbuffertarget, CLuint renderbuffer);
	typedef CLvoid (CL_GL1FUNC *ptr_glGetFramebufferAttachmentParameterivEXT)(CLenum target, CLenum attachment, CLenum pname, CLint *params);
	typedef CLvoid (CL_GL1FUNC *ptr_glGenerateMipmapEXT)(CLenum target);

	typedef CLvoid (CL_GL1FUNC *ptr_glBlitFramebufferEXT)(CLint srcX0, CLint srcY0, CLint srcX1, CLint srcY1, CLint dstX0, CLint dstY0, CLint dstX1, CLint dstY1, CLbitfield mask, CLenum filter);

#ifdef WIN32
    DECLARE_HANDLE(HPBUFFERARB);
	typedef HPBUFFERARB (CL_GL1FUNC *ptr_wglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
	typedef HDC (CL_GL1FUNC *ptr_wglGetPbufferDCARB)(HPBUFFERARB hPbuffer);
	typedef int (CL_GL1FUNC *ptr_wglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
	typedef BOOL (CL_GL1FUNC *ptr_wglDestroyPbufferARB)(HPBUFFERARB hPbuffer);
	typedef BOOL (CL_GL1FUNC *ptr_wglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
#endif

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

#ifdef WIN32
	ptr_wglCreatePbufferARB wglCreatePbufferARB;
	ptr_wglGetPbufferDCARB wglGetPbufferDCARB;
	ptr_wglReleasePbufferDCARB wglReleasePbufferDCARB;
	ptr_wglDestroyPbufferARB wglDestroyPbufferARB;
	ptr_wglQueryPbufferARB wglQueryPbufferARB;
#endif
/// \}
};

#define cl1Accum CL_GL1::functions->accum
#define cl1AlphaFunc CL_GL1::functions->alphaFunc
#define cl1AreTexturesResident CL_GL1::functions->areTexturesResident
#define cl1ArrayElement CL_GL1::functions->arrayElement
#define cl1Begin CL_GL1::functions->begin
#define cl1BindTexture CL_GL1::functions->bindTexture
#define cl1Bitmap CL_GL1::functions->bitmap
#define cl1BlendFunc CL_GL1::functions->blendFunc
#define cl1CallList CL_GL1::functions->callList
#define cl1CallLists CL_GL1::functions->callLists
#define cl1Clear CL_GL1::functions->clear
#define cl1ClearAccum CL_GL1::functions->clearAccum
#define cl1ClearColor CL_GL1::functions->clearColor
#define cl1ClearDepth CL_GL1::functions->clearDepth
#define cl1ClearIndex CL_GL1::functions->clearIndex
#define cl1ClearStencil CL_GL1::functions->clearStencil
#define cl1ClipPlane CL_GL1::functions->clipPlane
#define cl1Color3b CL_GL1::functions->color3b
#define cl1Color3bv CL_GL1::functions->color3bv
#define cl1Color3d CL_GL1::functions->color3d
#define cl1Color3dv CL_GL1::functions->color3dv
#define cl1Color3f CL_GL1::functions->color3f
#define cl1Color3fv CL_GL1::functions->color3fv
#define cl1Color3i CL_GL1::functions->color3i
#define cl1Color3iv CL_GL1::functions->color3iv
#define cl1Color3s CL_GL1::functions->color3s
#define cl1Color3sv CL_GL1::functions->color3sv
#define cl1Color3ub CL_GL1::functions->color3ub
#define cl1Color3ubv CL_GL1::functions->color3ubv
#define cl1Color3ui CL_GL1::functions->color3ui
#define cl1Color3uiv CL_GL1::functions->color3uiv
#define cl1Color3us CL_GL1::functions->color3us
#define cl1Color3usv CL_GL1::functions->color3usv
#define cl1Color4b CL_GL1::functions->color4b
#define cl1Color4bv CL_GL1::functions->color4bv
#define cl1Color4d CL_GL1::functions->color4d
#define cl1Color4dv CL_GL1::functions->color4dv
#define cl1Color4f CL_GL1::functions->color4f
#define cl1Color4fv CL_GL1::functions->color4fv
#define cl1Color4i CL_GL1::functions->color4i
#define cl1Color4iv CL_GL1::functions->color4iv
#define cl1Color4s CL_GL1::functions->color4s
#define cl1Color4sv CL_GL1::functions->color4sv
#define cl1Color4ub CL_GL1::functions->color4ub
#define cl1Color4ubv CL_GL1::functions->color4ubv
#define cl1Color4ui CL_GL1::functions->color4ui
#define cl1Color4uiv CL_GL1::functions->color4uiv
#define cl1Color4us CL_GL1::functions->color4us
#define cl1Color4usv CL_GL1::functions->color4usv
#define cl1ColorMask CL_GL1::functions->colorMask
#define cl1ColorMaterial CL_GL1::functions->colorMaterial
#define cl1ColorPointer CL_GL1::functions->colorPointer
#define cl1CopyPixels CL_GL1::functions->copyPixels
#define cl1CopyTexImage1D CL_GL1::functions->copyTexImage1D
#define cl1CopyTexImage2D CL_GL1::functions->copyTexImage2D
#define cl1CopyTexSubImage1D CL_GL1::functions->copyTexSubImage1D
#define cl1CopyTexSubImage2D CL_GL1::functions->copyTexSubImage2D
#define cl1CopyTexSubImage3D CL_GL1::functions->copyTexSubImage3D
#define cl1CullFace CL_GL1::functions->cullFace
#define cl1DeleteLists CL_GL1::functions->deleteLists
#define cl1DeleteTextures CL_GL1::functions->deleteTextures
#define cl1DepthFunc CL_GL1::functions->depthFunc
#define cl1DepthMask CL_GL1::functions->depthMask
#define cl1DepthRange CL_GL1::functions->depthRange
#define cl1Disable CL_GL1::functions->disable
#define cl1DisableClientState CL_GL1::functions->disableClientState
#define cl1DrawArrays CL_GL1::functions->drawArrays
#define cl1DrawBuffer CL_GL1::functions->drawBuffer
#define cl1DrawElements CL_GL1::functions->drawElements
#define cl1DrawPixels CL_GL1::functions->drawPixels
#define cl1EdgeFlag CL_GL1::functions->edgeFlag
#define cl1EdgeFlagPointer CL_GL1::functions->edgeFlagPointer
#define cl1EdgeFlagv CL_GL1::functions->edgeFlagv
#define cl1Enable CL_GL1::functions->enable
#define cl1EnableClientState CL_GL1::functions->enableClientState
#define cl1End CL_GL1::functions->end
#define cl1EndList CL_GL1::functions->endList
#define cl1EvalCoord1d CL_GL1::functions->evalCoord1d
#define cl1EvalCoord1dv CL_GL1::functions->evalCoord1dv
#define cl1EvalCoord1f CL_GL1::functions->evalCoord1f
#define cl1EvalCoord1fv CL_GL1::functions->evalCoord1fv
#define cl1EvalCoord2d CL_GL1::functions->evalCoord2d
#define cl1EvalCoord2dv CL_GL1::functions->evalCoord2dv
#define cl1EvalCoord2f CL_GL1::functions->evalCoord2f
#define cl1EvalCoord2fv CL_GL1::functions->evalCoord2fv
#define cl1EvalMesh1 CL_GL1::functions->evalMesh1
#define cl1EvalMesh2 CL_GL1::functions->evalMesh2
#define cl1EvalPoint1 CL_GL1::functions->evalPoint1
#define cl1EvalPoint2 CL_GL1::functions->evalPoint2
#define cl1FeedbackBuffer CL_GL1::functions->feedbackBuffer
#define cl1Finish CL_GL1::functions->finish
#define cl1Flush CL_GL1::functions->flush
#define cl1Fogf CL_GL1::functions->fogf
#define cl1Fogfv CL_GL1::functions->fogfv
#define cl1Fogi CL_GL1::functions->fogi
#define cl1Fogiv CL_GL1::functions->fogiv
#define cl1FrontFace CL_GL1::functions->frontFace
#define cl1Frustum CL_GL1::functions->frustum
#define cl1GenLists CL_GL1::functions->genLists
#define cl1GenTextures CL_GL1::functions->genTextures
#define cl1GetBooleanv CL_GL1::functions->getBooleanv
#define cl1GetClipPlane CL_GL1::functions->getClipPlane
#define cl1GetDoublev CL_GL1::functions->getDoublev
#define cl1GetError CL_GL1::functions->getError
#define cl1GetFloatv CL_GL1::functions->getFloatv
#define cl1GetIntegerv CL_GL1::functions->getIntegerv
#define cl1GetLightfv CL_GL1::functions->getLightfv
#define cl1GetLightiv CL_GL1::functions->getLightiv
#define cl1GetMapdv CL_GL1::functions->getMapdv
#define cl1GetMapfv CL_GL1::functions->getMapfv
#define cl1GetMapiv CL_GL1::functions->getMapiv
#define cl1GetMaterialfv CL_GL1::functions->getMaterialfv
#define cl1GetMaterialiv CL_GL1::functions->getMaterialiv
#define cl1GetPixelMapfv CL_GL1::functions->getPixelMapfv
#define cl1GetPixelMapuiv CL_GL1::functions->getPixelMapuiv
#define cl1GetPixelMapusv CL_GL1::functions->getPixelMapusv
#define cl1GetPointerv CL_GL1::functions->getPointerv
#define cl1GetPolygonStipple CL_GL1::functions->getPolygonStipple
#define cl1GetString CL_GL1::functions->getString
#define cl1GetTexEnvfv CL_GL1::functions->getTexEnvfv
#define cl1GetTexEnviv CL_GL1::functions->getTexEnviv
#define cl1GetTexGendv CL_GL1::functions->getTexGendv
#define cl1GetTexGenfv CL_GL1::functions->getTexGenfv
#define cl1GetTexGeniv CL_GL1::functions->getTexGeniv
#define cl1GetTexImage CL_GL1::functions->getTexImage
#define cl1GetTexLevelParameterfv CL_GL1::functions->getTexLevelParameterfv
#define cl1GetTexLevelParameteriv CL_GL1::functions->getTexLevelParameteriv
#define cl1GetTexParameterfv CL_GL1::functions->getTexParameterfv
#define cl1GetTexParameteriv CL_GL1::functions->getTexParameteriv
#define cl1Hint CL_GL1::functions->hint
#define cl1IndexMask CL_GL1::functions->indexMask
#define cl1IndexPointer CL_GL1::functions->indexPointer
#define cl1Indexd CL_GL1::functions->indexd
#define cl1Indexdv CL_GL1::functions->indexdv
#define cl1Indexf CL_GL1::functions->indexf
#define cl1Indexfv CL_GL1::functions->indexfv
#define cl1Indexi CL_GL1::functions->indexi
#define cl1Indexiv CL_GL1::functions->indexiv
#define cl1Indexs CL_GL1::functions->indexs
#define cl1Indexsv CL_GL1::functions->indexsv
#define cl1Indexub CL_GL1::functions->indexub
#define cl1Indexubv CL_GL1::functions->indexubv
#define cl1InitNames CL_GL1::functions->initNames
#define cl1InterleavedArrays CL_GL1::functions->interleavedArrays
#define cl1IsEnabled CL_GL1::functions->isEnabled
#define cl1IsList CL_GL1::functions->isList
#define cl1IsTexture CL_GL1::functions->isTexture
#define cl1LightModelf CL_GL1::functions->lightModelf
#define cl1LightModelfv CL_GL1::functions->lightModelfv
#define cl1LightModeli CL_GL1::functions->lightModeli
#define cl1LightModeliv CL_GL1::functions->lightModeliv
#define cl1Lightf CL_GL1::functions->lightf
#define cl1Lightfv CL_GL1::functions->lightfv
#define cl1Lighti CL_GL1::functions->lighti
#define cl1Lightiv CL_GL1::functions->lightiv
#define cl1LineStipple CL_GL1::functions->lineStipple
#define cl1LineWidth CL_GL1::functions->lineWidth
#define cl1ListBase CL_GL1::functions->listBase
#define cl1LoadIdentity CL_GL1::functions->loadIdentity
#define cl1LoadMatrixd CL_GL1::functions->loadMatrixd
#define cl1LoadMatrixf CL_GL1::functions->loadMatrixf
#define cl1LoadName CL_GL1::functions->loadName
#define cl1LogicOp CL_GL1::functions->logicOp
#define cl1Map1d CL_GL1::functions->map1d
#define cl1Map1f CL_GL1::functions->map1f
#define cl1Map2d CL_GL1::functions->map2d
#define cl1Map2f CL_GL1::functions->map2f
#define cl1MapGrid1d CL_GL1::functions->mapGrid1d
#define cl1MapGrid1f CL_GL1::functions->mapGrid1f
#define cl1MapGrid2d CL_GL1::functions->mapGrid2d
#define cl1MapGrid2f CL_GL1::functions->mapGrid2f
#define cl1Materialf CL_GL1::functions->materialf
#define cl1Materialfv CL_GL1::functions->materialfv
#define cl1Materiali CL_GL1::functions->materiali
#define cl1Materialiv CL_GL1::functions->materialiv
#define cl1MatrixMode CL_GL1::functions->matrixMode
#define cl1MultMatrixd CL_GL1::functions->multMatrixd
#define cl1MultMatrixf CL_GL1::functions->multMatrixf
#define cl1NewList CL_GL1::functions->newList
#define cl1Normal3b CL_GL1::functions->normal3b
#define cl1Normal3bv CL_GL1::functions->normal3bv
#define cl1Normal3d CL_GL1::functions->normal3d
#define cl1Normal3dv CL_GL1::functions->normal3dv
#define cl1Normal3f CL_GL1::functions->normal3f
#define cl1Normal3fv CL_GL1::functions->normal3fv
#define cl1Normal3i CL_GL1::functions->normal3i
#define cl1Normal3iv CL_GL1::functions->normal3iv
#define cl1Normal3s CL_GL1::functions->normal3s
#define cl1Normal3sv CL_GL1::functions->normal3sv
#define cl1NormalPointer CL_GL1::functions->normalPointer
#define cl1Ortho CL_GL1::functions->ortho
#define cl1PassThrough CL_GL1::functions->passThrough
#define cl1PixelMapfv CL_GL1::functions->pixelMapfv
#define cl1PixelMapuiv CL_GL1::functions->pixelMapuiv
#define cl1PixelMapusv CL_GL1::functions->pixelMapusv
#define cl1PixelStoref CL_GL1::functions->pixelStoref
#define cl1PixelStorei CL_GL1::functions->pixelStorei
#define cl1PixelTransferf CL_GL1::functions->pixelTransferf
#define cl1PixelTransferi CL_GL1::functions->pixelTransferi
#define cl1PixelZoom CL_GL1::functions->pixelZoom
#define cl1PointSize CL_GL1::functions->pointSize
#define cl1PolygonMode CL_GL1::functions->polygonMode
#define cl1PolygonOffset CL_GL1::functions->polygonOffset
#define cl1PolygonStipple CL_GL1::functions->polygonStipple
#define cl1PopAttrib CL_GL1::functions->popAttrib
#define cl1PopClientAttrib CL_GL1::functions->popClientAttrib
#define cl1PopMatrix CL_GL1::functions->popMatrix
#define cl1PopName CL_GL1::functions->popName
#define cl1PrioritizeTextures CL_GL1::functions->prioritizeTextures
#define cl1PushAttrib CL_GL1::functions->pushAttrib
#define cl1PushClientAttrib CL_GL1::functions->pushClientAttrib
#define cl1PushMatrix CL_GL1::functions->pushMatrix
#define cl1PushName CL_GL1::functions->pushName
#define cl1RasterPos2d CL_GL1::functions->rasterPos2d
#define cl1RasterPos2dv CL_GL1::functions->rasterPos2dv
#define cl1RasterPos2f CL_GL1::functions->rasterPos2f
#define cl1RasterPos2fv CL_GL1::functions->rasterPos2fv
#define cl1RasterPos2i CL_GL1::functions->rasterPos2i
#define cl1RasterPos2iv CL_GL1::functions->rasterPos2iv
#define cl1RasterPos2s CL_GL1::functions->rasterPos2s
#define cl1RasterPos2sv CL_GL1::functions->rasterPos2sv
#define cl1RasterPos3d CL_GL1::functions->rasterPos3d
#define cl1RasterPos3dv CL_GL1::functions->rasterPos3dv
#define cl1RasterPos3f CL_GL1::functions->rasterPos3f
#define cl1RasterPos3fv CL_GL1::functions->rasterPos3fv
#define cl1RasterPos3i CL_GL1::functions->rasterPos3i
#define cl1RasterPos3iv CL_GL1::functions->rasterPos3iv
#define cl1RasterPos3s CL_GL1::functions->rasterPos3s
#define cl1RasterPos3sv CL_GL1::functions->rasterPos3sv
#define cl1RasterPos4d CL_GL1::functions->rasterPos4d
#define cl1RasterPos4dv CL_GL1::functions->rasterPos4dv
#define cl1RasterPos4f CL_GL1::functions->rasterPos4f
#define cl1RasterPos4fv CL_GL1::functions->rasterPos4fv
#define cl1RasterPos4i CL_GL1::functions->rasterPos4i
#define cl1RasterPos4iv CL_GL1::functions->rasterPos4iv
#define cl1RasterPos4s CL_GL1::functions->rasterPos4s
#define cl1RasterPos4sv CL_GL1::functions->rasterPos4sv
#define cl1ReadBuffer CL_GL1::functions->readBuffer
#define cl1ReadPixels CL_GL1::functions->readPixels
#define cl1Rectd CL_GL1::functions->rectd
#define cl1Rectdv CL_GL1::functions->rectdv
#define cl1Rectf CL_GL1::functions->rectf
#define cl1Rectfv CL_GL1::functions->rectfv
#define cl1Recti CL_GL1::functions->recti
#define cl1Rectiv CL_GL1::functions->rectiv
#define cl1Rects CL_GL1::functions->rects
#define cl1Rectsv CL_GL1::functions->rectsv
#define cl1RenderMode CL_GL1::functions->renderMode
#define cl1Rotated CL_GL1::functions->rotated
#define cl1Rotatef CL_GL1::functions->rotatef
#define cl1Scaled CL_GL1::functions->scaled
#define cl1Scalef CL_GL1::functions->scalef
#define cl1Scissor CL_GL1::functions->scissor
#define cl1SelectBuffer CL_GL1::functions->selectBuffer
#define cl1ShadeModel CL_GL1::functions->shadeModel
#define cl1StencilFunc CL_GL1::functions->stencilFunc
#define cl1StencilMask CL_GL1::functions->stencilMask
#define cl1StencilMaskSeparate CL_GL1::functions->stencilMaskSeparate
#define cl1StencilOp CL_GL1::functions->stencilOp
#define cl1TexCoord1d CL_GL1::functions->texCoord1d
#define cl1TexCoord1dv CL_GL1::functions->texCoord1dv
#define cl1TexCoord1f CL_GL1::functions->texCoord1f
#define cl1TexCoord1fv CL_GL1::functions->texCoord1fv
#define cl1TexCoord1i CL_GL1::functions->texCoord1i
#define cl1TexCoord1iv CL_GL1::functions->texCoord1iv
#define cl1TexCoord1s CL_GL1::functions->texCoord1s
#define cl1TexCoord1sv CL_GL1::functions->texCoord1sv
#define cl1TexCoord2d CL_GL1::functions->texCoord2d
#define cl1TexCoord2dv CL_GL1::functions->texCoord2dv
#define cl1TexCoord2f CL_GL1::functions->texCoord2f
#define cl1TexCoord2fv CL_GL1::functions->texCoord2fv
#define cl1TexCoord2i CL_GL1::functions->texCoord2i
#define cl1TexCoord2iv CL_GL1::functions->texCoord2iv
#define cl1TexCoord2s CL_GL1::functions->texCoord2s
#define cl1TexCoord2sv CL_GL1::functions->texCoord2sv
#define cl1TexCoord3d CL_GL1::functions->texCoord3d
#define cl1TexCoord3dv CL_GL1::functions->texCoord3dv
#define cl1TexCoord3f CL_GL1::functions->texCoord3f
#define cl1TexCoord3fv CL_GL1::functions->texCoord3fv
#define cl1TexCoord3i CL_GL1::functions->texCoord3i
#define cl1TexCoord3iv CL_GL1::functions->texCoord3iv
#define cl1TexCoord3s CL_GL1::functions->texCoord3s
#define cl1TexCoord3sv CL_GL1::functions->texCoord3sv
#define cl1TexCoord4d CL_GL1::functions->texCoord4d
#define cl1TexCoord4dv CL_GL1::functions->texCoord4dv
#define cl1TexCoord4f CL_GL1::functions->texCoord4f
#define cl1TexCoord4fv CL_GL1::functions->texCoord4fv
#define cl1TexCoord4i CL_GL1::functions->texCoord4i
#define cl1TexCoord4iv CL_GL1::functions->texCoord4iv
#define cl1TexCoord4s CL_GL1::functions->texCoord4s
#define cl1TexCoord4sv CL_GL1::functions->texCoord4sv
#define cl1TexCoordPointer CL_GL1::functions->texCoordPointer
#define cl1TexEnvf CL_GL1::functions->texEnvf
#define cl1TexEnvfv CL_GL1::functions->texEnvfv
#define cl1TexEnvi CL_GL1::functions->texEnvi
#define cl1TexEnviv CL_GL1::functions->texEnviv
#define cl1TexGend CL_GL1::functions->texGend
#define cl1TexGendv CL_GL1::functions->texGendv
#define cl1TexGenf CL_GL1::functions->texGenf
#define cl1TexGenfv CL_GL1::functions->texGenfv
#define cl1TexGeni CL_GL1::functions->texGeni
#define cl1TexGeniv CL_GL1::functions->texGeniv
#define cl1TexImage1D CL_GL1::functions->texImage1D
#define cl1TexImage2D CL_GL1::functions->texImage2D
#define cl1TexParameterf CL_GL1::functions->texParameterf
#define cl1TexParameterfv CL_GL1::functions->texParameterfv
#define cl1TexParameteri CL_GL1::functions->texParameteri
#define cl1TexParameteriv CL_GL1::functions->texParameteriv
#define cl1TexSubImage1D CL_GL1::functions->texSubImage1D
#define cl1TexSubImage2D CL_GL1::functions->texSubImage2D
#define cl1TexSubImage3D CL_GL1::functions->texSubImage3D
#define cl1Translated CL_GL1::functions->translated
#define cl1Translatef CL_GL1::functions->translatef
#define cl1Vertex2d CL_GL1::functions->vertex2d
#define cl1Vertex2dv CL_GL1::functions->vertex2dv
#define cl1Vertex2f CL_GL1::functions->vertex2f
#define cl1Vertex2fv CL_GL1::functions->vertex2fv
#define cl1Vertex2i CL_GL1::functions->vertex2i
#define cl1Vertex2iv CL_GL1::functions->vertex2iv
#define cl1Vertex2s CL_GL1::functions->vertex2s
#define cl1Vertex2sv CL_GL1::functions->vertex2sv
#define cl1Vertex3d CL_GL1::functions->vertex3d
#define cl1Vertex3dv CL_GL1::functions->vertex3dv
#define cl1Vertex3f CL_GL1::functions->vertex3f
#define cl1Vertex3fv CL_GL1::functions->vertex3fv
#define cl1Vertex3i CL_GL1::functions->vertex3i
#define cl1Vertex3iv CL_GL1::functions->vertex3iv
#define cl1Vertex3s CL_GL1::functions->vertex3s
#define cl1Vertex3sv CL_GL1::functions->vertex3sv
#define cl1Vertex4d CL_GL1::functions->vertex4d
#define cl1Vertex4dv CL_GL1::functions->vertex4dv
#define cl1Vertex4f CL_GL1::functions->vertex4f
#define cl1Vertex4fv CL_GL1::functions->vertex4fv
#define cl1Vertex4i CL_GL1::functions->vertex4i
#define cl1Vertex4iv CL_GL1::functions->vertex4iv
#define cl1Vertex4s CL_GL1::functions->vertex4s
#define cl1Vertex4sv CL_GL1::functions->vertex4sv
#define cl1VertexPointer CL_GL1::functions->vertexPointer
#define cl1Viewport CL_GL1::functions->viewport
#define cl1DrawRangeElements CL_GL1::functions->drawRangeElements
#define cl1TexImage3D CL_GL1::functions->texImage3D
#define cl1ColorTable CL_GL1::functions->colorTable
#define cl1CopyColorTable CL_GL1::functions->copyColorTable
#define cl1ColorTableParameteriv CL_GL1::functions->colorTableParameteriv
#define cl1ColorTableParameterfv CL_GL1::functions->colorTableParameterfv
#define cl1GetColorTable CL_GL1::functions->getColorTable
#define cl1GetColorTableParameteriv CL_GL1::functions->getColorTableParameteriv
#define cl1GetColorTableParameterfv CL_GL1::functions->getColorTableParameterfv
#define cl1ColorSubTable CL_GL1::functions->colorSubTable
#define cl1CopyColorSubTable CL_GL1::functions->copyColorSubTable
#define cl1ConvolutionFilter1D CL_GL1::functions->convolutionFilter1D
#define cl1ConvolutionFilter2D CL_GL1::functions->convolutionFilter2D
#define cl1CopyConvolutionFilter1D CL_GL1::functions->copyConvolutionFilter1D
#define cl1CopyConvolutionFilter2D CL_GL1::functions->copyConvolutionFilter2D
#define cl1GetConvolutionFilter CL_GL1::functions->getConvolutionFilter
#define cl1SeparableFilter2D CL_GL1::functions->separableFilter2D
#define cl1GetSeparableFilter CL_GL1::functions->getSeparableFilter
#define cl1ConvolutionParameteri CL_GL1::functions->convolutionParameteri
#define cl1ConvolutionParameteriv CL_GL1::functions->convolutionParameteriv
#define cl1ConvolutionParameterf CL_GL1::functions->convolutionParameterf
#define cl1ConvolutionParameterfv CL_GL1::functions->convolutionParameterfv
#define cl1GetConvolutionParameteriv CL_GL1::functions->getConvolutionParameteriv
#define cl1GetConvolutionParameterfv CL_GL1::functions->getConvolutionParameterfv
#define cl1Histogram CL_GL1::functions->histogram
#define cl1ResetHistogram CL_GL1::functions->resetHistogram
#define cl1GetHistogram CL_GL1::functions->getHistogram
#define cl1GetHistogramParameteriv CL_GL1::functions->getHistogramParameteriv
#define cl1GetHistogramParameterfv CL_GL1::functions->getHistogramParameterfv
#define cl1Minmax CL_GL1::functions->minmax
#define cl1ResetMinmax CL_GL1::functions->resetMinmax
#define cl1GetMinmax CL_GL1::functions->getMinmax
#define cl1GetMinmaxParameteriv CL_GL1::functions->getMinmaxParameteriv
#define cl1GetMinmaxParameterfv CL_GL1::functions->getMinmaxParameterfv
#define cl1BlendColor CL_GL1::functions->blendColor
#define cl1BlendEquation CL_GL1::functions->blendEquation
#define cl1ActiveTexture CL_GL1::functions->activeTexture
#define cl1ClientActiveTexture CL_GL1::functions->clientActiveTexture
#define cl1MultiTexCoord1d CL_GL1::functions->multiTexCoord1d
#define cl1MultiTexCoord1dv CL_GL1::functions->multiTexCoord1dv
#define cl1MultiTexCoord1f CL_GL1::functions->multiTexCoord1f
#define cl1MultiTexCoord1fv CL_GL1::functions->multiTexCoord1fv
#define cl1MultiTexCoord1i CL_GL1::functions->multiTexCoord1i
#define cl1MultiTexCoord1iv CL_GL1::functions->multiTexCoord1iv
#define cl1MultiTexCoord1s CL_GL1::functions->multiTexCoord1s
#define cl1MultiTexCoord1sv CL_GL1::functions->multiTexCoord1sv
#define cl1MultiTexCoord2d CL_GL1::functions->multiTexCoord2d
#define cl1MultiTexCoord2dv CL_GL1::functions->multiTexCoord2dv
#define cl1MultiTexCoord2f CL_GL1::functions->multiTexCoord2f
#define cl1MultiTexCoord2fv CL_GL1::functions->multiTexCoord2fv
#define cl1MultiTexCoord2i CL_GL1::functions->multiTexCoord2i
#define cl1MultiTexCoord2iv CL_GL1::functions->multiTexCoord2iv
#define cl1MultiTexCoord2s CL_GL1::functions->multiTexCoord2s
#define cl1MultiTexCoord2sv CL_GL1::functions->multiTexCoord2sv
#define cl1MultiTexCoord3d CL_GL1::functions->multiTexCoord3d
#define cl1MultiTexCoord3dv CL_GL1::functions->multiTexCoord3dv
#define cl1MultiTexCoord3f CL_GL1::functions->multiTexCoord3f
#define cl1MultiTexCoord3fv CL_GL1::functions->multiTexCoord3fv
#define cl1MultiTexCoord3i CL_GL1::functions->multiTexCoord3i
#define cl1MultiTexCoord3iv CL_GL1::functions->multiTexCoord3iv
#define cl1MultiTexCoord3s CL_GL1::functions->multiTexCoord3s
#define cl1MultiTexCoord3sv CL_GL1::functions->multiTexCoord3sv
#define cl1MultiTexCoord4d CL_GL1::functions->multiTexCoord4d
#define cl1MultiTexCoord4dv CL_GL1::functions->multiTexCoord4dv
#define cl1MultiTexCoord4f CL_GL1::functions->multiTexCoord4f
#define cl1MultiTexCoord4fv CL_GL1::functions->multiTexCoord4fv
#define cl1MultiTexCoord4i CL_GL1::functions->multiTexCoord4i
#define cl1MultiTexCoord4iv CL_GL1::functions->multiTexCoord4iv
#define cl1MultiTexCoord4s CL_GL1::functions->multiTexCoord4s
#define cl1MultiTexCoord4sv CL_GL1::functions->multiTexCoord4sv
#define cl1CompressedTexImage1D CL_GL1::functions->compressedTexImage1D
#define cl1CompressedTexImage2D CL_GL1::functions->compressedTexImage2D
#define cl1CompressedTexImage3D CL_GL1::functions->compressedTexImage3D
#define cl1CompressedTexSubImage1D CL_GL1::functions->compressedTexSubImage1D
#define cl1CompressedTexSubImage2D CL_GL1::functions->compressedTexSubImage2D
#define cl1CompressedTexSubImage3D CL_GL1::functions->compressedTexSubImage3D
#define cl1GetCompressedTexImage CL_GL1::functions->getCompressedTexImage
#define cl1SampleCoverage CL_GL1::functions->sampleCoverage
#define cl1LoadTransposeMatrixd CL_GL1::functions->glLoadTransposeMatrixd
#define cl1LoadTransposeMatrixf CL_GL1::functions->glLoadTransposeMatrixf
#define cl1MultTransposeMatrixd CL_GL1::functions->glMultTransposeMatrixd
#define cl1MultTransposeMatrixf CL_GL1::functions->glMultTransposeMatrixf
#define cl1FogCoordd CL_GL1::functions->fogCoordd
#define cl1FogCoorddv CL_GL1::functions->fogCoorddv
#define cl1FogCoordf CL_GL1::functions->fogCoordf
#define cl1FogCoordfv CL_GL1::functions->fogCoordfv
#define cl1FogCoordPointer CL_GL1::functions->fogCoordPointer
#define cl1MultiDrawArrays CL_GL1::functions->multiDrawArrays
#define cl1MultiDrawElements CL_GL1::functions->multiDrawElementsEXT
#define cl1PointParameterf CL_GL1::functions->pointParameterf
#define cl1PointParameterfv CL_GL1::functions->pointParameterfv
#define cl1SecondaryColor3b CL_GL1::functions->secondaryColor3b
#define cl1SecondaryColor3bv CL_GL1::functions->secondaryColor3bv
#define cl1SecondaryColor3d CL_GL1::functions->secondaryColor3d
#define cl1SecondaryColor3dv CL_GL1::functions->secondaryColor3dv
#define cl1SecondaryColor3f CL_GL1::functions->secondaryColor3f
#define cl1SecondaryColor3fv CL_GL1::functions->secondaryColor3fv
#define cl1SecondaryColor3i CL_GL1::functions->secondaryColor3i
#define cl1SecondaryColor3iv CL_GL1::functions->secondaryColor3iv
#define cl1SecondaryColor3s CL_GL1::functions->secondaryColor3s
#define cl1SecondaryColor3sv CL_GL1::functions->secondaryColor3sv
#define cl1SecondaryColor3ub CL_GL1::functions->secondaryColor3ub
#define cl1SecondaryColor3ubv CL_GL1::functions->secondaryColor3ubv
#define cl1SecondaryColor3ui CL_GL1::functions->secondaryColor3ui
#define cl1SecondaryColor3uiv CL_GL1::functions->secondaryColor3uiv
#define cl1SecondaryColor3us CL_GL1::functions->secondaryColor3us
#define cl1SecondaryColor3usv CL_GL1::functions->secondaryColor3usv
#define cl1SecondaryColorPointer CL_GL1::functions->secondaryColorPointer
#define cl1BlendFuncSeparate CL_GL1::functions->blendFuncSeparate
#define cl1WindowPos2d CL_GL1::functions->windowPos2d
#define cl1WindowPos2dv CL_GL1::functions->windowPos2dv
#define cl1WindowPos2f CL_GL1::functions->windowPos2f
#define cl1WindowPos2fv CL_GL1::functions->windowPos2fv
#define cl1WindowPos2i CL_GL1::functions->windowPos2i
#define cl1WindowPos2iv CL_GL1::functions->windowPos2iv
#define cl1WindowPos2s CL_GL1::functions->windowPos2s
#define cl1WindowPos2sv CL_GL1::functions->windowPos2sv
#define cl1WindowPos3d CL_GL1::functions->windowPos3d
#define cl1WindowPos3dv CL_GL1::functions->windowPos3dv
#define cl1WindowPos3f CL_GL1::functions->windowPos3f
#define cl1WindowPos3fv CL_GL1::functions->windowPos3fv
#define cl1WindowPos3i CL_GL1::functions->windowPos3i
#define cl1WindowPos3iv CL_GL1::functions->windowPos3iv
#define cl1WindowPos3s CL_GL1::functions->windowPos3s
#define cl1WindowPos3sv CL_GL1::functions->windowPos3sv
#define cl1BindBuffer CL_GL1::functions->bindBuffer
#define cl1DeleteBuffers CL_GL1::functions->deleteBuffers
#define cl1GenBuffers CL_GL1::functions->genBuffers
#define cl1IsBuffer CL_GL1::functions->isBuffer
#define cl1BufferData CL_GL1::functions->bufferData
#define cl1BufferSubData CL_GL1::functions->bufferSubData
#define cl1GetBufferSubData CL_GL1::functions->getBufferSubData
#define cl1MapBuffer CL_GL1::functions->mapBuffer
#define cl1UnmapBuffer CL_GL1::functions->unmapBuffer
#define cl1GetBufferParameteriv CL_GL1::functions->getBufferParameteriv
#define cl1GetBufferPointerv CL_GL1::functions->getBufferPointerv
#define cl1GenQueries CL_GL1::functions->genQueries
#define cl1DeleteQueries CL_GL1::functions->deleteQueries
#define cl1IsQuery CL_GL1::functions->isQuery
#define cl1BeginQuery CL_GL1::functions->beginQuery
#define cl1EndQuery CL_GL1::functions->endQuery
#define cl1GetQueryiv CL_GL1::functions->getQueryiv
#define cl1GetQueryObjectiv CL_GL1::functions->getQueryObjectiv
#define cl1GetQueryObjectuiv CL_GL1::functions->getQueryObjectuiv
#define cl1DeleteShader CL_GL1::functions->deleteShader
#define cl1DeleteProgram CL_GL1::functions->deleteProgram
#define cl1GetHandle CL_GL1::functions->getHandle
#define cl1DetachShader CL_GL1::functions->detachShader
#define cl1CreateShader CL_GL1::functions->createShader
#define cl1ShaderSource CL_GL1::functions->shaderSource
#define cl1CompileShader CL_GL1::functions->compileShader
#define cl1CreateProgram CL_GL1::functions->createProgram
#define cl1AttachShader CL_GL1::functions->attachShader
#define cl1LinkProgram CL_GL1::functions->linkProgram
#define cl1UseProgram CL_GL1::functions->useProgram
#define cl1ValidateProgram CL_GL1::functions->validateProgram
#define cl1Uniform1f CL_GL1::functions->uniform1f
#define cl1Uniform2f CL_GL1::functions->uniform2f
#define cl1Uniform3f CL_GL1::functions->uniform3f
#define cl1Uniform4f CL_GL1::functions->uniform4f
#define cl1Uniform1i CL_GL1::functions->uniform1i
#define cl1Uniform2i CL_GL1::functions->uniform2i
#define cl1Uniform3i CL_GL1::functions->uniform3i
#define cl1Uniform4i CL_GL1::functions->uniform4i
#define cl1Uniform1fv CL_GL1::functions->uniform1fv
#define cl1Uniform2fv CL_GL1::functions->uniform2fv
#define cl1Uniform3fv CL_GL1::functions->uniform3fv
#define cl1Uniform4fv CL_GL1::functions->uniform4fv
#define cl1Uniform1iv CL_GL1::functions->uniform1iv
#define cl1Uniform2iv CL_GL1::functions->uniform2iv
#define cl1Uniform3iv CL_GL1::functions->uniform3iv
#define cl1Uniform4iv CL_GL1::functions->uniform4iv
#define cl1UniformMatrix2fv CL_GL1::functions->uniformMatrix2fv
#define cl1UniformMatrix3fv CL_GL1::functions->uniformMatrix3fv
#define cl1UniformMatrix4fv CL_GL1::functions->uniformMatrix4fv
#define cl1GetProgramfv CL_GL1::functions->getProgramfv
#define cl1GetProgramiv CL_GL1::functions->getProgramiv
#define cl1GetShaderfv CL_GL1::functions->getShaderfv
#define cl1GetShaderiv CL_GL1::functions->getShaderiv
#define cl1GetShaderInfoLog CL_GL1::functions->getShaderInfoLog
#define cl1GetProgramInfoLog CL_GL1::functions->getProgramInfoLog
#define cl1GetAttachedShaders CL_GL1::functions->getAttachedShaders
#define cl1GetUniformLocation CL_GL1::functions->getUniformLocation
#define cl1GetActiveUniform CL_GL1::functions->getActiveUniform
#define cl1GetUniformfv CL_GL1::functions->getUniformfv
#define cl1GetUniformiv CL_GL1::functions->getUniformiv
#define cl1GetShaderSource CL_GL1::functions->getShaderSource
#define cl1VertexAttrib1f CL_GL1::functions->vertexAttrib1f
#define cl1VertexAttrib1s CL_GL1::functions->vertexAttrib1s
#define cl1VertexAttrib1d CL_GL1::functions->vertexAttrib1d
#define cl1VertexAttrib2f CL_GL1::functions->vertexAttrib2f
#define cl1VertexAttrib2s CL_GL1::functions->vertexAttrib2s
#define cl1VertexAttrib2d CL_GL1::functions->vertexAttrib2d
#define cl1VertexAttrib3f CL_GL1::functions->vertexAttrib3f
#define cl1VertexAttrib3s CL_GL1::functions->vertexAttrib3s
#define cl1VertexAttrib3d CL_GL1::functions->vertexAttrib3d
#define cl1VertexAttrib4f CL_GL1::functions->vertexAttrib4f
#define cl1VertexAttrib4s CL_GL1::functions->vertexAttrib4s
#define cl1VertexAttrib4d CL_GL1::functions->vertexAttrib4d
#define cl1VertexAttrib4Nub CL_GL1::functions->vertexAttrib4Nub
#define cl1VertexAttrib1fv CL_GL1::functions->vertexAttrib1fv
#define cl1VertexAttrib1sv CL_GL1::functions->vertexAttrib1sv
#define cl1VertexAttrib1dv CL_GL1::functions->vertexAttrib1dv
#define cl1VertexAttrib2fv CL_GL1::functions->vertexAttrib2fv
#define cl1VertexAttrib2sv CL_GL1::functions->vertexAttrib2sv
#define cl1VertexAttrib2dv CL_GL1::functions->vertexAttrib2dv
#define cl1VertexAttrib3fv CL_GL1::functions->vertexAttrib3fv
#define cl1VertexAttrib3sv CL_GL1::functions->vertexAttrib3sv
#define cl1VertexAttrib3dv CL_GL1::functions->vertexAttrib3dv
#define cl1VertexAttrib4fv CL_GL1::functions->vertexAttrib4fv
#define cl1VertexAttrib4sv CL_GL1::functions->vertexAttrib4sv
#define cl1VertexAttrib4dv CL_GL1::functions->vertexAttrib4dv
#define cl1VertexAttrib4iv CL_GL1::functions->vertexAttrib4iv
#define cl1VertexAttrib4bv CL_GL1::functions->vertexAttrib4bv
#define cl1VertexAttrib4ubv CL_GL1::functions->vertexAttrib4ubv
#define cl1VertexAttrib4usv CL_GL1::functions->vertexAttrib4usv
#define cl1VertexAttrib4uiv CL_GL1::functions->vertexAttrib4uiv
#define cl1VertexAttrib4Nbv CL_GL1::functions->vertexAttrib4Nbv
#define cl1VertexAttrib4Nsv CL_GL1::functions->vertexAttrib4Nsv
#define cl1VertexAttrib4Niv CL_GL1::functions->vertexAttrib4Niv
#define cl1VertexAttrib4Nubv CL_GL1::functions->vertexAttrib4Nubv
#define cl1VertexAttrib4Nusv CL_GL1::functions->vertexAttrib4Nusv
#define cl1VertexAttrib4Nuiv CL_GL1::functions->vertexAttrib4Nuiv
#define cl1VertexAttribPointer CL_GL1::functions->vertexAttribPointer
#define cl1EnableVertexAttribArray CL_GL1::functions->enableVertexAttribArray
#define cl1DisableVertexAttribArray CL_GL1::functions->disableVertexAttribArray
#define cl1BindAttribLocation CL_GL1::functions->bindAttribLocation
#define cl1GetActiveAttrib CL_GL1::functions->getActiveAttrib
#define cl1GetAttribLocation CL_GL1::functions->getAttribLocation
#define cl1GetVertexAttribdv CL_GL1::functions->getVertexAttribdv
#define cl1GetVertexAttribfv CL_GL1::functions->getVertexAttribfv
#define cl1GetVertexAttribiv CL_GL1::functions->getVertexAttribiv
#define cl1GetVertexAttribPointerv CL_GL1::functions->getVertexAttribPointerv
#define cl1DrawBuffers CL_GL1::functions->drawBuffers
#define cl1StencilFuncSeparate CL_GL1::functions->stencilFuncSeparate
#define cl1StencilOpSeparate CL_GL1::functions->stencilOpSeparate
#define cl1IsRenderbuffer CL_GL1::functions->isRenderbuffer
#define cl1BindRenderbuffer CL_GL1::functions->bindRenderbuffer
#define cl1DeleteRenderbuffers CL_GL1::functions->deleteRenderbuffers
#define cl1GenRenderbuffers CL_GL1::functions->genRenderbuffers
#define cl1RenderbufferStorage CL_GL1::functions->renderbufferStorage
#define cl1GetRenderbufferParameteriv CL_GL1::functions->getRenderbufferParameteriv
#define cl1IsFramebuffer CL_GL1::functions->isFramebuffer
#define cl1BindFramebuffer CL_GL1::functions->bindFramebuffer
#define cl1DeleteFramebuffers CL_GL1::functions->deleteFramebuffers
#define cl1GenFramebuffers CL_GL1::functions->genFramebuffers
#define cl1CheckFramebufferStatus CL_GL1::functions->checkFramebufferStatus
#define cl1FramebufferTexture1D CL_GL1::functions->framebufferTexture1D
#define cl1FramebufferTexture2D CL_GL1::functions->framebufferTexture2D
#define cl1FramebufferTexture3D CL_GL1::functions->framebufferTexture3D
#define cl1FramebufferRenderbuffer CL_GL1::functions->framebufferRenderbuffer
#define cl1GetFramebufferAttachmentParameteriv CL_GL1::functions->getFramebufferAttachmentParameteriv
#define cl1GenerateMipmap CL_GL1::functions->generateMipmap
#define cl1BlitFramebuffer CL_GL1::functions->blitFramebuffer

#ifdef WIN32
#define cl1WglCreatePbufferARB CL_GL1::functions->wglCreatePbufferARB
#define cl1WglGetPbufferDCARB CL_GL1::functions->wglGetPbufferDCARB
#define cl1WglReleasePbufferDCARB CL_GL1::functions->wglReleasePbufferDCARB
#define cl1WglDestroyPbufferARB CL_GL1::functions->wglDestroyPbufferARB
#define cl1WglQueryPbufferARB CL_GL1::functions->wglQueryPbufferARB
#endif

/// \}

/// \}

