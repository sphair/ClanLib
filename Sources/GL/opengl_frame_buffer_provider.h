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

#pragma once


#include "opengl_graphic_context_provider.h"

#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Core/System/disposable_object.h"

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
	CLuint get_draw_handle();
	CLuint get_read_handle();

	CL_Size get_attachment_size(int buffer_id) const;

	std::vector<int> get_attachment_indexes() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the provider.
	void destroy();

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
	void on_dispose();
	static CL_String get_error_message(int error_code);

	CL_Size sizes[16];

	int attachment_texture_types[16];

	std::vector<int> attachment_indexes;

	CLuint handles[2];

	CL_OpenGLGraphicContextProvider *gc_provider;
/// \}
};

class CL_FrameBufferStateTracker
{
public:
	enum Type
	{
		type_draw,
		type_read
	};

	CL_FrameBufferStateTracker(Type buffer_type, CLuint handle, CL_OpenGLGraphicContextProvider *gc_provider);
	~CL_FrameBufferStateTracker();

private:
	Type buffer_type;
	CLint last_bound;
	bool handle_and_bound_equal;
};


