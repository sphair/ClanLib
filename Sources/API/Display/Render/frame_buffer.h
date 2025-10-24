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
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{


#pragma once


#include "../api_display.h"
#include "../../Core/System/sharedptr.h"

class CL_Size;
class CL_Rect;
class CL_Texture;
class CL_RenderBuffer;
class CL_GraphicContext;
class CL_FrameBufferProvider;
class CL_FrameBuffer_Impl;

enum CL_TextureSubtype
{
	cl_subtype_cube_map_positive_x,
	cl_subtype_cube_map_negative_x,
	cl_subtype_cube_map_positive_y,
	cl_subtype_cube_map_negative_y,
	cl_subtype_cube_map_positive_z,
	cl_subtype_cube_map_negative_z,
};

/// \brief Frame-buffer object class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_FrameBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a frame buffer.
	CL_FrameBuffer();

	CL_FrameBuffer(CL_GraphicContext context);

/// \}
/// \name Attributes
/// \{

public:
	CL_FrameBufferProvider *get_provider() const;

	CL_Size get_attachment_size(int buffer_id) const;


/// \}
/// \name Operations
/// \{

public:
	void attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer);

	void attach_color_buffer(int color_buffer, const CL_Texture &texture, int level = 0, int zoffset = 0);

	void attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);

	void detach_color_buffer(int color_buffer, const CL_Texture &texture, int level = 0, int zoffset = 0);

	void attach_stencil_buffer(const CL_RenderBuffer &render_buffer);

	void detach_stencil_buffer();

	void attach_depth_buffer(const CL_RenderBuffer &render_buffer);

	void attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);

	void detach_depth_buffer();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_FrameBuffer_Impl> impl;
/// \}
};


/// \}
