/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#pragma once


#include "API/Display/TargetProviders/frame_buffer_provider.h"

class CL_D3D9FrameBufferProvider : public CL_FrameBufferProvider
{
/// \name Construction
/// \{

public:
	CL_D3D9FrameBufferProvider();

	~CL_D3D9FrameBufferProvider();


/// \}
/// \name Attributes
/// \{

public:
	CL_Size get_attachment_size(int buffer_id) const;


/// \}
/// \name Operations
/// \{

public:
	void destroy();

	void attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer);

	void attach_color_buffer(int color_buffer, const CL_Texture &texture, int level = 0, int zoffset = 0);

	void attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);

	void detach_color_buffer(int color_buffer);

	void attach_stencil_buffer(const CL_RenderBuffer &render_buffer);

	void detach_stencil_buffer();

	void attach_depth_buffer(const CL_RenderBuffer &render_buffer);

	void attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);

	void detach_depth_buffer();


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


