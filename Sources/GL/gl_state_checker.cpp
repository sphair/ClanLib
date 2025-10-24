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
#include "Display/surface_target.h"
#include "Display/triangle_params.h"
#include "GL/gl_state.h"
#include "GL/gl_state_checker.h"
#include "API/Display/blend_func.h"
#include "API/Display/color.h"
#include "API/Display/graphic_context.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/GL/opengl_state.h"
#include "graphic_context_opengl.h"
#include "API/GL/opengl_wrap.h"
#include <cstring>

// Using 0.38 for the texture matrix instead of 0.375 - reduces some slight smooth (rounding errors?)
#define cl_pixelcenter_constant 0.38

/////////////////////////////////////////////////////////////////////////////
// CL_GLStateChecker construction:

CL_GLStateChecker::CL_GLStateChecker(CL_GraphicContext_OpenGL *gc_opengl)
: opengl_state(0), gc_opengl(gc_opengl), first_call(true)
{
	internal_state.blendfunc[0] = blend_src_alpha;
	internal_state.blendfunc[1] = blend_one_minus_src_alpha;
	internal_state.blendfunc[2] = blend_src_alpha;
	internal_state.blendfunc[3] = blend_one_minus_src_alpha;

	internal_state.color[0] = 255;
	internal_state.color[1] = 255;
	internal_state.color[2] = 255;
	internal_state.color[3] = 255;

	// -2 since we want to force the first render to set enable/disable_2d
	internal_state.texture_handle = -2;

	internal_state.fillmode = fillmode_solid;
	
	primitive = CL_DONT_CARE;
	primitive_count = 0;

	stencil_op_pass = CL_INCR;
	stencil_op_fail = CL_KEEP;	
	stencil_write_ref = 0;
	stencil_func_write = CL_ALWAYS;

	opengl_state = new CL_OpenGLState(gc_opengl);
	opengl_state->attach_data(this);

	tri_use_uv = false;
	tri_use_color = false;

	color_array_enabled = false;	
	color_array_range_zero2one = false;
	uv_array_range_zero2one = false;
}

CL_GLStateChecker::~CL_GLStateChecker()
{
	delete opengl_state;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GLStateChecker operations:

CL_GLStateChecker *CL_GLStateChecker::from_gc(CL_GraphicContext *gc)
{
	CL_GraphicContext_OpenGL *gl_impl = (CL_GraphicContext_OpenGL *) gc->impl;
	return gl_impl->state_checker;
}

void CL_GLStateChecker::add_clear(
	const CL_Color &color)
{
	flush();
	clClearColor(color.get_red() / 255.0f, color.get_green() / 255.0f, color.get_blue() / 255.0f, color.get_alpha() / 255.0f);
	clClear(CL_COLOR_BUFFER_BIT);
}

void CL_GLStateChecker::add_pixel(
	const CL_GLState &state,
	int x, int y)
{
	set_state(state, CL_POINTS);
	clVertex2i(x, y);
}

void CL_GLStateChecker::add_line(
	const CL_GLState &state,
	float x1, float y1, float x2, float y2)
{
	set_state(state, CL_LINES);
	clVertex2f(x1, y1);
	clVertex2f(x2, y2);
}

void CL_GLStateChecker::add_lines(
	const CL_GLState &state,
	unsigned int count,
	double *vertices)
{
	set_state(state, CL_LINE_STRIP);

	// todo: add batching

	clVertexPointer( 2, CL_DOUBLE, 0, vertices );
	clDrawArrays(CL_LINE_STRIP, 0, count);
}

void CL_GLStateChecker::add_gradient(
	const CL_GLState &state,
	const CL_Rectf &position,
	const CL_Gradient &gradient)
{
	set_state(state, CL_GRADIENT);
	add_quad_vertices( state, position.left, position.top, position.right, position.bottom );

	int a = primitive_count*16;

	// Bottom left vertex
	b_color_array[a++] = gradient.bottom_left.get_red();
	b_color_array[a++] = gradient.bottom_left.get_green();
	b_color_array[a++] = gradient.bottom_left.get_blue();
	b_color_array[a++] = gradient.bottom_left.get_alpha();

	// Bottom right vertex
	b_color_array[a++] = gradient.bottom_right.get_red();
	b_color_array[a++] = gradient.bottom_right.get_green();
	b_color_array[a++] = gradient.bottom_right.get_blue();
	b_color_array[a++] = gradient.bottom_right.get_alpha();

	// Top right vertex
	b_color_array[a++] = gradient.top_right.get_red();
	b_color_array[a++] = gradient.top_right.get_green();
	b_color_array[a++] = gradient.top_right.get_blue();
	b_color_array[a++] = gradient.top_right.get_alpha();

	// Top left vertex
	b_color_array[a++] = gradient.top_left.get_red();
	b_color_array[a++] = gradient.top_left.get_green();
	b_color_array[a++] = gradient.top_left.get_blue();
	b_color_array[a++] = gradient.top_left.get_alpha();

	primitive_count++;

	if( primitive_count == MAX_QUAD_COUNT )
		flush();
}

void CL_GLStateChecker::add_gradient(
	const CL_GLState &state,
	const CL_Quad &quad,
	const CL_Gradient &gradient)
{
	set_state(state, CL_GRADIENT);
	add_quad_vertices( state, (double)quad.x1, (double)quad.y1, (double)quad.x2, (double)quad.y2, (double)quad.x3, (double)quad.y3, (double)quad.x4, (double)quad.y4 );

	int a = primitive_count*16;

	// Top left vertex
	b_color_array[a++] = gradient.top_left.get_red();
	b_color_array[a++] = gradient.top_left.get_green();
	b_color_array[a++] = gradient.top_left.get_blue();
	b_color_array[a++] = gradient.top_left.get_alpha();

	// Top right vertex
	b_color_array[a++]  = gradient.top_right.get_red();
	b_color_array[a++]  = gradient.top_right.get_green();
	b_color_array[a++] =	gradient.top_right.get_blue();
	b_color_array[a++] =	gradient.top_right.get_alpha();

	// Bottom right vertex
	b_color_array[a++] = gradient.bottom_right.get_red();
	b_color_array[a++] = gradient.bottom_right.get_green();
	b_color_array[a++] = gradient.bottom_right.get_blue();
	b_color_array[a++] = gradient.bottom_right.get_alpha();

	// Bottom left vertex
	b_color_array[a++] = gradient.bottom_left.get_red();
	b_color_array[a++] = gradient.bottom_left.get_green();
	b_color_array[a++] = gradient.bottom_left.get_blue();
	b_color_array[a++] = gradient.bottom_left.get_alpha();

	primitive_count++;

	if( primitive_count == MAX_QUAD_COUNT )
		flush();
}

void CL_GLStateChecker::add_quad_vertices( 
	const CL_GLState &state,
	double px1,
	double py1,
	double px2,
	double py2 )
{
	int b = primitive_count * 8; // 8 values per quad

	vertex_array[b+0] = px1;
	vertex_array[b+1] = py2;
	vertex_array[b+2] = px2;
	vertex_array[b+3] = py2;
	vertex_array[b+4] = px2;
	vertex_array[b+5] = py1;
	vertex_array[b+6] = px1;
	vertex_array[b+7] = py1;
}

// used only by add_gradient with CL_Quad
void CL_GLStateChecker::add_quad_vertices( 
	const CL_GLState &state,
	double px1,
	double py1,
	double px2,
	double py2,
	double px3,
	double py3,
	double px4,
	double py4 )
{
	int b = primitive_count * 8; // 8 values per quad

	vertex_array[b+0] = px1;
	vertex_array[b+1] = py1;
	vertex_array[b+2] = px2;
	vertex_array[b+3] = py2;
	vertex_array[b+4] = px3;
	vertex_array[b+5] = py3;
	vertex_array[b+6] = px4;
	vertex_array[b+7] = py4;
}

void CL_GLStateChecker::add_quad(
	const CL_GLState &state,
	double px1,
	double py1,
	double px2,
	double py2,
	int tx1,
	int ty1,
	int tx2,
	int ty2)
{
	set_state(state, CL_QUADS);

	if( internal_state.texture_handle != -1 )
	{
		int a = primitive_count * 8; // offset in array. 8 values per quad 
		
		i_uv_array[a+0] = tx1;
		i_uv_array[a+1] = ty2;
		i_uv_array[a+2] = tx2;
		i_uv_array[a+3] = ty2;
		i_uv_array[a+4] = tx2;
		i_uv_array[a+5] = ty1;
		i_uv_array[a+6] = tx1;
		i_uv_array[a+7] = ty1;
	}

	add_quad_vertices( state, px1, py1, px2, py2 );
	
	primitive_count++;
		
	if( primitive_count == MAX_QUAD_COUNT )
		flush();
}

void CL_GLStateChecker::add_quad(
	const CL_GLState &state,
	const double destX[4],
	const double destY[4],
	const int tx[4],
	const int ty[4])
{
	set_state(state, CL_QUADS);

	int a = primitive_count * 8; // offset in array. 8 values per quad 

	if( internal_state.texture_handle != -1 )
	{
		i_uv_array[a+0] = tx[0];
		i_uv_array[a+1] = ty[0];
		i_uv_array[a+2] = tx[1];
		i_uv_array[a+3] = ty[1];
		i_uv_array[a+4] = tx[2];
		i_uv_array[a+5] = ty[2];
		i_uv_array[a+6] = tx[3];
		i_uv_array[a+7] = ty[3];
	}

	vertex_array[a+0] = destX[0];
	vertex_array[a+1] = destY[0];
	vertex_array[a+2] = destX[1];
	vertex_array[a+3] = destY[1];
	vertex_array[a+4] = destX[2];
	vertex_array[a+5] = destY[2];
	vertex_array[a+6] = destX[3];
	vertex_array[a+7] = destY[3];

	primitive_count++;
		
	if( primitive_count == MAX_QUAD_COUNT )
		flush();
}

void CL_GLStateChecker::add_triangles(
	const CL_GLState &state,
	CL_TriangleParams *params )
{
	tri_use_color = (params->single_color == 0);

	set_state(state, CL_TRIANGLES);

	tri_use_uv = ((params->uv != 0) || (params->int_uv != 0));

	primitive_texture = params->texture;

	// make sure used texture isn't deleted before drawing is complete
	if( primitive_texture )
		primitive_texture->add_ref();
	
	if( primitive_count + params->count > MAX_QUAD_COUNT )
	{
		if( params->count > 2*MAX_QUAD_COUNT-1 )
		{
			flush();

			// vertex pointer
			clVertexPointer( 3, CL_DOUBLE, 0, params->vertices );
			
			// texcoord pointer
			if( tri_use_uv )
			{
				if( uv_array_range_zero2one )
					clTexCoordPointer( 2, CL_DOUBLE, 0, params->uv );
				else
					clTexCoordPointer( 2, CL_INT, 0, params->int_uv );
			}

			// color pointer
			if( color_array_enabled )
			{
				if( color_array_range_zero2one )
					clColorPointer( 4, CL_DOUBLE, 0, params->color );
				else
					clColorPointer( 4, CL_UNSIGNED_BYTE, 0, params->uchar_color );
			}

			// Draw it!
			clDrawArrays(CL_TRIANGLES, 0, 3*params->count);

			if( primitive_texture )
				primitive_texture->release_ref();

			return;
		}

		unsigned int fit = MAX_QUAD_COUNT - primitive_count;
		
		memcpy( vertex_array+primitive_count*3*3, params->vertices, sizeof(double)*fit*3*3 );

		if( params->uv )
			memcpy( d_uv_array+primitive_count*3*2, params->uv, sizeof(double)*fit*3*2 );
		if( params->int_uv )
			memcpy( i_uv_array+primitive_count*2*3, params->int_uv, sizeof(int)*fit*2*3 );

		if( params->color )
			memcpy( d_color_array+primitive_count*3*4, params->color, sizeof(double)*fit*3*4 );
		if( params->uchar_color )
			memcpy( b_color_array+primitive_count*4*3, params->uchar_color, sizeof(unsigned char)*fit*4*3 );

		primitive_count += fit;
		
		flush();

		// make sure used texture isn't deleted before drawing is complete
		// need to renew ref, becaues flush released it.
		if( primitive_texture )
			primitive_texture->add_ref();
		
		memcpy( vertex_array, params->vertices+fit*3*3, sizeof(double)*(params->count-fit)*3*3 );

		if( params->uv )
			memcpy( d_uv_array+primitive_count*3*2, params->uv+fit*3*2, sizeof(double)*(params->count-fit)*3*2 );
		if( params->int_uv )
			memcpy( i_uv_array+primitive_count*3*2, params->int_uv+fit*3*2, sizeof(int)*(params->count-fit)*3*2 );

		if( params->color )
			memcpy( d_color_array+primitive_count*3*4, params->color+fit*3*4, sizeof(double)*(params->count-fit)*3*4 );
		if( params->uchar_color )
			memcpy( b_color_array+primitive_count*3*4, params->uchar_color+fit*3*4, sizeof(double)*(params->count-fit)*3*4 );
		
		primitive_count += params->count-fit;
	}
	else
	{
		memcpy( vertex_array+primitive_count*3*3, params->vertices, sizeof(double)*params->count*3*3 );

		if( params->uv )
			memcpy( d_uv_array+primitive_count*2*3, params->uv, sizeof(double)*params->count*2*3 );
		if( params->int_uv )
			memcpy( i_uv_array+primitive_count*2*3, params->int_uv, sizeof(int)*params->count*2*3 );

		if( params->color )
			memcpy( d_color_array+primitive_count*4*3, params->color, sizeof(double)*params->count*4*3 );
		if( params->uchar_color )
			memcpy( b_color_array+primitive_count*4*3, params->uchar_color, sizeof(unsigned char)*params->count*4*3 );

		primitive_count += params->count;
	}
}

void CL_GLStateChecker::flush(bool rebind_texture) const
{
	opengl_state->set_active();

	if (primitive == CL_LINES || primitive == CL_POINTS )
	{
		clEnd();
		primitive = CL_DONT_CARE; // carefull... CL_POINTS is defined as 0x0000
	}

	if( primitive_count != 0 )
	{
		////////////////////////////////////////////////////
		// vertice array
		
		int elements_per_vertex = 2;

		if( primitive == CL_TRIANGLES )
			elements_per_vertex = 3;

		clVertexPointer( elements_per_vertex, CL_DOUBLE, 0, vertex_array );

		////////////////////////////////////////////////////
		//  tex coord (uv) array

		if( primitive == CL_TRIANGLES )
		{
			if( tri_use_uv )
			{
				if( uv_array_range_zero2one )
					clTexCoordPointer( 2, CL_DOUBLE, 0, d_uv_array );
				else
					clTexCoordPointer( 2, CL_INT, 0, i_uv_array );
			}
		}
		else
		{
			clTexCoordPointer( 2, CL_INT, 0, i_uv_array );
		}

		////////////////////////////////////////////////////
		// color array

		if( primitive == CL_GRADIENT )
		{
			clColorPointer( 4, CL_UNSIGNED_BYTE, 0, b_color_array );
		}
		else if( primitive == CL_TRIANGLES )
		{
			if( color_array_enabled )
			{
				if( color_array_range_zero2one )
				{
					clColorPointer( 4, CL_DOUBLE, 0, d_color_array );
				}
				else
				{
					clColorPointer( 4, CL_UNSIGNED_BYTE, 0, b_color_array );
				}
			}
		}

		////////////////////////////////////////////////////
		// DRAW IT!

		if( primitive == CL_QUADS || primitive == CL_GRADIENT )
		{
			clDrawArrays(CL_QUADS, 0, 4*primitive_count);
		}
		else if( primitive == CL_TRIANGLES )
		{
			if( uv_array_range_zero2one )
			{
				clMatrixMode(CL_TEXTURE);
				clPushMatrix();
				clLoadIdentity();		
			}

			clDrawArrays(CL_TRIANGLES, 0, 3*primitive_count);

			if( uv_array_range_zero2one )
			{
				clPopMatrix();
				clMatrixMode(CL_MODELVIEW);
			}

			if( primitive_texture )
				primitive_texture->release_ref();
		}

		primitive_count = 0;
	}

	if( rebind_texture )
		internal_state.texture_handle = -2;
}

CL_Color CL_GLStateChecker::get_pixel(int x, int y) const
{
	flush();

	CLdouble viewport[4];
	clGetDoublev(CL_VIEWPORT, viewport);
	CLdouble height = viewport[3];

	CL_Color pixel;
	clReadPixels(x, (int)height-y, 1, 1, CL_RGB, CL_UNSIGNED_BYTE, &pixel.color);
	pixel.set_alpha( 255 );
	return pixel;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLStateData:

void CL_GLStateChecker::on_save()
{
	flush();
}

void CL_GLStateChecker::on_load()
{
	gc_opengl->sync_gc();
/*
	// restore opengl states
	clBlendFunc(internal_state.blendfunc[0], internal_state.blendfunc[1]);
	clColor4fv(internal_state.color);
	if (internal_state.texture_handle == -2 || internal_state.texture_handle == -1)
	{
		clDisable(CL_TEXTURE_2D);
	}
	else
	{
		clEnable(CL_TEXTURE_2D);
		clBindTexture(CL_TEXTURE_2D, internal_state.texture_handle);
		CLint texture_width = 1, texture_height = 1;
		clGetTexLevelParameteriv(CL_TEXTURE_2D, 0, CL_TEXTURE_WIDTH, &texture_width);
		clGetTexLevelParameteriv(CL_TEXTURE_2D, 0, CL_TEXTURE_HEIGHT, &texture_height);
		clMatrixMode(CL_TEXTURE);
		CLdouble scale_matrix[16];
		memset(scale_matrix, 0, sizeof(CLdouble)*16);
		scale_matrix[0] = 1.0/texture_width;
		scale_matrix[5] = 1.0/texture_height;
		scale_matrix[10] = 1.0;
		scale_matrix[15] = 1.0;
		clLoadMatrixd(scale_matrix);
		clMatrixMode(CL_MODELVIEW);
	}
*/
}

void CL_GLStateChecker::on_flush()
{
	flush();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GLStateTree implementation:

void CL_GLStateChecker::set_state(const CL_GLState &state, CLenum new_primitive)
{
	opengl_state->set_active();

	if (first_call)
	{
		opengl_state->setup_2d(gc_opengl->is_canvas_target());
		clEnableClientState(CL_VERTEX_ARRAY);
		clEnableClientState(CL_TEXTURE_COORD_ARRAY);
		clDisableClientState(CL_COLOR_ARRAY);
		set_blendfunc(internal_state);
		clColor4fv(internal_state.color);
		clEnable(CL_TEXTURE_2D);
		clEnable(CL_BLEND);
		clTexEnvf(CL_TEXTURE_ENV, CL_TEXTURE_ENV_MODE, CL_MODULATE);
		first_call = false;
	}

	/////////////////////////////////////////////////////
	// texturing

	if (internal_state.texture_handle != state.texture_handle)
	{
		internal_state.texture_handle = state.texture_handle;
		
		flush();

		if(state.texture_handle == -1)
		{
			clDisable(CL_TEXTURE_2D);
			clDisableClientState(CL_TEXTURE_COORD_ARRAY);
		}
		else
		{
			// Bind texture and convert internal texture coordinates to be
			// in pixels.
			clEnable(CL_TEXTURE_2D);
			clBindTexture(CL_TEXTURE_2D, (CLuint)state.texture_handle);
			clTexEnvf(CL_TEXTURE_ENV, CL_TEXTURE_ENV_MODE, CL_MODULATE);
			CLint texture_width = 1, texture_height = 1;
			clGetTexLevelParameteriv(CL_TEXTURE_2D, 0, CL_TEXTURE_WIDTH, &texture_width);
			clGetTexLevelParameteriv(CL_TEXTURE_2D, 0, CL_TEXTURE_HEIGHT, &texture_height);
			clMatrixMode(CL_TEXTURE);
			CLdouble scale_matrix[16];
			memset(scale_matrix, 0, sizeof(CLdouble)*16);
			scale_matrix[0] = 1.0/texture_width;
			scale_matrix[5] = 1.0/texture_height;
			scale_matrix[10] = 1.0;
			scale_matrix[15] = 1.0;
			clLoadMatrixd(scale_matrix);

			
			if (!state.sub_pixel_accuracy)
			{
				// Sync texture coordinates with screen coordinates for GL_LINEAR sampling:
				clTranslated(cl_pixelcenter_constant, cl_pixelcenter_constant, 0.0);
			}

			clMatrixMode(CL_MODELVIEW);

			clEnableClientState(CL_TEXTURE_COORD_ARRAY);
		}
	}

	/////////////////////////////////////////////////////
	// blendfuncs
	
	if (internal_state.blendfunc[0] != state.blendfunc[0] || 
		 internal_state.blendfunc[1] != state.blendfunc[1] ||
		 internal_state.blendfunc[2] != state.blendfunc[2] ||
		 internal_state.blendfunc[3] != state.blendfunc[3])
	{
		flush();

		internal_state.blendfunc[0] = state.blendfunc[0];
		internal_state.blendfunc[1] = state.blendfunc[1];
		internal_state.blendfunc[2] = state.blendfunc[2];
		internal_state.blendfunc[3] = state.blendfunc[3];

		set_blendfunc(state);
	}

	/////////////////////////////////////////////////////
	// fillmode 

	if (internal_state.fillmode != state.fillmode )
	{
		flush();
		internal_state.fillmode = state.fillmode;
		set_fillmode(state);
	}

	/////////////////////////////////////////////////////
	// color

	if (internal_state.color[0] != state.color[0] ||
		internal_state.color[1] != state.color[1] ||
		internal_state.color[2] != state.color[2] ||
		internal_state.color[3] != state.color[3])
	{
		flush();

		internal_state.color[0] = state.color[0];
		internal_state.color[1] = state.color[1];
		internal_state.color[2] = state.color[2];
		internal_state.color[3] = state.color[3];

		clColor4fv(state.color);
	}

	// Set vertex array client states for color array.
	if( new_primitive == CL_TRIANGLES )
	{
		if( tri_use_color != color_array_enabled )
		{
			flush();

			if( tri_use_color )
				clEnableClientState(CL_COLOR_ARRAY);
			else
				clDisableClientState(CL_COLOR_ARRAY);

			color_array_enabled = tri_use_color;
		}

		if( state.color_range_zero2one != color_array_range_zero2one )
		{
			flush();
			color_array_range_zero2one = state.color_range_zero2one;
		}

		if( state.uv_range_zero2one != uv_array_range_zero2one )
		{
			flush();
			uv_array_range_zero2one = state.uv_range_zero2one;
		}

	}
	else if( new_primitive == CL_GRADIENT )
	{
		if( !color_array_enabled )
		{
			flush();
			clEnableClientState(CL_COLOR_ARRAY);
			color_array_enabled = true;
		}
	}
	else
	{
		if( color_array_enabled )
		{
			flush();
			clDisableClientState(CL_COLOR_ARRAY);
			color_array_enabled = false;
		}
	}


	/////////////////////////////////////////////////////
	// primitive

	if ( primitive != new_primitive )
	{
		flush();

		if ( new_primitive == CL_LINES || new_primitive == CL_POINTS )
			clBegin(new_primitive);

		primitive = new_primitive;
	}
}

void CL_GLStateChecker::set_fillmode(const CL_GLState &state)
{
	switch( state.fillmode )
	{
	case fillmode_line:
		clPolygonMode( CL_FRONT_AND_BACK, CL_LINE );
		break;
	case fillmode_point:
		clPolygonMode( CL_FRONT_AND_BACK, CL_POINT );
		break;
	case fillmode_solid:
		clPolygonMode( CL_FRONT_AND_BACK, CL_FILL );
		break;
	}
}

void CL_GLStateChecker::set_blendfunc(const CL_GLState &state)
{
	CLenum blendfunc[4];
	for(int i = 0; i < 4; ++i)
	{			
		switch (state.blendfunc[i])
		{
		case blend_zero:
			blendfunc[i] = CL_ZERO;
			break;
		case blend_one:
			blendfunc[i] = CL_ONE;
			break;
		case blend_dest_color:
			blendfunc[i] = CL_DST_COLOR;
			break;
		case blend_src_color:
			blendfunc[i] = CL_SRC_COLOR;
			break;
		case blend_one_minus_dest_color:
			blendfunc[i] = CL_ONE_MINUS_DST_COLOR;
			break;
		case blend_one_minus_src_color:
			blendfunc[i] = CL_ONE_MINUS_SRC_COLOR;
			break;
		case blend_src_alpha:
			blendfunc[i] = CL_SRC_ALPHA;
			break;
		case blend_one_minus_src_alpha:
			blendfunc[i] = CL_ONE_MINUS_SRC_ALPHA;
			break;
		case blend_dst_alpha:
			blendfunc[i] = CL_DST_ALPHA;
			break;
		case blend_one_minus_dest_alpha:
			blendfunc[i] = CL_ONE_MINUS_DST_ALPHA;
			break;
		case blend_src_alpha_saturate:
			blendfunc[i] = CL_SRC_ALPHA_SATURATE;
			break;
		case blend_constant_color:
			blendfunc[i] = CL_CONSTANT_COLOR;
			break;
		case blend_constant_alpha:
			blendfunc[i] = CL_CONSTANT_ALPHA;
			break;
		case blend_one_minus_constant_color:
			blendfunc[i] = CL_ONE_MINUS_CONSTANT_COLOR;
			break;
		case blend_one_minus_constant_alpha:
			blendfunc[i] = CL_ONE_MINUS_CONSTANT_ALPHA;
			break;
		}
	}

	if (blendfunc[0] == blendfunc[2] && blendfunc[1] == blendfunc[3])
	{
		clBlendFunc(blendfunc[0], blendfunc[1]);
	}
	else
	{
		clBlendFuncSeparate(blendfunc[0], blendfunc[1], blendfunc[2], blendfunc[3]);
	}
}

void CL_GLStateChecker::clear_stencil(int value)
{
	opengl_state->set_active();
	
	flush();
	
	clClearStencil(value);
	clClear(CL_STENCIL_BUFFER_BIT);
}

void CL_GLStateChecker::enable_stencil_write(bool enabled, bool visible, double alpha_limit)
{
	opengl_state->set_active();

	flush();

	if( enabled )
	{
		if( !visible )
			clColorMask(0,0,0,0);

		clEnable(CL_STENCIL_TEST);
		clStencilFunc(stencil_func_write, stencil_write_ref, 255);
		clStencilOp(stencil_op_fail, CL_KEEP, stencil_op_pass); // CL_KEEP: action for stencil-pass & z-fail
		clEnable(CL_ALPHA_TEST);
		clAlphaFunc(CL_GEQUAL, CLclampf(alpha_limit));
	}
	else
	{
		clColorMask(1,1,1,1);
		clDisable(CL_STENCIL_TEST);
		clDisable(CL_ALPHA_TEST);  
	}
}

void CL_GLStateChecker::enable_stencil_test(bool enabled, CL_StencilFunc func, int ref)
{
	opengl_state->set_active();

	flush();

	int stencil_func = 0;

	switch( func )
	{
		case stencil_never: stencil_func = CL_NEVER; break;
		case stencil_less: stencil_func = CL_LESS; break;
		case stencil_lequal: stencil_func = CL_LEQUAL; break;
		case stencil_greater: stencil_func = CL_GREATER; break;
		case stencil_gequal: stencil_func = CL_GEQUAL; break;
		case stencil_equal: stencil_func = CL_EQUAL; break;
		case stencil_notequal: stencil_func = CL_NOTEQUAL; break;
		case stencil_always: stencil_func = CL_ALWAYS; break;
		default: stencil_func = CL_LEQUAL;
	}

	if( enabled )
	{
		clEnable(CL_STENCIL_TEST);	
		clStencilFunc(stencil_func, ref, 255); 
		clStencilOp(CL_KEEP, CL_KEEP, CL_KEEP);
	}
	else
	{
		clDisable(CL_STENCIL_TEST);	
	}
}

void CL_GLStateChecker::reinitialize_asap()
{
	first_call = true;	
}
