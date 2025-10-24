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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Render/buffer_control.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BufferControl_Impl Class:

class CL_BufferControl_Impl
{
public:
	CL_BufferControl_Impl() :
		stencil_pass_depth_pass_front(cl_stencil_incr),
		stencil_pass_depth_fail_front(cl_stencil_keep),
		stencil_fail_front(cl_stencil_keep),
		stencil_pass_depth_pass_back(cl_stencil_keep),
		stencil_pass_depth_fail_back(cl_stencil_keep),
		stencil_fail_back(cl_stencil_keep),
		stencil_func_front(cl_comparefunc_lequal),
		stencil_func_back(cl_comparefunc_lequal),
		stencil_test_enabled(false),
		stencil_ref_front(0), stencil_ref_back(0),
		stencil_compare_mask_front(255),
		stencil_compare_mask_back(255),
		stencil_enable_mask_front(255),
		stencil_enable_mask_back(255),
		depth_compare_func(cl_comparefunc_lequal),
		depth_write_enabled(true),
		depth_test_enabled(false),
		red_write_enabled(true),
		green_write_enabled(true),
		blue_write_enabled(true),
		alpha_write_enabled(true),
		draw_buffer(cl_buffer_back),
		logic_op_enabled(false)
	{
	}

	~CL_BufferControl_Impl()
	{
	}

	CL_StencilOp stencil_pass_depth_pass_front;
	CL_StencilOp stencil_pass_depth_fail_front;
	CL_StencilOp stencil_fail_front;
	CL_StencilOp stencil_pass_depth_pass_back;
	CL_StencilOp stencil_pass_depth_fail_back;
	CL_StencilOp stencil_fail_back;
	CL_CompareFunction stencil_func_front, stencil_func_back;
	bool stencil_test_enabled;
	int stencil_ref_front, stencil_ref_back;
	unsigned char stencil_compare_mask_front;
	unsigned char stencil_compare_mask_back;
	unsigned char stencil_enable_mask_front;
	unsigned char stencil_enable_mask_back;

	CL_CompareFunction depth_compare_func;
	bool depth_write_enabled;
	bool depth_test_enabled;

	bool red_write_enabled, green_write_enabled, blue_write_enabled, alpha_write_enabled;

	CL_DrawBuffer draw_buffer;

	bool logic_op_enabled;
	CL_LogicOp logic_op;
};

/////////////////////////////////////////////////////////////////////////////
// CL_BufferControl Construction:

CL_BufferControl::CL_BufferControl()
: impl(new CL_BufferControl_Impl)
{
}

CL_BufferControl::~CL_BufferControl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CL_BufferControl Attributes:

bool CL_BufferControl::is_stencil_test_enabled() const
{
	return impl->stencil_test_enabled;
}

CL_CompareFunction CL_BufferControl::get_stencil_compare_func_front() const
{
	return impl->stencil_func_front;
}

CL_CompareFunction CL_BufferControl::get_stencil_compare_func_back() const
{
	return impl->stencil_func_back;
}

int CL_BufferControl::get_stencil_compare_reference_front() const
{
	return impl->stencil_ref_front;
}

int CL_BufferControl::get_stencil_compare_reference_back() const
{
	return impl->stencil_ref_back;
}

CL_StencilOp CL_BufferControl::get_stencil_fail_front() const
{
	return impl->stencil_fail_front;
}

CL_StencilOp CL_BufferControl::get_stencil_fail_back() const
{
	return impl->stencil_fail_back;
}

CL_StencilOp CL_BufferControl::get_stencil_pass_depth_pass_front() const
{
	return impl->stencil_pass_depth_pass_front;
}

CL_StencilOp CL_BufferControl::get_stencil_pass_depth_pass_back() const
{
	return impl->stencil_pass_depth_pass_back;
}

CL_StencilOp CL_BufferControl::get_stencil_pass_depth_fail_front() const
{
	return impl->stencil_pass_depth_fail_front;
}

CL_StencilOp CL_BufferControl::get_stencil_pass_depth_fail_back() const
{
	return impl->stencil_pass_depth_fail_back;
}

unsigned char CL_BufferControl::get_stencil_compare_mask_front() const
{
	return impl->stencil_compare_mask_front;
}

unsigned char CL_BufferControl::get_stencil_compare_mask_back() const
{
	return impl->stencil_compare_mask_back;
}

unsigned char CL_BufferControl::get_stencil_write_mask_front() const
{
	return impl->stencil_enable_mask_front;
}

unsigned char CL_BufferControl::get_stencil_write_mask_back() const
{
	return impl->stencil_enable_mask_back;
}

bool CL_BufferControl::is_depth_test_enabled() const
{
	return impl->depth_test_enabled;
}

bool CL_BufferControl::is_depth_write_enabled() const
{
	return impl->depth_write_enabled;
}

CL_CompareFunction CL_BufferControl::get_depth_compare_function() const
{
	return impl->depth_compare_func;
}

void CL_BufferControl::is_color_write_enabled(bool &red, bool &green, bool &blue, bool &alpha) const
{
	red = impl->red_write_enabled;
	green = impl->green_write_enabled;
	blue = impl->blue_write_enabled;
	alpha = impl->alpha_write_enabled;
}

CL_DrawBuffer CL_BufferControl::get_draw_buffer() const
{
	return impl->draw_buffer;
}

bool CL_BufferControl::is_logic_op_enabled() const
{
	return impl->logic_op_enabled;
}

CL_LogicOp CL_BufferControl::get_logic_op() const
{
	return impl->logic_op;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BufferControl Operations:

void CL_BufferControl::set_stencil_pass_depth_pass(CL_StencilOp front_pass, CL_StencilOp back_pass)
{
	impl->stencil_pass_depth_pass_front = front_pass;
	impl->stencil_pass_depth_pass_back = back_pass;
}

void CL_BufferControl::set_stencil_fail(CL_StencilOp ffail, CL_StencilOp bfail)
{
	impl->stencil_fail_front = ffail;
	impl->stencil_fail_back = bfail;
}

void CL_BufferControl::set_stencil_pass_depth_fail(CL_StencilOp front, CL_StencilOp back)
{
	impl->stencil_pass_depth_fail_front = front;
	impl->stencil_pass_depth_fail_back = back;
}

void CL_BufferControl::set_stencil_compare_func(CL_CompareFunction front_func, CL_CompareFunction back_func)
{
	impl->stencil_func_front = front_func;
	impl->stencil_func_back = back_func;
}

void CL_BufferControl::set_stencil_compare_reference(int front_ref, int back_ref)
{
	impl->stencil_ref_front = front_ref;
	impl->stencil_ref_back = back_ref;
}

void CL_BufferControl::enable_stencil_test(bool enabled)
{
	impl->stencil_test_enabled = enabled;
}

void CL_BufferControl::enable_color_write(bool enabled)
{
	enable_color_write(enabled,enabled,enabled,enabled);
}

void CL_BufferControl::enable_color_write(bool red, bool green, bool blue, bool alpha)
{
	impl->red_write_enabled = red;
	impl->green_write_enabled = green;
	impl->blue_write_enabled = blue;
	impl->alpha_write_enabled = alpha;
}

void CL_BufferControl::set_stencil_write_mask(unsigned char front_facing_mask, unsigned char back_facing_mask)
{
	impl->stencil_enable_mask_front = front_facing_mask;
	impl->stencil_enable_mask_back = back_facing_mask;
}

void CL_BufferControl::set_stencil_compare_mask(int front_mask, int back_mask)
{
	impl->stencil_compare_mask_front = front_mask;
	impl->stencil_compare_mask_back = back_mask;
}

void CL_BufferControl::enable_depth_write(bool enabled)
{
	impl->depth_write_enabled = enabled;
}

void CL_BufferControl::enable_depth_test(bool enabled)
{
	impl->depth_test_enabled = enabled;
}

void CL_BufferControl::set_depth_compare_function(CL_CompareFunction func)
{
	impl->depth_compare_func = func;
}

void CL_BufferControl::set_draw_buffer(CL_DrawBuffer buffer)
{
	impl->draw_buffer = buffer;
}

void CL_BufferControl::enable_logic_op(bool enabled)
{
	impl->logic_op_enabled = enabled;
}

void CL_BufferControl::set_logic_op(CL_LogicOp op)
{
	impl->logic_op = op;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BufferControl Implementation:
