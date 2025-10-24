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
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../Render/frame_buffer.h"
#include <vector>

class CL_RenderBuffer;
class CL_Texture;

/// \brief Interface for implementing a CL_FrameBuffer target.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_FrameBufferProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_FrameBufferProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:

	virtual CL_Size get_size() const = 0;

	virtual CL_FrameBufferBindTarget get_bind_target() const = 0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the provider.
	virtual void destroy() = 0;

	/// \brief Attach color buffer
	///
	/// \param color_buffer = value
	/// \param render_buffer = Render Buffer
	virtual void attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer) = 0;

	/// \brief Detach color buffer
	///
	/// \param color_buffer = value
	/// \param render_buffer = Render Buffer
	virtual void detach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer) = 0;

	/// \brief Attach color buffer
	///
	/// \param color_buffer = value
	/// \param texture = Texture
	/// \param level = value
	/// \param zoffset = value
	virtual void attach_color_buffer(int color_buffer, const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;

	/// \brief Attach color buffer
	///
	/// \param color_buffer = value
	/// \param texture = Texture
	/// \param subtype = Texture Subtype
	/// \param level = value
	/// \param zoffset = value
	virtual void attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0) = 0;

	/// \brief Detach color buffer
	///
	/// \param color_buffer = value
	/// \param texture = Texture
	/// \param level = value
	/// \param zoffset = value
	virtual void detach_color_buffer(int color_buffer, const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;

	virtual void attach_stencil_buffer(const CL_RenderBuffer &render_buffer) = 0;
	virtual void detach_stencil_buffer(const CL_RenderBuffer &render_buffer) = 0;
	virtual void attach_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;
	virtual void attach_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0) = 0;
	virtual void detach_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;

	virtual void attach_depth_buffer(const CL_RenderBuffer &render_buffer) = 0;
	virtual void detach_depth_buffer(const CL_RenderBuffer &render_buffer) = 0;
	virtual void attach_depth_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;
	virtual void attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0) = 0;
	virtual void detach_depth_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;

	virtual void attach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer) = 0;
	virtual void detach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer) = 0;
	virtual void attach_depth_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;
	virtual void attach_depth_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0) = 0;
	virtual void detach_depth_stencil_buffer(const CL_Texture &texture, int level = 0, int zoffset = 0) = 0;

	virtual void set_bind_target(CL_FrameBufferBindTarget target) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
