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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "compare_function.h"

/// \brief Stencil operations
///
/// \xmlonly !group=Display/Display 2D! !header=display.h! \endxmlonly
enum CL_StencilOp
{
	cl_stencil_keep,
	cl_stencil_zero,
	cl_stencil_replace,
	cl_stencil_incr,
	cl_stencil_decr,
	cl_stencil_invert,
	cl_stencil_incr_wrap,
	cl_stencil_decr_wrap
};

/// \brief Drawing buffers.
enum CL_DrawBuffer
{
	cl_buffer_none,
	cl_buffer_front_left,
	cl_buffer_front_right,
	cl_buffer_back_left,
	cl_buffer_back_right,
	cl_buffer_front,
	cl_buffer_back,
	cl_buffer_left,
	cl_buffer_right,
	cl_buffer_front_and_back
};

/// \brief Logic Op
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_LogicOp
{
	cl_logic_op_clear,
	cl_logic_op_and, 
	cl_logic_op_and_reverse,
	cl_logic_op_copy,
	cl_logic_op_and_inverted,
	cl_logic_op_noop,
	cl_logic_op_xor,
	cl_logic_op_or,
	cl_logic_op_nor,
	cl_logic_op_equiv,
	cl_logic_op_invert,
	cl_logic_op_or_reverse,
	cl_logic_op_copy_inverted,
	cl_logic_op_or_inverted,
	cl_logic_op_nand,
	cl_logic_op_set
};

class CL_BufferControl_Impl;

/// \brief Container class for states related to reading, writing and tests performed on buffers.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_BufferControl
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a buffer control description object.
	CL_BufferControl();

	virtual ~CL_BufferControl();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if stencil test is enabled.
	bool is_stencil_test_enabled() const;

	/// \brief Returns the stencil compare function for front facing polygons, points, lines and bitmaps.
	CL_CompareFunction get_stencil_compare_func_front() const;

	/// \brief Returns the stencil compare function for back facing polygons.
	CL_CompareFunction get_stencil_compare_func_back() const;

	/// \brief Returns the stencil compare reference for front facing polygons, points, lines and bitmaps
	int get_stencil_compare_reference_front() const;

	/// \brief Returns the stencil compare reference for back facing polygons.
	int get_stencil_compare_reference_back() const;

	/// \brief Returns the stencil operation to perform when test fails for front facing polygons, points, lines and bitmaps
	CL_StencilOp get_stencil_fail_front() const;

	/// \brief Returns the stencil operation to perform when test fails for back facing polygons.
	CL_StencilOp get_stencil_fail_back() const;

	/// \brief Returns the stencil operation to perform when test passes for front facing polygons, points, lines and bitmaps
	CL_StencilOp get_stencil_pass_depth_pass_front() const;

	/// \brief Returns the stencil operation to perform when test passes for back facing polygons.
	CL_StencilOp get_stencil_pass_depth_pass_back() const;

	/// \brief Returns the stencil operation to perform when depth test passes but stencil test fails for front facing polygons, points, lines and bitmaps
	CL_StencilOp get_stencil_pass_depth_fail_front() const;

	/// \brief Returns the stencil operation to perform when depth test passes but stencil test fails for back facing polygons.
	CL_StencilOp get_stencil_pass_depth_fail_back() const;

	/// \brief Returns the stencil comparation mask for front facing polygons, points, lines and bitmaps.
	unsigned char get_stencil_compare_mask_front() const;

	/// \brief Returns the stencil comparation mask for back facing polygons.
	unsigned char get_stencil_compare_mask_back() const;

	/// \brief Returns the stencil write mask for front facing polygons, points, lines and bitmaps.
	unsigned char get_stencil_write_mask_front() const;

	/// \brief Returns the stencil write mask for back facing polygons.
	unsigned char get_stencil_write_mask_back() const;

	/// \brief Returns true if the depth test is enabled.
	bool is_depth_test_enabled() const;

	/// \brief Returns true if depth writing is enabled.
	bool is_depth_write_enabled() const;

	/// \brief Returns the depth test function.
	CL_CompareFunction get_depth_compare_function() const;

	/// \brief Retrieves which color components are write enabled.
	void is_color_write_enabled(bool &red, bool &green, bool &blue, bool &alpha) const;

	/// \brief Returns the used draw buffer.
	CL_DrawBuffer get_draw_buffer() const;

	/// \brief Returns true if logic operations are enabled.
	bool is_logic_op_enabled() const;

	/// \brief Returns the set logic operations.
	CL_LogicOp get_logic_op() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Enables/disables stencil testing.
	void enable_stencil_test(bool enabled);

	/// \brief Set stencil compare function.
	void set_stencil_compare_func(CL_CompareFunction front, CL_CompareFunction back);

	/// \brief Set stencil compare reference values.
	void set_stencil_compare_reference(int front_ref, int back_ref);

	/// \brief Set stencil write masks.
	void set_stencil_write_mask(unsigned char front_facing_mask, unsigned char back_facing_mask);

	/// \brief Set stencil failed test operations.
	void set_stencil_fail(CL_StencilOp front, CL_StencilOp back);

	/// \brief Set stencil passed test operations.
	void set_stencil_pass_depth_pass(CL_StencilOp front, CL_StencilOp back);

	/// \brief Set stencil passed depth test but failed stencil test operations.
	void set_stencil_pass_depth_fail(CL_StencilOp front, CL_StencilOp back);

	/// \brief Set stencil comparation masks.
	void set_stencil_compare_mask(int front_mask, int back_mask);

	/// \brief Enables/disables depth testing.
	void enable_depth_test(bool enabled);

	/// \brief Enables/disables depth writing.
	void enable_depth_write(bool enabled);

	/// \brief Set depth test function.
	void set_depth_compare_function(CL_CompareFunction func);

	/// \brief Enable/disable writing to the color buffer
	void enable_color_write(bool enabled);

	/// \brief Enable color write
	///
	/// \param red = bool
	/// \param green = bool
	/// \param blue = bool
	/// \param alpha = bool
	void enable_color_write(bool red, bool green, bool blue, bool alpha);

	/// \brief Set used draw buffer.
	void set_draw_buffer(CL_DrawBuffer buffer);

	/// \brief Enable/disable logic op.
	void enable_logic_op(bool enabled);

	/// \brief Set logic operation.
	void set_logic_op(CL_LogicOp op);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_BufferControl_Impl> impl;
/// \}
};

/// \}
