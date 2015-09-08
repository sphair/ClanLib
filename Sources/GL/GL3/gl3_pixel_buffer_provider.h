/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#pragma once


#include "API/Display/TargetProviders/pixel_buffer_provider.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"
#include "gl3_buffer_object_provider.h"

namespace clan
{
	class GL3GraphicContextProvider;

	class GL3PixelBufferProvider : public PixelBufferProvider
	{
	public:
		GL3PixelBufferProvider();
		~GL3PixelBufferProvider();
		void create(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage) override;

		void *get_data() override;
		int get_pitch() const override { return pitch; }
		Size get_size() const override { return size; }

		bool is_gpu() const override { return true; }

		TextureFormat get_format() const override { return texture_format; };

		GLuint get_handle() const { return buffer.get_handle(); }
		GLenum get_binding() const { return buffer.get_binding(); }
		GLenum get_target() const { return buffer.get_target(); }

		void lock(GraphicContext &gc, BufferAccess access) override { data_locked = true; buffer.lock(gc, access); }
		void unlock() override { data_locked = false; buffer.unlock(); }
		void upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data) override;

	private:
		GL3BufferObjectProvider buffer;
		Size size;
		TextureFormat texture_format;
		int pitch;
		int bytes_per_pixel;
		GLenum selected_binding;
		GLenum selected_target;
		bool data_locked;	// lock() has been called
	};
}
