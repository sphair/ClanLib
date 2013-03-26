/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/uniform_buffer.h"
#include "API/Display/Render/storage_buffer.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/Render/depth_stencil_state.h"
#include "API/Display/Render/rasterizer_state.h"

namespace clan
{

class GraphicContext_State
{
public:
	GraphicContext_State();
	void copy_state(const GraphicContext_State *other);
	void reset_blend_mode_states();
	void reset_pen_states();
	void reset_buffer_control_states();
	void reset_polygon_rasterizer_states();

	FrameBuffer read_frame_buffer;
	FrameBuffer write_frame_buffer;

	std::vector<Texture> textures;
	std::vector<Texture> image_textures;
	std::vector<UniformBuffer> uniform_buffers;
	std::vector<StorageBuffer> storage_buffers;

	Rect scissor;
	bool scissor_set;

	std::vector<Rectf> viewport;
	std::vector<Sizef> depth_range;

	StandardProgram program_standard;
	bool program_standard_set;
	ProgramObject program;	//<-- Note this is valid when program_standard_set is true

	RasterizerState rasterizer_state;
	BlendState blend_state;
	Vec4f blend_color;
	unsigned int sample_mask;
	DepthStencilState depth_stencil_state;
	int stencil_ref;

	// Old Pen
	float pen_point_size;
	float pen_point_fade_treshold_size;
	float pen_line_width;
//	bool pen_line_antialiased;
	bool pen_vertex_shader_point_sizes;
	PointSpriteOrigin pen_point_sprite_origin;
/*
	// Old BlendMode
	BlendFunc blend_src_func, blend_dest_func, blend_src_alpha_func, blend_dest_alpha_func;
	BlendEquation blend_color_equation, blend_alpha_equation;
	Colorf blend_blend_color;
	bool blend_blending_enabled;

	// Old BufferControl
	StencilOp stencil_pass_depth_pass_front;
	StencilOp stencil_pass_depth_fail_front;
	StencilOp stencil_fail_front;
	StencilOp stencil_pass_depth_pass_back;
	StencilOp stencil_pass_depth_fail_back;
	StencilOp stencil_fail_back;
	CompareFunction stencil_compare_func_front, stencil_compare_func_back;
	bool stencil_test_enabled;
	int stencil_compare_ref_front, stencil_compare_ref_back;
	unsigned char stencil_compare_mask_front;
	unsigned char stencil_compare_mask_back;
	unsigned char stencil_enable_mask_front;
	unsigned char stencil_enable_mask_back;
	CompareFunction depth_compare_func;
	bool depth_write_enabled;
	bool depth_test_enabled;
	bool red_write_enabled, green_write_enabled, blue_write_enabled, alpha_write_enabled;
*/
	DrawBuffer draw_buffer;
	bool logic_op_enabled;
	LogicOp logic_op;

	// Old PolygonRasterizer
	bool is_antialiased;
//	bool is_culled;
	bool is_offset_point;
	bool is_offset_line;
	bool is_offset_fill;
//	CullMode face_cull_mode;
//	FillMode face_fill_mode;
//	FaceSide front_face_side;
	float offset_factor;
	float offset_units;
};

}
