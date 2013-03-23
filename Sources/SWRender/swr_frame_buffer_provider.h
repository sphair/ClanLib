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
#include "API/Display/Render/render_buffer.h"
#include "API/Display/Render/texture.h"

namespace clan
{

class CL_PixelBuffer;

class CL_SWRenderFrameBufferProvider : public CL_FrameBufferProvider
{
/// \name Construction
/// \{

public:
	CL_SWRenderFrameBufferProvider();
	~CL_SWRenderFrameBufferProvider();


/// \}
/// \name Attributes
/// \{

public:
	CL_Size get_size() const;

	CL_PixelBuffer get_colorbuffer0() const;

	CL_Signal_v0 &get_sig_changed_event() {return sig_changed_event;}

	CL_FrameBufferBindTarget get_bind_target() const;

/// \}
/// \name Operations
/// \{

public:
	void destroy();
	void attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer);
	void detach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer);
	void attach_color_buffer(int color_buffer, const CL_Texture &texture, int level = 0, int zoffset = 0);
	void attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);
	void detach_color_buffer(int color_buffer, const CL_Texture &texture, int level = 0, int zoffset = 0);

	void attach_stencil_buffer(const CL_RenderBuffer &render_buffer);
	void detach_stencil_buffer(const CL_RenderBuffer &render_buffer);
	void attach_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0);
	void attach_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);
	void detach_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0);

	void attach_depth_buffer(const CL_RenderBuffer &render_buffer);
	void detach_depth_buffer(const CL_RenderBuffer &render_buffer);
	void attach_depth_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0);
	void attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);
	void detach_depth_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0);

	void attach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer);
	void detach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer);
	void attach_depth_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0);
	void attach_depth_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);
	void detach_depth_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0);

	void set_bind_target(CL_FrameBufferBindTarget target);

/// \}
/// \name Implementation
/// \{
private:
	enum Type { type_none, type_render, type_texture };
	Type colorbuffer0_type;
	CL_RenderBuffer colorbuffer0_render;
	CL_Texture colorbuffer0_texture;
	CL_Signal_v0 sig_changed_event;
	mutable std::vector<int> attachment_indexes;
/// \}
};


}
