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

#ifndef header_opengl_state_generic
#define header_opengl_state_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include <map>
#ifndef WIN32
#ifdef __APPLE__
#include <AGL/agl.h>
#else
#include <GL/glx.h>
#endif
#endif
#include "API/GL/opengl.h"

#include "API/signals.h"

class CL_OpenGLStateData;
class CL_GraphicContext;
class CL_GraphicContext_OpenGL;
class CL_GLFunctions;

class CL_OpenGLState_Generic
{
//! Construction:
public:
	CL_OpenGLState_Generic(CL_GraphicContext *gc);

	CL_OpenGLState_Generic(CL_GraphicContext_OpenGL *gc_opengl);

	virtual ~CL_OpenGLState_Generic();

//! Attributes:
public:
	CL_GraphicContext_OpenGL *gc_opengl;

	std::list< std::pair<CL_OpenGLStateData *, bool> > data_objects;

	//: Returns true if opengl state is currently active.
	bool is_active() const;

//! Operations:
public:
	void set_active();

	//: Begin window 2D rendering mode.
	//param upside_down: If set to true, (0,0) will be in the lower left corner.
	//- <p> This effectively loads a Orto 2D projection matrix and makes sure
	//- the device are using sane texture parameter values.</p>
	void setup_2d(bool upside_down);

//! Implementation:
private:
	void init();

	//: Updates scissor if opengl state is currently active.
	void on_update_scissor();

	void on_update_modelview();

	void on_update_viewport();

	void flush();

	void save();

	void load();

#ifdef WIN32
	// Selects specified context into opengl state machine.
	static void set_context(HDC drawable, HGLRC context);
#else
#ifdef __APPLE__
	// Selects specified context into opengl state machine.
	static void set_context(AGLContext context);
#else
	// Selects specified context into opengl state machine.
	static void set_context(Display *disp, GLXDrawable drawable, GLXContext context);
#endif
#endif

	static void clear_context (void);

#ifdef WIN32
	//: Active opengl drawable.
	static HDC cur_drawable;

	//: Active opengl context.
	static HGLRC cur_context;

	//: Context to function pointers:
	static std::map<std::pair<HDC, HGLRC>, CL_GLFunctions *> function_map;
#else
#ifdef __APPLE__
	//: Active opengl context.
	static AGLContext cur_context;

	//: Context to function pointers:
	static std::map<AGLContext, CL_GLFunctions *> function_map;
#else
	//: Active opengl drawable.
	static GLXDrawable cur_drawable;

	//: Active opengl context.
	static GLXContext cur_context;

	//: Context to function pointers:
	static std::map<std::pair<GLXDrawable, GLXContext>, CL_GLFunctions *> function_map;
#endif
#endif
	static CL_OpenGLState_Generic *active_state;

	static CL_GLFunctions *setup_binds();

	bool in_2d;

	CLboolean state_texture_2d;

	CLint state_bind_texture_2d;

	CLboolean state_blend;

	CLboolean state_fog;

	CLboolean state_depth_test;

	CLboolean state_scissor_test;

	CLint state_viewport[4];

	CLint state_scissor_box[4];

	CLdouble state_clear_color[4];

	CLint state_blendfunc_sfactor;

	CLint state_blendfunc_dfactor;

	CLint state_blendfunc_sfactor_alpha;

	CLint state_blendfunc_dfactor_alpha;

	CLdouble state_color[4];

	CLint state_vertex_pointer_size;

	CLint state_vertex_pointer_type;

	CLsizei state_vertex_pointer_stride;

	void *state_vertex_pointer;

	CLint state_color_pointer_size;

	CLint state_color_pointer_type;

	CLsizei state_color_pointer_stride;

	void *state_color_pointer;

	CLint state_texcoord_pointer_size;

	CLint state_texcoord_pointer_type;

	CLsizei state_texcoord_pointer_stride;

	void *state_texcoord_pointer;

	CLboolean state_vertex_array;

	CLboolean state_color_array;

	CLboolean state_texture_coord_array;

	CLdouble state_matrix_texture[16];

	CLdouble state_matrix_projection[16];

	CLdouble state_matrix_modelview[16];

	CLint state_matrix_mode;

	CLfloat state_texture_env_mode;

	CLboolean state_stencil_test;

	CLint state_stencil_func;
	
	CLint state_stencil_value_mask;
	
	CLint state_stencil_ref;

	CLint state_stencil_fail;

	CLint state_stencil_pass_depth_pass;

	CLint state_stencil_pass_depth_fail;

	CLint state_alpha_test_func;

	CLdouble state_alpha_test_ref;

	CLboolean state_alpha_test;
	
	CLboolean state_color_write_mask[4];

	CLint state_polygon_mode[2];

	bool is_upside_down;

	bool need_update_viewport;

	CL_SlotContainer slots;

	friend class CL_SetupGL;
};

#endif
