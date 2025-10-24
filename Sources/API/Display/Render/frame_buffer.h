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
#include "../../Core/System/sharedptr.h"

class CL_Size;
class CL_Rect;
class CL_Texture;
class CL_RenderBuffer;
class CL_GraphicContext;
class CL_FrameBufferProvider;
class CL_FrameBuffer_Impl;

/// \brief Texture Subtype
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_TextureSubtype
{
	cl_subtype_cube_map_positive_x,
	cl_subtype_cube_map_negative_x,
	cl_subtype_cube_map_positive_y,
	cl_subtype_cube_map_negative_y,
	cl_subtype_cube_map_positive_z,
	cl_subtype_cube_map_negative_z
};

/// \brief Framebuffer bind target.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_FrameBufferBindTarget 
{
	cl_framebuffer_draw,
	cl_framebuffer_read
};

/// \brief Frame-buffer object class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_FrameBuffer
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	CL_FrameBuffer();

	/// \brief Constructs a FrameBuffer
	///
	/// By default, the bind target is set to cl_framebuffer_draw. See set_bind_target()
	///
	/// \param context = Graphic Context
	CL_FrameBuffer(CL_GraphicContext &context);

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get Provider
	///
	/// \return provider
	CL_FrameBufferProvider *get_provider() const;

	/// \brief Get the minumum size of all the frame buffer attachments
	///
	/// \return size
	CL_Size get_size() const;

	/// \brief Get the bind target of the framebuffer
	///
	/// \return The bind target
	CL_FrameBufferBindTarget get_bind_target() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Equality operator
	bool operator==(const CL_FrameBuffer &other) const;

	/// \brief Attach color buffer
	///
	/// \param attachment_index = value
	/// \param render_buffer = Render Buffer
	void attach_color_buffer(int attachment_index, const CL_RenderBuffer &render_buffer);

	/// \brief Attach color buffer
	///
	/// \param attachment_index = value
	/// \param render_buffer = Render Buffer
	void detach_color_buffer(int attachment_index, const CL_RenderBuffer &render_buffer);

	/// \brief Attach color buffer
	///
	/// \param attachment_index = value
	/// \param texture = Texture
	/// \param level = value
	/// \param zoffset = value
	void attach_color_buffer(int attachment_index, const CL_Texture &texture, int level = 0, int zoffset = 0);

	/// \brief Attach color buffer
	///
	/// \param attachment_index = value
	/// \param texture = Texture
	/// \param subtype = Texture Subtype
	/// \param level = value
	/// \param zoffset = value
	void attach_color_buffer(int attachment_index, const CL_Texture &texture, CL_TextureSubtype subtype, int level = 0, int zoffset = 0);

	/// \brief Detach color buffer
	///
	/// \param attachment_index = value
	/// \param texture = Texture
	/// \param level = value
	/// \param zoffset = value
	void detach_color_buffer(int attachment_index, const CL_Texture &texture, int level = 0, int zoffset = 0);

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

	/// \brief Set the bind target of the framebuffer to either drawn to or read from.
	///
	/// Detach existing textures and renderbuffers before setting a new bind target
	///
	/// \param target = Target
	void set_bind_target(CL_FrameBufferBindTarget target);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_FrameBuffer_Impl> impl;
/// \}
};

/// \}
