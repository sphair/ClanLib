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
**    Magnus Norddahl
**    Harry Storbacka
*/

#pragma once


#include "opengl_graphic_context_provider.h"

#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/render_buffer.h"

class CL_API_DISPLAY CL_OpenGLFrameBufferProvider : public CL_FrameBufferProvider, CL_DisposableObject
{
/// \name Construction
/// \{

public:
	CL_OpenGLFrameBufferProvider(CL_OpenGLGraphicContextProvider *gc_provider);

	~CL_OpenGLFrameBufferProvider();


/// \}
/// \name Attributes
/// \{

public:
	CLuint get_handle();

	CL_Size get_size() const;

	CL_FrameBufferBindTarget get_bind_target() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the provider.
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

	void check_framebuffer_complete();

/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	static CL_String get_error_message(int error_code);

	CLuint decode_texture_subtype(CL_TextureSubtype subtype);
	void detach_object(CLenum opengl_attachment, const CL_Texture &texture);
	void detach_object(CLenum opengl_attachment, const CL_RenderBuffer &render_buffer);

	// Returns true if the object was replaced
	bool attach_object(CLenum opengl_attachment, const CL_Texture &texture, int level, int zoffset, CLuint texture_target);
	bool attach_object(CLenum opengl_attachment, const CL_RenderBuffer &render_buffer);

	int decode_internal_attachment_offset(CLenum opengl_attachment);

	static const int depth_attachment_offset = 0;
	static const int stencil_attachment_offset = 1;
	static const int depth_stencil_attachment_offset = 2;
	static const int color_attachment_offset = 3;
	static const int max_color_attachments = 16;
	static const int num_attachment_offsets = color_attachment_offset + max_color_attachments;

	CL_Texture attached_textures[num_attachment_offsets];
	CL_RenderBuffer attached_renderbuffers[num_attachment_offsets];

	int count_color_attachments;
	CLuint handle;

	CL_FrameBufferBindTarget bind_target;

	CL_OpenGLGraphicContextProvider *gc_provider;
/// \}
};

class CL_FrameBufferStateTracker
{
public:
	CL_FrameBufferStateTracker(CL_FrameBufferBindTarget target, CLuint handle, CL_OpenGLGraphicContextProvider *gc_provider);
	~CL_FrameBufferStateTracker();

private:
	CL_FrameBufferBindTarget bind_target;
	CLint last_bound;
	bool handle_and_bound_equal;
};


