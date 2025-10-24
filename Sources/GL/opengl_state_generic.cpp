/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "opengl_state_generic.h"
#include "API/Display/graphic_context.h"
#include "API/GL/opengl_state_data.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "graphic_context_opengl.h"
#ifdef __APPLE__
#include <AGL/agl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <map>

#define cl_pixelcenter_constant 0.375

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState_Generic construction:

CL_OpenGLState_Generic::CL_OpenGLState_Generic(CL_GraphicContext *gc)
: gc_opengl((CL_GraphicContext_OpenGL *) gc->impl), in_2d(false), is_upside_down(false), need_update_viewport(false)
{
	init();
}

CL_OpenGLState_Generic::CL_OpenGLState_Generic(CL_GraphicContext_OpenGL *gc_opengl)
: gc_opengl(gc_opengl), in_2d(false), is_upside_down(false), need_update_viewport(false)
{
	init();
}

CL_OpenGLState_Generic::~CL_OpenGLState_Generic()
{
	if (active_state == this)
	{
		save();
		active_state = 0;
		#ifdef WIN32
			set_context(gc_opengl->get_drawable(), 0);
		#else
		#ifdef __APPLE__
			set_context(0);
		#else
			set_context(gc_opengl->get_display(), 0, 0);
		#endif
		#endif
	}

	std::list< std::pair<CL_OpenGLStateData *, bool> >::iterator it;
	for (it = data_objects.begin(); it != data_objects.end(); ++it)
	{
		if ((*it).second) delete (*it).first;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState_Generic attributes:

bool CL_OpenGLState_Generic::is_active() const
{
	return (active_state == this);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState_Generic operations:

void CL_OpenGLState_Generic::set_active()
{
	if (active_state == this) return;

	// Cannot be made active yet. Throw exception instead?
	if (gc_opengl->get_context() == 0) return;

	if (active_state) active_state->save();
	#ifdef WIN32
		set_context(gc_opengl->get_drawable(), gc_opengl->get_context());
	#else
	#ifdef __APPLE__
		set_context(gc_opengl->get_context());
	#else
		set_context(gc_opengl->get_display(), gc_opengl->get_drawable(), gc_opengl->get_context());
	#endif
	#endif
	active_state = this;
	active_state->load();
}

void CL_OpenGLState_Generic::setup_2d(bool upside_down)
{
	set_active();

	in_2d = true;
	is_upside_down = upside_down;

	clMatrixMode(CL_PROJECTION);
	clLoadIdentity();

	CLdouble viewport[4];
	clGetDoublev(CL_VIEWPORT, viewport);

	CLdouble width = viewport[2];
	CLdouble height = viewport[3];
	if (upside_down) 
		glOrtho(0, width, 0., height, -1000.0, 1000.0);
	else
		glOrtho(0.0, width, height, 0.0, -1000.0, 1000.0);

	clMatrixMode(CL_MODELVIEW);
	clLoadIdentity();
	clTranslated(cl_pixelcenter_constant, cl_pixelcenter_constant, 0.0);
	clMultMatrixd(gc_opengl->get_modelview());

	clEnable(CL_SCISSOR_TEST);
	on_update_scissor();

//	if (surface_target) on_resize((int)width, (int)height);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLState_Generic implementation:

void CL_OpenGLState_Generic::init()
{
	slots.connect(gc_opengl->sig_update_scissor, this, &CL_OpenGLState_Generic::on_update_scissor);
	slots.connect(gc_opengl->sig_update_modelview, this, &CL_OpenGLState_Generic::on_update_modelview);
	slots.connect(gc_opengl->sig_update_viewport, this, &CL_OpenGLState_Generic::on_update_viewport);

	state_texture_2d = CL_FALSE;
	state_bind_texture_2d = 0;
	state_blend = CL_FALSE;
	state_fog = CL_FALSE;
	state_depth_test = CL_FALSE;
	state_scissor_test = CL_FALSE;
	memset(state_viewport, 0, sizeof(CLint)*4);
	memset(state_scissor_box, 0, sizeof(CLint)*4);
	memset(state_clear_color, 0, sizeof(CLdouble)*4);
	state_blendfunc_sfactor       = CL_ZERO;
	state_blendfunc_dfactor       = CL_ZERO;
	state_blendfunc_sfactor_alpha = CL_ZERO;
	state_blendfunc_dfactor_alpha = CL_ZERO;
	state_color[0] = 1.0;
	state_color[1] = 1.0;
	state_color[2] = 1.0;
	state_color[3] = 1.0;
	state_vertex_pointer_size = 0;
	state_vertex_pointer_type = 0;
	state_vertex_pointer_stride = 0;
	state_vertex_pointer = 0;
	state_color_pointer_size = 0;
	state_color_pointer_type = 0;
	state_color_pointer_stride = 0;
	state_color_pointer = 0;
	state_texcoord_pointer_size = 0;
	state_texcoord_pointer_type = 0;
	state_texcoord_pointer_stride = 0;
	state_texcoord_pointer = 0;
	state_vertex_array = CL_FALSE;
	state_color_array = CL_FALSE;
	state_texture_coord_array = CL_FALSE;
	memset(state_matrix_texture, 0, sizeof(CLdouble)*16);
	memset(state_matrix_projection, 0, sizeof(CLdouble)*16);
	memset(state_matrix_modelview, 0, sizeof(CLdouble)*16);
	state_matrix_texture[0] = 1.0;
	state_matrix_texture[5] = 1.0;
	state_matrix_texture[10] = 1.0;
	state_matrix_texture[15] = 1.0;
	state_matrix_projection[0] = 1.0;
	state_matrix_projection[5] = 1.0;
	state_matrix_projection[10] = 1.0;
	state_matrix_projection[15] = 1.0;
	state_matrix_modelview[0] = 1.0;
	state_matrix_modelview[5] = 1.0;
	state_matrix_modelview[10] = 1.0;
	state_matrix_modelview[15] = 1.0;
	state_matrix_mode = CL_MODELVIEW;
	state_texture_env_mode = CL_MODULATE;
	state_stencil_test = CL_FALSE;
	state_stencil_func = CL_LEQUAL;
	state_stencil_value_mask = 0;
	state_stencil_ref = 0;
	state_stencil_fail = CL_KEEP;
	state_stencil_pass_depth_pass = CL_KEEP;
	state_stencil_pass_depth_fail = CL_KEEP;
	state_alpha_test = CL_FALSE;
	state_alpha_test_func = CL_GEQUAL;
	state_alpha_test_ref = 0.0;
	state_polygon_mode[0] = CL_FRONT_AND_BACK;
	state_polygon_mode[1] = CL_FILL;
	memset(state_color_write_mask, 1, sizeof(CLboolean)*4);
}

void CL_OpenGLState_Generic::on_update_scissor()
{
	if (is_active() == false) return;
	flush();

	CLdouble viewport[4];
	clGetDoublev(CL_VIEWPORT, viewport);

	//CLdouble &viewport_width  = viewport[2];
	CLdouble &viewport_height = viewport[3];

	const CL_Rect &clip = gc_opengl->get_cliprect();
	int clip_width  = clip.get_width();
	int clip_height = clip.get_height();

	clScissor(clip.left, static_cast<int>(viewport_height - clip.top - clip_height), clip_width, clip_height);
}

void CL_OpenGLState_Generic::on_update_modelview()
{
	if (is_active() == false) return;
	if (in_2d)
	{
		flush();
		clLoadIdentity();
		clTranslated(cl_pixelcenter_constant, cl_pixelcenter_constant, 0.0);
		clMultMatrixd(gc_opengl->get_modelview());
	}
}

void CL_OpenGLState_Generic::on_update_viewport()
{
	if (is_active() == false) return;
	need_update_viewport = false;

	CLint width = (CLint) gc_opengl->get_width();
	CLint height = (CLint) gc_opengl->get_height();
	clViewport(0, 0, width ,height );

	if (in_2d)
	{
		on_update_scissor();

		clMatrixMode(CL_PROJECTION);
		clLoadIdentity();
		if (is_upside_down)
			glOrtho(0.0, width, 0.0, height, -1000.0, 1000.0);
		else
			glOrtho(0.0, width, height, 0.0, -1000.0, 1000.0);
		clMatrixMode(CL_MODELVIEW);
	}
}

void CL_OpenGLState_Generic::flush()
{
	std::list< std::pair<CL_OpenGLStateData *, bool> >::iterator it;
	for (it = data_objects.begin(); it != data_objects.end(); ++it)
	{
		(*it).first->on_flush();
	}
}

void CL_OpenGLState_Generic::save()
{
	std::list< std::pair<CL_OpenGLStateData *, bool> >::reverse_iterator it;
	for (it = data_objects.rbegin(); it != data_objects.rend(); ++it)
	{
		(*it).first->on_save();
	}

	if (in_2d)
	{
		// todo: save 2d states.
	}

	state_texture_2d = clIsEnabled(CL_TEXTURE_2D);
	clGetIntegerv(CL_TEXTURE_2D_BINDING, &state_bind_texture_2d);
	state_blend = clIsEnabled(CL_BLEND);
	state_fog = clIsEnabled(CL_FOG);
	state_depth_test = clIsEnabled(CL_DEPTH_TEST);
	state_scissor_test = clIsEnabled(CL_SCISSOR_TEST);
	clGetIntegerv(CL_VIEWPORT, state_viewport);
	clGetIntegerv(CL_SCISSOR_BOX, state_scissor_box);
	clGetDoublev(CL_COLOR_CLEAR_VALUE, state_clear_color);

	clGetIntegerv(CL_BLEND_SRC,       &state_blendfunc_sfactor);
	clGetIntegerv(CL_BLEND_DST,       &state_blendfunc_dfactor);
	clGetIntegerv(CL_BLEND_SRC_ALPHA, &state_blendfunc_sfactor_alpha);
	clGetIntegerv(CL_BLEND_DST_ALPHA, &state_blendfunc_dfactor_alpha);

	clGetDoublev(CL_CURRENT_COLOR, state_color);
	clGetIntegerv(CL_VERTEX_ARRAY_SIZE, &state_vertex_pointer_size);
	clGetIntegerv(CL_VERTEX_ARRAY_TYPE, &state_vertex_pointer_type);
	clGetIntegerv(CL_VERTEX_ARRAY_STRIDE, &state_vertex_pointer_stride);
	clGetPointerv(CL_VERTEX_ARRAY_POINTER, &state_vertex_pointer);
	clGetIntegerv(CL_COLOR_ARRAY_SIZE, &state_color_pointer_size);
	clGetIntegerv(CL_COLOR_ARRAY_TYPE, &state_color_pointer_type);
	clGetIntegerv(CL_COLOR_ARRAY_STRIDE, &state_color_pointer_stride);
	clGetPointerv(CL_COLOR_ARRAY_POINTER, &state_color_pointer);
	clGetIntegerv(CL_TEXTURE_COORD_ARRAY_SIZE, &state_texcoord_pointer_size);
	clGetIntegerv(CL_TEXTURE_COORD_ARRAY_TYPE, &state_texcoord_pointer_type);
	clGetIntegerv(CL_TEXTURE_COORD_ARRAY_STRIDE, &state_texcoord_pointer_stride);
	clGetPointerv(CL_TEXTURE_COORD_ARRAY_POINTER, &state_texcoord_pointer);
	state_vertex_array = clIsEnabled(CL_VERTEX_ARRAY);
	state_color_array = clIsEnabled(CL_COLOR_ARRAY);
	state_texture_coord_array = clIsEnabled(CL_TEXTURE_COORD_ARRAY);
	clGetIntegerv(CL_MATRIX_MODE, &state_matrix_mode);
	clGetDoublev(CL_TEXTURE_MATRIX, state_matrix_texture);
	clGetDoublev(CL_PROJECTION_MATRIX, state_matrix_projection);
	clGetDoublev(CL_MODELVIEW_MATRIX, state_matrix_modelview);
	clGetTexEnvfv(CL_TEXTURE_ENV, CL_TEXTURE_ENV_MODE, &state_texture_env_mode);
	state_stencil_test = clIsEnabled(CL_STENCIL_TEST);
	clGetIntegerv(CL_STENCIL_VALUE_MASK, &state_stencil_value_mask);
	clGetIntegerv(CL_STENCIL_REF, &state_stencil_ref);
	clGetIntegerv(CL_STENCIL_FUNC, &state_stencil_func);
	clGetIntegerv(CL_STENCIL_FAIL, &state_stencil_fail);
	clGetIntegerv(CL_STENCIL_PASS_DEPTH_PASS, &state_stencil_pass_depth_pass);
	clGetIntegerv(CL_STENCIL_PASS_DEPTH_FAIL, &state_stencil_pass_depth_fail);
	state_alpha_test = clIsEnabled(CL_ALPHA_TEST);
	clGetIntegerv(CL_ALPHA_TEST_FUNC, &state_alpha_test_func);
	clGetDoublev(CL_ALPHA_TEST_REF, &state_alpha_test_ref);
	clGetBooleanv(CL_COLOR_WRITEMASK, state_color_write_mask);
	clGetIntegerv(CL_POLYGON_MODE, state_polygon_mode);
}

void CL_OpenGLState_Generic::load()
{
	// Make sure context exists
	if (CL_OpenGL::functions == 0) 
		return;

	// If there's no viewport, set it up.
	if (state_viewport[0] == 0 && state_viewport[1] == 0 && state_viewport[2] == 0 && state_viewport[3] == 0)
	{
		state_viewport[2] = gc_opengl->get_width();
		state_viewport[3] = gc_opengl->get_height();
	}

	if (state_texture_2d) clEnable(CL_TEXTURE_2D); else clDisable(CL_TEXTURE_2D);
	if (state_texture_2d && state_bind_texture_2d) clBindTexture(CL_TEXTURE_2D, state_bind_texture_2d);
	if (state_blend) clEnable(CL_BLEND); else clDisable(CL_BLEND);
	if (state_fog) clEnable(CL_FOG); else clDisable(CL_FOG);
	if (state_depth_test) clEnable(CL_DEPTH_TEST); else clDisable(CL_DEPTH_TEST);
	if (state_scissor_test) clEnable(CL_SCISSOR_TEST); else clDisable(CL_SCISSOR_TEST);
	clViewport(state_viewport[0], state_viewport[1], state_viewport[2], state_viewport[3]);
	clScissor(state_scissor_box[0], state_scissor_box[1], state_scissor_box[2], state_scissor_box[3]);
	clClearColor((GLclampf) state_clear_color[0], (GLclampf) state_clear_color[1], (GLclampf) state_clear_color[2], (GLclampf) state_clear_color[3]);

	if ( !clBlendFuncSeparate ||
		 (state_blendfunc_sfactor == state_blendfunc_sfactor_alpha &&
		  state_blendfunc_dfactor == state_blendfunc_dfactor_alpha) )
	{
		clBlendFunc(state_blendfunc_sfactor, state_blendfunc_dfactor);
	}
	else
	{
		clBlendFuncSeparate(state_blendfunc_sfactor, state_blendfunc_dfactor, state_blendfunc_sfactor_alpha, state_blendfunc_dfactor_alpha);
	}

	clColor4dv(state_color);
	if (state_vertex_pointer) clVertexPointer(state_vertex_pointer_size, state_vertex_pointer_type, state_vertex_pointer_stride, state_vertex_pointer);
	if (state_color_pointer) clColorPointer(state_color_pointer_size, state_color_pointer_type, state_color_pointer_stride, state_color_pointer);
	if (state_texcoord_pointer) clTexCoordPointer(state_texcoord_pointer_size, state_texcoord_pointer_type, state_texcoord_pointer_stride, state_texcoord_pointer);
	if (state_vertex_array) clEnableClientState(CL_VERTEX_ARRAY); else clDisableClientState(CL_VERTEX_ARRAY);
	if (state_color_array) clEnableClientState(CL_COLOR_ARRAY); else clDisableClientState(CL_COLOR_ARRAY);
	if (state_texture_coord_array) clEnableClientState(CL_TEXTURE_COORD_ARRAY); else clDisableClientState(CL_TEXTURE_COORD_ARRAY);

	clMatrixMode(CL_TEXTURE);
	clLoadMatrixd(state_matrix_texture);
	clMatrixMode(CL_PROJECTION);
	clLoadMatrixd(state_matrix_projection);
	clMatrixMode(CL_MODELVIEW);
	clLoadMatrixd(state_matrix_modelview);
	clMatrixMode(state_matrix_mode);
	clTexEnvf(CL_TEXTURE_ENV, CL_TEXTURE_ENV_MODE, state_texture_env_mode);

	if (state_stencil_test) clEnable(CL_STENCIL_TEST); else clDisable(CL_STENCIL_TEST);
	clStencilFunc(state_stencil_func, state_stencil_ref, state_stencil_value_mask);
	clStencilOp(state_stencil_fail,state_stencil_pass_depth_fail,state_stencil_pass_depth_pass);
	if (state_alpha_test) clEnable(CL_ALPHA_TEST); else clDisable(CL_ALPHA_TEST);
	clAlphaFunc(state_alpha_test_func, state_alpha_test_ref);
	clColorMask(state_color_write_mask[0],state_color_write_mask[1],state_color_write_mask[2],state_color_write_mask[3]);

	clPolygonMode(CL_FRONT_AND_BACK, state_polygon_mode[1]);

	if (need_update_viewport) on_update_viewport();
	if (in_2d) setup_2d(is_upside_down);

	std::list< std::pair<CL_OpenGLStateData *, bool> >::iterator it;
	for (it = data_objects.begin(); it != data_objects.end(); ++it)
	{
		(*it).first->on_load();
	}
}

#ifdef WIN32

void CL_OpenGLState_Generic::set_context(HDC hdc, HGLRC context)
{
	if (cur_drawable != hdc && cur_context != context)
	{
		wglMakeCurrent(hdc, context);
		cur_drawable = hdc;
		cur_context = context;

		std::map<std::pair<HDC, HGLRC>, CL_GLFunctions *>::iterator it;
		it = function_map.find(std::pair<HDC, HGLRC>(hdc, context));
		if (it == function_map.end())
		{
			CL_GLFunctions *functions = setup_binds();
			function_map[std::pair<HDC, HGLRC>(hdc, context)] = functions;
			CL_OpenGL::functions = functions;
		}
		else
		{
			CL_OpenGL::functions = it->second;
		}
	}
}

#else
#ifdef __APPLE__

void CL_OpenGLState_Generic::set_context(AGLContext context)
{
	if (cur_context != context)
	{
		aglSetCurrentContext(context);
		cur_context = context;

		std::map<AGLContext, CL_GLFunctions *>::iterator it;
		it = function_map.find(context);
		if (it == function_map.end())
		{
			CL_GLFunctions *functions = setup_binds();
			function_map[context] = functions;
			CL_OpenGL::functions = functions;
		}
		else
		{
			CL_OpenGL::functions = it->second;
		}
	}
}

#else

void CL_OpenGLState_Generic::set_context(Display *disp, GLXDrawable drawable, GLXContext context)
{
	if (disp == 0)
		return;

	if (cur_drawable != drawable && cur_context != context)
	{
		glXMakeCurrent(disp, drawable, context);
		cur_drawable = drawable;
		cur_context = context;

		std::map<std::pair<GLXDrawable, GLXContext>, CL_GLFunctions *>::iterator it;
		it = function_map.find(std::pair<GLXDrawable, GLXContext>(drawable, context));
		if (it == function_map.end())
		{
			CL_GLFunctions *functions = setup_binds();
			function_map[std::pair<GLXDrawable, GLXContext>(drawable, context)] = functions;
			CL_OpenGL::functions = functions;
		}
		else
		{
			CL_OpenGL::functions = it->second;
		}
	}
}

#endif
#endif

void CL_OpenGLState_Generic::clear_context (void)
{
	while (!function_map.empty())
	{
		delete function_map.begin()->second;
		function_map.erase(function_map.begin());
	}
	CL_OpenGL::functions = 0;
}

#ifdef WIN32
	HDC CL_OpenGLState_Generic::cur_drawable = 0;

	HGLRC CL_OpenGLState_Generic::cur_context = 0;

	std::map<std::pair<HDC, HGLRC>, CL_GLFunctions *> CL_OpenGLState_Generic::function_map;
#else
#ifdef __APPLE__
	AGLContext CL_OpenGLState_Generic::cur_context = 0;
	
	std::map<AGLContext, CL_GLFunctions *> CL_OpenGLState_Generic::function_map;
#else
	GLXDrawable CL_OpenGLState_Generic::cur_drawable = 0;

	GLXContext CL_OpenGLState_Generic::cur_context = 0;

	std::map<std::pair<GLXDrawable, GLXContext>, CL_GLFunctions *> CL_OpenGLState_Generic::function_map;
#endif
#endif

CL_OpenGLState_Generic *CL_OpenGLState_Generic::active_state = 0;

CL_GLFunctions *CL_OpenGLState_Generic::setup_binds()
{
	CL_GLFunctions *functions = new CL_GLFunctions;
	memset(functions, 0, sizeof(CL_GLFunctions));

	// Binds for OpenGL 1.1:

#ifdef GL_VERSION_1_1
	functions->accum = (CL_GLFunctions::ptr_glAccum) &glAccum;
	functions->alphaFunc = (CL_GLFunctions::ptr_glAlphaFunc) &glAlphaFunc;
	functions->areTexturesResident = (CL_GLFunctions::ptr_glAreTexturesResident) &glAreTexturesResident;
	functions->arrayElement = (CL_GLFunctions::ptr_glArrayElement) &glArrayElement;
	functions->begin = (CL_GLFunctions::ptr_glBegin) &glBegin;
	functions->bindTexture = (CL_GLFunctions::ptr_glBindTexture) &glBindTexture;
	functions->bitmap = (CL_GLFunctions::ptr_glBitmap) &glBitmap;
	functions->blendFunc = (CL_GLFunctions::ptr_glBlendFunc) &glBlendFunc;
	functions->callList = (CL_GLFunctions::ptr_glCallList) &glCallList;
	functions->callLists = (CL_GLFunctions::ptr_glCallLists) &glCallLists;
	functions->clear = (CL_GLFunctions::ptr_glClear) &glClear;
	functions->clearAccum = (CL_GLFunctions::ptr_glClearAccum) &glClearAccum;
	functions->clearColor = (CL_GLFunctions::ptr_glClearColor) &glClearColor;
	functions->clearDepth = (CL_GLFunctions::ptr_glClearDepth) &glClearDepth;
	functions->clearIndex = (CL_GLFunctions::ptr_glClearIndex) &glClearIndex;
	functions->clearStencil = (CL_GLFunctions::ptr_glClearStencil) &glClearStencil;
	functions->clipPlane = (CL_GLFunctions::ptr_glClipPlane) &glClipPlane;
	functions->color3b = (CL_GLFunctions::ptr_glColor3b) &glColor3b;
	functions->color3bv = (CL_GLFunctions::ptr_glColor3bv) &glColor3bv;
	functions->color3d = (CL_GLFunctions::ptr_glColor3d) &glColor3d;
	functions->color3dv = (CL_GLFunctions::ptr_glColor3dv) &glColor3dv;
	functions->color3f = (CL_GLFunctions::ptr_glColor3f) &glColor3f;
	functions->color3fv = (CL_GLFunctions::ptr_glColor3fv) &glColor3fv;
	functions->color3i = (CL_GLFunctions::ptr_glColor3i) &glColor3i;
	functions->color3iv = (CL_GLFunctions::ptr_glColor3iv) &glColor3iv;
	functions->color3s = (CL_GLFunctions::ptr_glColor3s) &glColor3s;
	functions->color3sv = (CL_GLFunctions::ptr_glColor3sv) &glColor3sv;
	functions->color3ub = (CL_GLFunctions::ptr_glColor3ub) &glColor3ub;
	functions->color3ubv = (CL_GLFunctions::ptr_glColor3ubv) &glColor3ubv;
	functions->color3ui = (CL_GLFunctions::ptr_glColor3ui) &glColor3ui;
	functions->color3uiv = (CL_GLFunctions::ptr_glColor3uiv) &glColor3uiv;
	functions->color3us = (CL_GLFunctions::ptr_glColor3us) &glColor3us;
	functions->color3usv = (CL_GLFunctions::ptr_glColor3usv) &glColor3usv;
	functions->color4b = (CL_GLFunctions::ptr_glColor4b) &glColor4b;
	functions->color4bv = (CL_GLFunctions::ptr_glColor4bv) &glColor4bv;
	functions->color4d = (CL_GLFunctions::ptr_glColor4d) &glColor4d;
	functions->color4dv = (CL_GLFunctions::ptr_glColor4dv) &glColor4dv;
	functions->color4f = (CL_GLFunctions::ptr_glColor4f) &glColor4f;
	functions->color4fv = (CL_GLFunctions::ptr_glColor4fv) &glColor4fv;
	functions->color4i = (CL_GLFunctions::ptr_glColor4i) &glColor4i;
	functions->color4iv = (CL_GLFunctions::ptr_glColor4iv) &glColor4iv;
	functions->color4s = (CL_GLFunctions::ptr_glColor4s) &glColor4s;
	functions->color4sv = (CL_GLFunctions::ptr_glColor4sv) &glColor4sv;
	functions->color4ub = (CL_GLFunctions::ptr_glColor4ub) &glColor4ub;
	functions->color4ubv = (CL_GLFunctions::ptr_glColor4ubv) &glColor4ubv;
	functions->color4ui = (CL_GLFunctions::ptr_glColor4ui) &glColor4ui;
	functions->color4uiv = (CL_GLFunctions::ptr_glColor4uiv) &glColor4uiv;
	functions->color4us = (CL_GLFunctions::ptr_glColor4us) &glColor4us;
	functions->color4usv = (CL_GLFunctions::ptr_glColor4usv) &glColor4usv;
	functions->colorMask = (CL_GLFunctions::ptr_glColorMask) &glColorMask;
	functions->colorMaterial = (CL_GLFunctions::ptr_glColorMaterial) &glColorMaterial;
	functions->colorPointer = (CL_GLFunctions::ptr_glColorPointer) &glColorPointer;
	functions->copyPixels = (CL_GLFunctions::ptr_glCopyPixels) &glCopyPixels;
	functions->copyTexImage1D = (CL_GLFunctions::ptr_glCopyTexImage1D) &glCopyTexImage1D;
	functions->copyTexImage2D = (CL_GLFunctions::ptr_glCopyTexImage2D) &glCopyTexImage2D;
	functions->copyTexSubImage1D = (CL_GLFunctions::ptr_glCopyTexSubImage1D) &glCopyTexSubImage1D;
	functions->copyTexSubImage2D = (CL_GLFunctions::ptr_glCopyTexSubImage2D) &glCopyTexSubImage2D;
	functions->cullFace = (CL_GLFunctions::ptr_glCullFace) &glCullFace;
	functions->deleteLists = (CL_GLFunctions::ptr_glDeleteLists) &glDeleteLists;
	functions->deleteTextures = (CL_GLFunctions::ptr_glDeleteTextures) &glDeleteTextures;
	functions->depthFunc = (CL_GLFunctions::ptr_glDepthFunc) &glDepthFunc;
	functions->depthMask = (CL_GLFunctions::ptr_glDepthMask) &glDepthMask;
	functions->depthRange = (CL_GLFunctions::ptr_glDepthRange) &glDepthRange;
	functions->disable = (CL_GLFunctions::ptr_glDisable) &glDisable;
	functions->disableClientState = (CL_GLFunctions::ptr_glDisableClientState) &glDisableClientState;
	functions->drawArrays = (CL_GLFunctions::ptr_glDrawArrays) &glDrawArrays;
	functions->drawBuffer = (CL_GLFunctions::ptr_glDrawBuffer) &glDrawBuffer;
	functions->drawElements = (CL_GLFunctions::ptr_glDrawElements) &glDrawElements;
	functions->drawPixels = (CL_GLFunctions::ptr_glDrawPixels) &glDrawPixels;
	functions->edgeFlag = (CL_GLFunctions::ptr_glEdgeFlag) &glEdgeFlag;
	functions->edgeFlagPointer = (CL_GLFunctions::ptr_glEdgeFlagPointer) &glEdgeFlagPointer;
	functions->edgeFlagv = (CL_GLFunctions::ptr_glEdgeFlagv) &glEdgeFlagv;
	functions->enable = (CL_GLFunctions::ptr_glEnable) &glEnable;
	functions->enableClientState = (CL_GLFunctions::ptr_glEnableClientState) &glEnableClientState;
	functions->end = (CL_GLFunctions::ptr_glEnd) &glEnd;
	functions->endList = (CL_GLFunctions::ptr_glEndList) &glEndList;
	functions->evalCoord1d = (CL_GLFunctions::ptr_glEvalCoord1d) &glEvalCoord1d;
	functions->evalCoord1dv = (CL_GLFunctions::ptr_glEvalCoord1dv) &glEvalCoord1dv;
	functions->evalCoord1f = (CL_GLFunctions::ptr_glEvalCoord1f) &glEvalCoord1f;
	functions->evalCoord1fv = (CL_GLFunctions::ptr_glEvalCoord1fv) &glEvalCoord1fv;
	functions->evalCoord2d = (CL_GLFunctions::ptr_glEvalCoord2d) &glEvalCoord2d;
	functions->evalCoord2dv = (CL_GLFunctions::ptr_glEvalCoord2dv) &glEvalCoord2dv;
	functions->evalCoord2f = (CL_GLFunctions::ptr_glEvalCoord2f) &glEvalCoord2f;
	functions->evalCoord2fv = (CL_GLFunctions::ptr_glEvalCoord2fv) &glEvalCoord2fv;
	functions->evalMesh1 = (CL_GLFunctions::ptr_glEvalMesh1) &glEvalMesh1;
	functions->evalMesh2 = (CL_GLFunctions::ptr_glEvalMesh2) &glEvalMesh2;
	functions->evalPoint1 = (CL_GLFunctions::ptr_glEvalPoint1) &glEvalPoint1;
	functions->evalPoint2 = (CL_GLFunctions::ptr_glEvalPoint2) &glEvalPoint2;
	functions->feedbackBuffer = (CL_GLFunctions::ptr_glFeedbackBuffer) &glFeedbackBuffer;
	functions->finish = (CL_GLFunctions::ptr_glFinish) &glFinish;
	functions->flush = (CL_GLFunctions::ptr_glFlush) &glFlush;
	functions->fogf = (CL_GLFunctions::ptr_glFogf) &glFogf;
	functions->fogfv = (CL_GLFunctions::ptr_glFogfv) &glFogfv;
	functions->fogi = (CL_GLFunctions::ptr_glFogi) &glFogi;
	functions->fogiv = (CL_GLFunctions::ptr_glFogiv) &glFogiv;
	functions->frontFace = (CL_GLFunctions::ptr_glFrontFace) &glFrontFace;
	functions->frustum = (CL_GLFunctions::ptr_glFrustum) &glFrustum;
	functions->genLists = (CL_GLFunctions::ptr_glGenLists) &glGenLists;
	functions->genTextures = (CL_GLFunctions::ptr_glGenTextures) &glGenTextures;
	functions->getBooleanv = (CL_GLFunctions::ptr_glGetBooleanv) &glGetBooleanv;
	functions->getClipPlane = (CL_GLFunctions::ptr_glGetClipPlane) &glGetClipPlane;
	functions->getDoublev = (CL_GLFunctions::ptr_glGetDoublev) &glGetDoublev;
	functions->getError = (CL_GLFunctions::ptr_glGetError) &glGetError;
	functions->getFloatv = (CL_GLFunctions::ptr_glGetFloatv) &glGetFloatv;
	functions->getIntegerv = (CL_GLFunctions::ptr_glGetIntegerv) &glGetIntegerv;
	functions->getLightfv = (CL_GLFunctions::ptr_glGetLightfv) &glGetLightfv;
	functions->getLightiv = (CL_GLFunctions::ptr_glGetLightiv) &glGetLightiv;
	functions->getMapdv = (CL_GLFunctions::ptr_glGetMapdv) &glGetMapdv;
	functions->getMapfv = (CL_GLFunctions::ptr_glGetMapfv) &glGetMapfv;
	functions->getMapiv = (CL_GLFunctions::ptr_glGetMapiv) &glGetMapiv;
	functions->getMaterialfv = (CL_GLFunctions::ptr_glGetMaterialfv) &glGetMaterialfv;
	functions->getMaterialiv = (CL_GLFunctions::ptr_glGetMaterialiv) &glGetMaterialiv;
	functions->getPixelMapfv = (CL_GLFunctions::ptr_glGetPixelMapfv) &glGetPixelMapfv;
	functions->getPixelMapuiv = (CL_GLFunctions::ptr_glGetPixelMapuiv) &glGetPixelMapuiv;
	functions->getPixelMapusv = (CL_GLFunctions::ptr_glGetPixelMapusv) &glGetPixelMapusv;
	functions->getPointerv = (CL_GLFunctions::ptr_glGetPointerv) &glGetPointerv;
	functions->getPolygonStipple = (CL_GLFunctions::ptr_glGetPolygonStipple) &glGetPolygonStipple;
	functions->getString = (CL_GLFunctions::ptr_glGetString) &glGetString;
	functions->getTexEnvfv = (CL_GLFunctions::ptr_glGetTexEnvfv) &glGetTexEnvfv;
	functions->getTexEnviv = (CL_GLFunctions::ptr_glGetTexEnviv) &glGetTexEnviv;
	functions->getTexGendv = (CL_GLFunctions::ptr_glGetTexGendv) &glGetTexGendv;
	functions->getTexGenfv = (CL_GLFunctions::ptr_glGetTexGenfv) &glGetTexGenfv;
	functions->getTexGeniv = (CL_GLFunctions::ptr_glGetTexGeniv) &glGetTexGeniv;
	functions->getTexImage = (CL_GLFunctions::ptr_glGetTexImage) &glGetTexImage;
	functions->getTexLevelParameterfv = (CL_GLFunctions::ptr_glGetTexLevelParameterfv) &glGetTexLevelParameterfv;
	functions->getTexLevelParameteriv = (CL_GLFunctions::ptr_glGetTexLevelParameteriv) &glGetTexLevelParameteriv;
	functions->getTexParameterfv = (CL_GLFunctions::ptr_glGetTexParameterfv) &glGetTexParameterfv;
	functions->getTexParameteriv = (CL_GLFunctions::ptr_glGetTexParameteriv) &glGetTexParameteriv;
	functions->hint = (CL_GLFunctions::ptr_glHint) &glHint;
	functions->indexMask = (CL_GLFunctions::ptr_glIndexMask) &glIndexMask;
	functions->indexPointer = (CL_GLFunctions::ptr_glIndexPointer) &glIndexPointer;
	functions->indexd = (CL_GLFunctions::ptr_glIndexd) &glIndexd;
	functions->indexdv = (CL_GLFunctions::ptr_glIndexdv) &glIndexdv;
	functions->indexf = (CL_GLFunctions::ptr_glIndexf) &glIndexf;
	functions->indexfv = (CL_GLFunctions::ptr_glIndexfv) &glIndexfv;
	functions->indexi = (CL_GLFunctions::ptr_glIndexi) &glIndexi;
	functions->indexiv = (CL_GLFunctions::ptr_glIndexiv) &glIndexiv;
	functions->indexs = (CL_GLFunctions::ptr_glIndexs) &glIndexs;
	functions->indexsv = (CL_GLFunctions::ptr_glIndexsv) &glIndexsv;
	functions->indexub = (CL_GLFunctions::ptr_glIndexub) &glIndexub;
	functions->indexubv = (CL_GLFunctions::ptr_glIndexubv) &glIndexubv;
	functions->initNames = (CL_GLFunctions::ptr_glInitNames) &glInitNames;
	functions->interleavedArrays = (CL_GLFunctions::ptr_glInterleavedArrays) &glInterleavedArrays;
	functions->isEnabled = (CL_GLFunctions::ptr_glIsEnabled) &glIsEnabled;
	functions->isList = (CL_GLFunctions::ptr_glIsList) &glIsList;
	functions->isTexture = (CL_GLFunctions::ptr_glIsTexture) &glIsTexture;
	functions->lightModelf = (CL_GLFunctions::ptr_glLightModelf) &glLightModelf;
	functions->lightModelfv = (CL_GLFunctions::ptr_glLightModelfv) &glLightModelfv;
	functions->lightModeli = (CL_GLFunctions::ptr_glLightModeli) &glLightModeli;
	functions->lightModeliv = (CL_GLFunctions::ptr_glLightModeliv) &glLightModeliv;
	functions->lightf = (CL_GLFunctions::ptr_glLightf) &glLightf;
	functions->lightfv = (CL_GLFunctions::ptr_glLightfv) &glLightfv;
	functions->lighti = (CL_GLFunctions::ptr_glLighti) &glLighti;
	functions->lightiv = (CL_GLFunctions::ptr_glLightiv) &glLightiv;
	functions->lineStipple = (CL_GLFunctions::ptr_glLineStipple) &glLineStipple;
	functions->lineWidth = (CL_GLFunctions::ptr_glLineWidth) &glLineWidth;
	functions->listBase = (CL_GLFunctions::ptr_glListBase) &glListBase;
	functions->loadIdentity = (CL_GLFunctions::ptr_glLoadIdentity) &glLoadIdentity;
	functions->loadMatrixd = (CL_GLFunctions::ptr_glLoadMatrixd) &glLoadMatrixd;
	functions->loadMatrixf = (CL_GLFunctions::ptr_glLoadMatrixf) &glLoadMatrixf;
	functions->loadName = (CL_GLFunctions::ptr_glLoadName) &glLoadName;
	functions->logicOp = (CL_GLFunctions::ptr_glLogicOp) &glLogicOp;
	functions->map1d = (CL_GLFunctions::ptr_glMap1d) &glMap1d;
	functions->map1f = (CL_GLFunctions::ptr_glMap1f) &glMap1f;
	functions->map2d = (CL_GLFunctions::ptr_glMap2d) &glMap2d;
	functions->map2f = (CL_GLFunctions::ptr_glMap2f) &glMap2f;
	functions->mapGrid1d = (CL_GLFunctions::ptr_glMapGrid1d) &glMapGrid1d;
	functions->mapGrid1f = (CL_GLFunctions::ptr_glMapGrid1f) &glMapGrid1f;
	functions->mapGrid2d = (CL_GLFunctions::ptr_glMapGrid2d) &glMapGrid2d;
	functions->mapGrid2f = (CL_GLFunctions::ptr_glMapGrid2f) &glMapGrid2f;
	functions->materialf = (CL_GLFunctions::ptr_glMaterialf) &glMaterialf;
	functions->materialfv = (CL_GLFunctions::ptr_glMaterialfv) &glMaterialfv;
	functions->materiali = (CL_GLFunctions::ptr_glMateriali) &glMateriali;
	functions->materialiv = (CL_GLFunctions::ptr_glMaterialiv) &glMaterialiv;
	functions->matrixMode = (CL_GLFunctions::ptr_glMatrixMode) &glMatrixMode;
	functions->multMatrixd = (CL_GLFunctions::ptr_glMultMatrixd) &glMultMatrixd;
	functions->multMatrixf = (CL_GLFunctions::ptr_glMultMatrixf) &glMultMatrixf;
	functions->newList = (CL_GLFunctions::ptr_glNewList) &glNewList;
	functions->normal3b = (CL_GLFunctions::ptr_glNormal3b) &glNormal3b;
	functions->normal3bv = (CL_GLFunctions::ptr_glNormal3bv) &glNormal3bv;
	functions->normal3d = (CL_GLFunctions::ptr_glNormal3d) &glNormal3d;
	functions->normal3dv = (CL_GLFunctions::ptr_glNormal3dv) &glNormal3dv;
	functions->normal3f = (CL_GLFunctions::ptr_glNormal3f) &glNormal3f;
	functions->normal3fv = (CL_GLFunctions::ptr_glNormal3fv) &glNormal3fv;
	functions->normal3i = (CL_GLFunctions::ptr_glNormal3i) &glNormal3i;
	functions->normal3iv = (CL_GLFunctions::ptr_glNormal3iv) &glNormal3iv;
	functions->normal3s = (CL_GLFunctions::ptr_glNormal3s) &glNormal3s;
	functions->normal3sv = (CL_GLFunctions::ptr_glNormal3sv) &glNormal3sv;
	functions->normalPointer = (CL_GLFunctions::ptr_glNormalPointer) &glNormalPointer;
	functions->ortho = (CL_GLFunctions::ptr_glOrtho) &glOrtho;
	functions->passThrough = (CL_GLFunctions::ptr_glPassThrough) &glPassThrough;
	functions->pixelMapfv = (CL_GLFunctions::ptr_glPixelMapfv) &glPixelMapfv;
	functions->pixelMapuiv = (CL_GLFunctions::ptr_glPixelMapuiv) &glPixelMapuiv;
	functions->pixelMapusv = (CL_GLFunctions::ptr_glPixelMapusv) &glPixelMapusv;
	functions->pixelStoref = (CL_GLFunctions::ptr_glPixelStoref) &glPixelStoref;
	functions->pixelStorei = (CL_GLFunctions::ptr_glPixelStorei) &glPixelStorei;
	functions->pixelTransferf = (CL_GLFunctions::ptr_glPixelTransferf) &glPixelTransferf;
	functions->pixelTransferi = (CL_GLFunctions::ptr_glPixelTransferi) &glPixelTransferi;
	functions->pixelZoom = (CL_GLFunctions::ptr_glPixelZoom) &glPixelZoom;
	functions->pointSize = (CL_GLFunctions::ptr_glPointSize) &glPointSize;
	functions->polygonMode = (CL_GLFunctions::ptr_glPolygonMode) &glPolygonMode;
	functions->polygonOffset = (CL_GLFunctions::ptr_glPolygonOffset) &glPolygonOffset;
	functions->polygonStipple = (CL_GLFunctions::ptr_glPolygonStipple) &glPolygonStipple;
	functions->popAttrib = (CL_GLFunctions::ptr_glPopAttrib) &glPopAttrib;
	functions->popClientAttrib = (CL_GLFunctions::ptr_glPopClientAttrib) &glPopClientAttrib;
	functions->popMatrix = (CL_GLFunctions::ptr_glPopMatrix) &glPopMatrix;
	functions->popName = (CL_GLFunctions::ptr_glPopName) &glPopName;
	functions->prioritizeTextures = (CL_GLFunctions::ptr_glPrioritizeTextures) &glPrioritizeTextures;
	functions->pushAttrib = (CL_GLFunctions::ptr_glPushAttrib) &glPushAttrib;
	functions->pushClientAttrib = (CL_GLFunctions::ptr_glPushClientAttrib) &glPushClientAttrib;
	functions->pushMatrix = (CL_GLFunctions::ptr_glPushMatrix) &glPushMatrix;
	functions->pushName = (CL_GLFunctions::ptr_glPushName) &glPushName;
	functions->rasterPos2d = (CL_GLFunctions::ptr_glRasterPos2d) &glRasterPos2d;
	functions->rasterPos2dv = (CL_GLFunctions::ptr_glRasterPos2dv) &glRasterPos2dv;
	functions->rasterPos2f = (CL_GLFunctions::ptr_glRasterPos2f) &glRasterPos2f;
	functions->rasterPos2fv = (CL_GLFunctions::ptr_glRasterPos2fv) &glRasterPos2fv;
	functions->rasterPos2i = (CL_GLFunctions::ptr_glRasterPos2i) &glRasterPos2i;
	functions->rasterPos2iv = (CL_GLFunctions::ptr_glRasterPos2iv) &glRasterPos2iv;
	functions->rasterPos2s = (CL_GLFunctions::ptr_glRasterPos2s) &glRasterPos2s;
	functions->rasterPos2sv = (CL_GLFunctions::ptr_glRasterPos2sv) &glRasterPos2sv;
	functions->rasterPos3d = (CL_GLFunctions::ptr_glRasterPos3d) &glRasterPos3d;
	functions->rasterPos3dv = (CL_GLFunctions::ptr_glRasterPos3dv) &glRasterPos3dv;
	functions->rasterPos3f = (CL_GLFunctions::ptr_glRasterPos3f) &glRasterPos3f;
	functions->rasterPos3fv = (CL_GLFunctions::ptr_glRasterPos3fv) &glRasterPos3fv;
	functions->rasterPos3i = (CL_GLFunctions::ptr_glRasterPos3i) &glRasterPos3i;
	functions->rasterPos3iv = (CL_GLFunctions::ptr_glRasterPos3iv) &glRasterPos3iv;
	functions->rasterPos3s = (CL_GLFunctions::ptr_glRasterPos3s) &glRasterPos3s;
	functions->rasterPos3sv = (CL_GLFunctions::ptr_glRasterPos3sv) &glRasterPos3sv;
	functions->rasterPos4d = (CL_GLFunctions::ptr_glRasterPos4d) &glRasterPos4d;
	functions->rasterPos4dv = (CL_GLFunctions::ptr_glRasterPos4dv) &glRasterPos4dv;
	functions->rasterPos4f = (CL_GLFunctions::ptr_glRasterPos4f) &glRasterPos4f;
	functions->rasterPos4fv = (CL_GLFunctions::ptr_glRasterPos4fv) &glRasterPos4fv;
	functions->rasterPos4i = (CL_GLFunctions::ptr_glRasterPos4i) &glRasterPos4i;
	functions->rasterPos4iv = (CL_GLFunctions::ptr_glRasterPos4iv) &glRasterPos4iv;
	functions->rasterPos4s = (CL_GLFunctions::ptr_glRasterPos4s) &glRasterPos4s;
	functions->rasterPos4sv = (CL_GLFunctions::ptr_glRasterPos4sv) &glRasterPos4sv;
	functions->readBuffer = (CL_GLFunctions::ptr_glReadBuffer) &glReadBuffer;
	functions->readPixels = (CL_GLFunctions::ptr_glReadPixels) &glReadPixels;
	functions->rectd = (CL_GLFunctions::ptr_glRectd) &glRectd;
	functions->rectdv = (CL_GLFunctions::ptr_glRectdv) &glRectdv;
	functions->rectf = (CL_GLFunctions::ptr_glRectf) &glRectf;
	functions->rectfv = (CL_GLFunctions::ptr_glRectfv) &glRectfv;
	functions->recti = (CL_GLFunctions::ptr_glRecti) &glRecti;
	functions->rectiv = (CL_GLFunctions::ptr_glRectiv) &glRectiv;
	functions->rects = (CL_GLFunctions::ptr_glRects) &glRects;
	functions->rectsv = (CL_GLFunctions::ptr_glRectsv) &glRectsv;
	functions->renderMode = (CL_GLFunctions::ptr_glRenderMode) &glRenderMode;
	functions->rotated = (CL_GLFunctions::ptr_glRotated) &glRotated;
	functions->rotatef = (CL_GLFunctions::ptr_glRotatef) &glRotatef;
	functions->scaled = (CL_GLFunctions::ptr_glScaled) &glScaled;
	functions->scalef = (CL_GLFunctions::ptr_glScalef) &glScalef;
	functions->scissor = (CL_GLFunctions::ptr_glScissor) &glScissor;
	functions->selectBuffer = (CL_GLFunctions::ptr_glSelectBuffer) &glSelectBuffer;
	functions->shadeModel = (CL_GLFunctions::ptr_glShadeModel) &glShadeModel;
	functions->stencilFunc = (CL_GLFunctions::ptr_glStencilFunc) &glStencilFunc;
	functions->stencilMask = (CL_GLFunctions::ptr_glStencilMask) &glStencilMask;
	functions->stencilOp = (CL_GLFunctions::ptr_glStencilOp) &glStencilOp;
	functions->texCoord1d = (CL_GLFunctions::ptr_glTexCoord1d) &glTexCoord1d;
	functions->texCoord1dv = (CL_GLFunctions::ptr_glTexCoord1dv) &glTexCoord1dv;
	functions->texCoord1f = (CL_GLFunctions::ptr_glTexCoord1f) &glTexCoord1f;
	functions->texCoord1fv = (CL_GLFunctions::ptr_glTexCoord1fv) &glTexCoord1fv;
	functions->texCoord1i = (CL_GLFunctions::ptr_glTexCoord1i) &glTexCoord1i;
	functions->texCoord1iv = (CL_GLFunctions::ptr_glTexCoord1iv) &glTexCoord1iv;
	functions->texCoord1s = (CL_GLFunctions::ptr_glTexCoord1s) &glTexCoord1s;
	functions->texCoord1sv = (CL_GLFunctions::ptr_glTexCoord1sv) &glTexCoord1sv;
	functions->texCoord2d = (CL_GLFunctions::ptr_glTexCoord2d) &glTexCoord2d;
	functions->texCoord2dv = (CL_GLFunctions::ptr_glTexCoord2dv) &glTexCoord2dv;
	functions->texCoord2f = (CL_GLFunctions::ptr_glTexCoord2f) &glTexCoord2f;
	functions->texCoord2fv = (CL_GLFunctions::ptr_glTexCoord2fv) &glTexCoord2fv;
	functions->texCoord2i = (CL_GLFunctions::ptr_glTexCoord2i) &glTexCoord2i;
	functions->texCoord2iv = (CL_GLFunctions::ptr_glTexCoord2iv) &glTexCoord2iv;
	functions->texCoord2s = (CL_GLFunctions::ptr_glTexCoord2s) &glTexCoord2s;
	functions->texCoord2sv = (CL_GLFunctions::ptr_glTexCoord2sv) &glTexCoord2sv;
	functions->texCoord3d = (CL_GLFunctions::ptr_glTexCoord3d) &glTexCoord3d;
	functions->texCoord3dv = (CL_GLFunctions::ptr_glTexCoord3dv) &glTexCoord3dv;
	functions->texCoord3f = (CL_GLFunctions::ptr_glTexCoord3f) &glTexCoord3f;
	functions->texCoord3fv = (CL_GLFunctions::ptr_glTexCoord3fv) &glTexCoord3fv;
	functions->texCoord3i = (CL_GLFunctions::ptr_glTexCoord3i) &glTexCoord3i;
	functions->texCoord3iv = (CL_GLFunctions::ptr_glTexCoord3iv) &glTexCoord3iv;
	functions->texCoord3s = (CL_GLFunctions::ptr_glTexCoord3s) &glTexCoord3s;
	functions->texCoord3sv = (CL_GLFunctions::ptr_glTexCoord3sv) &glTexCoord3sv;
	functions->texCoord4d = (CL_GLFunctions::ptr_glTexCoord4d) &glTexCoord4d;
	functions->texCoord4dv = (CL_GLFunctions::ptr_glTexCoord4dv) &glTexCoord4dv;
	functions->texCoord4f = (CL_GLFunctions::ptr_glTexCoord4f) &glTexCoord4f;
	functions->texCoord4fv = (CL_GLFunctions::ptr_glTexCoord4fv) &glTexCoord4fv;
	functions->texCoord4i = (CL_GLFunctions::ptr_glTexCoord4i) &glTexCoord4i;
	functions->texCoord4iv = (CL_GLFunctions::ptr_glTexCoord4iv) &glTexCoord4iv;
	functions->texCoord4s = (CL_GLFunctions::ptr_glTexCoord4s) &glTexCoord4s;
	functions->texCoord4sv = (CL_GLFunctions::ptr_glTexCoord4sv) &glTexCoord4sv;
	functions->texCoordPointer = (CL_GLFunctions::ptr_glTexCoordPointer) &glTexCoordPointer;
	functions->texEnvf = (CL_GLFunctions::ptr_glTexEnvf) &glTexEnvf;
	functions->texEnvfv = (CL_GLFunctions::ptr_glTexEnvfv) &glTexEnvfv;
	functions->texEnvi = (CL_GLFunctions::ptr_glTexEnvi) &glTexEnvi;
	functions->texEnviv = (CL_GLFunctions::ptr_glTexEnviv) &glTexEnviv;
	functions->texGend = (CL_GLFunctions::ptr_glTexGend) &glTexGend;
	functions->texGendv = (CL_GLFunctions::ptr_glTexGendv) &glTexGendv;
	functions->texGenf = (CL_GLFunctions::ptr_glTexGenf) &glTexGenf;
	functions->texGenfv = (CL_GLFunctions::ptr_glTexGenfv) &glTexGenfv;
	functions->texGeni = (CL_GLFunctions::ptr_glTexGeni) &glTexGeni;
	functions->texGeniv = (CL_GLFunctions::ptr_glTexGeniv) &glTexGeniv;
	functions->texImage1D = (CL_GLFunctions::ptr_glTexImage1D) &glTexImage1D;
	functions->texImage2D = (CL_GLFunctions::ptr_glTexImage2D) &glTexImage2D;
	functions->texParameterf = (CL_GLFunctions::ptr_glTexParameterf) &glTexParameterf;
	functions->texParameterfv = (CL_GLFunctions::ptr_glTexParameterfv) &glTexParameterfv;
	functions->texParameteri = (CL_GLFunctions::ptr_glTexParameteri) &glTexParameteri;
	functions->texParameteriv = (CL_GLFunctions::ptr_glTexParameteriv) &glTexParameteriv;
	functions->texSubImage1D = (CL_GLFunctions::ptr_glTexSubImage1D) &glTexSubImage1D;
	functions->texSubImage2D = (CL_GLFunctions::ptr_glTexSubImage2D) &glTexSubImage2D;
	functions->translated = (CL_GLFunctions::ptr_glTranslated) &glTranslated;
	functions->translatef = (CL_GLFunctions::ptr_glTranslatef) &glTranslatef;
	functions->vertex2d = (CL_GLFunctions::ptr_glVertex2d) &glVertex2d;
	functions->vertex2dv = (CL_GLFunctions::ptr_glVertex2dv) &glVertex2dv;
	functions->vertex2f = (CL_GLFunctions::ptr_glVertex2f) &glVertex2f;
	functions->vertex2fv = (CL_GLFunctions::ptr_glVertex2fv) &glVertex2fv;
	functions->vertex2i = (CL_GLFunctions::ptr_glVertex2i) &glVertex2i;
	functions->vertex2iv = (CL_GLFunctions::ptr_glVertex2iv) &glVertex2iv;
	functions->vertex2s = (CL_GLFunctions::ptr_glVertex2s) &glVertex2s;
	functions->vertex2sv = (CL_GLFunctions::ptr_glVertex2sv) &glVertex2sv;
	functions->vertex3d = (CL_GLFunctions::ptr_glVertex3d) &glVertex3d;
	functions->vertex3dv = (CL_GLFunctions::ptr_glVertex3dv) &glVertex3dv;
	functions->vertex3f = (CL_GLFunctions::ptr_glVertex3f) &glVertex3f;
	functions->vertex3fv = (CL_GLFunctions::ptr_glVertex3fv) &glVertex3fv;
	functions->vertex3i = (CL_GLFunctions::ptr_glVertex3i) &glVertex3i;
	functions->vertex3iv = (CL_GLFunctions::ptr_glVertex3iv) &glVertex3iv;
	functions->vertex3s = (CL_GLFunctions::ptr_glVertex3s) &glVertex3s;
	functions->vertex3sv = (CL_GLFunctions::ptr_glVertex3sv) &glVertex3sv;
	functions->vertex4d = (CL_GLFunctions::ptr_glVertex4d) &glVertex4d;
	functions->vertex4dv = (CL_GLFunctions::ptr_glVertex4dv) &glVertex4dv;
	functions->vertex4f = (CL_GLFunctions::ptr_glVertex4f) &glVertex4f;
	functions->vertex4fv = (CL_GLFunctions::ptr_glVertex4fv) &glVertex4fv;
	functions->vertex4i = (CL_GLFunctions::ptr_glVertex4i) &glVertex4i;
	functions->vertex4iv = (CL_GLFunctions::ptr_glVertex4iv) &glVertex4iv;
	functions->vertex4s = (CL_GLFunctions::ptr_glVertex4s) &glVertex4s;
	functions->vertex4sv = (CL_GLFunctions::ptr_glVertex4sv) &glVertex4sv;
	functions->vertexPointer = (CL_GLFunctions::ptr_glVertexPointer) &glVertexPointer;
	functions->viewport = (CL_GLFunctions::ptr_glViewport) &glViewport;
#else
	functions->accum = (CL_GLFunctions::ptr_glAccum) CL_OpenGL::get_proc_address("glAccum");
	functions->alphaFunc = (CL_GLFunctions::ptr_glAlphaFunc) CL_OpenGL::get_proc_address("glAlphaFunc");
	functions->areTexturesResident = (CL_GLFunctions::ptr_glAreTexturesResident) CL_OpenGL::get_proc_address("glAreTexturesResident");
	functions->arrayElement = (CL_GLFunctions::ptr_glArrayElement) CL_OpenGL::get_proc_address("glArrayElement");
	functions->begin = (CL_GLFunctions::ptr_glBegin) CL_OpenGL::get_proc_address("glBegin");
	functions->bindTexture = (CL_GLFunctions::ptr_glBindTexture) CL_OpenGL::get_proc_address("glBindTexture");
	functions->bitmap = (CL_GLFunctions::ptr_glBitmap) CL_OpenGL::get_proc_address("glBitmap");
	functions->blendFunc = (CL_GLFunctions::ptr_glBlendFunc) CL_OpenGL::get_proc_address("glBlendFunc");
	functions->callList = (CL_GLFunctions::ptr_glCallList) CL_OpenGL::get_proc_address("glCallList");
	functions->callLists = (CL_GLFunctions::ptr_glCallLists) CL_OpenGL::get_proc_address("glCallLists");
	functions->clear = (CL_GLFunctions::ptr_glClear) CL_OpenGL::get_proc_address("glClear");
	functions->clearAccum = (CL_GLFunctions::ptr_glClearAccum) CL_OpenGL::get_proc_address("glClearAccum");
	functions->clearColor = (CL_GLFunctions::ptr_glClearColor) CL_OpenGL::get_proc_address("glClearColor");
	functions->clearDepth = (CL_GLFunctions::ptr_glClearDepth) CL_OpenGL::get_proc_address("glClearDepth");
	functions->clearIndex = (CL_GLFunctions::ptr_glClearIndex) CL_OpenGL::get_proc_address("glClearIndex");
	functions->clearStencil = (CL_GLFunctions::ptr_glClearStencil) CL_OpenGL::get_proc_address("glClearStencil");
	functions->clipPlane = (CL_GLFunctions::ptr_glClipPlane) CL_OpenGL::get_proc_address("glClipPlane");
	functions->color3b = (CL_GLFunctions::ptr_glColor3b) CL_OpenGL::get_proc_address("glColor3b");
	functions->color3bv = (CL_GLFunctions::ptr_glColor3bv) CL_OpenGL::get_proc_address("glColor3bv");
	functions->color3d = (CL_GLFunctions::ptr_glColor3d) CL_OpenGL::get_proc_address("glColor3d");
	functions->color3dv = (CL_GLFunctions::ptr_glColor3dv) CL_OpenGL::get_proc_address("glColor3dv");
	functions->color3f = (CL_GLFunctions::ptr_glColor3f) CL_OpenGL::get_proc_address("glColor3f");
	functions->color3fv = (CL_GLFunctions::ptr_glColor3fv) CL_OpenGL::get_proc_address("glColor3fv");
	functions->color3i = (CL_GLFunctions::ptr_glColor3i) CL_OpenGL::get_proc_address("glColor3i");
	functions->color3iv = (CL_GLFunctions::ptr_glColor3iv) CL_OpenGL::get_proc_address("glColor3iv");
	functions->color3s = (CL_GLFunctions::ptr_glColor3s) CL_OpenGL::get_proc_address("glColor3s");
	functions->color3sv = (CL_GLFunctions::ptr_glColor3sv) CL_OpenGL::get_proc_address("glColor3sv");
	functions->color3ub = (CL_GLFunctions::ptr_glColor3ub) CL_OpenGL::get_proc_address("glColor3ub");
	functions->color3ubv = (CL_GLFunctions::ptr_glColor3ubv) CL_OpenGL::get_proc_address("glColor3ubv");
	functions->color3ui = (CL_GLFunctions::ptr_glColor3ui) CL_OpenGL::get_proc_address("glColor3ui");
	functions->color3uiv = (CL_GLFunctions::ptr_glColor3uiv) CL_OpenGL::get_proc_address("glColor3uiv");
	functions->color3us = (CL_GLFunctions::ptr_glColor3us) CL_OpenGL::get_proc_address("glColor3us");
	functions->color3usv = (CL_GLFunctions::ptr_glColor3usv) CL_OpenGL::get_proc_address("glColor3usv");
	functions->color4b = (CL_GLFunctions::ptr_glColor4b) CL_OpenGL::get_proc_address("glColor4b");
	functions->color4bv = (CL_GLFunctions::ptr_glColor4bv) CL_OpenGL::get_proc_address("glColor4bv");
	functions->color4d = (CL_GLFunctions::ptr_glColor4d) CL_OpenGL::get_proc_address("glColor4d");
	functions->color4dv = (CL_GLFunctions::ptr_glColor4dv) CL_OpenGL::get_proc_address("glColor4dv");
	functions->color4f = (CL_GLFunctions::ptr_glColor4f) CL_OpenGL::get_proc_address("glColor4f");
	functions->color4fv = (CL_GLFunctions::ptr_glColor4fv) CL_OpenGL::get_proc_address("glColor4fv");
	functions->color4i = (CL_GLFunctions::ptr_glColor4i) CL_OpenGL::get_proc_address("glColor4i");
	functions->color4iv = (CL_GLFunctions::ptr_glColor4iv) CL_OpenGL::get_proc_address("glColor4iv");
	functions->color4s = (CL_GLFunctions::ptr_glColor4s) CL_OpenGL::get_proc_address("glColor4s");
	functions->color4sv = (CL_GLFunctions::ptr_glColor4sv) CL_OpenGL::get_proc_address("glColor4sv");
	functions->color4ub = (CL_GLFunctions::ptr_glColor4ub) CL_OpenGL::get_proc_address("glColor4ub");
	functions->color4ubv = (CL_GLFunctions::ptr_glColor4ubv) CL_OpenGL::get_proc_address("glColor4ubv");
	functions->color4ui = (CL_GLFunctions::ptr_glColor4ui) CL_OpenGL::get_proc_address("glColor4ui");
	functions->color4uiv = (CL_GLFunctions::ptr_glColor4uiv) CL_OpenGL::get_proc_address("glColor4uiv");
	functions->color4us = (CL_GLFunctions::ptr_glColor4us) CL_OpenGL::get_proc_address("glColor4us");
	functions->color4usv = (CL_GLFunctions::ptr_glColor4usv) CL_OpenGL::get_proc_address("glColor4usv");
	functions->colorMask = (CL_GLFunctions::ptr_glColorMask) CL_OpenGL::get_proc_address("glColorMask");
	functions->colorMaterial = (CL_GLFunctions::ptr_glColorMaterial) CL_OpenGL::get_proc_address("glColorMaterial");
	functions->colorPointer = (CL_GLFunctions::ptr_glColorPointer) CL_OpenGL::get_proc_address("glColorPointer");
	functions->copyPixels = (CL_GLFunctions::ptr_glCopyPixels) CL_OpenGL::get_proc_address("glCopyPixels");
	functions->copyTexImage1D = (CL_GLFunctions::ptr_glCopyTexImage1D) CL_OpenGL::get_proc_address("glCopyTexImage1D");
	functions->copyTexImage2D = (CL_GLFunctions::ptr_glCopyTexImage2D) CL_OpenGL::get_proc_address("glCopyTexImage2D");
	functions->copyTexSubImage1D = (CL_GLFunctions::ptr_glCopyTexSubImage1D) CL_OpenGL::get_proc_address("glCopyTexSubImage1D");
	functions->copyTexSubImage2D = (CL_GLFunctions::ptr_glCopyTexSubImage2D) CL_OpenGL::get_proc_address("glCopyTexSubImage2D");
	functions->cullFace = (CL_GLFunctions::ptr_glCullFace) CL_OpenGL::get_proc_address("glCullFace");
	functions->deleteLists = (CL_GLFunctions::ptr_glDeleteLists) CL_OpenGL::get_proc_address("glDeleteLists");
	functions->deleteTextures = (CL_GLFunctions::ptr_glDeleteTextures) CL_OpenGL::get_proc_address("glDeleteTextures");
	functions->depthFunc = (CL_GLFunctions::ptr_glDepthFunc) CL_OpenGL::get_proc_address("glDepthFunc");
	functions->depthMask = (CL_GLFunctions::ptr_glDepthMask) CL_OpenGL::get_proc_address("glDepthMask");
	functions->depthRange = (CL_GLFunctions::ptr_glDepthRange) CL_OpenGL::get_proc_address("glDepthRange");
	functions->disable = (CL_GLFunctions::ptr_glDisable) CL_OpenGL::get_proc_address("glDisable");
	functions->disableClientState = (CL_GLFunctions::ptr_glDisableClientState) CL_OpenGL::get_proc_address("glDisableClientState");
	functions->drawArrays = (CL_GLFunctions::ptr_glDrawArrays) CL_OpenGL::get_proc_address("glDrawArrays");
	functions->drawBuffer = (CL_GLFunctions::ptr_glDrawBuffer) CL_OpenGL::get_proc_address("glDrawBuffer");
	functions->drawElements = (CL_GLFunctions::ptr_glDrawElements) CL_OpenGL::get_proc_address("glDrawElements");
	functions->drawPixels = (CL_GLFunctions::ptr_glDrawPixels) CL_OpenGL::get_proc_address("glDrawPixels");
	functions->edgeFlag = (CL_GLFunctions::ptr_glEdgeFlag) CL_OpenGL::get_proc_address("glEdgeFlag");
	functions->edgeFlagPointer = (CL_GLFunctions::ptr_glEdgeFlagPointer) CL_OpenGL::get_proc_address("glEdgeFlagPointer");
	functions->edgeFlagv = (CL_GLFunctions::ptr_glEdgeFlagv) CL_OpenGL::get_proc_address("glEdgeFlagv");
	functions->enable = (CL_GLFunctions::ptr_glEnable) CL_OpenGL::get_proc_address("glEnable");
	functions->enableClientState = (CL_GLFunctions::ptr_glEnableClientState) CL_OpenGL::get_proc_address("glEnableClientState");
	functions->end = (CL_GLFunctions::ptr_glEnd) CL_OpenGL::get_proc_address("glEnd");
	functions->endList = (CL_GLFunctions::ptr_glEndList) CL_OpenGL::get_proc_address("glEndList");
	functions->evalCoord1d = (CL_GLFunctions::ptr_glEvalCoord1d) CL_OpenGL::get_proc_address("glEvalCoord1d");
	functions->evalCoord1dv = (CL_GLFunctions::ptr_glEvalCoord1dv) CL_OpenGL::get_proc_address("glEvalCoord1dv");
	functions->evalCoord1f = (CL_GLFunctions::ptr_glEvalCoord1f) CL_OpenGL::get_proc_address("glEvalCoord1f");
	functions->evalCoord1fv = (CL_GLFunctions::ptr_glEvalCoord1fv) CL_OpenGL::get_proc_address("glEvalCoord1fv");
	functions->evalCoord2d = (CL_GLFunctions::ptr_glEvalCoord2d) CL_OpenGL::get_proc_address("glEvalCoord2d");
	functions->evalCoord2dv = (CL_GLFunctions::ptr_glEvalCoord2dv) CL_OpenGL::get_proc_address("glEvalCoord2dv");
	functions->evalCoord2f = (CL_GLFunctions::ptr_glEvalCoord2f) CL_OpenGL::get_proc_address("glEvalCoord2f");
	functions->evalCoord2fv = (CL_GLFunctions::ptr_glEvalCoord2fv) CL_OpenGL::get_proc_address("glEvalCoord2fv");
	functions->evalMesh1 = (CL_GLFunctions::ptr_glEvalMesh1) CL_OpenGL::get_proc_address("glEvalMesh1");
	functions->evalMesh2 = (CL_GLFunctions::ptr_glEvalMesh2) CL_OpenGL::get_proc_address("glEvalMesh2");
	functions->evalPoint1 = (CL_GLFunctions::ptr_glEvalPoint1) CL_OpenGL::get_proc_address("glEvalPoint1");
	functions->evalPoint2 = (CL_GLFunctions::ptr_glEvalPoint2) CL_OpenGL::get_proc_address("glEvalPoint2");
	functions->feedbackBuffer = (CL_GLFunctions::ptr_glFeedbackBuffer) CL_OpenGL::get_proc_address("glFeedbackBuffer");
	functions->finish = (CL_GLFunctions::ptr_glFinish) CL_OpenGL::get_proc_address("glFinish");
	functions->flush = (CL_GLFunctions::ptr_glFlush) CL_OpenGL::get_proc_address("glFlush");
	functions->fogf = (CL_GLFunctions::ptr_glFogf) CL_OpenGL::get_proc_address("glFogf");
	functions->fogfv = (CL_GLFunctions::ptr_glFogfv) CL_OpenGL::get_proc_address("glFogfv");
	functions->fogi = (CL_GLFunctions::ptr_glFogi) CL_OpenGL::get_proc_address("glFogi");
	functions->fogiv = (CL_GLFunctions::ptr_glFogiv) CL_OpenGL::get_proc_address("glFogiv");
	functions->frontFace = (CL_GLFunctions::ptr_glFrontFace) CL_OpenGL::get_proc_address("glFrontFace");
	functions->frustum = (CL_GLFunctions::ptr_glFrustum) CL_OpenGL::get_proc_address("glFrustum");
	functions->genLists = (CL_GLFunctions::ptr_glGenLists) CL_OpenGL::get_proc_address("glGenLists");
	functions->genTextures = (CL_GLFunctions::ptr_glGenTextures) CL_OpenGL::get_proc_address("glGenTextures");
	functions->getBooleanv = (CL_GLFunctions::ptr_glGetBooleanv) CL_OpenGL::get_proc_address("glGetBooleanv");
	functions->getClipPlane = (CL_GLFunctions::ptr_glGetClipPlane) CL_OpenGL::get_proc_address("glGetClipPlane");
	functions->getDoublev = (CL_GLFunctions::ptr_glGetDoublev) CL_OpenGL::get_proc_address("glGetDoublev");
	functions->getError = (CL_GLFunctions::ptr_glGetError) CL_OpenGL::get_proc_address("glGetError");
	functions->getFloatv = (CL_GLFunctions::ptr_glGetFloatv) CL_OpenGL::get_proc_address("glGetFloatv");
	functions->getIntegerv = (CL_GLFunctions::ptr_glGetIntegerv) CL_OpenGL::get_proc_address("glGetIntegerv");
	functions->getLightfv = (CL_GLFunctions::ptr_glGetLightfv) CL_OpenGL::get_proc_address("glGetLightfv");
	functions->getLightiv = (CL_GLFunctions::ptr_glGetLightiv) CL_OpenGL::get_proc_address("glGetLightiv");
	functions->getMapdv = (CL_GLFunctions::ptr_glGetMapdv) CL_OpenGL::get_proc_address("glGetMapdv");
	functions->getMapfv = (CL_GLFunctions::ptr_glGetMapfv) CL_OpenGL::get_proc_address("glGetMapfv");
	functions->getMapiv = (CL_GLFunctions::ptr_glGetMapiv) CL_OpenGL::get_proc_address("glGetMapiv");
	functions->getMaterialfv = (CL_GLFunctions::ptr_glGetMaterialfv) CL_OpenGL::get_proc_address("glGetMaterialfv");
	functions->getMaterialiv = (CL_GLFunctions::ptr_glGetMaterialiv) CL_OpenGL::get_proc_address("glGetMaterialiv");
	functions->getPixelMapfv = (CL_GLFunctions::ptr_glGetPixelMapfv) CL_OpenGL::get_proc_address("glGetPixelMapfv");
	functions->getPixelMapuiv = (CL_GLFunctions::ptr_glGetPixelMapuiv) CL_OpenGL::get_proc_address("glGetPixelMapuiv");
	functions->getPixelMapusv = (CL_GLFunctions::ptr_glGetPixelMapusv) CL_OpenGL::get_proc_address("glGetPixelMapusv");
	functions->getPointerv = (CL_GLFunctions::ptr_glGetPointerv) CL_OpenGL::get_proc_address("glGetPointerv");
	functions->getPolygonStipple = (CL_GLFunctions::ptr_glGetPolygonStipple) CL_OpenGL::get_proc_address("glGetPolygonStipple");
	functions->getString = (CL_GLFunctions::ptr_glGetString) CL_OpenGL::get_proc_address("glGetString");
	functions->getTexEnvfv = (CL_GLFunctions::ptr_glGetTexEnvfv) CL_OpenGL::get_proc_address("glGetTexEnvfv");
	functions->getTexEnviv = (CL_GLFunctions::ptr_glGetTexEnviv) CL_OpenGL::get_proc_address("glGetTexEnviv");
	functions->getTexGendv = (CL_GLFunctions::ptr_glGetTexGendv) CL_OpenGL::get_proc_address("glGetTexGendv");
	functions->getTexGenfv = (CL_GLFunctions::ptr_glGetTexGenfv) CL_OpenGL::get_proc_address("glGetTexGenfv");
	functions->getTexGeniv = (CL_GLFunctions::ptr_glGetTexGeniv) CL_OpenGL::get_proc_address("glGetTexGeniv");
	functions->getTexImage = (CL_GLFunctions::ptr_glGetTexImage) CL_OpenGL::get_proc_address("glGetTexImage");
	functions->getTexLevelParameterfv = (CL_GLFunctions::ptr_glGetTexLevelParameterfv) CL_OpenGL::get_proc_address("glGetTexLevelParameterfv");
	functions->getTexLevelParameteriv = (CL_GLFunctions::ptr_glGetTexLevelParameteriv) CL_OpenGL::get_proc_address("glGetTexLevelParameteriv");
	functions->getTexParameterfv = (CL_GLFunctions::ptr_glGetTexParameterfv) CL_OpenGL::get_proc_address("glGetTexParameterfv");
	functions->getTexParameteriv = (CL_GLFunctions::ptr_glGetTexParameteriv) CL_OpenGL::get_proc_address("glGetTexParameteriv");
	functions->hint = (CL_GLFunctions::ptr_glHint) CL_OpenGL::get_proc_address("glHint");
	functions->indexMask = (CL_GLFunctions::ptr_glIndexMask) CL_OpenGL::get_proc_address("glIndexMask");
	functions->indexPointer = (CL_GLFunctions::ptr_glIndexPointer) CL_OpenGL::get_proc_address("glIndexPointer");
	functions->indexd = (CL_GLFunctions::ptr_glIndexd) CL_OpenGL::get_proc_address("glIndexd");
	functions->indexdv = (CL_GLFunctions::ptr_glIndexdv) CL_OpenGL::get_proc_address("glIndexdv");
	functions->indexf = (CL_GLFunctions::ptr_glIndexf) CL_OpenGL::get_proc_address("glIndexf");
	functions->indexfv = (CL_GLFunctions::ptr_glIndexfv) CL_OpenGL::get_proc_address("glIndexfv");
	functions->indexi = (CL_GLFunctions::ptr_glIndexi) CL_OpenGL::get_proc_address("glIndexi");
	functions->indexiv = (CL_GLFunctions::ptr_glIndexiv) CL_OpenGL::get_proc_address("glIndexiv");
	functions->indexs = (CL_GLFunctions::ptr_glIndexs) CL_OpenGL::get_proc_address("glIndexs");
	functions->indexsv = (CL_GLFunctions::ptr_glIndexsv) CL_OpenGL::get_proc_address("glIndexsv");
	functions->indexub = (CL_GLFunctions::ptr_glIndexub) CL_OpenGL::get_proc_address("glIndexub");
	functions->indexubv = (CL_GLFunctions::ptr_glIndexubv) CL_OpenGL::get_proc_address("glIndexubv");
	functions->initNames = (CL_GLFunctions::ptr_glInitNames) CL_OpenGL::get_proc_address("glInitNames");
	functions->interleavedArrays = (CL_GLFunctions::ptr_glInterleavedArrays) CL_OpenGL::get_proc_address("glInterleavedArrays");
	functions->isEnabled = (CL_GLFunctions::ptr_glIsEnabled) CL_OpenGL::get_proc_address("glIsEnabled");
	functions->isList = (CL_GLFunctions::ptr_glIsList) CL_OpenGL::get_proc_address("glIsList");
	functions->isTexture = (CL_GLFunctions::ptr_glIsTexture) CL_OpenGL::get_proc_address("glIsTexture");
	functions->lightModelf = (CL_GLFunctions::ptr_glLightModelf) CL_OpenGL::get_proc_address("glLightModelf");
	functions->lightModelfv = (CL_GLFunctions::ptr_glLightModelfv) CL_OpenGL::get_proc_address("glLightModelfv");
	functions->lightModeli = (CL_GLFunctions::ptr_glLightModeli) CL_OpenGL::get_proc_address("glLightModeli");
	functions->lightModeliv = (CL_GLFunctions::ptr_glLightModeliv) CL_OpenGL::get_proc_address("glLightModeliv");
	functions->lightf = (CL_GLFunctions::ptr_glLightf) CL_OpenGL::get_proc_address("glLightf");
	functions->lightfv = (CL_GLFunctions::ptr_glLightfv) CL_OpenGL::get_proc_address("glLightfv");
	functions->lighti = (CL_GLFunctions::ptr_glLighti) CL_OpenGL::get_proc_address("glLighti");
	functions->lightiv = (CL_GLFunctions::ptr_glLightiv) CL_OpenGL::get_proc_address("glLightiv");
	functions->lineStipple = (CL_GLFunctions::ptr_glLineStipple) CL_OpenGL::get_proc_address("glLineStipple");
	functions->lineWidth = (CL_GLFunctions::ptr_glLineWidth) CL_OpenGL::get_proc_address("glLineWidth");
	functions->listBase = (CL_GLFunctions::ptr_glListBase) CL_OpenGL::get_proc_address("glListBase");
	functions->loadIdentity = (CL_GLFunctions::ptr_glLoadIdentity) CL_OpenGL::get_proc_address("glLoadIdentity");
	functions->loadMatrixd = (CL_GLFunctions::ptr_glLoadMatrixd) CL_OpenGL::get_proc_address("glLoadMatrixd");
	functions->loadMatrixf = (CL_GLFunctions::ptr_glLoadMatrixf) CL_OpenGL::get_proc_address("glLoadMatrixf");
	functions->loadName = (CL_GLFunctions::ptr_glLoadName) CL_OpenGL::get_proc_address("glLoadName");
	functions->logicOp = (CL_GLFunctions::ptr_glLogicOp) CL_OpenGL::get_proc_address("glLogicOp");
	functions->map1d = (CL_GLFunctions::ptr_glMap1d) CL_OpenGL::get_proc_address("glMap1d");
	functions->map1f = (CL_GLFunctions::ptr_glMap1f) CL_OpenGL::get_proc_address("glMap1f");
	functions->map2d = (CL_GLFunctions::ptr_glMap2d) CL_OpenGL::get_proc_address("glMap2d");
	functions->map2f = (CL_GLFunctions::ptr_glMap2f) CL_OpenGL::get_proc_address("glMap2f");
	functions->mapGrid1d = (CL_GLFunctions::ptr_glMapGrid1d) CL_OpenGL::get_proc_address("glMapGrid1d");
	functions->mapGrid1f = (CL_GLFunctions::ptr_glMapGrid1f) CL_OpenGL::get_proc_address("glMapGrid1f");
	functions->mapGrid2d = (CL_GLFunctions::ptr_glMapGrid2d) CL_OpenGL::get_proc_address("glMapGrid2d");
	functions->mapGrid2f = (CL_GLFunctions::ptr_glMapGrid2f) CL_OpenGL::get_proc_address("glMapGrid2f");
	functions->materialf = (CL_GLFunctions::ptr_glMaterialf) CL_OpenGL::get_proc_address("glMaterialf");
	functions->materialfv = (CL_GLFunctions::ptr_glMaterialfv) CL_OpenGL::get_proc_address("glMaterialfv");
	functions->materiali = (CL_GLFunctions::ptr_glMateriali) CL_OpenGL::get_proc_address("glMateriali");
	functions->materialiv = (CL_GLFunctions::ptr_glMaterialiv) CL_OpenGL::get_proc_address("glMaterialiv");
	functions->matrixMode = (CL_GLFunctions::ptr_glMatrixMode) CL_OpenGL::get_proc_address("glMatrixMode");
	functions->multMatrixd = (CL_GLFunctions::ptr_glMultMatrixd) CL_OpenGL::get_proc_address("glMultMatrixd");
	functions->multMatrixf = (CL_GLFunctions::ptr_glMultMatrixf) CL_OpenGL::get_proc_address("glMultMatrixf");
	functions->newList = (CL_GLFunctions::ptr_glNewList) CL_OpenGL::get_proc_address("glNewList");
	functions->normal3b = (CL_GLFunctions::ptr_glNormal3b) CL_OpenGL::get_proc_address("glNormal3b");
	functions->normal3bv = (CL_GLFunctions::ptr_glNormal3bv) CL_OpenGL::get_proc_address("glNormal3bv");
	functions->normal3d = (CL_GLFunctions::ptr_glNormal3d) CL_OpenGL::get_proc_address("glNormal3d");
	functions->normal3dv = (CL_GLFunctions::ptr_glNormal3dv) CL_OpenGL::get_proc_address("glNormal3dv");
	functions->normal3f = (CL_GLFunctions::ptr_glNormal3f) CL_OpenGL::get_proc_address("glNormal3f");
	functions->normal3fv = (CL_GLFunctions::ptr_glNormal3fv) CL_OpenGL::get_proc_address("glNormal3fv");
	functions->normal3i = (CL_GLFunctions::ptr_glNormal3i) CL_OpenGL::get_proc_address("glNormal3i");
	functions->normal3iv = (CL_GLFunctions::ptr_glNormal3iv) CL_OpenGL::get_proc_address("glNormal3iv");
	functions->normal3s = (CL_GLFunctions::ptr_glNormal3s) CL_OpenGL::get_proc_address("glNormal3s");
	functions->normal3sv = (CL_GLFunctions::ptr_glNormal3sv) CL_OpenGL::get_proc_address("glNormal3sv");
	functions->normalPointer = (CL_GLFunctions::ptr_glNormalPointer) CL_OpenGL::get_proc_address("glNormalPointer");
	functions->ortho = (CL_GLFunctions::ptr_glOrtho) CL_OpenGL::get_proc_address("glOrtho");
	functions->passThrough = (CL_GLFunctions::ptr_glPassThrough) CL_OpenGL::get_proc_address("glPassThrough");
	functions->pixelMapfv = (CL_GLFunctions::ptr_glPixelMapfv) CL_OpenGL::get_proc_address("glPixelMapfv");
	functions->pixelMapuiv = (CL_GLFunctions::ptr_glPixelMapuiv) CL_OpenGL::get_proc_address("glPixelMapuiv");
	functions->pixelMapusv = (CL_GLFunctions::ptr_glPixelMapusv) CL_OpenGL::get_proc_address("glPixelMapusv");
	functions->pixelStoref = (CL_GLFunctions::ptr_glPixelStoref) CL_OpenGL::get_proc_address("glPixelStoref");
	functions->pixelStorei = (CL_GLFunctions::ptr_glPixelStorei) CL_OpenGL::get_proc_address("glPixelStorei");
	functions->pixelTransferf = (CL_GLFunctions::ptr_glPixelTransferf) CL_OpenGL::get_proc_address("glPixelTransferf");
	functions->pixelTransferi = (CL_GLFunctions::ptr_glPixelTransferi) CL_OpenGL::get_proc_address("glPixelTransferi");
	functions->pixelZoom = (CL_GLFunctions::ptr_glPixelZoom) CL_OpenGL::get_proc_address("glPixelZoom");
	functions->pointSize = (CL_GLFunctions::ptr_glPointSize) CL_OpenGL::get_proc_address("glPointSize");
	functions->polygonMode = (CL_GLFunctions::ptr_glPolygonMode) CL_OpenGL::get_proc_address("glPolygonMode");
	functions->polygonOffset = (CL_GLFunctions::ptr_glPolygonOffset) CL_OpenGL::get_proc_address("glPolygonOffset");
	functions->polygonStipple = (CL_GLFunctions::ptr_glPolygonStipple) CL_OpenGL::get_proc_address("glPolygonStipple");
	functions->popAttrib = (CL_GLFunctions::ptr_glPopAttrib) CL_OpenGL::get_proc_address("glPopAttrib");
	functions->popClientAttrib = (CL_GLFunctions::ptr_glPopClientAttrib) CL_OpenGL::get_proc_address("glPopClientAttrib");
	functions->popMatrix = (CL_GLFunctions::ptr_glPopMatrix) CL_OpenGL::get_proc_address("glPopMatrix");
	functions->popName = (CL_GLFunctions::ptr_glPopName) CL_OpenGL::get_proc_address("glPopName");
	functions->prioritizeTextures = (CL_GLFunctions::ptr_glPrioritizeTextures) CL_OpenGL::get_proc_address("glPrioritizeTextures");
	functions->pushAttrib = (CL_GLFunctions::ptr_glPushAttrib) CL_OpenGL::get_proc_address("glPushAttrib");
	functions->pushClientAttrib = (CL_GLFunctions::ptr_glPushClientAttrib) CL_OpenGL::get_proc_address("glPushClientAttrib");
	functions->pushMatrix = (CL_GLFunctions::ptr_glPushMatrix) CL_OpenGL::get_proc_address("glPushMatrix");
	functions->pushName = (CL_GLFunctions::ptr_glPushName) CL_OpenGL::get_proc_address("glPushName");
	functions->rasterPos2d = (CL_GLFunctions::ptr_glRasterPos2d) CL_OpenGL::get_proc_address("glRasterPos2d");
	functions->rasterPos2dv = (CL_GLFunctions::ptr_glRasterPos2dv) CL_OpenGL::get_proc_address("glRasterPos2dv");
	functions->rasterPos2f = (CL_GLFunctions::ptr_glRasterPos2f) CL_OpenGL::get_proc_address("glRasterPos2f");
	functions->rasterPos2fv = (CL_GLFunctions::ptr_glRasterPos2fv) CL_OpenGL::get_proc_address("glRasterPos2fv");
	functions->rasterPos2i = (CL_GLFunctions::ptr_glRasterPos2i) CL_OpenGL::get_proc_address("glRasterPos2i");
	functions->rasterPos2iv = (CL_GLFunctions::ptr_glRasterPos2iv) CL_OpenGL::get_proc_address("glRasterPos2iv");
	functions->rasterPos2s = (CL_GLFunctions::ptr_glRasterPos2s) CL_OpenGL::get_proc_address("glRasterPos2s");
	functions->rasterPos2sv = (CL_GLFunctions::ptr_glRasterPos2sv) CL_OpenGL::get_proc_address("glRasterPos2sv");
	functions->rasterPos3d = (CL_GLFunctions::ptr_glRasterPos3d) CL_OpenGL::get_proc_address("glRasterPos3d");
	functions->rasterPos3dv = (CL_GLFunctions::ptr_glRasterPos3dv) CL_OpenGL::get_proc_address("glRasterPos3dv");
	functions->rasterPos3f = (CL_GLFunctions::ptr_glRasterPos3f) CL_OpenGL::get_proc_address("glRasterPos3f");
	functions->rasterPos3fv = (CL_GLFunctions::ptr_glRasterPos3fv) CL_OpenGL::get_proc_address("glRasterPos3fv");
	functions->rasterPos3i = (CL_GLFunctions::ptr_glRasterPos3i) CL_OpenGL::get_proc_address("glRasterPos3i");
	functions->rasterPos3iv = (CL_GLFunctions::ptr_glRasterPos3iv) CL_OpenGL::get_proc_address("glRasterPos3iv");
	functions->rasterPos3s = (CL_GLFunctions::ptr_glRasterPos3s) CL_OpenGL::get_proc_address("glRasterPos3s");
	functions->rasterPos3sv = (CL_GLFunctions::ptr_glRasterPos3sv) CL_OpenGL::get_proc_address("glRasterPos3sv");
	functions->rasterPos4d = (CL_GLFunctions::ptr_glRasterPos4d) CL_OpenGL::get_proc_address("glRasterPos4d");
	functions->rasterPos4dv = (CL_GLFunctions::ptr_glRasterPos4dv) CL_OpenGL::get_proc_address("glRasterPos4dv");
	functions->rasterPos4f = (CL_GLFunctions::ptr_glRasterPos4f) CL_OpenGL::get_proc_address("glRasterPos4f");
	functions->rasterPos4fv = (CL_GLFunctions::ptr_glRasterPos4fv) CL_OpenGL::get_proc_address("glRasterPos4fv");
	functions->rasterPos4i = (CL_GLFunctions::ptr_glRasterPos4i) CL_OpenGL::get_proc_address("glRasterPos4i");
	functions->rasterPos4iv = (CL_GLFunctions::ptr_glRasterPos4iv) CL_OpenGL::get_proc_address("glRasterPos4iv");
	functions->rasterPos4s = (CL_GLFunctions::ptr_glRasterPos4s) CL_OpenGL::get_proc_address("glRasterPos4s");
	functions->rasterPos4sv = (CL_GLFunctions::ptr_glRasterPos4sv) CL_OpenGL::get_proc_address("glRasterPos4sv");
	functions->readBuffer = (CL_GLFunctions::ptr_glReadBuffer) CL_OpenGL::get_proc_address("glReadBuffer");
	functions->readPixels = (CL_GLFunctions::ptr_glReadPixels) CL_OpenGL::get_proc_address("glReadPixels");
	functions->rectd = (CL_GLFunctions::ptr_glRectd) CL_OpenGL::get_proc_address("glRectd");
	functions->rectdv = (CL_GLFunctions::ptr_glRectdv) CL_OpenGL::get_proc_address("glRectdv");
	functions->rectf = (CL_GLFunctions::ptr_glRectf) CL_OpenGL::get_proc_address("glRectf");
	functions->rectfv = (CL_GLFunctions::ptr_glRectfv) CL_OpenGL::get_proc_address("glRectfv");
	functions->recti = (CL_GLFunctions::ptr_glRecti) CL_OpenGL::get_proc_address("glRecti");
	functions->rectiv = (CL_GLFunctions::ptr_glRectiv) CL_OpenGL::get_proc_address("glRectiv");
	functions->rects = (CL_GLFunctions::ptr_glRects) CL_OpenGL::get_proc_address("glRects");
	functions->rectsv = (CL_GLFunctions::ptr_glRectsv) CL_OpenGL::get_proc_address("glRectsv");
	functions->renderMode = (CL_GLFunctions::ptr_glRenderMode) CL_OpenGL::get_proc_address("glRenderMode");
	functions->rotated = (CL_GLFunctions::ptr_glRotated) CL_OpenGL::get_proc_address("glRotated");
	functions->rotatef = (CL_GLFunctions::ptr_glRotatef) CL_OpenGL::get_proc_address("glRotatef");
	functions->scaled = (CL_GLFunctions::ptr_glScaled) CL_OpenGL::get_proc_address("glScaled");
	functions->scalef = (CL_GLFunctions::ptr_glScalef) CL_OpenGL::get_proc_address("glScalef");
	functions->scissor = (CL_GLFunctions::ptr_glScissor) CL_OpenGL::get_proc_address("glScissor");
	functions->selectBuffer = (CL_GLFunctions::ptr_glSelectBuffer) CL_OpenGL::get_proc_address("glSelectBuffer");
	functions->shadeModel = (CL_GLFunctions::ptr_glShadeModel) CL_OpenGL::get_proc_address("glShadeModel");
	functions->stencilFunc = (CL_GLFunctions::ptr_glStencilFunc) CL_OpenGL::get_proc_address("glStencilFunc");
	functions->stencilMask = (CL_GLFunctions::ptr_glStencilMask) CL_OpenGL::get_proc_address("glStencilMask");
	functions->stencilOp = (CL_GLFunctions::ptr_glStencilOp) CL_OpenGL::get_proc_address("glStencilOp");
	functions->texCoord1d = (CL_GLFunctions::ptr_glTexCoord1d) CL_OpenGL::get_proc_address("glTexCoord1d");
	functions->texCoord1dv = (CL_GLFunctions::ptr_glTexCoord1dv) CL_OpenGL::get_proc_address("glTexCoord1dv");
	functions->texCoord1f = (CL_GLFunctions::ptr_glTexCoord1f) CL_OpenGL::get_proc_address("glTexCoord1f");
	functions->texCoord1fv = (CL_GLFunctions::ptr_glTexCoord1fv) CL_OpenGL::get_proc_address("glTexCoord1fv");
	functions->texCoord1i = (CL_GLFunctions::ptr_glTexCoord1i) CL_OpenGL::get_proc_address("glTexCoord1i");
	functions->texCoord1iv = (CL_GLFunctions::ptr_glTexCoord1iv) CL_OpenGL::get_proc_address("glTexCoord1iv");
	functions->texCoord1s = (CL_GLFunctions::ptr_glTexCoord1s) CL_OpenGL::get_proc_address("glTexCoord1s");
	functions->texCoord1sv = (CL_GLFunctions::ptr_glTexCoord1sv) CL_OpenGL::get_proc_address("glTexCoord1sv");
	functions->texCoord2d = (CL_GLFunctions::ptr_glTexCoord2d) CL_OpenGL::get_proc_address("glTexCoord2d");
	functions->texCoord2dv = (CL_GLFunctions::ptr_glTexCoord2dv) CL_OpenGL::get_proc_address("glTexCoord2dv");
	functions->texCoord2f = (CL_GLFunctions::ptr_glTexCoord2f) CL_OpenGL::get_proc_address("glTexCoord2f");
	functions->texCoord2fv = (CL_GLFunctions::ptr_glTexCoord2fv) CL_OpenGL::get_proc_address("glTexCoord2fv");
	functions->texCoord2i = (CL_GLFunctions::ptr_glTexCoord2i) CL_OpenGL::get_proc_address("glTexCoord2i");
	functions->texCoord2iv = (CL_GLFunctions::ptr_glTexCoord2iv) CL_OpenGL::get_proc_address("glTexCoord2iv");
	functions->texCoord2s = (CL_GLFunctions::ptr_glTexCoord2s) CL_OpenGL::get_proc_address("glTexCoord2s");
	functions->texCoord2sv = (CL_GLFunctions::ptr_glTexCoord2sv) CL_OpenGL::get_proc_address("glTexCoord2sv");
	functions->texCoord3d = (CL_GLFunctions::ptr_glTexCoord3d) CL_OpenGL::get_proc_address("glTexCoord3d");
	functions->texCoord3dv = (CL_GLFunctions::ptr_glTexCoord3dv) CL_OpenGL::get_proc_address("glTexCoord3dv");
	functions->texCoord3f = (CL_GLFunctions::ptr_glTexCoord3f) CL_OpenGL::get_proc_address("glTexCoord3f");
	functions->texCoord3fv = (CL_GLFunctions::ptr_glTexCoord3fv) CL_OpenGL::get_proc_address("glTexCoord3fv");
	functions->texCoord3i = (CL_GLFunctions::ptr_glTexCoord3i) CL_OpenGL::get_proc_address("glTexCoord3i");
	functions->texCoord3iv = (CL_GLFunctions::ptr_glTexCoord3iv) CL_OpenGL::get_proc_address("glTexCoord3iv");
	functions->texCoord3s = (CL_GLFunctions::ptr_glTexCoord3s) CL_OpenGL::get_proc_address("glTexCoord3s");
	functions->texCoord3sv = (CL_GLFunctions::ptr_glTexCoord3sv) CL_OpenGL::get_proc_address("glTexCoord3sv");
	functions->texCoord4d = (CL_GLFunctions::ptr_glTexCoord4d) CL_OpenGL::get_proc_address("glTexCoord4d");
	functions->texCoord4dv = (CL_GLFunctions::ptr_glTexCoord4dv) CL_OpenGL::get_proc_address("glTexCoord4dv");
	functions->texCoord4f = (CL_GLFunctions::ptr_glTexCoord4f) CL_OpenGL::get_proc_address("glTexCoord4f");
	functions->texCoord4fv = (CL_GLFunctions::ptr_glTexCoord4fv) CL_OpenGL::get_proc_address("glTexCoord4fv");
	functions->texCoord4i = (CL_GLFunctions::ptr_glTexCoord4i) CL_OpenGL::get_proc_address("glTexCoord4i");
	functions->texCoord4iv = (CL_GLFunctions::ptr_glTexCoord4iv) CL_OpenGL::get_proc_address("glTexCoord4iv");
	functions->texCoord4s = (CL_GLFunctions::ptr_glTexCoord4s) CL_OpenGL::get_proc_address("glTexCoord4s");
	functions->texCoord4sv = (CL_GLFunctions::ptr_glTexCoord4sv) CL_OpenGL::get_proc_address("glTexCoord4sv");
	functions->texCoordPointer = (CL_GLFunctions::ptr_glTexCoordPointer) CL_OpenGL::get_proc_address("glTexCoordPointer");
	functions->texEnvf = (CL_GLFunctions::ptr_glTexEnvf) CL_OpenGL::get_proc_address("glTexEnvf");
	functions->texEnvfv = (CL_GLFunctions::ptr_glTexEnvfv) CL_OpenGL::get_proc_address("glTexEnvfv");
	functions->texEnvi = (CL_GLFunctions::ptr_glTexEnvi) CL_OpenGL::get_proc_address("glTexEnvi");
	functions->texEnviv = (CL_GLFunctions::ptr_glTexEnviv) CL_OpenGL::get_proc_address("glTexEnviv");
	functions->texGend = (CL_GLFunctions::ptr_glTexGend) CL_OpenGL::get_proc_address("glTexGend");
	functions->texGendv = (CL_GLFunctions::ptr_glTexGendv) CL_OpenGL::get_proc_address("glTexGendv");
	functions->texGenf = (CL_GLFunctions::ptr_glTexGenf) CL_OpenGL::get_proc_address("glTexGenf");
	functions->texGenfv = (CL_GLFunctions::ptr_glTexGenfv) CL_OpenGL::get_proc_address("glTexGenfv");
	functions->texGeni = (CL_GLFunctions::ptr_glTexGeni) CL_OpenGL::get_proc_address("glTexGeni");
	functions->texGeniv = (CL_GLFunctions::ptr_glTexGeniv) CL_OpenGL::get_proc_address("glTexGeniv");
	functions->texImage1D = (CL_GLFunctions::ptr_glTexImage1D) CL_OpenGL::get_proc_address("glTexImage1D");
	functions->texImage2D = (CL_GLFunctions::ptr_glTexImage2D) CL_OpenGL::get_proc_address("glTexImage2D");
	functions->texParameterf = (CL_GLFunctions::ptr_glTexParameterf) CL_OpenGL::get_proc_address("glTexParameterf");
	functions->texParameterfv = (CL_GLFunctions::ptr_glTexParameterfv) CL_OpenGL::get_proc_address("glTexParameterfv");
	functions->texParameteri = (CL_GLFunctions::ptr_glTexParameteri) CL_OpenGL::get_proc_address("glTexParameteri");
	functions->texParameteriv = (CL_GLFunctions::ptr_glTexParameteriv) CL_OpenGL::get_proc_address("glTexParameteriv");
	functions->texSubImage1D = (CL_GLFunctions::ptr_glTexSubImage1D) CL_OpenGL::get_proc_address("glTexSubImage1D");
	functions->texSubImage2D = (CL_GLFunctions::ptr_glTexSubImage2D) CL_OpenGL::get_proc_address("glTexSubImage2D");
	functions->translated = (CL_GLFunctions::ptr_glTranslated) CL_OpenGL::get_proc_address("glTranslated");
	functions->translatef = (CL_GLFunctions::ptr_glTranslatef) CL_OpenGL::get_proc_address("glTranslatef");
	functions->vertex2d = (CL_GLFunctions::ptr_glVertex2d) CL_OpenGL::get_proc_address("glVertex2d");
	functions->vertex2dv = (CL_GLFunctions::ptr_glVertex2dv) CL_OpenGL::get_proc_address("glVertex2dv");
	functions->vertex2f = (CL_GLFunctions::ptr_glVertex2f) CL_OpenGL::get_proc_address("glVertex2f");
	functions->vertex2fv = (CL_GLFunctions::ptr_glVertex2fv) CL_OpenGL::get_proc_address("glVertex2fv");
	functions->vertex2i = (CL_GLFunctions::ptr_glVertex2i) CL_OpenGL::get_proc_address("glVertex2i");
	functions->vertex2iv = (CL_GLFunctions::ptr_glVertex2iv) CL_OpenGL::get_proc_address("glVertex2iv");
	functions->vertex2s = (CL_GLFunctions::ptr_glVertex2s) CL_OpenGL::get_proc_address("glVertex2s");
	functions->vertex2sv = (CL_GLFunctions::ptr_glVertex2sv) CL_OpenGL::get_proc_address("glVertex2sv");
	functions->vertex3d = (CL_GLFunctions::ptr_glVertex3d) CL_OpenGL::get_proc_address("glVertex3d");
	functions->vertex3dv = (CL_GLFunctions::ptr_glVertex3dv) CL_OpenGL::get_proc_address("glVertex3dv");
	functions->vertex3f = (CL_GLFunctions::ptr_glVertex3f) CL_OpenGL::get_proc_address("glVertex3f");
	functions->vertex3fv = (CL_GLFunctions::ptr_glVertex3fv) CL_OpenGL::get_proc_address("glVertex3fv");
	functions->vertex3i = (CL_GLFunctions::ptr_glVertex3i) CL_OpenGL::get_proc_address("glVertex3i");
	functions->vertex3iv = (CL_GLFunctions::ptr_glVertex3iv) CL_OpenGL::get_proc_address("glVertex3iv");
	functions->vertex3s = (CL_GLFunctions::ptr_glVertex3s) CL_OpenGL::get_proc_address("glVertex3s");
	functions->vertex3sv = (CL_GLFunctions::ptr_glVertex3sv) CL_OpenGL::get_proc_address("glVertex3sv");
	functions->vertex4d = (CL_GLFunctions::ptr_glVertex4d) CL_OpenGL::get_proc_address("glVertex4d");
	functions->vertex4dv = (CL_GLFunctions::ptr_glVertex4dv) CL_OpenGL::get_proc_address("glVertex4dv");
	functions->vertex4f = (CL_GLFunctions::ptr_glVertex4f) CL_OpenGL::get_proc_address("glVertex4f");
	functions->vertex4fv = (CL_GLFunctions::ptr_glVertex4fv) CL_OpenGL::get_proc_address("glVertex4fv");
	functions->vertex4i = (CL_GLFunctions::ptr_glVertex4i) CL_OpenGL::get_proc_address("glVertex4i");
	functions->vertex4iv = (CL_GLFunctions::ptr_glVertex4iv) CL_OpenGL::get_proc_address("glVertex4iv");
	functions->vertex4s = (CL_GLFunctions::ptr_glVertex4s) CL_OpenGL::get_proc_address("glVertex4s");
	functions->vertex4sv = (CL_GLFunctions::ptr_glVertex4sv) CL_OpenGL::get_proc_address("glVertex4sv");
	functions->vertexPointer = (CL_GLFunctions::ptr_glVertexPointer) CL_OpenGL::get_proc_address("glVertexPointer");
	functions->viewport = (CL_GLFunctions::ptr_glViewport) CL_OpenGL::get_proc_address("glViewport");
#endif

	// Binds for OpenGL 1.2:

#ifdef __APPLE__
	functions->drawRangeElements = (CL_GLFunctions::ptr_glDrawRangeElementsEXT) &glDrawRangeElements;
	functions->texImage3D = (CL_GLFunctions::ptr_glTexImage3DEXT) &glTexImage3D;
	functions->texSubImage3D = (CL_GLFunctions::ptr_glTexSubImage3DEXT) &glTexSubImage3D;
	functions->copyTexSubImage3D = (CL_GLFunctions::ptr_glCopyTexSubImage3DEXT) &glCopyTexSubImage3D;
	functions->colorTable = (CL_GLFunctions::ptr_glColorTableSGI) &glColorTable;
	functions->copyColorTable = (CL_GLFunctions::ptr_glCopyColorTableSGI) &glCopyColorTable;
	functions->colorTableParameteriv = (CL_GLFunctions::ptr_glColorTableParameterivSGI) &glColorTableParameteriv;
	functions->colorTableParameterfv = (CL_GLFunctions::ptr_glColorTableParameterfvSGI) &glColorTableParameterfv;
	functions->getColorTable = (CL_GLFunctions::ptr_glGetColorTableSGI) &glGetColorTable;
	functions->getColorTableParameteriv = (CL_GLFunctions::ptr_glGetColorTableParameterivSGI) &glGetColorTableParameteriv;
	functions->getColorTableParameterfv = (CL_GLFunctions::ptr_glGetColorTableParameterfvSGI) &glGetColorTableParameterfv;
	functions->colorSubTable = (CL_GLFunctions::ptr_glColorSubTableEXT) &glColorSubTable;
	functions->copyColorSubTable = (CL_GLFunctions::ptr_glCopyColorSubTableEXT) &glCopyColorSubTable;
	functions->convolutionFilter1D = (CL_GLFunctions::ptr_glConvolutionFilter1DEXT) &glConvolutionFilter1D;
	functions->convolutionFilter2D = (CL_GLFunctions::ptr_glConvolutionFilter2DEXT) &glConvolutionFilter2D;
	functions->copyConvolutionFilter1D = (CL_GLFunctions::ptr_glCopyConvolutionFilter1DEXT) &glCopyConvolutionFilter1D;
	functions->copyConvolutionFilter2D = (CL_GLFunctions::ptr_glCopyConvolutionFilter2DEXT) &glCopyConvolutionFilter2D;
	functions->getConvolutionFilter = (CL_GLFunctions::ptr_glGetConvolutionFilterEXT) &glGetConvolutionFilter;
	functions->separableFilter2D = (CL_GLFunctions::ptr_glSeparableFilter2DEXT) &glSeparableFilter2D;
	functions->getSeparableFilter = (CL_GLFunctions::ptr_glGetSeparableFilterEXT) &glGetSeparableFilter;
	functions->convolutionParameteri = (CL_GLFunctions::ptr_glConvolutionParameteriEXT) &glConvolutionParameteri;
	functions->convolutionParameteriv = (CL_GLFunctions::ptr_glConvolutionParameterivEXT) &glConvolutionParameteriv;
	functions->convolutionParameterf = (CL_GLFunctions::ptr_glConvolutionParameterfEXT) &glConvolutionParameterf;
	functions->convolutionParameterfv = (CL_GLFunctions::ptr_glConvolutionParameterfvEXT) &glConvolutionParameterfv;
	functions->getConvolutionParameteriv = (CL_GLFunctions::ptr_glGetConvolutionParameterivEXT) &glGetConvolutionParameteriv;
	functions->getConvolutionParameterfv = (CL_GLFunctions::ptr_glGetConvolutionParameterfvEXT) &glGetConvolutionParameterfv;
	functions->histogram = (CL_GLFunctions::ptr_glHistogramEXT) &glHistogram;
	functions->resetHistogram = (CL_GLFunctions::ptr_glResetHistogramEXT) &glResetHistogram;
	functions->getHistogram = (CL_GLFunctions::ptr_glGetHistogramEXT) &glGetHistogram;
	functions->getHistogramParameteriv = (CL_GLFunctions::ptr_glGetHistogramParameterivEXT) &glGetHistogramParameteriv;
	functions->getHistogramParameterfv = (CL_GLFunctions::ptr_glGetHistogramParameterfvEXT) &glGetHistogramParameterfv;
	functions->minmax = (CL_GLFunctions::ptr_glMinmaxEXT) &glMinmax;
	functions->resetMinmax = (CL_GLFunctions::ptr_glResetMinmaxEXT) &glResetMinmax;
	functions->getMinmax = (CL_GLFunctions::ptr_glGetMinmaxEXT) &glGetMinmax;
	functions->getMinmaxParameteriv = (CL_GLFunctions::ptr_glGetMinmaxParameterivEXT) &glGetMinmaxParameteriv;
	functions->getMinmaxParameterfv = (CL_GLFunctions::ptr_glGetMinmaxParameterfvEXT) &glGetMinmaxParameterfv;
	functions->blendColor = (CL_GLFunctions::ptr_glBlendColorEXT) &glBlendColor;
	functions->blendEquation = (CL_GLFunctions::ptr_glBlendEquationEXT) &glBlendEquation;
#else
	functions->drawRangeElements = (CL_GLFunctions::ptr_glDrawRangeElementsEXT) CL_OpenGL::get_proc_address("glDrawRangeElementsEXT");
	functions->texImage3D = (CL_GLFunctions::ptr_glTexImage3DEXT) CL_OpenGL::get_proc_address("glTexImage3DEXT");
	functions->texSubImage3D = (CL_GLFunctions::ptr_glTexSubImage3DEXT) CL_OpenGL::get_proc_address("glTexSubImage3DEXT");
	functions->copyTexSubImage3D = (CL_GLFunctions::ptr_glCopyTexSubImage3DEXT) CL_OpenGL::get_proc_address("glCopyTexSubImage3DEXT");
	functions->colorTable = (CL_GLFunctions::ptr_glColorTableSGI) CL_OpenGL::get_proc_address("glColorTableSGI");
	functions->copyColorTable = (CL_GLFunctions::ptr_glCopyColorTableSGI) CL_OpenGL::get_proc_address("glCopyColorTableSGI");
	functions->colorTableParameteriv = (CL_GLFunctions::ptr_glColorTableParameterivSGI) CL_OpenGL::get_proc_address("glColorTableParameterivSGI");
	functions->colorTableParameterfv = (CL_GLFunctions::ptr_glColorTableParameterfvSGI) CL_OpenGL::get_proc_address("glColorTableParameterfvSGI");
	functions->getColorTable = (CL_GLFunctions::ptr_glGetColorTableSGI) CL_OpenGL::get_proc_address("glGetColorTableSGI");
	functions->getColorTableParameteriv = (CL_GLFunctions::ptr_glGetColorTableParameterivSGI) CL_OpenGL::get_proc_address("glGetColorTableParameterivSGI");
	functions->getColorTableParameterfv = (CL_GLFunctions::ptr_glGetColorTableParameterfvSGI) CL_OpenGL::get_proc_address("glGetColorTableParameterfvSGI");
	functions->colorSubTable = (CL_GLFunctions::ptr_glColorSubTableEXT) CL_OpenGL::get_proc_address("glColorSubTableEXT");
	functions->copyColorSubTable = (CL_GLFunctions::ptr_glCopyColorSubTableEXT) CL_OpenGL::get_proc_address("glCopyColorSubTableEXT");
	functions->convolutionFilter1D = (CL_GLFunctions::ptr_glConvolutionFilter1DEXT) CL_OpenGL::get_proc_address("glConvolutionFilter1DEXT");
	functions->convolutionFilter2D = (CL_GLFunctions::ptr_glConvolutionFilter2DEXT) CL_OpenGL::get_proc_address("glConvolutionFilter2DEXT");
	functions->copyConvolutionFilter1D = (CL_GLFunctions::ptr_glCopyConvolutionFilter1DEXT) CL_OpenGL::get_proc_address("glCopyConvolutionFilter1DEXT");
	functions->copyConvolutionFilter2D = (CL_GLFunctions::ptr_glCopyConvolutionFilter2DEXT) CL_OpenGL::get_proc_address("glCopyConvolutionFilter2DEXT");
	functions->getConvolutionFilter = (CL_GLFunctions::ptr_glGetConvolutionFilterEXT) CL_OpenGL::get_proc_address("glGetConvolutionFilterEXT");
	functions->separableFilter2D = (CL_GLFunctions::ptr_glSeparableFilter2DEXT) CL_OpenGL::get_proc_address("glSeparableFilter2DEXT");
	functions->getSeparableFilter = (CL_GLFunctions::ptr_glGetSeparableFilterEXT) CL_OpenGL::get_proc_address("glGetSeparableFilterEXT");
	functions->convolutionParameteri = (CL_GLFunctions::ptr_glConvolutionParameteriEXT) CL_OpenGL::get_proc_address("glConvolutionParameteriEXT");
	functions->convolutionParameteriv = (CL_GLFunctions::ptr_glConvolutionParameterivEXT) CL_OpenGL::get_proc_address("glConvolutionParameterivEXT");
	functions->convolutionParameterf = (CL_GLFunctions::ptr_glConvolutionParameterfEXT) CL_OpenGL::get_proc_address("glConvolutionParameterfEXT");
	functions->convolutionParameterfv = (CL_GLFunctions::ptr_glConvolutionParameterfvEXT) CL_OpenGL::get_proc_address("glConvolutionParameterfvEXT");
	functions->getConvolutionParameteriv = (CL_GLFunctions::ptr_glGetConvolutionParameterivEXT) CL_OpenGL::get_proc_address("glGetConvolutionParameterivEXT");
	functions->getConvolutionParameterfv = (CL_GLFunctions::ptr_glGetConvolutionParameterfvEXT) CL_OpenGL::get_proc_address("glGetConvolutionParameterfvEXT");
	functions->histogram = (CL_GLFunctions::ptr_glHistogramEXT) CL_OpenGL::get_proc_address("glHistogramEXT");
	functions->resetHistogram = (CL_GLFunctions::ptr_glResetHistogramEXT) CL_OpenGL::get_proc_address("glResetHistogramEXT");
	functions->getHistogram = (CL_GLFunctions::ptr_glGetHistogramEXT) CL_OpenGL::get_proc_address("glGetHistogramEXT");
	functions->getHistogramParameteriv = (CL_GLFunctions::ptr_glGetHistogramParameterivEXT) CL_OpenGL::get_proc_address("glGetHistogramParameterivEXT");
	functions->getHistogramParameterfv = (CL_GLFunctions::ptr_glGetHistogramParameterfvEXT) CL_OpenGL::get_proc_address("glGetHistogramParameterfvEXT");
	functions->minmax = (CL_GLFunctions::ptr_glMinmaxEXT) CL_OpenGL::get_proc_address("glMinmaxEXT");
	functions->resetMinmax = (CL_GLFunctions::ptr_glResetMinmaxEXT) CL_OpenGL::get_proc_address("glResetMinmaxEXT");
	functions->getMinmax = (CL_GLFunctions::ptr_glGetMinmaxEXT) CL_OpenGL::get_proc_address("glGetMinmaxEXT");
	functions->getMinmaxParameteriv = (CL_GLFunctions::ptr_glGetMinmaxParameterivEXT) CL_OpenGL::get_proc_address("glGetMinmaxParameterivEXT");
	functions->getMinmaxParameterfv = (CL_GLFunctions::ptr_glGetMinmaxParameterfvEXT) CL_OpenGL::get_proc_address("glGetMinmaxParameterfvEXT");
	functions->blendColor = (CL_GLFunctions::ptr_glBlendColorEXT) CL_OpenGL::get_proc_address("glBlendColorEXT");
	functions->blendEquation = (CL_GLFunctions::ptr_glBlendEquationEXT) CL_OpenGL::get_proc_address("glBlendEquationEXT");
#endif

	// Binds for OpenGL 1.2.1:

#ifdef __APPLE__
	functions->activeTexture = (CL_GLFunctions::ptr_glActiveTextureARB) &glActiveTexture;
	functions->clientActiveTexture = (CL_GLFunctions::ptr_glClientActiveTextureARB) &glClientActiveTexture;
	functions->multiTexCoord1d = (CL_GLFunctions::ptr_glMultiTexCoord1dARB) &glMultiTexCoord1d;
	functions->multiTexCoord1dv = (CL_GLFunctions::ptr_glMultiTexCoord1dvARB) &glMultiTexCoord1dv;
	functions->multiTexCoord1f = (CL_GLFunctions::ptr_glMultiTexCoord1fARB) &glMultiTexCoord1f;
	functions->multiTexCoord1fv = (CL_GLFunctions::ptr_glMultiTexCoord1fvARB) &glMultiTexCoord1fv;
	functions->multiTexCoord1i = (CL_GLFunctions::ptr_glMultiTexCoord1iARB) &glMultiTexCoord1i;
	functions->multiTexCoord1iv = (CL_GLFunctions::ptr_glMultiTexCoord1ivARB) &glMultiTexCoord1iv;
	functions->multiTexCoord1s = (CL_GLFunctions::ptr_glMultiTexCoord1sARB) &glMultiTexCoord1s;
	functions->multiTexCoord1sv = (CL_GLFunctions::ptr_glMultiTexCoord1svARB) &glMultiTexCoord1sv;
	functions->multiTexCoord2d = (CL_GLFunctions::ptr_glMultiTexCoord2dARB) &glMultiTexCoord2d;
	functions->multiTexCoord2dv = (CL_GLFunctions::ptr_glMultiTexCoord2dvARB) &glMultiTexCoord2dv;
	functions->multiTexCoord2f = (CL_GLFunctions::ptr_glMultiTexCoord2fARB) &glMultiTexCoord2f;
	functions->multiTexCoord2fv = (CL_GLFunctions::ptr_glMultiTexCoord2fvARB) &glMultiTexCoord2fv;
	functions->multiTexCoord2i = (CL_GLFunctions::ptr_glMultiTexCoord2iARB) &glMultiTexCoord2i;
	functions->multiTexCoord2iv = (CL_GLFunctions::ptr_glMultiTexCoord2ivARB) &glMultiTexCoord2iv;
	functions->multiTexCoord2s = (CL_GLFunctions::ptr_glMultiTexCoord2sARB) &glMultiTexCoord2s;
	functions->multiTexCoord2sv = (CL_GLFunctions::ptr_glMultiTexCoord2svARB) &glMultiTexCoord2sv;
	functions->multiTexCoord3d = (CL_GLFunctions::ptr_glMultiTexCoord3dARB) &glMultiTexCoord3d;
	functions->multiTexCoord3dv = (CL_GLFunctions::ptr_glMultiTexCoord3dvARB) &glMultiTexCoord3dv;
	functions->multiTexCoord3f = (CL_GLFunctions::ptr_glMultiTexCoord3fARB) &glMultiTexCoord3f;
	functions->multiTexCoord3fv = (CL_GLFunctions::ptr_glMultiTexCoord3fvARB) &glMultiTexCoord3fv;
	functions->multiTexCoord3i = (CL_GLFunctions::ptr_glMultiTexCoord3iARB) &glMultiTexCoord3i;
	functions->multiTexCoord3iv = (CL_GLFunctions::ptr_glMultiTexCoord3ivARB) &glMultiTexCoord3iv;
	functions->multiTexCoord3s = (CL_GLFunctions::ptr_glMultiTexCoord3sARB) &glMultiTexCoord3s;
	functions->multiTexCoord3sv = (CL_GLFunctions::ptr_glMultiTexCoord3svARB) &glMultiTexCoord3sv;
	functions->multiTexCoord4d = (CL_GLFunctions::ptr_glMultiTexCoord4dARB) &glMultiTexCoord4d;
	functions->multiTexCoord4dv = (CL_GLFunctions::ptr_glMultiTexCoord4dvARB) &glMultiTexCoord4dv;
	functions->multiTexCoord4f = (CL_GLFunctions::ptr_glMultiTexCoord4fARB) &glMultiTexCoord4f;
	functions->multiTexCoord4fv = (CL_GLFunctions::ptr_glMultiTexCoord4fvARB) &glMultiTexCoord4fv;
	functions->multiTexCoord4i = (CL_GLFunctions::ptr_glMultiTexCoord4iARB) &glMultiTexCoord4i;
	functions->multiTexCoord4iv = (CL_GLFunctions::ptr_glMultiTexCoord4ivARB) &glMultiTexCoord4iv;
	functions->multiTexCoord4s = (CL_GLFunctions::ptr_glMultiTexCoord4sARB) &glMultiTexCoord4s;
	functions->multiTexCoord4sv = (CL_GLFunctions::ptr_glMultiTexCoord4svARB) &glMultiTexCoord4sv;
#else
	functions->activeTexture = (CL_GLFunctions::ptr_glActiveTextureARB) CL_OpenGL::get_proc_address("glActiveTextureARB");
	functions->clientActiveTexture = (CL_GLFunctions::ptr_glClientActiveTextureARB) CL_OpenGL::get_proc_address("glClientActiveTextureARB");
	functions->multiTexCoord1d = (CL_GLFunctions::ptr_glMultiTexCoord1dARB) CL_OpenGL::get_proc_address("glMultiTexCoord1dARB");
	functions->multiTexCoord1dv = (CL_GLFunctions::ptr_glMultiTexCoord1dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord1dvARB");
	functions->multiTexCoord1f = (CL_GLFunctions::ptr_glMultiTexCoord1fARB) CL_OpenGL::get_proc_address("glMultiTexCoord1fARB");
	functions->multiTexCoord1fv = (CL_GLFunctions::ptr_glMultiTexCoord1fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord1fvARB");
	functions->multiTexCoord1i = (CL_GLFunctions::ptr_glMultiTexCoord1iARB) CL_OpenGL::get_proc_address("glMultiTexCoord1iARB");
	functions->multiTexCoord1iv = (CL_GLFunctions::ptr_glMultiTexCoord1ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord1ivARB");
	functions->multiTexCoord1s = (CL_GLFunctions::ptr_glMultiTexCoord1sARB) CL_OpenGL::get_proc_address("glMultiTexCoord1sARB");
	functions->multiTexCoord1sv = (CL_GLFunctions::ptr_glMultiTexCoord1svARB) CL_OpenGL::get_proc_address("glMultiTexCoord1svARB");
	functions->multiTexCoord2d = (CL_GLFunctions::ptr_glMultiTexCoord2dARB) CL_OpenGL::get_proc_address("glMultiTexCoord2dARB");
	functions->multiTexCoord2dv = (CL_GLFunctions::ptr_glMultiTexCoord2dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord2dvARB");
	functions->multiTexCoord2f = (CL_GLFunctions::ptr_glMultiTexCoord2fARB) CL_OpenGL::get_proc_address("glMultiTexCoord2fARB");
	functions->multiTexCoord2fv = (CL_GLFunctions::ptr_glMultiTexCoord2fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord2fvARB");
	functions->multiTexCoord2i = (CL_GLFunctions::ptr_glMultiTexCoord2iARB) CL_OpenGL::get_proc_address("glMultiTexCoord2iARB");
	functions->multiTexCoord2iv = (CL_GLFunctions::ptr_glMultiTexCoord2ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord2ivARB");
	functions->multiTexCoord2s = (CL_GLFunctions::ptr_glMultiTexCoord2sARB) CL_OpenGL::get_proc_address("glMultiTexCoord2sARB");
	functions->multiTexCoord2sv = (CL_GLFunctions::ptr_glMultiTexCoord2svARB) CL_OpenGL::get_proc_address("glMultiTexCoord2svARB");
	functions->multiTexCoord3d = (CL_GLFunctions::ptr_glMultiTexCoord3dARB) CL_OpenGL::get_proc_address("glMultiTexCoord3dARB");
	functions->multiTexCoord3dv = (CL_GLFunctions::ptr_glMultiTexCoord3dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord3dvARB");
	functions->multiTexCoord3f = (CL_GLFunctions::ptr_glMultiTexCoord3fARB) CL_OpenGL::get_proc_address("glMultiTexCoord3fARB");
	functions->multiTexCoord3fv = (CL_GLFunctions::ptr_glMultiTexCoord3fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord3fvARB");
	functions->multiTexCoord3i = (CL_GLFunctions::ptr_glMultiTexCoord3iARB) CL_OpenGL::get_proc_address("glMultiTexCoord3iARB");
	functions->multiTexCoord3iv = (CL_GLFunctions::ptr_glMultiTexCoord3ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord3ivARB");
	functions->multiTexCoord3s = (CL_GLFunctions::ptr_glMultiTexCoord3sARB) CL_OpenGL::get_proc_address("glMultiTexCoord3sARB");
	functions->multiTexCoord3sv = (CL_GLFunctions::ptr_glMultiTexCoord3svARB) CL_OpenGL::get_proc_address("glMultiTexCoord3svARB");
	functions->multiTexCoord4d = (CL_GLFunctions::ptr_glMultiTexCoord4dARB) CL_OpenGL::get_proc_address("glMultiTexCoord4dARB");
	functions->multiTexCoord4dv = (CL_GLFunctions::ptr_glMultiTexCoord4dvARB) CL_OpenGL::get_proc_address("glMultiTexCoord4dvARB");
	functions->multiTexCoord4f = (CL_GLFunctions::ptr_glMultiTexCoord4fARB) CL_OpenGL::get_proc_address("glMultiTexCoord4fARB");
	functions->multiTexCoord4fv = (CL_GLFunctions::ptr_glMultiTexCoord4fvARB) CL_OpenGL::get_proc_address("glMultiTexCoord4fvARB");
	functions->multiTexCoord4i = (CL_GLFunctions::ptr_glMultiTexCoord4iARB) CL_OpenGL::get_proc_address("glMultiTexCoord4iARB");
	functions->multiTexCoord4iv = (CL_GLFunctions::ptr_glMultiTexCoord4ivARB) CL_OpenGL::get_proc_address("glMultiTexCoord4ivARB");
	functions->multiTexCoord4s = (CL_GLFunctions::ptr_glMultiTexCoord4sARB) CL_OpenGL::get_proc_address("glMultiTexCoord4sARB");
	functions->multiTexCoord4sv = (CL_GLFunctions::ptr_glMultiTexCoord4svARB) CL_OpenGL::get_proc_address("glMultiTexCoord4svARB");
#endif

	// Binds for OpenGL 1.3:

#ifdef __APPLE__
	functions->compressedTexImage1D = (CL_GLFunctions::ptr_glCompressedTexImage1DARB) &glCompressedTexImage1D;
	functions->compressedTexImage2D = (CL_GLFunctions::ptr_glCompressedTexImage2DARB) &glCompressedTexImage2D;
	functions->compressedTexImage3D = (CL_GLFunctions::ptr_glCompressedTexImage3DARB) &glCompressedTexImage3D;
	functions->compressedTexSubImage1D = (CL_GLFunctions::ptr_glCompressedTexSubImage1DARB) &glCompressedTexSubImage1D;
	functions->compressedTexSubImage2D = (CL_GLFunctions::ptr_glCompressedTexSubImage2DARB) &glCompressedTexSubImage2D;
	functions->compressedTexSubImage3D = (CL_GLFunctions::ptr_glCompressedTexSubImage3DARB) &glCompressedTexSubImage3D;
	functions->getCompressedTexImage = (CL_GLFunctions::ptr_glGetCompressedTexImageARB) &glGetCompressedTexImage;
	functions->sampleCoverage = (CL_GLFunctions::ptr_glSampleCoverageARB) &glSampleCoverage;
	functions->glLoadTransposeMatrixd = (CL_GLFunctions::ptr_glLoadTransposeMatrixdARB) &glLoadTransposeMatrixd;
	functions->glLoadTransposeMatrixf = (CL_GLFunctions::ptr_glLoadTransposeMatrixfARB) &glLoadTransposeMatrixf;
	functions->glMultTransposeMatrixd = (CL_GLFunctions::ptr_glMultTransposeMatrixdARB) &glMultTransposeMatrixd;
	functions->glMultTransposeMatrixf = (CL_GLFunctions::ptr_glMultTransposeMatrixfARB) &glMultTransposeMatrixf;
#else
	functions->compressedTexImage1D = (CL_GLFunctions::ptr_glCompressedTexImage1DARB) CL_OpenGL::get_proc_address("glCompressedTexImage1DARB");
	functions->compressedTexImage2D = (CL_GLFunctions::ptr_glCompressedTexImage2DARB) CL_OpenGL::get_proc_address("glCompressedTexImage2DARB");
	functions->compressedTexImage3D = (CL_GLFunctions::ptr_glCompressedTexImage3DARB) CL_OpenGL::get_proc_address("glCompressedTexImage3DARB");
	functions->compressedTexSubImage1D = (CL_GLFunctions::ptr_glCompressedTexSubImage1DARB) CL_OpenGL::get_proc_address("glCompressedTexSubImage1DARB");
	functions->compressedTexSubImage2D = (CL_GLFunctions::ptr_glCompressedTexSubImage2DARB) CL_OpenGL::get_proc_address("glCompressedTexSubImage2DARB");
	functions->compressedTexSubImage3D = (CL_GLFunctions::ptr_glCompressedTexSubImage3DARB) CL_OpenGL::get_proc_address("glCompressedTexSubImage3DARB");
	functions->getCompressedTexImage = (CL_GLFunctions::ptr_glGetCompressedTexImageARB) CL_OpenGL::get_proc_address("glGetCompressedTexImageARB");
	functions->sampleCoverage = (CL_GLFunctions::ptr_glSampleCoverageARB) CL_OpenGL::get_proc_address("glSampleCoverageARB");
	functions->glLoadTransposeMatrixd = (CL_GLFunctions::ptr_glLoadTransposeMatrixdARB) CL_OpenGL::get_proc_address("glLoadTransposeMatrixdARB");
	functions->glLoadTransposeMatrixf = (CL_GLFunctions::ptr_glLoadTransposeMatrixfARB) CL_OpenGL::get_proc_address("glLoadTransposeMatrixfARB");
	functions->glMultTransposeMatrixd = (CL_GLFunctions::ptr_glMultTransposeMatrixdARB) CL_OpenGL::get_proc_address("glMultTransposeMatrixdARB");
	functions->glMultTransposeMatrixf = (CL_GLFunctions::ptr_glMultTransposeMatrixfARB) CL_OpenGL::get_proc_address("glMultTransposeMatrixfARB");
#endif

	// Binds for OpenGL 1.4:

#ifdef __APPLE__
	functions->fogCoordd = (CL_GLFunctions::ptr_glFogCoorddEXT) &glFogCoordd;
	functions->fogCoorddv = (CL_GLFunctions::ptr_glFogCoorddvEXT) &glFogCoorddv;
	functions->fogCoordf = (CL_GLFunctions::ptr_glFogCoordfEXT) &glFogCoordf;
	functions->fogCoordfv = (CL_GLFunctions::ptr_glFogCoordfvEXT) &glFogCoordfv;
	functions->fogCoordPointer = (CL_GLFunctions::ptr_glFogCoordPointerEXT) &glFogCoordPointer;
	functions->multiDrawArrays = (CL_GLFunctions::ptr_glMultiDrawArraysEXT) &glMultiDrawArrays;
	functions->multiDrawElementsEXT = (CL_GLFunctions::ptr_glMultiDrawElementsEXT) &glMultiDrawElements;
	functions->pointParameterf = (CL_GLFunctions::ptr_glPointParameterfARB) &glPointParameterf;
	functions->pointParameterfv = (CL_GLFunctions::ptr_glPointParameterfvARB) &glPointParameterfv;
	functions->secondaryColor3b = (CL_GLFunctions::ptr_glSecondaryColor3bEXT) &glSecondaryColor3b;
	functions->secondaryColor3bv = (CL_GLFunctions::ptr_glSecondaryColor3bvEXT) &glSecondaryColor3bv;
	functions->secondaryColor3d = (CL_GLFunctions::ptr_glSecondaryColor3dEXT) &glSecondaryColor3d;
	functions->secondaryColor3dv = (CL_GLFunctions::ptr_glSecondaryColor3dvEXT) &glSecondaryColor3dv;
	functions->secondaryColor3f = (CL_GLFunctions::ptr_glSecondaryColor3fEXT) &glSecondaryColor3f;
	functions->secondaryColor3fv = (CL_GLFunctions::ptr_glSecondaryColor3fvEXT) &glSecondaryColor3fv;
	functions->secondaryColor3i = (CL_GLFunctions::ptr_glSecondaryColor3iEXT) &glSecondaryColor3i;
	functions->secondaryColor3iv = (CL_GLFunctions::ptr_glSecondaryColor3ivEXT) &glSecondaryColor3iv;
	functions->secondaryColor3s = (CL_GLFunctions::ptr_glSecondaryColor3sEXT) &glSecondaryColor3s;
	functions->secondaryColor3sv = (CL_GLFunctions::ptr_glSecondaryColor3svEXT) &glSecondaryColor3sv;
	functions->secondaryColor3ub = (CL_GLFunctions::ptr_glSecondaryColor3ubEXT) &glSecondaryColor3ub;
	functions->secondaryColor3ubv = (CL_GLFunctions::ptr_glSecondaryColor3ubvEXT) &glSecondaryColor3ubv;
	functions->secondaryColor3ui = (CL_GLFunctions::ptr_glSecondaryColor3uiEXT) &glSecondaryColor3ui;
	functions->secondaryColor3uiv = (CL_GLFunctions::ptr_glSecondaryColor3uivEXT) &glSecondaryColor3uiv;
	functions->secondaryColor3us = (CL_GLFunctions::ptr_glSecondaryColor3usEXT) &glSecondaryColor3us;
	functions->secondaryColor3usv = (CL_GLFunctions::ptr_glSecondaryColor3usvEXT) &glSecondaryColor3usv;
	functions->secondaryColorPointer = (CL_GLFunctions::ptr_glSecondaryColorPointerEXT) &glSecondaryColorPointer;
	functions->blendFuncSeparate = (CL_GLFunctions::ptr_glBlendFuncSeparateEXT) &glBlendFuncSeparate;
	
	// need to manually link these, since some llama linux opengl driver says its 1.4 while its not! >:(
/*	
	functions->windowPos2d = (CL_GLFunctions::ptr_glWindowPos2dARB) CL_OpenGL::get_proc_address("glWindowPos2dARB");
	functions->windowPos2dv = (CL_GLFunctions::ptr_glWindowPos2dvARB) CL_OpenGL::get_proc_address("glWindowPos2dvARB");
	functions->windowPos2f = (CL_GLFunctions::ptr_glWindowPos2fARB) CL_OpenGL::get_proc_address("glWindowPos2fARB");
	functions->windowPos2fv = (CL_GLFunctions::ptr_glWindowPos2fvARB) CL_OpenGL::get_proc_address("glWindowPos2fvARB");
	functions->windowPos2i = (CL_GLFunctions::ptr_glWindowPos2iARB) CL_OpenGL::get_proc_address("glWindowPos2iARB");
	functions->windowPos2iv = (CL_GLFunctions::ptr_glWindowPos2ivARB) CL_OpenGL::get_proc_address("glWindowPos2ivARB");
	functions->windowPos2s = (CL_GLFunctions::ptr_glWindowPos2sARB) CL_OpenGL::get_proc_address("glWindowPos2sARB");
	functions->windowPos2sv = (CL_GLFunctions::ptr_glWindowPos2svARB) CL_OpenGL::get_proc_address("glWindowPos2svARB");
	functions->windowPos3d = (CL_GLFunctions::ptr_glWindowPos3dARB) CL_OpenGL::get_proc_address("glWindowPos3dARB");
	functions->windowPos3dv = (CL_GLFunctions::ptr_glWindowPos3dvARB) CL_OpenGL::get_proc_address("glWindowPos3dvARB");
	functions->windowPos3f = (CL_GLFunctions::ptr_glWindowPos3fARB) CL_OpenGL::get_proc_address("glWindowPos3fARB");
	functions->windowPos3fv = (CL_GLFunctions::ptr_glWindowPos3fvARB) CL_OpenGL::get_proc_address("glWindowPos3fvARB");
	functions->windowPos3i = (CL_GLFunctions::ptr_glWindowPos3iARB) CL_OpenGL::get_proc_address("glWindowPos3iARB");
	functions->windowPos3iv = (CL_GLFunctions::ptr_glWindowPos3ivARB) CL_OpenGL::get_proc_address("glWindowPos3ivARB");
	functions->windowPos3s = (CL_GLFunctions::ptr_glWindowPos3sARB) CL_OpenGL::get_proc_address("glWindowPos3sARB");
	functions->windowPos3sv = (CL_GLFunctions::ptr_glWindowPos3svARB) CL_OpenGL::get_proc_address("glWindowPos3svARB");
*/
	functions->windowPos2d = &glWindowPos2d;
	functions->windowPos2dv = &glWindowPos2dv;
	functions->windowPos2f = &glWindowPos2f;
	functions->windowPos2fv = &glWindowPos2fv;
	functions->windowPos2i = &glWindowPos2i;
	functions->windowPos2iv = &glWindowPos2iv;
	functions->windowPos2s = &glWindowPos2s;
	functions->windowPos2sv = &glWindowPos2sv;
	functions->windowPos3d = &glWindowPos3d;
	functions->windowPos3dv = &glWindowPos3dv;
	functions->windowPos3f = &glWindowPos3f;
	functions->windowPos3fv = &glWindowPos3fv;
	functions->windowPos3i = &glWindowPos3i;
	functions->windowPos3iv = &glWindowPos3iv;
	functions->windowPos3s = &glWindowPos3s;
	functions->windowPos3sv = &glWindowPos3sv;
#else
	functions->fogCoordd = (CL_GLFunctions::ptr_glFogCoorddEXT) CL_OpenGL::get_proc_address("glFogCoorddEXT");
	functions->fogCoorddv = (CL_GLFunctions::ptr_glFogCoorddvEXT) CL_OpenGL::get_proc_address("glFogCoorddvEXT");
	functions->fogCoordf = (CL_GLFunctions::ptr_glFogCoordfEXT) CL_OpenGL::get_proc_address("glFogCoordfEXT");
	functions->fogCoordfv = (CL_GLFunctions::ptr_glFogCoordfvEXT) CL_OpenGL::get_proc_address("glFogCoordfvEXT");
	functions->fogCoordPointer = (CL_GLFunctions::ptr_glFogCoordPointerEXT) CL_OpenGL::get_proc_address("glFogCoordPointerEXT");
	functions->multiDrawArrays = (CL_GLFunctions::ptr_glMultiDrawArraysEXT) CL_OpenGL::get_proc_address("glMultiDrawArraysEXT");
	functions->multiDrawElementsEXT = (CL_GLFunctions::ptr_glMultiDrawElementsEXT) CL_OpenGL::get_proc_address("glMultiDrawElementsEXT");
	functions->pointParameterf = (CL_GLFunctions::ptr_glPointParameterfARB) CL_OpenGL::get_proc_address("glPointParameterfARB");
	functions->pointParameterfv = (CL_GLFunctions::ptr_glPointParameterfvARB) CL_OpenGL::get_proc_address("glPointParameterfvARB");
	functions->secondaryColor3b = (CL_GLFunctions::ptr_glSecondaryColor3bEXT) CL_OpenGL::get_proc_address("glSecondaryColor3bEXT");
	functions->secondaryColor3bv = (CL_GLFunctions::ptr_glSecondaryColor3bvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3bvEXT");
	functions->secondaryColor3d = (CL_GLFunctions::ptr_glSecondaryColor3dEXT) CL_OpenGL::get_proc_address("glSecondaryColor3dEXT");
	functions->secondaryColor3dv = (CL_GLFunctions::ptr_glSecondaryColor3dvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3dvEXT");
	functions->secondaryColor3f = (CL_GLFunctions::ptr_glSecondaryColor3fEXT) CL_OpenGL::get_proc_address("glSecondaryColor3fEXT");
	functions->secondaryColor3fv = (CL_GLFunctions::ptr_glSecondaryColor3fvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3fvEXT");
	functions->secondaryColor3i = (CL_GLFunctions::ptr_glSecondaryColor3iEXT) CL_OpenGL::get_proc_address("glSecondaryColor3iEXT");
	functions->secondaryColor3iv = (CL_GLFunctions::ptr_glSecondaryColor3ivEXT) CL_OpenGL::get_proc_address("glSecondaryColor3ivEXT");
	functions->secondaryColor3s = (CL_GLFunctions::ptr_glSecondaryColor3sEXT) CL_OpenGL::get_proc_address("glSecondaryColor3sEXT");
	functions->secondaryColor3sv = (CL_GLFunctions::ptr_glSecondaryColor3svEXT) CL_OpenGL::get_proc_address("glSecondaryColor3svEXT");
	functions->secondaryColor3ub = (CL_GLFunctions::ptr_glSecondaryColor3ubEXT) CL_OpenGL::get_proc_address("glSecondaryColor3ubEXT");
	functions->secondaryColor3ubv = (CL_GLFunctions::ptr_glSecondaryColor3ubvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3ubvEXT");
	functions->secondaryColor3ui = (CL_GLFunctions::ptr_glSecondaryColor3uiEXT) CL_OpenGL::get_proc_address("glSecondaryColor3uiEXT");
	functions->secondaryColor3uiv = (CL_GLFunctions::ptr_glSecondaryColor3uivEXT) CL_OpenGL::get_proc_address("glSecondaryColor3uivEXT");
	functions->secondaryColor3us = (CL_GLFunctions::ptr_glSecondaryColor3usEXT) CL_OpenGL::get_proc_address("glSecondaryColor3usEXT");
	functions->secondaryColor3usv = (CL_GLFunctions::ptr_glSecondaryColor3usvEXT) CL_OpenGL::get_proc_address("glSecondaryColor3usvEXT");
	functions->secondaryColorPointer = (CL_GLFunctions::ptr_glSecondaryColorPointerEXT) CL_OpenGL::get_proc_address("glSecondaryColorPointerEXT");
	functions->blendFuncSeparate = (CL_GLFunctions::ptr_glBlendFuncSeparateEXT) CL_OpenGL::get_proc_address("glBlendFuncSeparateEXT");
	functions->windowPos2d = (CL_GLFunctions::ptr_glWindowPos2dARB) CL_OpenGL::get_proc_address("glWindowPos2dARB");
	functions->windowPos2dv = (CL_GLFunctions::ptr_glWindowPos2dvARB) CL_OpenGL::get_proc_address("glWindowPos2dvARB");
	functions->windowPos2f = (CL_GLFunctions::ptr_glWindowPos2fARB) CL_OpenGL::get_proc_address("glWindowPos2fARB");
	functions->windowPos2fv = (CL_GLFunctions::ptr_glWindowPos2fvARB) CL_OpenGL::get_proc_address("glWindowPos2fvARB");
	functions->windowPos2i = (CL_GLFunctions::ptr_glWindowPos2iARB) CL_OpenGL::get_proc_address("glWindowPos2iARB");
	functions->windowPos2iv = (CL_GLFunctions::ptr_glWindowPos2ivARB) CL_OpenGL::get_proc_address("glWindowPos2ivARB");
	functions->windowPos2s = (CL_GLFunctions::ptr_glWindowPos2sARB) CL_OpenGL::get_proc_address("glWindowPos2sARB");
	functions->windowPos2sv = (CL_GLFunctions::ptr_glWindowPos2svARB) CL_OpenGL::get_proc_address("glWindowPos2svARB");
	functions->windowPos3d = (CL_GLFunctions::ptr_glWindowPos3dARB) CL_OpenGL::get_proc_address("glWindowPos3dARB");
	functions->windowPos3dv = (CL_GLFunctions::ptr_glWindowPos3dvARB) CL_OpenGL::get_proc_address("glWindowPos3dvARB");
	functions->windowPos3f = (CL_GLFunctions::ptr_glWindowPos3fARB) CL_OpenGL::get_proc_address("glWindowPos3fARB");
	functions->windowPos3fv = (CL_GLFunctions::ptr_glWindowPos3fvARB) CL_OpenGL::get_proc_address("glWindowPos3fvARB");
	functions->windowPos3i = (CL_GLFunctions::ptr_glWindowPos3iARB) CL_OpenGL::get_proc_address("glWindowPos3iARB");
	functions->windowPos3iv = (CL_GLFunctions::ptr_glWindowPos3ivARB) CL_OpenGL::get_proc_address("glWindowPos3ivARB");
	functions->windowPos3s = (CL_GLFunctions::ptr_glWindowPos3sARB) CL_OpenGL::get_proc_address("glWindowPos3sARB");
	functions->windowPos3sv = (CL_GLFunctions::ptr_glWindowPos3svARB) CL_OpenGL::get_proc_address("glWindowPos3svARB");
#endif

	// Binds for OpenGL 1.5:

#ifdef __NOBODY_YET_HEHE__
	functions->bindBuffer = (CL_GLFunctions::ptr_glBindBufferARB) &glBindBuffer;
	functions->deleteBuffers = (CL_GLFunctions::ptr_glDeleteBuffersARB) &glDeleteBuffers;
	functions->genBuffers = (CL_GLFunctions::ptr_glGenBuffersARB) &glGenBuffers;
	functions->isBuffer = (CL_GLFunctions::ptr_glIsBufferARB) &glIsBuffer;
	functions->bufferData = (CL_GLFunctions::ptr_glBufferDataARB) &glBufferData;
	functions->bufferSubData = (CL_GLFunctions::ptr_glBufferSubDataARB) &glBufferSubData;
	functions->getBufferSubData = (CL_GLFunctions::ptr_glGetBufferSubDataARB) &glGetBufferSubData;
	functions->mapBuffer = (CL_GLFunctions::ptr_glMapBufferARB) &glMapBuffer;
	functions->unmapBuffer = (CL_GLFunctions::ptr_glUnmapBufferARB) &glUnmapBuffer;
	functions->getBufferParameteriv = (CL_GLFunctions::ptr_glGetBufferParameterivARB) &glGetBufferParameteriv;
	functions->getBufferPointerv = (CL_GLFunctions::ptr_glGetBufferPointervARB) &glGetBufferPointerv;
	functions->genQueries = (CL_GLFunctions::ptr_glGenQueriesARB) &glGenQueries;
	functions->deleteQueries = (CL_GLFunctions::ptr_glDeleteQueriesARB) &glDeleteQueries;
	functions->isQuery = (CL_GLFunctions::ptr_glIsQueryARB) &glIsQuery;
	functions->beginQuery = (CL_GLFunctions::ptr_glBeginQueryARB) &glBeginQuery;
	functions->endQuery = (CL_GLFunctions::ptr_glEndQueryARB) &glEndQuery;
	functions->getQueryiv = (CL_GLFunctions::ptr_glGetQueryivARB) &glGetQueryiv;
	functions->getQueryObjectiv = (CL_GLFunctions::ptr_glGetQueryObjectivARB) &glGetQueryObjectiv;
	functions->getQueryObjectuiv = (CL_GLFunctions::ptr_glGetQueryObjectuivARB) &glGetQueryObjectuiv;
#else
	functions->bindBuffer = (CL_GLFunctions::ptr_glBindBufferARB) CL_OpenGL::get_proc_address("glBindBufferARB");
	functions->deleteBuffers = (CL_GLFunctions::ptr_glDeleteBuffersARB) CL_OpenGL::get_proc_address("glDeleteBuffersARB");
	functions->genBuffers = (CL_GLFunctions::ptr_glGenBuffersARB) CL_OpenGL::get_proc_address("glGenBuffersARB");
	functions->isBuffer = (CL_GLFunctions::ptr_glIsBufferARB) CL_OpenGL::get_proc_address("glIsBufferARB");
	functions->bufferData = (CL_GLFunctions::ptr_glBufferDataARB) CL_OpenGL::get_proc_address("glBufferDataARB");
	functions->bufferSubData = (CL_GLFunctions::ptr_glBufferSubDataARB) CL_OpenGL::get_proc_address("glBufferSubDataARB");
	functions->getBufferSubData = (CL_GLFunctions::ptr_glGetBufferSubDataARB) CL_OpenGL::get_proc_address("glGetBufferSubDataARB");
	functions->mapBuffer = (CL_GLFunctions::ptr_glMapBufferARB) CL_OpenGL::get_proc_address("glMapBufferARB");
	functions->unmapBuffer = (CL_GLFunctions::ptr_glUnmapBufferARB) CL_OpenGL::get_proc_address("glUnmapBufferARB");
	functions->getBufferParameteriv = (CL_GLFunctions::ptr_glGetBufferParameterivARB) CL_OpenGL::get_proc_address("glGetBufferParameterivARB");
	functions->getBufferPointerv = (CL_GLFunctions::ptr_glGetBufferPointervARB) CL_OpenGL::get_proc_address("glGetBufferPointervARB");
	functions->genQueries = (CL_GLFunctions::ptr_glGenQueriesARB) CL_OpenGL::get_proc_address("glGenQueriesARB");
	functions->deleteQueries = (CL_GLFunctions::ptr_glDeleteQueriesARB) CL_OpenGL::get_proc_address("glDeleteQueriesARB");
	functions->isQuery = (CL_GLFunctions::ptr_glIsQueryARB) CL_OpenGL::get_proc_address("glIsQueryARB");
	functions->beginQuery = (CL_GLFunctions::ptr_glBeginQueryARB) CL_OpenGL::get_proc_address("glBeginQueryARB");
	functions->endQuery = (CL_GLFunctions::ptr_glEndQueryARB) CL_OpenGL::get_proc_address("glEndQueryARB");
	functions->getQueryiv = (CL_GLFunctions::ptr_glGetQueryivARB) CL_OpenGL::get_proc_address("glGetQueryivARB");
	functions->getQueryObjectiv = (CL_GLFunctions::ptr_glGetQueryObjectivARB) CL_OpenGL::get_proc_address("glGetQueryObjectivARB");
	functions->getQueryObjectuiv = (CL_GLFunctions::ptr_glGetQueryObjectuivARB) CL_OpenGL::get_proc_address("glGetQueryObjectuivARB");
#endif

	// Binds for OpenGL 2.0:

#ifdef __NOBODY_YET_HEHE__
	functions->deleteShader = (CL_GLFunctions::ptr_glDeleteObjectARB) &glDeleteShader;
	functions->deleteProgram = (CL_GLFunctions::ptr_glDeleteObjectARB) &glDeleteProgram;
	functions->getHandle = (CL_GLFunctions::ptr_glGetHandleARB) &glGetHandle;
	functions->detachShader = (CL_GLFunctions::ptr_glDetachObjectARB) &glDetachShader;
	functions->createShader = (CL_GLFunctions::ptr_glCreateShaderObjectARB) &glCreateShader;
	functions->shaderSource = (CL_GLFunctions::ptr_glShaderSourceARB) &glShaderSource;
	functions->compileShader = (CL_GLFunctions::ptr_glCompileShaderARB) &glCompileShader;
	functions->createProgram = (CL_GLFunctions::ptr_glCreateProgramObjectARB) &glCreateProgram;
	functions->attachShader = (CL_GLFunctions::ptr_glAttachObjectARB) &glAttachShader;
	functions->linkProgram = (CL_GLFunctions::ptr_glLinkProgramARB) &glLinkProgram;
	functions->useProgram = (CL_GLFunctions::ptr_glUseProgramObjectARB) &glUseProgram;
	functions->validateProgram = (CL_GLFunctions::ptr_glValidateProgramARB) &glValidateProgram;
	functions->uniform1f = (CL_GLFunctions::ptr_glUniform1fARB) &glUniform1f;
	functions->uniform2f = (CL_GLFunctions::ptr_glUniform2fARB) &glUniform2f;
	functions->uniform3f = (CL_GLFunctions::ptr_glUniform3fARB) &glUniform3f;
	functions->uniform4f = (CL_GLFunctions::ptr_glUniform4fARB) &glUniform4f;
	functions->uniform1i = (CL_GLFunctions::ptr_glUniform1iARB) &glUniform1i;
	functions->uniform2i = (CL_GLFunctions::ptr_glUniform2iARB) &glUniform2i;
	functions->uniform3i = (CL_GLFunctions::ptr_glUniform3iARB) &glUniform3i;
	functions->uniform4i = (CL_GLFunctions::ptr_glUniform4iARB) &glUniform4i;
	functions->uniform1fv = (CL_GLFunctions::ptr_glUniform1fvARB) &glUniform1fv;
	functions->uniform2fv = (CL_GLFunctions::ptr_glUniform2fvARB) &glUniform2fv;
	functions->uniform3fv = (CL_GLFunctions::ptr_glUniform3fvARB) &glUniform3fv;
	functions->uniform4fv = (CL_GLFunctions::ptr_glUniform4fvARB) &glUniform4fv;
	functions->uniform1iv = (CL_GLFunctions::ptr_glUniform1ivARB) &glUniform1iv;
	functions->uniform2iv = (CL_GLFunctions::ptr_glUniform2ivARB) &glUniform2iv;
	functions->uniform3iv = (CL_GLFunctions::ptr_glUniform3ivARB) &glUniform3iv;
	functions->uniform4iv = (CL_GLFunctions::ptr_glUniform4ivARB) &glUniform4iv;
	functions->uniformMatrix2fv = (CL_GLFunctions::ptr_glUniformMatrix2fvARB) &glUniformMatrix2fv;
	functions->uniformMatrix3fv = (CL_GLFunctions::ptr_glUniformMatrix3fvARB) &glUniformMatrix3fv;
	functions->uniformMatrix4fv = (CL_GLFunctions::ptr_glUniformMatrix4fvARB) &glUniformMatrix4fv;
	functions->getProgramfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) &glProgramfv;
	functions->getProgramiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) &glProgramiv;
	functions->getShaderfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) &glShaderfv;
	functions->getShaderiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) &glShaderiv;
	functions->getShaderInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) &glGetShaderInfoLog;
	functions->getProgramInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) &glGetProgramInfoLog;
	functions->getAttachedShaders = (CL_GLFunctions::ptr_glGetAttachedObjectsARB) &glGetAttachedShaders;
	functions->getUniformLocation = (CL_GLFunctions::ptr_glGetUniformLocationARB) &glGetUniformLocation;
	functions->getActiveUniform = (CL_GLFunctions::ptr_glGetActiveUniformARB) &glGetActiveUniform;
	functions->getUniformfv = (CL_GLFunctions::ptr_glGetUniformfvARB) &glGetUniformfv;
	functions->getUniformiv = (CL_GLFunctions::ptr_glGetUniformivARB) &glGetUniformiv;
	functions->getShaderSource = (CL_GLFunctions::ptr_glGetShaderSourceARB) &glGetShaderSource;
	functions->vertexAttrib1f = (CL_GLFunctions::ptr_glVertexAttrib1fARB) &glVertexAttrib1f;
	functions->vertexAttrib1s = (CL_GLFunctions::ptr_glVertexAttrib1sARB) &glVertexAttrib1s;
	functions->vertexAttrib1d = (CL_GLFunctions::ptr_glVertexAttrib1dARB) &glVertexAttrib1d;
	functions->vertexAttrib2f = (CL_GLFunctions::ptr_glVertexAttrib2fARB) &glVertexAttrib2f;
	functions->vertexAttrib2s = (CL_GLFunctions::ptr_glVertexAttrib2sARB) &glVertexAttrib2s;
	functions->vertexAttrib2d = (CL_GLFunctions::ptr_glVertexAttrib2dARB) &glVertexAttrib2d;
	functions->vertexAttrib3f = (CL_GLFunctions::ptr_glVertexAttrib3fARB) &glVertexAttrib3f;
	functions->vertexAttrib3s = (CL_GLFunctions::ptr_glVertexAttrib3sARB) &glVertexAttrib3s;
	functions->vertexAttrib3d = (CL_GLFunctions::ptr_glVertexAttrib3dARB) &glVertexAttrib3d;
	functions->vertexAttrib4f = (CL_GLFunctions::ptr_glVertexAttrib4fARB) &glVertexAttrib4f;
	functions->vertexAttrib4s = (CL_GLFunctions::ptr_glVertexAttrib4sARB) &glVertexAttrib4s;
	functions->vertexAttrib4d = (CL_GLFunctions::ptr_glVertexAttrib4dARB) &glVertexAttrib4d;
	functions->vertexAttrib4Nub = (CL_GLFunctions::ptr_glVertexAttrib4NubARB) &glVertexAttrib4Nub;
	functions->vertexAttrib1fv = (CL_GLFunctions::ptr_glVertexAttrib1fvARB) &glVertexAttrib1fv;
	functions->vertexAttrib1sv = (CL_GLFunctions::ptr_glVertexAttrib1svARB) &glVertexAttrib1sv;
	functions->vertexAttrib1dv = (CL_GLFunctions::ptr_glVertexAttrib1dvARB) &glVertexAttrib1dv;
	functions->vertexAttrib2fv = (CL_GLFunctions::ptr_glVertexAttrib2fvARB) &glVertexAttrib2fv;
	functions->vertexAttrib2sv = (CL_GLFunctions::ptr_glVertexAttrib2svARB) &glVertexAttrib2sv;
	functions->vertexAttrib2dv = (CL_GLFunctions::ptr_glVertexAttrib2dvARB) &glVertexAttrib2dv;
	functions->vertexAttrib3fv = (CL_GLFunctions::ptr_glVertexAttrib3fvARB) &glVertexAttrib3fv;
	functions->vertexAttrib3sv = (CL_GLFunctions::ptr_glVertexAttrib3svARB) &glVertexAttrib3sv;
	functions->activeStencilFaceEXT =(CL_GLFunctions::ptr_glActiveStencilFaceEXT) &glActiveStencilFace;
	functions->vertexAttrib3dv = (CL_GLFunctions::ptr_glVertexAttrib3dvARB) &glVertexAttrib3dv;
	functions->vertexAttrib4fv = (CL_GLFunctions::ptr_glVertexAttrib4fvARB) &glVertexAttrib4fv;
	functions->vertexAttrib4sv = (CL_GLFunctions::ptr_glVertexAttrib4svARB) &glVertexAttrib4sv;
	functions->vertexAttrib4dv = (CL_GLFunctions::ptr_glVertexAttrib4dvARB) &glVertexAttrib4dv;
	functions->vertexAttrib4iv = (CL_GLFunctions::ptr_glVertexAttrib4ivARB) &glVertexAttrib4iv;
	functions->vertexAttrib4bv = (CL_GLFunctions::ptr_glVertexAttrib4bvARB) &glVertexAttrib4bv;
	functions->vertexAttrib4ubv = (CL_GLFunctions::ptr_glVertexAttrib4ubvARB) &glVertexAttrib4ubv;
	functions->vertexAttrib4usv = (CL_GLFunctions::ptr_glVertexAttrib4usvARB) &glVertexAttrib4usv;
	functions->vertexAttrib4uiv = (CL_GLFunctions::ptr_glVertexAttrib4uivARB) &glVertexAttrib4uiv;
	functions->vertexAttrib4Nbv = (CL_GLFunctions::ptr_glVertexAttrib4NbvARB) &glVertexAttrib4Nbv;
	functions->vertexAttrib4Nsv = (CL_GLFunctions::ptr_glVertexAttrib4NsvARB) &glVertexAttrib4Nsv;
	functions->vertexAttrib4Niv = (CL_GLFunctions::ptr_glVertexAttrib4NivARB) &glVertexAttrib4Niv;
	functions->vertexAttrib4Nubv = (CL_GLFunctions::ptr_glVertexAttrib4NubvARB) &glVertexAttrib4Nubv;
	functions->vertexAttrib4Nusv = (CL_GLFunctions::ptr_glVertexAttrib4NusvARB) &glVertexAttrib4Nusv;
	functions->vertexAttrib4Nuiv = (CL_GLFunctions::ptr_glVertexAttrib4NuivARB) &glVertexAttrib4Nuiv;
	functions->vertexAttribPointer = (CL_GLFunctions::ptr_glVertexAttribPointerARB) &glVertexAttribPointer;
	functions->enableVertexAttribArray = (CL_GLFunctions::ptr_glEnableVertexAttribArrayARB) &glEnableVertexAttribArray;
	functions->disableVertexAttribArray = (CL_GLFunctions::ptr_glDisableVertexAttribArrayARB) &glDisableVertexAttribArray;
	functions->bindAttribLocation = (CL_GLFunctions::ptr_glBindAttribLocationARB) &glBindAttribLocation;
	functions->getActiveAttrib = (CL_GLFunctions::ptr_glGetActiveAttribARB) &glGetActiveAttrib;
	functions->getAttribLocation = (CL_GLFunctions::ptr_glGetAttribLocationARB) &glGetAttribLocation;
	functions->getVertexAttribdv = (CL_GLFunctions::ptr_glGetVertexAttribdvARB) &glGetVertexAttribdv;
	functions->getVertexAttribfv = (CL_GLFunctions::ptr_glGetVertexAttribfvARB) &glGetVertexAttribfv;
	functions->getVertexAttribiv = (CL_GLFunctions::ptr_glGetVertexAttribivARB) &glGetVertexAttribiv;
	functions->getVertexAttribPointerv = (CL_GLFunctions::ptr_glGetVertexAttribPointervARB) &glGetVertexAttribPointerv;
	functions->drawBuffers = (CL_GLFunctions::ptr_glDrawBuffersARB) &glDrawBuffers;
	functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparateATI) &glStencilFuncSeparate;
	functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparateATI) &glStencilOpSeparate;
#elif __APPLE__
	functions->deleteShader = (CL_GLFunctions::ptr_glDeleteObjectARB) CL_OpenGL::get_proc_address("glDeleteShader");
	functions->deleteProgram = (CL_GLFunctions::ptr_glDeleteObjectARB) CL_OpenGL::get_proc_address("glDeleteProgram");
	functions->getHandle = (CL_GLFunctions::ptr_glGetHandleARB) CL_OpenGL::get_proc_address("glGetHandle");
	functions->detachShader = (CL_GLFunctions::ptr_glDetachObjectARB) CL_OpenGL::get_proc_address("glDetachShader");
	functions->createShader = (CL_GLFunctions::ptr_glCreateShaderObjectARB) CL_OpenGL::get_proc_address("glCreateShader");
	functions->shaderSource = (CL_GLFunctions::ptr_glShaderSourceARB) CL_OpenGL::get_proc_address("glShaderSource");
	functions->compileShader = (CL_GLFunctions::ptr_glCompileShaderARB) CL_OpenGL::get_proc_address("glCompileShader");
	functions->createProgram = (CL_GLFunctions::ptr_glCreateProgramObjectARB) CL_OpenGL::get_proc_address("glCreateProgram");
	functions->attachShader = (CL_GLFunctions::ptr_glAttachObjectARB) CL_OpenGL::get_proc_address("glAttachShader");
	functions->linkProgram = (CL_GLFunctions::ptr_glLinkProgramARB) CL_OpenGL::get_proc_address("glLinkProgram");
	functions->useProgram = (CL_GLFunctions::ptr_glUseProgramObjectARB) CL_OpenGL::get_proc_address("glUseProgram");
	functions->validateProgram = (CL_GLFunctions::ptr_glValidateProgramARB) CL_OpenGL::get_proc_address("glValidateProgram");
	functions->uniform1f = (CL_GLFunctions::ptr_glUniform1fARB) CL_OpenGL::get_proc_address("glUniform1f");
	functions->uniform2f = (CL_GLFunctions::ptr_glUniform2fARB) CL_OpenGL::get_proc_address("glUniform2f");
	functions->uniform3f = (CL_GLFunctions::ptr_glUniform3fARB) CL_OpenGL::get_proc_address("glUniform3f");
	functions->uniform4f = (CL_GLFunctions::ptr_glUniform4fARB) CL_OpenGL::get_proc_address("glUniform4f");
	functions->uniform1i = (CL_GLFunctions::ptr_glUniform1iARB) CL_OpenGL::get_proc_address("glUniform1i");
	functions->uniform2i = (CL_GLFunctions::ptr_glUniform2iARB) CL_OpenGL::get_proc_address("glUniform2i");
	functions->uniform3i = (CL_GLFunctions::ptr_glUniform3iARB) CL_OpenGL::get_proc_address("glUniform3i");
	functions->uniform4i = (CL_GLFunctions::ptr_glUniform4iARB) CL_OpenGL::get_proc_address("glUniform4i");
	functions->uniform1fv = (CL_GLFunctions::ptr_glUniform1fvARB) CL_OpenGL::get_proc_address("glUniform1fv");
	functions->uniform2fv = (CL_GLFunctions::ptr_glUniform2fvARB) CL_OpenGL::get_proc_address("glUniform2fv");
	functions->uniform3fv = (CL_GLFunctions::ptr_glUniform3fvARB) CL_OpenGL::get_proc_address("glUniform3fv");
	functions->uniform4fv = (CL_GLFunctions::ptr_glUniform4fvARB) CL_OpenGL::get_proc_address("glUniform4fv");
	functions->uniform1iv = (CL_GLFunctions::ptr_glUniform1ivARB) CL_OpenGL::get_proc_address("glUniform1iv");
	functions->uniform2iv = (CL_GLFunctions::ptr_glUniform2ivARB) CL_OpenGL::get_proc_address("glUniform2iv");
	functions->uniform3iv = (CL_GLFunctions::ptr_glUniform3ivARB) CL_OpenGL::get_proc_address("glUniform3iv");
	functions->uniform4iv = (CL_GLFunctions::ptr_glUniform4ivARB) CL_OpenGL::get_proc_address("glUniform4iv");
	functions->uniformMatrix2fv = (CL_GLFunctions::ptr_glUniformMatrix2fvARB) CL_OpenGL::get_proc_address("glUniformMatrix2fv");
	functions->uniformMatrix3fv = (CL_GLFunctions::ptr_glUniformMatrix3fvARB) CL_OpenGL::get_proc_address("glUniformMatrix3fv");
	functions->uniformMatrix4fv = (CL_GLFunctions::ptr_glUniformMatrix4fvARB) CL_OpenGL::get_proc_address("glUniformMatrix4fv");
	functions->getProgramfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) CL_OpenGL::get_proc_address("glGetProgramfv");
	functions->getProgramiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) CL_OpenGL::get_proc_address("glGetProgramiv");
	functions->getShaderfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) CL_OpenGL::get_proc_address("glGetShaderfv");
	functions->getShaderiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) CL_OpenGL::get_proc_address("glGetShaderiv");
	functions->getShaderInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) CL_OpenGL::get_proc_address("glGetShaderInfoLog");
	functions->getProgramInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) CL_OpenGL::get_proc_address("glGetProgramInfoLog");
	functions->getAttachedShaders = (CL_GLFunctions::ptr_glGetAttachedObjectsARB) CL_OpenGL::get_proc_address("glGetAttachedShaders");
	functions->getUniformLocation = (CL_GLFunctions::ptr_glGetUniformLocationARB) CL_OpenGL::get_proc_address("glGetUniformLocation");
	functions->getActiveUniform = (CL_GLFunctions::ptr_glGetActiveUniformARB) CL_OpenGL::get_proc_address("glGetActiveUniform");
	functions->getUniformfv = (CL_GLFunctions::ptr_glGetUniformfvARB) CL_OpenGL::get_proc_address("glGetUniformfv");
	functions->getUniformiv = (CL_GLFunctions::ptr_glGetUniformivARB) CL_OpenGL::get_proc_address("glGetUniformiv");
	functions->getShaderSource = (CL_GLFunctions::ptr_glGetShaderSourceARB) CL_OpenGL::get_proc_address("glGetShaderSource");
	functions->vertexAttrib1f = (CL_GLFunctions::ptr_glVertexAttrib1fARB) CL_OpenGL::get_proc_address("glVertexAttrib1f");
	functions->vertexAttrib1s = (CL_GLFunctions::ptr_glVertexAttrib1sARB) CL_OpenGL::get_proc_address("glVertexAttrib1s");
	functions->vertexAttrib1d = (CL_GLFunctions::ptr_glVertexAttrib1dARB) CL_OpenGL::get_proc_address("glVertexAttrib1d");
	functions->vertexAttrib2f = (CL_GLFunctions::ptr_glVertexAttrib2fARB) CL_OpenGL::get_proc_address("glVertexAttrib2f");
	functions->vertexAttrib2s = (CL_GLFunctions::ptr_glVertexAttrib2sARB) CL_OpenGL::get_proc_address("glVertexAttrib2s");
	functions->vertexAttrib2d = (CL_GLFunctions::ptr_glVertexAttrib2dARB) CL_OpenGL::get_proc_address("glVertexAttrib2d");
	functions->vertexAttrib3f = (CL_GLFunctions::ptr_glVertexAttrib3fARB) CL_OpenGL::get_proc_address("glVertexAttrib3f");
	functions->vertexAttrib3s = (CL_GLFunctions::ptr_glVertexAttrib3sARB) CL_OpenGL::get_proc_address("glVertexAttrib3s");
	functions->vertexAttrib3d = (CL_GLFunctions::ptr_glVertexAttrib3dARB) CL_OpenGL::get_proc_address("glVertexAttrib3d");
	functions->vertexAttrib4f = (CL_GLFunctions::ptr_glVertexAttrib4fARB) CL_OpenGL::get_proc_address("glVertexAttrib4f");
	functions->vertexAttrib4s = (CL_GLFunctions::ptr_glVertexAttrib4sARB) CL_OpenGL::get_proc_address("glVertexAttrib4s");
	functions->vertexAttrib4d = (CL_GLFunctions::ptr_glVertexAttrib4dARB) CL_OpenGL::get_proc_address("glVertexAttrib4d");
	functions->vertexAttrib4Nub = (CL_GLFunctions::ptr_glVertexAttrib4NubARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nub");
	functions->vertexAttrib1fv = (CL_GLFunctions::ptr_glVertexAttrib1fvARB) CL_OpenGL::get_proc_address("glVertexAttrib1fv");
	functions->vertexAttrib1sv = (CL_GLFunctions::ptr_glVertexAttrib1svARB) CL_OpenGL::get_proc_address("glVertexAttrib1sv");
	functions->vertexAttrib1dv = (CL_GLFunctions::ptr_glVertexAttrib1dvARB) CL_OpenGL::get_proc_address("glVertexAttrib1dv");
	functions->vertexAttrib2fv = (CL_GLFunctions::ptr_glVertexAttrib2fvARB) CL_OpenGL::get_proc_address("glVertexAttrib2fv");
	functions->vertexAttrib2sv = (CL_GLFunctions::ptr_glVertexAttrib2svARB) CL_OpenGL::get_proc_address("glVertexAttrib2sv");
	functions->vertexAttrib2dv = (CL_GLFunctions::ptr_glVertexAttrib2dvARB) CL_OpenGL::get_proc_address("glVertexAttrib2dv");
	functions->vertexAttrib3fv = (CL_GLFunctions::ptr_glVertexAttrib3fvARB) CL_OpenGL::get_proc_address("glVertexAttrib3fv");
	functions->vertexAttrib3sv = (CL_GLFunctions::ptr_glVertexAttrib3svARB) CL_OpenGL::get_proc_address("glVertexAttrib3sv");
	functions->vertexAttrib3dv = (CL_GLFunctions::ptr_glVertexAttrib3dvARB) CL_OpenGL::get_proc_address("glVertexAttrib3dv");
	functions->vertexAttrib4fv = (CL_GLFunctions::ptr_glVertexAttrib4fvARB) CL_OpenGL::get_proc_address("glVertexAttrib4fv");
	functions->vertexAttrib4sv = (CL_GLFunctions::ptr_glVertexAttrib4svARB) CL_OpenGL::get_proc_address("glVertexAttrib4sv");
	functions->vertexAttrib4dv = (CL_GLFunctions::ptr_glVertexAttrib4dvARB) CL_OpenGL::get_proc_address("glVertexAttrib4dv");
	functions->vertexAttrib4iv = (CL_GLFunctions::ptr_glVertexAttrib4ivARB) CL_OpenGL::get_proc_address("glVertexAttrib4iv");
	functions->vertexAttrib4bv = (CL_GLFunctions::ptr_glVertexAttrib4bvARB) CL_OpenGL::get_proc_address("glVertexAttrib4bv");
	functions->vertexAttrib4ubv = (CL_GLFunctions::ptr_glVertexAttrib4ubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4ubv");
	functions->vertexAttrib4usv = (CL_GLFunctions::ptr_glVertexAttrib4usvARB) CL_OpenGL::get_proc_address("glVertexAttrib4usv");
	functions->vertexAttrib4uiv = (CL_GLFunctions::ptr_glVertexAttrib4uivARB) CL_OpenGL::get_proc_address("glVertexAttrib4uiv");
	functions->vertexAttrib4Nbv = (CL_GLFunctions::ptr_glVertexAttrib4NbvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nbv");
	functions->vertexAttrib4Nsv = (CL_GLFunctions::ptr_glVertexAttrib4NsvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nsv");
	functions->vertexAttrib4Niv = (CL_GLFunctions::ptr_glVertexAttrib4NivARB) CL_OpenGL::get_proc_address("glVertexAttrib4Niv");
	functions->vertexAttrib4Nubv = (CL_GLFunctions::ptr_glVertexAttrib4NubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nubv");
	functions->vertexAttrib4Nusv = (CL_GLFunctions::ptr_glVertexAttrib4NusvARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nusv");
	functions->vertexAttrib4Nuiv = (CL_GLFunctions::ptr_glVertexAttrib4NuivARB) CL_OpenGL::get_proc_address("glVertexAttrib4Nuiv");
	functions->vertexAttribPointer = (CL_GLFunctions::ptr_glVertexAttribPointerARB) CL_OpenGL::get_proc_address("glVertexAttribPointer");
	functions->enableVertexAttribArray = (CL_GLFunctions::ptr_glEnableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glEnableVertexAttribArray");
	functions->disableVertexAttribArray = (CL_GLFunctions::ptr_glDisableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glDisableVertexAttribArray");
	functions->bindAttribLocation = (CL_GLFunctions::ptr_glBindAttribLocationARB) CL_OpenGL::get_proc_address("glBindAttribLocation");
	functions->getActiveAttrib = (CL_GLFunctions::ptr_glGetActiveAttribARB) CL_OpenGL::get_proc_address("glGetActiveAttrib");
	functions->getAttribLocation = (CL_GLFunctions::ptr_glGetAttribLocationARB) CL_OpenGL::get_proc_address("glGetAttribLocation");
	functions->getVertexAttribdv = (CL_GLFunctions::ptr_glGetVertexAttribdvARB) CL_OpenGL::get_proc_address("glGetVertexAttribdv");
	functions->getVertexAttribfv = (CL_GLFunctions::ptr_glGetVertexAttribfvARB) CL_OpenGL::get_proc_address("glGetVertexAttribfv");
	functions->getVertexAttribiv = (CL_GLFunctions::ptr_glGetVertexAttribivARB) CL_OpenGL::get_proc_address("glGetVertexAttribiv");
	functions->getVertexAttribPointerv = (CL_GLFunctions::ptr_glGetVertexAttribPointervARB) CL_OpenGL::get_proc_address("glGetVertexAttribPointerv");
	functions->drawBuffers = (CL_GLFunctions::ptr_glDrawBuffersARB) CL_OpenGL::get_proc_address("glDrawBuffers");
	functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparateATI) CL_OpenGL::get_proc_address("glStencilFuncSeparate");
	functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparateATI) CL_OpenGL::get_proc_address("glStencilOpSeparate");
#else
	functions->deleteShader = (CL_GLFunctions::ptr_glDeleteObjectARB) CL_OpenGL::get_proc_address("glDeleteObjectARB");
	functions->deleteProgram = functions->deleteShader;
	functions->getHandle = (CL_GLFunctions::ptr_glGetHandleARB) CL_OpenGL::get_proc_address("glGetHandleARB");
	functions->detachShader = (CL_GLFunctions::ptr_glDetachObjectARB) CL_OpenGL::get_proc_address("glDetachObjectARB");
	functions->createShader = (CL_GLFunctions::ptr_glCreateShaderObjectARB) CL_OpenGL::get_proc_address("glCreateShaderObjectARB");
	functions->shaderSource = (CL_GLFunctions::ptr_glShaderSourceARB) CL_OpenGL::get_proc_address("glShaderSourceARB");
	functions->compileShader = (CL_GLFunctions::ptr_glCompileShaderARB) CL_OpenGL::get_proc_address("glCompileShaderARB");
	functions->createProgram = (CL_GLFunctions::ptr_glCreateProgramObjectARB) CL_OpenGL::get_proc_address("glCreateProgramObjectARB");
	functions->attachShader = (CL_GLFunctions::ptr_glAttachObjectARB) CL_OpenGL::get_proc_address("glAttachObjectARB");
	functions->linkProgram = (CL_GLFunctions::ptr_glLinkProgramARB) CL_OpenGL::get_proc_address("glLinkProgramARB");
	functions->useProgram = (CL_GLFunctions::ptr_glUseProgramObjectARB) CL_OpenGL::get_proc_address("glUseProgramObjectARB");
	functions->validateProgram = (CL_GLFunctions::ptr_glValidateProgramARB) CL_OpenGL::get_proc_address("glValidateProgramARB");
	functions->uniform1f = (CL_GLFunctions::ptr_glUniform1fARB) CL_OpenGL::get_proc_address("glUniform1fARB");
	functions->uniform2f = (CL_GLFunctions::ptr_glUniform2fARB) CL_OpenGL::get_proc_address("glUniform2fARB");
	functions->uniform3f = (CL_GLFunctions::ptr_glUniform3fARB) CL_OpenGL::get_proc_address("glUniform3fARB");
	functions->uniform4f = (CL_GLFunctions::ptr_glUniform4fARB) CL_OpenGL::get_proc_address("glUniform4fARB");
	functions->uniform1i = (CL_GLFunctions::ptr_glUniform1iARB) CL_OpenGL::get_proc_address("glUniform1iARB");
	functions->uniform2i = (CL_GLFunctions::ptr_glUniform2iARB) CL_OpenGL::get_proc_address("glUniform2iARB");
	functions->uniform3i = (CL_GLFunctions::ptr_glUniform3iARB) CL_OpenGL::get_proc_address("glUniform3iARB");
	functions->uniform4i = (CL_GLFunctions::ptr_glUniform4iARB) CL_OpenGL::get_proc_address("glUniform4iARB");
	functions->uniform1fv = (CL_GLFunctions::ptr_glUniform1fvARB) CL_OpenGL::get_proc_address("glUniform1fvARB");
	functions->uniform2fv = (CL_GLFunctions::ptr_glUniform2fvARB) CL_OpenGL::get_proc_address("glUniform2fvARB");
	functions->uniform3fv = (CL_GLFunctions::ptr_glUniform3fvARB) CL_OpenGL::get_proc_address("glUniform3fvARB");
	functions->uniform4fv = (CL_GLFunctions::ptr_glUniform4fvARB) CL_OpenGL::get_proc_address("glUniform4fvARB");
	functions->uniform1iv = (CL_GLFunctions::ptr_glUniform1ivARB) CL_OpenGL::get_proc_address("glUniform1ivARB");
	functions->uniform2iv = (CL_GLFunctions::ptr_glUniform2ivARB) CL_OpenGL::get_proc_address("glUniform2ivARB");
	functions->uniform3iv = (CL_GLFunctions::ptr_glUniform3ivARB) CL_OpenGL::get_proc_address("glUniform3ivARB");
	functions->uniform4iv = (CL_GLFunctions::ptr_glUniform4ivARB) CL_OpenGL::get_proc_address("glUniform4ivARB");
	functions->uniformMatrix2fv = (CL_GLFunctions::ptr_glUniformMatrix2fvARB) CL_OpenGL::get_proc_address("glUniformMatrix2fvARB");
	functions->uniformMatrix3fv = (CL_GLFunctions::ptr_glUniformMatrix3fvARB) CL_OpenGL::get_proc_address("glUniformMatrix3fvARB");
	functions->uniformMatrix4fv = (CL_GLFunctions::ptr_glUniformMatrix4fvARB) CL_OpenGL::get_proc_address("glUniformMatrix4fvARB");
	functions->getProgramfv = (CL_GLFunctions::ptr_glGetObjectParameterfvARB) CL_OpenGL::get_proc_address("glGetObjectParameterfvARB");
	functions->getProgramiv = (CL_GLFunctions::ptr_glGetObjectParameterivARB) CL_OpenGL::get_proc_address("glGetObjectParameterivARB");
	functions->getShaderfv = functions->getProgramfv;
	functions->getShaderiv = functions->getProgramiv;
	functions->getShaderInfoLog = (CL_GLFunctions::ptr_glGetInfoLogARB) CL_OpenGL::get_proc_address("glGetInfoLogARB");
	functions->getProgramInfoLog = functions->getShaderInfoLog;
	functions->getAttachedShaders = (CL_GLFunctions::ptr_glGetAttachedObjectsARB) CL_OpenGL::get_proc_address("glGetAttachedObjectsARB");
	functions->getUniformLocation = (CL_GLFunctions::ptr_glGetUniformLocationARB) CL_OpenGL::get_proc_address("glGetUniformLocationARB");
	functions->getActiveUniform = (CL_GLFunctions::ptr_glGetActiveUniformARB) CL_OpenGL::get_proc_address("glGetActiveUniformARB");
	functions->getUniformfv = (CL_GLFunctions::ptr_glGetUniformfvARB) CL_OpenGL::get_proc_address("glGetUniformfvARB");
	functions->getUniformiv = (CL_GLFunctions::ptr_glGetUniformivARB) CL_OpenGL::get_proc_address("glGetUniformivARB");
	functions->getShaderSource = (CL_GLFunctions::ptr_glGetShaderSourceARB) CL_OpenGL::get_proc_address("glGetShaderSourceARB");
	functions->vertexAttrib1f = (CL_GLFunctions::ptr_glVertexAttrib1fARB) CL_OpenGL::get_proc_address("glVertexAttrib1fARB");
	functions->vertexAttrib1s = (CL_GLFunctions::ptr_glVertexAttrib1sARB) CL_OpenGL::get_proc_address("glVertexAttrib1sARB");
	functions->vertexAttrib1d = (CL_GLFunctions::ptr_glVertexAttrib1dARB) CL_OpenGL::get_proc_address("glVertexAttrib1dARB");
	functions->vertexAttrib2f = (CL_GLFunctions::ptr_glVertexAttrib2fARB) CL_OpenGL::get_proc_address("glVertexAttrib2fARB");
	functions->vertexAttrib2s = (CL_GLFunctions::ptr_glVertexAttrib2sARB) CL_OpenGL::get_proc_address("glVertexAttrib2sARB");
	functions->vertexAttrib2d = (CL_GLFunctions::ptr_glVertexAttrib2dARB) CL_OpenGL::get_proc_address("glVertexAttrib2dARB");
	functions->vertexAttrib3f = (CL_GLFunctions::ptr_glVertexAttrib3fARB) CL_OpenGL::get_proc_address("glVertexAttrib3fARB");
	functions->vertexAttrib3s = (CL_GLFunctions::ptr_glVertexAttrib3sARB) CL_OpenGL::get_proc_address("glVertexAttrib3sARB");
	functions->vertexAttrib3d = (CL_GLFunctions::ptr_glVertexAttrib3dARB) CL_OpenGL::get_proc_address("glVertexAttrib3dARB");
	functions->vertexAttrib4f = (CL_GLFunctions::ptr_glVertexAttrib4fARB) CL_OpenGL::get_proc_address("glVertexAttrib4fARB");
	functions->vertexAttrib4s = (CL_GLFunctions::ptr_glVertexAttrib4sARB) CL_OpenGL::get_proc_address("glVertexAttrib4sARB");
	functions->vertexAttrib4d = (CL_GLFunctions::ptr_glVertexAttrib4dARB) CL_OpenGL::get_proc_address("glVertexAttrib4dARB");
	functions->vertexAttrib4Nub = (CL_GLFunctions::ptr_glVertexAttrib4NubARB) CL_OpenGL::get_proc_address("glVertexAttrib4NubARB");
	functions->vertexAttrib1fv = (CL_GLFunctions::ptr_glVertexAttrib1fvARB) CL_OpenGL::get_proc_address("glVertexAttrib1fvARB");
	functions->vertexAttrib1sv = (CL_GLFunctions::ptr_glVertexAttrib1svARB) CL_OpenGL::get_proc_address("glVertexAttrib1svARB");
	functions->vertexAttrib1dv = (CL_GLFunctions::ptr_glVertexAttrib1dvARB) CL_OpenGL::get_proc_address("glVertexAttrib1dvARB");
	functions->vertexAttrib2fv = (CL_GLFunctions::ptr_glVertexAttrib2fvARB) CL_OpenGL::get_proc_address("glVertexAttrib2fvARB");
	functions->vertexAttrib2sv = (CL_GLFunctions::ptr_glVertexAttrib2svARB) CL_OpenGL::get_proc_address("glVertexAttrib2svARB");
	functions->vertexAttrib2dv = (CL_GLFunctions::ptr_glVertexAttrib2dvARB) CL_OpenGL::get_proc_address("glVertexAttrib2dvARB");
	functions->vertexAttrib3fv = (CL_GLFunctions::ptr_glVertexAttrib3fvARB) CL_OpenGL::get_proc_address("glVertexAttrib3fvARB");
	functions->vertexAttrib3sv = (CL_GLFunctions::ptr_glVertexAttrib3svARB) CL_OpenGL::get_proc_address("glVertexAttrib3svARB");
	functions->vertexAttrib3dv = (CL_GLFunctions::ptr_glVertexAttrib3dvARB) CL_OpenGL::get_proc_address("glVertexAttrib3dvARB");
	functions->vertexAttrib4fv = (CL_GLFunctions::ptr_glVertexAttrib4fvARB) CL_OpenGL::get_proc_address("glVertexAttrib4fvARB");
	functions->vertexAttrib4sv = (CL_GLFunctions::ptr_glVertexAttrib4svARB) CL_OpenGL::get_proc_address("glVertexAttrib4svARB");
	functions->vertexAttrib4dv = (CL_GLFunctions::ptr_glVertexAttrib4dvARB) CL_OpenGL::get_proc_address("glVertexAttrib4dvARB");
	functions->vertexAttrib4iv = (CL_GLFunctions::ptr_glVertexAttrib4ivARB) CL_OpenGL::get_proc_address("glVertexAttrib4ivARB");
	functions->vertexAttrib4bv = (CL_GLFunctions::ptr_glVertexAttrib4bvARB) CL_OpenGL::get_proc_address("glVertexAttrib4bvARB");
	functions->vertexAttrib4ubv = (CL_GLFunctions::ptr_glVertexAttrib4ubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4ubvARB");
	functions->vertexAttrib4usv = (CL_GLFunctions::ptr_glVertexAttrib4usvARB) CL_OpenGL::get_proc_address("glVertexAttrib4usvARB");
	functions->vertexAttrib4uiv = (CL_GLFunctions::ptr_glVertexAttrib4uivARB) CL_OpenGL::get_proc_address("glVertexAttrib4uivARB");
	functions->vertexAttrib4Nbv = (CL_GLFunctions::ptr_glVertexAttrib4NbvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NbvARB");
	functions->vertexAttrib4Nsv = (CL_GLFunctions::ptr_glVertexAttrib4NsvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NsvARB");
	functions->vertexAttrib4Niv = (CL_GLFunctions::ptr_glVertexAttrib4NivARB) CL_OpenGL::get_proc_address("glVertexAttrib4NivARB");
	functions->vertexAttrib4Nubv = (CL_GLFunctions::ptr_glVertexAttrib4NubvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NubvARB");
	functions->vertexAttrib4Nusv = (CL_GLFunctions::ptr_glVertexAttrib4NusvARB) CL_OpenGL::get_proc_address("glVertexAttrib4NusvARB");
	functions->vertexAttrib4Nuiv = (CL_GLFunctions::ptr_glVertexAttrib4NuivARB) CL_OpenGL::get_proc_address("glVertexAttrib4NuivARB");
	functions->vertexAttribPointer = (CL_GLFunctions::ptr_glVertexAttribPointerARB) CL_OpenGL::get_proc_address("glVertexAttribPointerARB");
	functions->enableVertexAttribArray = (CL_GLFunctions::ptr_glEnableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glEnableVertexAttribArrayARB");
	functions->disableVertexAttribArray = (CL_GLFunctions::ptr_glDisableVertexAttribArrayARB) CL_OpenGL::get_proc_address("glDisableVertexAttribArrayARB");
	functions->bindAttribLocation = (CL_GLFunctions::ptr_glBindAttribLocationARB) CL_OpenGL::get_proc_address("glBindAttribLocationARB");
	functions->getActiveAttrib = (CL_GLFunctions::ptr_glGetActiveAttribARB) CL_OpenGL::get_proc_address("glGetActiveAttribARB");
	functions->getAttribLocation = (CL_GLFunctions::ptr_glGetAttribLocationARB) CL_OpenGL::get_proc_address("glGetAttribLocationARB");
	functions->getVertexAttribdv = (CL_GLFunctions::ptr_glGetVertexAttribdvARB) CL_OpenGL::get_proc_address("glGetVertexAttribdvARB");
	functions->getVertexAttribfv = (CL_GLFunctions::ptr_glGetVertexAttribfvARB) CL_OpenGL::get_proc_address("glGetVertexAttribfvARB");
	functions->getVertexAttribiv = (CL_GLFunctions::ptr_glGetVertexAttribivARB) CL_OpenGL::get_proc_address("glGetVertexAttribivARB");
	functions->getVertexAttribPointerv = (CL_GLFunctions::ptr_glGetVertexAttribPointervARB) CL_OpenGL::get_proc_address("glGetVertexAttribPointervARB");
	functions->drawBuffers = (CL_GLFunctions::ptr_glDrawBuffersARB) CL_OpenGL::get_proc_address("glDrawBuffersARB");
	functions->stencilFuncSeparate = (CL_GLFunctions::ptr_glStencilFuncSeparateATI) CL_OpenGL::get_proc_address("glStencilFuncSeparateATI");
	functions->stencilOpSeparate = (CL_GLFunctions::ptr_glStencilOpSeparateATI) CL_OpenGL::get_proc_address("glStencilOpSeparateATI");
	functions->activeStencilFaceEXT = (CL_GLFunctions::ptr_glActiveStencilFaceEXT) CL_OpenGL::get_proc_address("glActiveStencilFaceEXT");
#endif

	return functions;
}
